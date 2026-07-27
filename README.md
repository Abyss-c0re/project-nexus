# Project Nexus

**Project Nexus** is an open-source platform for structured trace analysis, secure collaboration, and privacy-preserving data workflows.

This repository is maintained as a **professional B2B-ready open source project**: clean history, clear docs, security-first defaults, and no product-unrelated material.

## Principles

| Pillar | Commitment |
|--------|------------|
| **Privacy by design** | Personal data is minimized, purpose-bound, and protected end-to-end |
| **Security baseline** | MFA-friendly ops, least privilege, no secrets in tree, audit-friendly process |
| **Professional hygiene** | No game/fan content, no large media dumps, no mixed personal projects |
| **Agent + human collab** | Continuous manager process under `.agents/` with durable handoff state |

## Documentation

- [Security policy](docs/SECURITY.md)
- [Privacy & data protection](docs/PRIVACY.md)
- [B2B partner compliance](docs/COMPLIANCE.md)
- [Code of conduct](docs/CODE_OF_CONDUCT.md)
- [Contributing](docs/CONTRIBUTING.md)

## Repository layout

```
docs/           Public product & compliance documentation
.agents/        Hidden ops plane for humans and AI agents (handoff, manager, watcher)
```

Agent operating rules live in [`.agents/AGENTS.md`](.agents/AGENTS.md).  
Current collaboration state: [`.agents/STATE.md`](.agents/STATE.md).

## Status

Early foundation. Security and privacy controls are documented first; implementation follows the same standards.

## License

TBD — license selection tracked in `.agents/STATE.md`.
