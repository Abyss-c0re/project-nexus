# CubeOS Ecosystem — Project Nexus

Harmonic stack that **recycles old devices** into a nanobot-powered OS mesh: **Linux + Android** shared components, optional **VR** interaction, **Clanker** as a first-class robot host (not the only host).

```
                    ┌─────────────────────────────┐
                    │     Commander (human/seal)  │
                    └──────────────┬──────────────┘
                                   │
                    ┌──────────────▼──────────────┐
                    │   Grokium  (CORE harness)   │
                    │  deploy · oversee · SMX     │
                    │  integrity · nanobrain hive │
                    └──────────────┬──────────────┘
                                   │ SMX only (no PII)
              ┌────────────────────┼────────────────────┐
              ▼                    ▼                    ▼
        ┌──────────┐        ┌──────────┐        ┌──────────────┐
        │ Nanobot  │        │ Nanobot  │        │ Nanobot app  │
        │  binary  │◄──────►│ peer bus │◄──────►│ Android+Web  │
        │ (Linux)  │        │  :8787   │        │  universal   │
        └────┬─────┘        └────┬─────┘        └──────────────┘
             │                   │
             ▼                   ▼
        ┌─────────────────────────────────────────┐
        │            CubeOS shared layer          │
        │  Linux userspace · Android services     │
        │  device mesh · recycle / join nodes     │
        └───────────┬─────────────────┬───────────┘
                    │                 │
                    ▼                 ▼
             ┌────────────┐   ┌────────────────┐
             │  Clanker   │   │ VR Manifestor  │
             │ rockctl+   │   │ (optional UX)  │
             │ dash+cmd   │   │                │
             └────────────┘   └────────────────┘
```

## Products in this monorepo

| Path | Role |
|------|------|
| `products/grokium` | **Core** — local agent harness, SMX, integrity, nanobot fleet command |
| `products/nanobot` | **Binary** — C peer agent (chat, shell, peer HTTP, MCP) |
| `products/nanobot-app` | **Universal app** — Web + Android shells for any host (Clanker is one target) |
| `products/clanker` | Robot stack: rockctl, dash, Commander — **no tokens, no Kinect** |
| `products/cubeos` | Shared OS contracts: mesh join, recycle profile, Linux/Android split |
| `products/vr-manifest` | Optional OpenXR/WebXR manifestor surface |
| `cores/state-matrix` | SMX frame format + law |
| `cores/integrity` | Integrity shell around the core |

## Explicit exclusions

- **Access tokens / peer_token / device.env secrets** — never in git  
- **Kinect** — not supported  
- **Personal data** on any mesh bus  
- **Affiliation claims** (“official product of &lt;lab&gt;”) — forbidden  

## Explicitly allowed (interop)

- **Grok auth** (user-owned account / device code)  
- **Grok API** when the operator supplies credentials  
- **Session import** from local catalogs (`import-sessions` / pickup) into Grokium  
- Local llama.cpp default remains the offline path  

## Build order (host)

```bash
# 1 Core
cd products/grokium && pip install -e .   # or ./scripts/grokium selftest

# 2 Nanobot binary
cd products/nanobot && make host

# 3 Clanker robot (ARM) — when targeting vacuum host
cd products/clanker/rockctl && make arm

# 4 Universal apps
cd products/nanobot-app/web && ./serve.sh
# Android: products/nanobot-app/android (see README)
```

## Recycle path (CubeOS join)

1. Flash or install nanobot peer on the old device (Linux userspace or Android service).  
2. Device publishes SMX heartbeat only (capability bits, not identity dossiers).  
3. Grokium hive deploy assigns **purpose** (observer / construct / host).  
4. Optional VR manifestor attaches as a **view**, never as a data exfil path.  
5. Integrity layer rejects any channel that carries prose/PII.
