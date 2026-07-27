#!/usr/bin/env bash
# Integrity shell — fail closed on telemetry/PII paths
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
fail=0
# privacy hard-off in monorepo config
if grep -q 'telemetry = true' "$ROOT/config/cubeos.toml" 2>/dev/null; then
  echo "DENY: telemetry true"; fail=1
fi
# secrets in tree
if find "$ROOT/products" "$ROOT/var" -name 'peer_token' -o -name 'device.env' -o -name 'commander.sk' 2>/dev/null | grep -q .; then
  # var/home may create peer tokens at runtime — only fail if under products/
  if find "$ROOT/products" -name 'peer_token' -o -name 'device.env' -o -name 'commander.sk' 2>/dev/null | grep -q .; then
    echo "DENY: secrets under products/"; fail=1
  fi
fi
# kinect
if find "$ROOT/products" -iname '*kinect*' 2>/dev/null | grep -q .; then
  echo "DENY: kinect present"; fail=1
fi
if [[ $fail -eq 0 ]]; then
  echo "integrity: OK"
  exit 0
fi
echo "integrity: FAIL"
exit 1
