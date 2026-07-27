# Cube way service contract

| Field | Value |
|-------|--------|
| **id** | cube_way |
| **role** | OS way into The Cube + NexusCore (not a sovereign OS) |
| **impl** | `<PROPHECY_CUBE_ROOT>/c_cubeos` |
| **port** | `127.0.0.1:17333` (local-first; LAN only if Commander sets `CUBE_CONTROL_HOST`) |
| **depends** | devcore `:17300`, optional NexusCore `:8861`, llama `:1212`, nanobot peers |
| **law** | ProjectNexus `docs/PROPHECY_OF_THE_CUBE.md` |

## Health

```bash
curl -fsS http://127.0.0.1:17333/health
curl -fsS http://127.0.0.1:17333/v1/status
```
