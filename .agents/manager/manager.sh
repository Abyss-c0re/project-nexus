#!/usr/bin/env bash
# Project Nexus — always-on collaboration manager
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
AGENTS="$ROOT/.agents"
DIR="$AGENTS/manager"
PIDFILE="$DIR/manager.pid"
HB="$DIR/heartbeat.json"
LOG="$AGENTS/logs/manager.log"
WATCHER="$AGENTS/watcher/watch_repo.sh"
INTERVAL="${MANAGER_INTERVAL:-45}"

mkdir -p "$DIR" "$AGENTS/logs" "$AGENTS/bus"/{incoming,processing,incorporated,done}

log() { printf '%s %s\n' "$(date -Iseconds)" "$*" | tee -a "$LOG" >/dev/null; }

heartbeat() {
  local status="${1:-ok}"
  local incoming processing
  incoming=$(find "$AGENTS/bus/incoming" -maxdepth 1 -type f ! -name '.gitkeep' 2>/dev/null | wc -l | tr -d ' ')
  processing=$(find "$AGENTS/bus/processing" -maxdepth 1 -type f ! -name '.gitkeep' 2>/dev/null | wc -l | tr -d ' ')
  cat > "$HB" <<JSON
{
  "ts": "$(date -Iseconds)",
  "status": "$status",
  "pid": $$,
  "root": "$ROOT",
  "incoming": $incoming,
  "processing": $processing,
  "role": "manager"
}
JSON
}

list_incoming() {
  find "$AGENTS/bus/incoming" -maxdepth 1 -type f ! -name '.gitkeep' | sort
}

# Lightweight triage: acknowledge new reports in ledger (does not replace human/agent deep work)
triage_incoming() {
  local f base dest
  while IFS= read -r f; do
    [[ -z "$f" ]] && continue
    base=$(basename "$f")
    # claim
    dest="$AGENTS/bus/processing/$base"
    if mv "$f" "$dest" 2>/dev/null; then
      log "CLAIMED $base"
      # classify by name keywords
      class="P1"
      echo "$base" | grep -Eiq 'secret|breach|quarantine|security|p0' && class="P0"
      receipt="$AGENTS/bus/incorporated/${base}.receipt.md"
      cat > "$receipt" <<REC
# Receipt: $base

- claimed: $(date -Iseconds)
- completed: $(date -Iseconds)
- class: $class
- summary: Manager acknowledged report \`$base\`. Deep work may still be required by a specialist agent.
- actions: [moved to processing, wrote receipt stub]
- follow_up: .agents/bus/processing/$base
REC
      # move original to incorporated
      mv "$dest" "$AGENTS/bus/incorporated/$base" 2>/dev/null || true
      echo "$(date -Iseconds) manager_ack $class $base" >> "$AGENTS/memory/ledger.md"
      log "ACK $class $base"
    fi
  done < <(list_incoming)
}

ensure_watcher() {
  # run a single scan each manager tick (avoids duplicate long-lived watchers on flaky mounts)
  if [[ -x "$WATCHER" ]]; then
    "$WATCHER" --once >> "$LOG" 2>&1 || log "WATCHER_SCAN_FAILED"
  fi
}

loop() {
  log "MANAGER_START root=$ROOT interval=${INTERVAL}s"
  heartbeat "starting"
  while true; do
    ensure_watcher
    triage_incoming
    heartbeat "ok"
    # refresh STATE stamp lightly
    if [[ -f "$AGENTS/STATE.md" ]]; then
      # touch only heartbeat pointer line via separate file
      echo "manager_ok $(date -Iseconds)" > "$DIR/last_ok.txt"
    fi
    sleep "$INTERVAL"
  done
}

is_running() {
  [[ -f "$PIDFILE" ]] || return 1
  local pid
  pid=$(cat "$PIDFILE" 2>/dev/null || true)
  [[ -n "${pid:-}" ]] && kill -0 "$pid" 2>/dev/null
}

cmd="${1:-status}"
case "$cmd" in
  start)
    if is_running; then
      echo "manager already running pid=$(cat "$PIDFILE")"
      exit 0
    fi
    nohup bash "$0" run >> "$LOG" 2>&1 &
    echo $! > "$PIDFILE"
    sleep 0.3
    echo "manager started pid=$(cat "$PIDFILE")"
    ;;
  run)
    echo $$ > "$PIDFILE"
    trap 'log MANAGER_STOP; rm -f "$PIDFILE"; exit 0' INT TERM
    loop
    ;;
  stop)
    if is_running; then
      kill "$(cat "$PIDFILE")" 2>/dev/null || true
      rm -f "$PIDFILE"
      echo "manager stopped"
    else
      rm -f "$PIDFILE"
      echo "manager not running"
    fi
    ;;
  status)
    if is_running; then
      echo "running pid=$(cat "$PIDFILE")"
      [[ -f "$HB" ]] && cat "$HB"
    else
      echo "stopped"
      exit 1
    fi
    ;;
  restart)
    "$0" stop || true
    "$0" start
    ;;
  *)
    echo "Usage: $0 start|stop|status|restart"
    exit 2
    ;;
esac
