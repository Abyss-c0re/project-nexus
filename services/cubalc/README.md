# CubalC — Cube LEGO language (C runtime)

See `docs/CUBALC_BOOK.md` for the full language book and **nanobot API**.

```bash
make -C services/cubalc
./services/cubalc/out/cubalc genesis 'NEXUS_COORD v1 | …'
```

Cube way HTTP: `services/cube_way/` (`POST /v1/cubalc/*` on control plane).

**Secrets:** never commit `peer_token`, device serials, or lab-only IPs. Use env placeholders.
