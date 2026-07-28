# Ecosystem version control

## Law

| Layer | Git | Updates |
|-------|-----|---------|
| Product (nanobot, grokium, …) | **Own repo** | Tag releases (`vX.Y.Z`) |
| Public index | **ProjectNexus** | Bump pins only |
| Company | **project-nexus** (GitLab) | Separate clone |
| Lab station | `~/Projects/ProjectNexus` filesystem + symlinks | Not the public git dump |

## Dev machine map

| Product | Lab path | Public SoT |
|---------|----------|------------|
| nanobot | `~/Dev/AI/nanobot` | github.com/Abyss-c0re/nanobot |
| grokium | `~/Dev/grokium` | github.com/Abyss-c0re/grokium |
| braincube | `~/Dev/AI/braincube` | github.com/Abyss-c0re/braincube (private) |
| titanus2 | `~/Dev/device-workshop/products/titanus2` | not public monorepo |
| Clanker | `~/Dev/Clanker` | TBD |

Station `~/Projects/ProjectNexus/products/*` should be **symlinks** to lab trees (already true for nanobot/titanus2), not copies.

## Release flow

1. Land fix in product repo → test → tag → push product GitHub  
2. Edit `products/MANIFEST.toml` pin in ProjectNexus  
3. `git push github master` on ProjectNexus only  

## Forbidden

- Vendoring full nanobot/grokium trees into ProjectNexus  
- Dual remotes (github+gitlab) on one working tree  
- Company emails / lab SFTP in public history  
