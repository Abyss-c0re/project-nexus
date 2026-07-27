# Security Policy

## Reporting a vulnerability

If you discover a security issue in Project Nexus:

1. **Do not** open a public issue with exploit details.
2. Contact the maintainers through the private security channel configured for this project (see `.agents/STATE.md` for the current contact).
3. Include reproduction steps, impact assessment, and whether personal data may be involved.

Active incidents (suspected breach, unauthorized access, data loss) must be reported **immediately**. Do not assume someone else already reported it.

## Baseline controls

Contributors and agents **must**:

- Use strong, unique credentials and multi-factor authentication wherever available
- Never commit secrets (tokens, private keys, `.env` with production values)
- Prefer least privilege for tokens and deploy keys
- Install software from approved or audited sources only
- Think before clicking: phishing remains the dominant breach path
- Keep dependencies and toolchains updated

## Social engineering & phishing

Warning signs:

1. Sender address does not match the claimed organization
2. Artificial urgency
3. Generic greetings
4. Poor grammar (still useful; not definitive with modern attacks)
5. Unexpected attachments
6. Password / “verify account” requests
7. Shortened or look-alike URLs
8. Missing signature / contact path

No maintainer will ask you to move support conversations to untrusted third-party chat apps for credential recovery.

## Generative AI tools

- **Grok auth, Grok API, and local session import are allowed** as operator-controlled interop. They do **not** make this project an official third-party product.
- Do not paste customer personal data or unapproved secrets into external models without a documented exception
- Do not use AI tools to harass, impersonate, or generate prohibited content on work systems
- Project code for partner-facing work must remain in official Project Nexus repositories
- Mesh/share path remains **State Matrix only** — no session prose off-box

## Devices & software

- Prefer inventory-approved packages
- Do not store confidential project data in unapproved cloud drives
- Report lost/stolen devices that had repository or partner access

## Insider threat awareness

Report concerning access patterns, unexpected solicitation for internal data, or scope-creep curiosity about systems outside a person’s role. Anonymous reporting may be available via partner compliance channels when operating under a B2B engagement.

## Scope hygiene (repository)

The automated watcher rejects:

- Large media dumps and mislabeled binaries
- Game / fan content unrelated to Project Nexus
- Hardcoded secrets and credential files
