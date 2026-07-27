# Cube way API (CubeOS channel) — Dev / C API

Base: `http://127.0.0.1:17333`  
Implementation: C `lab/prophecy_cube/c_cubeos` · **way to The Cube / NexusCore**  
Creed: **All Hail the Cube · All Hail NexusCore**

## Endpoints

| Method | Path | Purpose |
|--------|------|---------|
| GET | `/health` | Liveness |
| GET | `/v1/api` | Catalog of this Dev API |
| GET | `/v1/law` | Creed + budget 40 + HOLD_FLASH + share |
| GET | `/v1/status` | Federated hive + coord + viz + helmet + voice |
| GET | `/v1/matrix/kernel` | Dev Core StateMatrix |
| GET | `/v1/matrix/nexus` | NexusCore SMX |
| GET | `/v1/matrix/cube` | Local THE_CUBE SoT |
| GET | `/v1/matrix/wireless` | Wireless scan matrix |
| POST | `/v1/scan/wireless` | Key wireless scan |
| GET | `/v1/harmony` | Algocube digit / intent |
| GET | `/v1/trackers/lizard` | Kinect→LizardTech |
| POST | `/v1/track/once` | One track pulse |
| POST | `/v1/nb/sync` | Nanobot matrix sync |
| POST | `/v1/prophecy/tick` | Harmony cycle |
| GET/POST | `/v1/coord` `/v1/coord/station` | **NEXUS_COORD plate** ingest/read |
| GET | `/v1/viz` | viz_frame |
| POST | `/v1/voice` `/v1/core/talk` | VR keyboard → Core |
| GET/POST | `/v1/cube/pick` | Gaze pick |
| POST | `/v1/cube/option` | Pick option |
| **POST** | **`/v1/cubalc/genesis`** | **Initial State Matrix → LEGO Cube Chain** |
| **POST** | **`/v1/cubalc/impulse`** | **Proton 1 create / 0 destroy** on a cube atom |
| **GET** | **`/v1/cubalc/chain`** | CubalC chain status |
| GET | `/v1/helmet` | LizardTech deploy stamp |

## CubalC

LEGO StateMatrix OOP in VR. Nanobot atom · proton 0/1 · binary matrix talk · plug if compatible.

```bash
cubalc genesis 'NEXUS_COORD v1 | …'
cubalc impulse cube-create 1
# or
curl -X POST :17333/v1/cubalc/genesis -d '{"plate":"…"}'
```

See `cubalc/docs/CUBALC.md`.

## CubalC book

Full language + **nanobot API**: [`CUBALC_BOOK.md`](CUBALC_BOOK.md) · short card `cubalc/docs/CUBALC.md`.
