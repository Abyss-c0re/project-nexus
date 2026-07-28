# ProjectNexus · CubeOS ecosystem index

**Public GitHub monorepo for the hive map** — law, docs, and **version pins** to real product repos.

**Not affiliated** with third-party AI labs. Grok auth/API interop allowed with *your* credentials; no official-product claims.

| Repo | Role |
|------|------|
| **This** · [Abyss-c0re/ProjectNexus](https://github.com/Abyss-c0re/ProjectNexus) | Public ecosystem index |
| Company **project-nexus** (GitLab) | Private — **separate clone**, separate history |
| [Abyss-c0re/nanobot](https://github.com/Abyss-c0re/nanobot) | C peer binary (SoT) |
| [Abyss-c0re/grokium](https://github.com/Abyss-c0re/grokium) | Agent harness (SoT) |

## Why not one giant monorepo of code?

An ecosystem is versioned as **many product git roots + one index**, not stale copies of every tree.  
Pins live in [`products/MANIFEST.toml`](products/MANIFEST.toml).

## Quick start

```bash
# Product that actually runs:
git clone https://github.com/Abyss-c0re/nanobot.git && cd nanobot && make host

# Harness:
git clone https://github.com/Abyss-c0re/grokium.git

# This index only:
git clone https://github.com/Abyss-c0re/ProjectNexus.git
```

## Tree

```
docs/           Cube law, publish policy, ecosystem
products/       PIN packages only (README + UPSTREAM_PIN)
cores/          Public notes (not full Neural* dumps)
.agents/        Public agent ops plane (no secrets)
```

## Publish

See [`docs/PUBLISH.md`](docs/PUBLISH.md): push **only** `github` → ProjectNexus.  
Never document company/lab hostpaths in this tree.

## License

Per-product (MIT nanobot · Apache-2.0 grokium · see product repos).
