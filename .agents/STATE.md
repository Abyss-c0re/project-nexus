# Project state

**Updated:** 2026-07-27T21:55:00+03:00  
**Branch:** master  
**Remote:** `[company-gitlab redacted]`  
**Phase:** foundation — docs + ops plane

## Active goals

1. Maintain professional OSS baseline (docs, security, privacy, compliance)
2. Keep always-on **manager** agent for collaboration continuity
3. Enforce repo hygiene via **watcher** (no game content, no media dumps, no secrets)
4. Implement product surface for trace analysis (next engineering phase)

## Contacts (fill when available)

| Role | Contact |
|------|---------|
| Security reports | _TBD — set private channel_ |
| Maintainer | [maintainer redacted] |
| Partner compliance | Follow engagement contract |

## Manager

- Script: `.agents/manager/manager.sh`
- Expected: process running; heartbeat age < 120s

## Open decisions

- [ ] SPDX license selection
- [ ] Public security contact address
- [ ] CI pipeline (lint + secret scan)

## Forbidden in tree

- Prophecy of the Cube / cube lore / hive-mind game assets
- Large media (mp4, wav, multi‑MB png dumps)
- Third-party AI lab branding / co-marketing
- Secrets and credential material
