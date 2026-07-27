# B2B Partner Compliance

Project Nexus is developed and operated as a **professional open source project** suitable for B2B engagements. Maintainers, contractors, and automated agents must comply with these requirements when work touches partner environments, customer data, or contractual obligations.

> This document captures **operational obligations** derived from mandatory security and privacy training for partner work. It does **not** create product affiliation, co-branding, or endorsement with any third-party AI vendor.

## Program posture

- Comply with applicable laws, regulations, and **commercial agreements**
- Honor public and contractual privacy commitments
- Prefer controls that survive audit (logs, reviews, least privilege)

## Personal data handling

- Classify data before use; assume personal data if identity linkage is plausible
- Processing includes storage and backups — “just sitting there” still counts
- Public identifiers remain personal data

## Enterprise / regulated data

When an engagement may involve regulated categories (for example **PHI under HIPAA** as a business associate scenario):

- Prefer **zero data retention (ZDR)** for regulated payloads
- Do not store or browse regulated content outside the approved environment
- Use or disclose only as permitted by the controlling agreement (e.g. BAA)
- Implement appropriate safeguards if regulated data is ever received
- Report incidents on contractual timelines
- Flow down requirements to subcontractors
- Return or destroy regulated data at engagement end
- Non-ZDR use requires legal review **before** implementation

## Regulatory examples (industry context)

Large platforms have faced multi-million-dollar enforcement for misusing contact data collected for security/auth purposes. EU GDPR fines for industry peers have exceeded €1B. Treat purpose limitation and user transparency as non-negotiable.

## Public statements

External statements about **privacy and security practices** must be accurate. Do not invent certifications, retention promises, or “we never store X” claims without review.

## Source code & IP

- Keep Project Nexus work in official project remotes (not personal dump repos)
- Use organizational SSO / project identity for git where required
- Prefer VPN or equivalent secure path when policy demands it for partner systems

## Travel & physical security (partner environments)

When traveling with access to partner systems:

1. Report international travel when required by the engagement
2. Check destination risk level
3. Follow device and access restrictions for high-risk locations
4. Complete any required briefings
5. Remove work accounts from personal devices when instructed

## Workplace safety

Threats or violence are not tolerated. Report safety concerns promptly. In immediate danger, contact local emergency services first.

## Agent obligations

AI agents operating in this repository must:

- Read `.agents/AGENTS.md` and `.agents/STATE.md` before acting
- Refuse to introduce out-of-scope entertainment content
- Avoid writing secrets into the tree
- Log material decisions to `.agents/memory/`
- Leave a handoff when finishing a session
