# Product sources (SoT)

**ProjectNexus does not vendor product source dumps.**  
Each product has its own version control. This monorepo is an **ecosystem index**.

| Product | Public SoT | Lab path |
|---------|------------|----------|
| **nanobot** | https://github.com/Abyss-c0re/nanobot | `~/Dev/AI/nanobot` |
| **grokium** | https://github.com/Abyss-c0re/grokium | `~/Dev/grokium` |
| **braincube** | https://github.com/Abyss-c0re/braincube (private) | `~/Dev/AI/braincube` |
| **clanker** | TBD public | `~/Dev/Clanker` |
| **titanus2** | not public monorepo | `~/Dev/device-workshop/products/titanus2` |
| **nanobot-wrapper** | https://github.com/Abyss-c0re/nanobot-wrapper | `~/Dev/AI/nanobot-wrapper` |

Machine-readable pins: [`MANIFEST.toml`](MANIFEST.toml).

## How to version-control the ecosystem

```text
                    ┌──────────────────────────┐
                    │  ProjectNexus (GitHub)   │
                    │  index + law + pins only │
                    └────────────┬─────────────┘
           pins (tag/commit)     │
     ┌───────────┬───────────────┼───────────────┐
     ▼           ▼               ▼               ▼
  nanobot     grokium        clanker*        titanus2*
  own repo    own repo       own when ready  lab product
```

1. **Develop** in each product’s own git root under `~/Dev/...`
2. **Tag/release** on that product’s GitHub (e.g. nanobot `v0.5.1`)
3. **Bump pin** in `products/MANIFEST.toml` + `products/<name>/UPSTREAM_PIN.txt`
4. **Push ProjectNexus** — only index/docs change, not megabytes of stale copies
5. **Company** `project-nexus` (GitLab) is a **separate clone** — never mixed remotes

\* not required to be public
