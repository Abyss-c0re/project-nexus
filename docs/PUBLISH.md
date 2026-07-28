# Publish policy — two separate repositories

| Name | Host | Role | This public clone |
|------|------|------|-------------------|
| **ProjectNexus** | GitHub `Abyss-c0re/ProjectNexus` | Public OSS / Cube surface | **push yes** (`github`) |
| **project-nexus** | Company GitLab (private) | Company work only | **never push from here** |

They are **different repos** (different remotes, different history after scrub).  
Do **not** treat them as two remotes of one tree.

## Rules

1. Public clone remotes: **only** `github` → `https://github.com/Abyss-c0re/ProjectNexus.git`
2. Company work: **separate clone** of company `project-nexus` (GitLab) on a private path
3. Never document company GitLab hostnames, lab LAN/SFTP paths, or personal work emails in **ProjectNexus**
4. Move code between worlds with **reviewed patches**, not shared remotes / force-sync
5. Product code that has its own public repo (e.g. `nanobot`, future `grokium`) is **linked/pinned**, not forever vendored stale

## Public push

```bash
cd ~/Dev/project-nexus   # public working tree
git remote -v            # github → Abyss-c0re/ProjectNexus only
git push github master
```

## Company push

```bash
cd ~/Dev/project-nexus-company   # private clone of company project-nexus
git push origin master          # company remote only
```
