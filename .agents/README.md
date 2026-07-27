# `.agents` — Project Nexus ops plane

Hidden collaboration layer for **humans and AI agents**. Everything needed to resume work lives here.

## Layout

| Path | Role |
|------|------|
| `AGENTS.md` | Standing orders for every agent session |
| `STATE.md` | Live project state (read first) |
| `HANDOFF.md` | Latest session handoff (write last) |
| `manager/` | Always-on manager process |
| `watcher/` | Repository hygiene watcher |
| `bus/incoming/` | Drop zone for reports / tasks |
| `bus/processing/` | Claimed work |
| `bus/incorporated/` | Done work + receipts |
| `bus/done/` | Closed archive |
| `memory/` | Durable decisions & ledger |
| `compliance/` | Partner training digests (debranded) |
| `quarantine/` | Auto-moved out-of-scope files |
| `templates/` | Report / receipt skeletons |
| `scripts/` | Helper scripts |
| `logs/` | Runtime logs (gitignored content) |

## Resume protocol (any agent joining)

1. Read `STATE.md` and `HANDOFF.md`
2. Check `bus/incoming/` and `bus/processing/`
3. Confirm manager heartbeat (`.agents/manager/heartbeat.json`)
4. Claim work with atomic `mv` into `processing/`
5. Before exit: update `HANDOFF.md` + `memory/ledger.md`
