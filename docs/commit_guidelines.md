# Commit Message Guidelines

## Goals

This guidance applies to both human and AI contributors.

- Improve clarity when browsing history
- Make `git bisect` easier by keeping commits focused and well-described
- Enable automated release notes / changelog generation (when desired)

## Message format

Each commit message consists of:

- Header (required)
- Body (optional, recommended for non-trivial changes)
- Footer (optional; required for breaking changes, recommended for issue references)

### Template

```text
<type>(<scope>): <subject>

<body>

<footer>
```

### Scope (strongly suggested)

Scope goes in parentheses after the type and identifies *where* the change
happened (subsystem, module, feature area, etc.).

- Scope is **strongly suggested** and should be treated as the default.
- Omit scope only with a good reason (e.g., truly cross-cutting change where a
  single scope would be misleading).
- Parentheses are optional when omitting scope.

Examples:

- `feat(cli): add --verbose flag`
- `fix(auth): handle expired session tokens`
- `docs(readme): document environment variables`
- `test(storage): add coverage for eviction edge cases`

Omit scope only when a single scope would be misleading:

- `chore: bump dependencies`

## Line length

- Subject line: aim for 50 characters or less
- Body and footer lines: wrap at 72 characters or less

This keeps messages readable in terminals, email-style views, and many Git tools.

## Header fields

### Type

Use one of the following types (extend only if your project truly needs it):

- feat: new feature (user-facing behavior)
- fix: bug fix (user-facing behavior)
- docs: documentation-only change
- style: formatting only (whitespace, linting, no behavior change)
- refactor: code change that is neither feature nor fix
- perf: performance improvement
- test: add or update tests
- build: build system, tooling, dependency changes
- ci: CI configuration/scripts changes
- chore: maintenance tasks that don’t fit above
- revert: revert a prior commit

If you generate release notes automatically, commonly only `feat` and `fix` are
included—document that in your release process if applicable.

### Subject

The subject is a concise description of what changed.

Rules:

- Use imperative, present tense: "add", "fix", "change", "remove"
- Do not end with a period
- Keep it short, clear, and specific

Practical workflow (recommended):

1) Summarize the slice in 1-2 bullets of what changed (not why).
2) Extract the top-level themes from those bullets.
3) Write a subject that covers all themes (use an umbrella phrase if needed).
4) If the subject cannot cover all themes without becoming vague, split the
   slice or ask for guidance before committing.

Examples:

- `fix(parser): handle empty config file`
- `feat(api): add export command`

Avoid:

- `fix: stuff`
- `update: changes`

## Body

Use the body to explain why the change was made and what it affects.

Guidelines:

- Wrap at 72 characters
- Use imperative, present tense
- Include motivation, constraints, and behavior changes
- For complex changes, consider bullets

Example:

```text
refactor(storage): simplify session refresh logic

Avoid duplicate refresh paths by consolidating checks into one flow.
This reduces edge-case divergence and makes testing easier.
```

## Footer

Use the footer for metadata that helps tooling and collaboration.

### Breaking changes (required when applicable)

If a change is not backward compatible, include a footer entry starting with:

```text
BREAKING CHANGE: <description>
```

Include:

- What changed
- Why it changed (briefly)
- How to migrate (steps or example)

Example:

```text
feat(api): change default pagination

BREAKING CHANGE: default page size is now 50 instead of 25.
Migration: clients that assumed 25 should request ?limit=25.
```

### Referencing issues and tickets

When a commit closes or references work items, list them in the footer, one per
line. Use formats appropriate for your tracker.

GitHub-style examples:

```text
Closes #123
Refs #456
```

Guidance:

- Use `Closes` when it should close the issue automatically.
- Use `Refs` when related but not closing.

## Merge and history policy

- Merge commits are allowed.
- In shared branches, merges are typically squashed so history remains linear
  and each change lands as one coherent commit message.

## Recommendations

- Prefer one logical change per commit
- Do not mix refactors with behavior changes unless necessary
- Choose the type based on intent:
  - user-visible behavior: `feat` / `fix`
  - internal restructure: `refactor`
  - formatting-only: `style`
  - build/automation: `build` / `ci` / `chore`

## Examples

```text
feat(cli): add config validation
```

```text
fix(parser): prevent crash on empty input
```

```text
docs(README): document environment variables
```

```text
test(storage): add eviction edge-case coverage
```

```text
refactor(errors): centralize error handling

Remove duplicated error mapping logic across entry points.
```

```text
feat(api): change default pagination

BREAKING CHANGE: default page size is now 50 instead of 25.
Migration: clients that assumed 25 should request ?limit=25.

Closes #217
```

```text
ci(github): run unit tests on pull requests

Closes #123
```

Rare (scope omitted only when a single scope would be misleading):

```text
chore: bump dependencies
```
