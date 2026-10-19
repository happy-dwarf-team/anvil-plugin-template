# Anvil plugin template

A starting point for an [Anvil](https://github.com/happy-dwarf-team/anvil) plugin for
Europa Universalis IV 1.37.5 (Steam, Windows x64), written in C++20 against plugin API 2.
It builds with the Anvil SDK's CMake package, checks the plugin in CI without the game
and releases it as a zip for `anvil/plugins/` and as a mod folder for the Workshop.

The example plugin hooks a function (the game-speed object), hooks the middle of another
one (the return to the main menu) and has one setting.

> **Before Anvil 1.0.** Plugin API 2 ships with Anvil 1.0, which is not released yet.
> Until then `CMakeLists.txt` builds the SDK from Anvil's sources at a pinned commit, and
> loading the plugin in the game needs an Anvil built from the same `main`
> ([its README](https://github.com/happy-dwarf-team/anvil#readme)). Anvil 0.2 loads only
> API 1 plugins.

## Your first plugin in 15 minutes

You need Windows x64, [Visual Studio 2022](https://visualstudio.microsoft.com/) or its
Build Tools with **Desktop development with C++** (it brings CMake 3.25 or newer), Git,
and EU4 1.37.5 with Anvil installed (`version.dll` next to `eu4.exe` and the `anvil/`
folder).

1. **Create your repository.** On GitHub, **Use this template → Create a new repository**,
   then clone it.

2. **Name your plugin.** Change the names below; the plugin id is lowercase letters,
   digits and `-`.

   | File | What to change |
   | --- | --- |
   | `plugin.toml` | `id`, `name`, `entry` (the DLL), `authors`, `description`, `issues`, `repository`, `affects_gameplay` |
   | `CMakeLists.txt` | `project(my_plugin …)` and the target in `anvil_add_plugin(my_plugin …)`; the `my_plugin-check` and `my_plugin-deploy` targets follow it |
   | `workshop/descriptor.mod` | `name` (the mod's name in the launcher) |
   | `LICENSE` | the copyright holder, or another license |

3. **Declare your symbols.** Every game address the plugin touches is a
   `[symbol.<name>]` in `symbols.toml`: its `rva`, the `bytes` expected there, the
   `memory` kind and, for a function, its C `signature`. The two symbols in the template
   are real for the Steam 1.37.5 `eu4.exe`. `anvil gen` turns them into
   `symbols.gen.hpp` at build time (`sym::speed_object`), so a hook whose function does
   not match the signature does not compile. See
   [design §5](https://github.com/happy-dwarf-team/anvil/blob/main/docs/design.md) for
   every key.

4. **Build.** Point `EU4_EXE` at the game's `eu4.exe` and `EU4_DIR` at its folder:

   ```
   cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DEU4_EXE="C:/Program Files (x86)/Steam/steamapps/common/Europa Universalis IV/eu4.exe" -DEU4_DIR="C:/Program Files (x86)/Steam/steamapps/common/Europa Universalis IV"
   cmake --build build --config Release
   ```

   The first configure fetches the SDK (before Anvil 1.0 it also builds `anvil.exe`,
   which takes a few minutes). The plugin lands in `build/plugin/my-plugin/`:
   `plugin.toml`, `symbols.toml`, `settings.toml` and `my_plugin.dll`.

5. **Check it against the game.**

   ```
   cmake --build build --config Release --target my_plugin-check
   ```

   `anvil check --exe` verifies every symbol against `eu4.exe` (build, bytes, memory,
   instruction boundaries) and rewrites `symbols.lock`. Commit `symbols.lock` with
   `symbols.toml`: CI has no game, so it fails when the lock does not match
   `symbols.toml` or says `result = "problems"`.

6. **Deploy.**

   ```
   cmake --build build --config Release --target my_plugin-deploy
   ```

   copies `build/plugin/my-plugin/` to `<EU4>/anvil/plugins/my-plugin/`. Close the game
   first: Windows does not let a loaded DLL be replaced.

7. **Run the game and read `anvil.log`.** Start EU4 as usual (Steam or the launcher)
   and open `<EU4>/anvil/anvil.log`. It lists your plugin with its status and its
   changes, then the plugin's own lines: `loaded on Anvil …`, the first game-speed
   object, and a line each time you return to the main menu. When a symbol does not
   verify, the log names the symbol and the byte, and the plugin is skipped while the
   game runs on.

Now replace `src/plugin.cpp` with your own plugin. `anvil.hpp` (the C++ wrapper) and
`anvil_plugin.h` (the C API underneath) document every host call.

## Settings

`settings.toml` declares what the player can change; `anvil gen` writes
`settings.gen.hpp`, so `cfg::log_menu_returns.get()` reads the current value. Anvil keeps
the player's values in `<EU4>/anvil/data/<id>/settings.toml`. Remove `settings` from
`plugin.toml` and the include from `src/plugin.cpp` when the plugin has none.

## CI and releases

`.github/workflows/build.yml` runs on every pull request and push to `main`:

- builds the plugin with warnings as errors;
- runs `anvil check . --format github`: the schemas of the three TOML files and the
  `symbols.lock` check, as annotations on the files;
- packages both release zips (below) and keeps them as the run's artifact.

`pr-title.yml` checks that pull request titles follow
[Conventional Commits](https://www.conventionalcommits.org/) (see
[CONTRIBUTING.md](CONTRIBUTING.md)); they become the changelog. `release.yml` runs
[release-please](https://github.com/googleapis/release-please): it keeps a release pull
request with the next version (in `version.txt` and `plugin.toml`) and `CHANGELOG.md`.
Merging it tags `vX.Y.Z` and attaches to the GitHub release:

| Asset | For |
| --- | --- |
| `<id>-<version>.zip` | players: unpack into `<EU4>/anvil/plugins/` |
| `<id>-<version>-workshop.zip` | you: unpack into `Documents/Paradox Interactive/Europa Universalis IV/mod/` and upload the mod with the Paradox launcher |
| `SHA256SUMS.txt` | the hashes; the zips also get a build attestation |

The Workshop mod holds only `descriptor.mod` and `anvil/<id>/`, which keeps the game's
checksum and ironman achievements. Anvil loads plugins from enabled mods from version 1.3
on, after the player approves each version
([design §14](https://github.com/happy-dwarf-team/anvil/blob/main/docs/design.md));
until then players install the plugin zip.

Repository settings that make this work (Settings on GitHub):

- **General → Pull Requests:** allow squash merging only, allow auto-merge,
  automatically delete head branches.
- **Rules → Rulesets:** a ruleset for the default branch that requires a pull request and
  the status checks `windows` and `conventional-title`, and blocks force pushes.
- **Actions → General → Workflow permissions:** allow GitHub Actions to create and approve
  pull requests (release-please opens its pull request with the workflow token).
- Optional secret `RELEASE_PLEASE_TOKEN` (a fine-grained token with contents and pull
  requests write): with it, CI also runs on the release pull request.

## Files

| Path | What |
| --- | --- |
| `plugin.toml` | the manifest: id, version, API, entry DLL, symbols, settings |
| `symbols.toml` | the game addresses the plugin uses, verified before the DLL loads |
| `symbols.lock` | the result of `anvil check --exe`; CI checks it is current |
| `settings.toml` | the player's settings |
| `src/plugin.cpp` | the plugin |
| `CMakeLists.txt` | fetches the Anvil SDK and calls `anvil_add_plugin` |
| `workshop/descriptor.mod` | the mod descriptor of the Workshop zip (its `version` comes from `plugin.toml`) |
| `.github/` | CI, pull request titles, releases, Dependabot for the pinned actions |

## License

[MIT](LICENSE). Change it for your plugin as you like.
