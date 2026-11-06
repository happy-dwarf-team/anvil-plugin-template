# Security policy

This template gives an Anvil plugin its build, checks and release. A plugin is native code that runs inside the game process. Replace this file in your plugin, with its own address. Please report security problems privately, not in public issues.

## Reporting a vulnerability

Use GitHub's private vulnerability reporting:
[Report a vulnerability](https://github.com/happy-dwarf-team/anvil-plugin-template/security/advisories/new).
Include the version, how to reproduce the problem, and what an attacker gains.

This is a volunteer project; reports are handled on a best-effort basis. We
will confirm the report, agree on a fix and a disclosure date with you, and
credit you in the advisory unless you prefer otherwise.

## Supported versions

Only the latest release receives security fixes.

## In scope

- The workflows: anything that lets a pull request reach a secret or a published release.
- Example code a plugin author would copy with a memory safety bug in it.

## Out of scope

- Problems in Anvil itself: report them to
  [Anvil](https://github.com/happy-dwarf-team/anvil/security/advisories/new).
- Vulnerabilities in Europa Universalis IV itself; report those to Paradox
  Interactive.
