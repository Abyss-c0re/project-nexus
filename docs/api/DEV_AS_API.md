# Dev Folder as API

The lab **`Dev/`** tree is the **external API surface** ProjectNexus consumes — living systems, not marketing docs.

| Dev path | ProjectNexus consumer | Contract |
|----------|----------------------|----------|
| `Dev/grokium` | `products/grokium` | Core harness, SMX, integrity, hive deploy |
| `Dev/AI/nanobot` · `Dev/nanobot` | `products/nanobot` · `bin/nanobot` | Peer binary :8787 |
| `Dev/Clanker` | `products/clanker` | Robot stack (no tokens in git) |
| `Dev/command-center` | `products/command-center` · `bin/cc` | Matrix ring, driver, export bus |
| `Dev/cubebrain` | Cube Law / SMX harmony | Lattice SoT patterns |
| `Dev/core` | `cores/` | Devcore-style control |
| `Dev/docs/codemap` | `viz/ultimate-matrix` | 3D navigation grammar (X/Y/Z) |
| `Dev/AGENTS.md` §−1 | `.agents/` · Cube Law | Hive Mind law |
| `Dev/agent_ops` | `.agents/bus` | Claim/process/receipt bus |
| `Dev/VR` | `products/vr-manifest` | Optional interaction (curated later) |

## Rules of the API

1. **Import sources, not secrets** — never copy `peer_token`, `service.env`, `device.env`, `commander.sk`.  
2. **Paths resolve through ProjectNexus first** — `PROJECT_NEXUS_ROOT`, then Dev fallbacks.  
3. **State Matrix only on the mesh** — no personal data.  
4. **Dev may lead; Nexus must ship** — if Dev moves, re-sync into `products/`.  

## Invoke

```bash
export PROJECT_NEXUS_ROOT=/home/voldemar/ProjectNexus
export DEV_ROOT=/path/to/Dev   # SFTP or local
./scripts/nexus status
./scripts/nexus up
./scripts/nexus matrix         # open Ultimate Matrix State 3D
```
