# Project state

**Updated:** 2026-07-27T22:25:00+03:00  
**Branch:** master  
**Remote:** `[company-gitlab redacted]`  
**Phase:** ecosystem import — CubeOS monorepo

## Active goals

1. Grokium core deploys/oversees nanobot fleet (SMX + integrity)  
2. Nanobot binary + universal Web/Android apps  
3. Clanker stack (no tokens, no Kinect)  
4. CubeOS join/recycle contracts Linux+Android  
5. Optional VR manifestor  
6. Always-on manager + hygiene watcher  

## Restrictions (hard)

- No personal data on mesh  
- No access tokens in git  
- No Kinect  
- No third-party AI lab affiliation claims  
- Integrity fail-closed  

## Manager

`.agents/manager/manager.sh` — must remain running on active workstations.

## Next engineering

- [ ] Wire grokium `hive deploy` against in-tree nanobot binary  
- [ ] Android universal app beyond scaffold  
- [ ] CI: secret scan + build nanobot host  
- [ ] SPDX root license  
