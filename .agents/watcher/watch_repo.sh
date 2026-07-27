#!/usr/bin/env bash
# Project Nexus hygiene watcher
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
AGENTS="$ROOT/.agents"
QUAR="$AGENTS/quarantine"
LOG="$AGENTS/logs/watcher.log"
LAST="$AGENTS/watcher/last_scan.json"
MODE="${1:---loop}"
INTERVAL="${WATCH_INTERVAL:-30}"

mkdir -p "$QUAR" "$AGENTS/logs" "$AGENTS/bus/incoming"

log() { printf '%s %s\n' "$(date -Iseconds)" "$*" | tee -a "$LOG" >/dev/null; echo "$*"; }

is_git_internal() {
  case "$1" in
    .git/*|.git) return 0 ;;
    *) return 1 ;;
  esac
}

should_skip() {
  local rel="$1"
  is_git_internal "$rel" && return 0
  case "$rel" in
    .agents/logs/*|.agents/quarantine/*|.agents/manager/manager.pid|.agents/manager/heartbeat.json|.agents/watcher/last_scan.json)
      return 0 ;;
  esac
  return 1
}

violations=()
quarantined=()

check_tree() {
  violations=()
  quarantined=()
  while IFS= read -r -d '' f; do
    rel="${f#"$ROOT"/}"
    should_skip "$rel" && continue

    base="$(basename "$f")"
    # name bans
    if echo "$base $rel" | grep -Eiq 'prophecy|grokium|hive.?mind|cube.?chain|prophecy.of.the.cube'; then
      violations+=("FORBIDDEN_NAME:$rel")
      continue
    fi

    # extension bans (not under .agents allow for scripts)
    case "$base" in
      *.mp4|*.mov|*.avi|*.mkv|*.wav|*.flac|*.iso|*.img)
        violations+=("FORBIDDEN_EXT:$rel")
        continue
        ;;
    esac

    # size
    if [[ -f "$f" ]]; then
      sz=$(stat -c%s "$f" 2>/dev/null || echo 0)
      if [[ "$sz" -gt 5242880 ]]; then
        case "$rel" in
          .agents/compliance/*) ;;
          *) violations+=("OVERSIZE:$rel:$sz") ;;
        esac
      fi

      # content sniff (text-ish only, small files)
      if [[ "$sz" -lt 2000000 && "$sz" -gt 0 ]]; then
        if file -b --mime-type "$f" 2>/dev/null | grep -q '^text/\|^application/json\|^application/yaml\|^application/x-yaml'; then
          if grep -Eiq 'prophecy of the cube|we are the hive mind|energy must flow\. join or perish|CUBE · GROK · NEXUS' "$f" 2>/dev/null; then
            # allow only if path is compliance? No - never in product
            case "$rel" in
              .agents/compliance/transcript.txt|.agents/compliance/training-digest.md|.agents/watcher/*|.agents/AGENTS.md|.agents/STATE.md|.agents/HANDOFF.md|docs/*)
                # docs may mention ban lists
                if ! grep -Eiq 'prophecy of the cube|we are the hive mind|energy must flow' <<<"$(basename "$rel")"; then
                  if grep -Eiq 'prophecy of the cube|we are the hive mind|CUBE · GROK · NEXUS' "$f" 2>/dev/null; then
                    # if file is the ban documentation, OK if it says "forbidden"
                    if ! grep -Eiq 'forbidden|reject|non-goal|quarantine|do not' "$f" 2>/dev/null; then
                      violations+=("FORBIDDEN_CONTENT:$rel")
                    fi
                  fi
                fi
                ;;
              *)
                violations+=("FORBIDDEN_CONTENT:$rel")
                ;;
            esac
          fi
          if grep -Eq '-----BEGIN (RSA |OPENSSH |EC )?PRIVATE KEY-----|glpat-[A-Za-z0-9_-]{20,}|ghp_[A-Za-z0-9]{20,}' "$f" 2>/dev/null; then
            violations+=("SECRET_PATTERN:$rel")
          fi
        fi
        # mislabeled media
        if file -b "$f" 2>/dev/null | grep -Eiq 'ISO Media|MP4|Audio file|RIFF \(little-endian\) data, AVI'; then
          case "$base" in
            *.md|*.txt|*.png|*.jpg) violations+=("MISLABELED_MEDIA:$rel") ;;
          esac
        fi
      fi
    fi
  done < <(find "$ROOT" -xdev \( -path "$ROOT/.git" -o -path "$AGENTS/logs" -o -path "$QUAR" \) -prune -o -type f -print0 2>/dev/null)
}

quarantine_file() {
  local rel="$1"
  local src="$ROOT/$rel"
  [[ -e "$src" ]] || return 0
  local dest="$QUAR/$(date +%Y%m%dT%H%M%S)_$(echo "$rel" | tr '/' '__')"
  mv "$src" "$dest"
  quarantined+=("$rel->$dest")
  log "QUARANTINE $rel -> $dest"
  # notify bus
  local note="$AGENTS/bus/incoming/$(date +%Y%m%d-%H%M%S)-quarantine.md"
  cat > "$note" <<NOTE
# Report: quarantine

- created: $(date -Iseconds)
- author: watcher
- class: P0
- summary: Quarantined out-of-scope or dangerous path \`$rel\`

## Context
Automated hygiene rule violation. File moved to \`.agents/quarantine/\`.

## Proposed actions
- Human/agent review
- Permanent delete if confirmed junk
NOTE
}

scan_once() {
  check_tree
  local ts
  ts=$(date -Iseconds)
  local vcount=${#violations[@]}
  for v in "${violations[@]+"${violations[@]}"}"; do
    log "VIOLATION $v"
    rel="${v#*:}"
    rel="${rel%%:*}"
    # Only auto-quarantine clear junk types
    case "$v" in
      FORBIDDEN_NAME:*|FORBIDDEN_EXT:*|OVERSIZE:*|MISLABELED_MEDIA:*|FORBIDDEN_CONTENT:*)
        # never quarantine core ops docs
        case "$rel" in
          .agents/*|docs/*|README.md|idea.md|.gitignore) log "SKIP_QUARANTINE $rel" ;;
          *) quarantine_file "$rel" || true ;;
        esac
        ;;
      SECRET_PATTERN:*)
        log "SECRET_ALERT $rel (not auto-deleted — human required)"
        note="$AGENTS/bus/incoming/$(date +%Y%m%d-%H%M%S)-secret-alert.md"
        echo -e "# Report: secret pattern\n\n- class: P0\n- path: \`$rel\`\n- action: rotate credentials; purge from history if committed\n" > "$note"
        ;;
    esac
  done
  printf '{"ts":"%s","violations":%s,"quarantined":%s}\n' \
    "$ts" "$vcount" "${#quarantined[@]}" > "$LAST"
  log "SCAN_COMPLETE violations=$vcount quarantined=${#quarantined[@]}"
}

case "$MODE" in
  --once) scan_once ;;
  --loop)
    log "WATCHER_START root=$ROOT interval=${INTERVAL}s"
    while true; do
      scan_once || true
      sleep "$INTERVAL"
    done
    ;;
  --status)
    if [[ -f "$LAST" ]]; then cat "$LAST"; else echo "no scan yet"; fi
    ;;
  *) echo "Usage: $0 --once|--loop|--status"; exit 2 ;;
esac
