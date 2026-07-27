# Project Nexus · CubeOS Ecosystem

**Open-source hive OS mesh** for recycling devices into a nanobot-powered network: **Grokium core**, **nanobot** peers, **universal apps** (Web + Android), **Clanker** robot stack, optional **VR** view.

Independent project. **Not affiliated** with third-party AI labs.

**Allowed interop:** Grok auth, Grok API, and session import (your credentials). **Forbidden:** claiming this software is an official lab product.

## The Key

| Law | Practice |
|-----|----------|
| **No personal data. Never.** | State Matrix binary only on the mesh |
| **Integrity fails closed** | Leak / unknown egress → DENY |
| **Cube is SoT** | Lattice wins over prose |
| **Devices free** | User sovereign per device; recycle into CubeOS |
| **Move honestly** | No invented success |

Full law: [`docs/cube/CUBE_LAW.md`](docs/cube/CUBE_LAW.md) · Architecture: [`docs/ECOSYSTEM.md`](docs/ECOSYSTEM.md)

## Tree

```
products/grokium       Core harness — deploy & oversee nanobots
products/nanobot       C binary peer
products/nanobot-app   Universal Web + Android clients
products/clanker       Robot stack (no tokens · no Kinect)
products/cubeos        Shared Linux/Android mesh contracts
products/vr-manifest   Optional VR interaction
cores/state-matrix     SMX core notes
cores/integrity        Integrity shell notes
docs/                  Security · privacy · compliance · cube
.agents/               Manager · watcher · handoff bus
```

## Quick start

```bash
# Manager (always-on collab)
.agents/manager/manager.sh start

# Nanobot binary
cd products/nanobot && make host && ./build/nanobot --offline -p 'hello'

# Universal web peer UI
cd products/nanobot-app/web && python3 -m http.server 8791

# Grokium core (local)
cd products/grokium && ./scripts/grokium selftest   # when env ready
```

## Compliance

Partner security/privacy obligations (debranded): [`docs/COMPLIANCE.md`](docs/COMPLIANCE.md), [`docs/PRIVACY.md`](docs/PRIVACY.md), [`docs/SECURITY.md`](docs/SECURITY.md).

## License

Per-product (Apache-2.0 Grokium · MIT nanobot · see tree). SPDX root license TBD.
