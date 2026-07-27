# Project Nexus publish policy

| Remote | URL | Push |
|--------|-----|------|
| **github** | `github.com/Abyss-c0re/project-nexus` | **yes** (default) |
| **station** (SFTP/SSH) | `[lab-station-mirror redacted]` | **yes** (lab SoT mirror) |
| **gitlab** | `[company-gitlab redacted]` | **no** (do not push) |

Fetch from gitlab is optional; never `git push gitlab` unless Commander explicitly re-enables.
