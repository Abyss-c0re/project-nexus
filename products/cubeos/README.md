# CubeOS — shared mesh OS layer

**Goal:** turn spare Linux and Android devices into one **CubeOS** hive node mesh powered by nanobots.

## Contracts

| Contract | File |
|----------|------|
| Node identity (non-PII) | `contracts/node-profile.schema.json` |
| Join / recycle | `contracts/join-mesh.md` |
| Shared services | `contracts/shared-services.md` |

## Principles

- **Linux + Android** share protocol and SMX, not a single bloated userspace binary.  
- **Nanobot** is the always-on peer on each node.  
- **Grokium** assigns purpose and oversees; it does not own the device.  
- **No personal data** in profiles — capability bits and public keys only.

## Status

Scaffold. Implementation tracks Grokium hive deploy + nanobot peer.
