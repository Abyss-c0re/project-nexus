#!/usr/bin/env bash
# Deploy ProjectNexus as Prophecy of the Cube — Cube way API (C)
set -euo pipefail
PC="${PROPHECY_CUBE:-${PROPHECY_CUBE:-$HOME/lab/prophecy_cube}}"
C="$PC/c_cubeos"
echo "=== Prophecy of the Cube · deploy ==="
make -C "$C" -j4 all
# restart daemon
if [[ -f $PC/state/cube_daemon.pid ]]; then
  kill "$(cat $PC/state/cube_daemon.pid)" 2>/dev/null || true
  sleep 0.5
fi
fuser -k 17333/tcp 2>/dev/null || true
sleep 0.3
"$C/out/nb_manifest" || true
"$C/out/cube_core_tick" "$PC/state" || true
nohup "$C/out/cube_daemon" --hz "${KINECT_HZ:-2}" \
  >>"$PC/state/cube_daemon.log" 2>&1 &
echo $! >"$PC/state/cube_daemon.pid"
sleep 1
echo "--- health ---"
curl -fsS http://127.0.0.1:17333/health; echo
echo "--- law ---"
curl -fsS http://127.0.0.1:17333/v1/law; echo
echo "--- status (keys) ---"
curl -fsS -m 5 http://127.0.0.1:17333/v1/status | python3 -c 'import sys,json;d=json.load(sys.stdin);print("ok",d.get("ok"),"philosophy",d.get("philosophy"));print("devcore",bool(d.get("devcore")));print("nexuscore",bool(d.get("nexuscore_smx")));print("llama",bool(d.get("llama")));print("quest",bool(d.get("quest_nanobot")));print("cube_sot",bool(d.get("cube_sot")));print("lizard",bool(d.get("lizard_track")))'
echo "--- prophecy tick ---"
curl -fsS -m 5 -X POST http://127.0.0.1:17333/v1/prophecy/tick | head -c 300; echo
echo "=== DEPLOYED pid=$(cat $PC/state/cube_daemon.pid) ==="
echo "API: http://127.0.0.1:17333  docs: ProjectNexus/docs/PROPHECY_OF_THE_CUBE.md"
