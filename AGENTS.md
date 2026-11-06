# Anvil plugin template: repository rules

- A starting point for Anvil plugins. People copy it, so what it shows is what they will do:
  keep the example small, and keep the workflows pinned and with the least permissions.
- [CONTRIBUTING.md](CONTRIBUTING.md) lists the title types, the required checks and
  `symbols.toml` with its lock. Run the build before finishing a change; CI runs it.
- English in code and docs. Do not imply affiliation with Paradox Interactive.
- The agent owns the pull request flow: it works on a branch with Conventional Commits,
  pushes it and opens the PR as a draft. Once ready (green CI, the change tested as above,
  no blocking review findings, not put on hold by the user) it marks the PR ready and
  squash-merges it; GitHub does not allow auto-merge on a draft. Pull requests from other
  contributors get a review and a recommendation; the maintainer decides on merging them.
  Do not bypass CI or branch protection, and do not change repository settings beyond what
  auto-merge needs.
