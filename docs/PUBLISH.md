# Project Nexus publish policy (public GitHub tree)

| Remote | Role | Push from **this** public tree |
|--------|------|--------------------------------|
| **github** | Public OSS (`github.com/Abyss-c0re/project-nexus`) | **yes** (default) |
| **company** | Private company GitLab | **never** |
| **lab-station** | Private lab SoT | lab only; **never document host/IP/SFTP paths in-tree** |

## Rules

1. **GitHub and company GitLab are separate remotes / separate policies.** Do not mirror secrets either way.
2. Public commits must **not** contain: company GitLab hostnames, LAN IPs, SFTP/SSH lab paths, personal work emails, or internal product URLs.
3. Lab topology lives only on lab machines (gitignored local config or out-of-tree notes).
4. Grokium is a **separate** public product when published — not a nested brand dump inside this monorepo without its own repo boundary.

## Public push only

```bash
git remote -v   # expect github only on clean public clones
git push github master
```

Never `git push` company or station remotes from an agent session unless Commander explicitly orders it **and** the tree is scrubbed.
