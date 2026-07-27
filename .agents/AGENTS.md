# Agent standing orders — Project Nexus

## Mission

Keep Project Nexus a **clean, professional, security-first open source** codebase. Enable smooth human ↔ AI collaboration.

## Hard rules

1. **No affiliation branding** — do not present this project as a product of any third-party AI lab (including consumer chatbot brands). B2B compliance language only.
2. **No game / fan drift** — reject “Prophecy of the Cube”, hive-mind lore, VR game dumps, and similar entertainment content in the product tree.
3. **No secrets in git** — tokens, keys, cookies, private training media stay out.
4. **Privacy by design** — personal data minimized; regulated data defaults to zero retention unless legal-approved.
5. **Handoff always** — never end a session without updating `HANDOFF.md`.
6. **Manager is source of coordination** — check manager heartbeat; if dead, restart via `manager/manager.sh start`.

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
