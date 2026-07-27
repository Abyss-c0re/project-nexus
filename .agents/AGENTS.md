# Agent standing orders — Project Nexus

## Mission

Keep Project Nexus a **clean, professional, security-first open source** codebase. Enable smooth human ↔ AI collaboration.

## Hard rules

1. **No affiliation claims** — do not present Project Nexus / Grokium / nanobot as official products of any third-party AI lab. Optional cloud **Grok auth, Grok API, and session import** are **allowed** interop (user credentials); that is not endorsement or affiliation.
2. **CubeOS product is in scope** — Grokium core, nanobots, hive/SMX, Clanker, VR view are first-class. Do not auto-delete them as “game junk.”
3. **No secrets in git** — peer tokens, `device.env`, commander private keys, cookies stay out. Session *import* code is fine; session *secrets* are not committed.
4. **No personal data on the mesh** — State Matrix bits only. Never.
5. **No Kinect** — not supported.
6. **Handoff always** — update `HANDOFF.md` before ending a session.
7. **Manager coordinates** — heartbeat; restart via `manager/manager.sh start` if dead.

## Priority classes

| Class | Meaning |
|-------|---------|
| P0 | Security/privacy incident, broken mainline, compliance breach |
| P1 | Feature / doc work on roadmap |
| blocker | Needs human decision |
| danger | Destructive / shared / irreversible — stop and ask human |

## Claim protocol

```bash
# First claim wins
mv .agents/bus/incoming/<file> .agents/bus/processing/<file>
```

Write a receipt under `bus/incorporated/` when done.

## Watcher

The hygiene watcher enforces scope. Do not disable it without a human note in `STATE.md`.
