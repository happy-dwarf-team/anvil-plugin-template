cmake_minimum_required(VERSION 3.25)
project(my_plugin LANGUAGES CXX)

include(FetchContent)

# The Anvil SDK: the AnvilPlugin CMake package (anvil_add_plugin) and anvil.exe.
#
# TODO(happy-dwarf-team/anvil#57): no Anvil release carries the SDK archive yet.
# Until v1.0.0 is out, the SDK is built from Anvil's sources at a pinned commit
# of main (only anvil.exe and the headers are used; tests, examples and
# benchmarks are off). After the 1.0 release, replace this block with:
#
#   FetchContent_Declare(anvil_sdk
#       URL https://github.com/happy-dwarf-team/anvil/releases/download/v1.0.0/anvil-sdk-windows-x64.zip
#       URL_HASH SHA256=<hash of anvil-sdk-windows-x64.zip in the release's SHA256SUMS.txt>)
#   FetchContent_MakeAvailable(anvil_sdk)
#
# and drop enable_language(C) below and the second anvil.exe path in
# .github/workflows/build.yml.
#
# Anvil's tree enables C, and anvil_add_plugin then asks for C11 in this
# directory too, which needs C enabled here as well.
enable_language(C)
set(ANVIL_BUILD_TESTS OFF)
set(ANVIL_BUILD_EXAMPLES OFF)
set(ANVIL_BUILD_BENCH OFF)
FetchContent_Declare(anvil_sdk
    GIT_REPOSITORY https://github.com/happy-dwarf-team/anvil.git
    GIT_TAG ab8f7e699b408e94f117f2b8d5f89e19773d0114  # main, 2026-10-19
    EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(anvil_sdk)

# Reads plugin.toml, runs `anvil gen` on symbols.toml and settings.toml, builds
# the DLL and lays out build/plugin/<id>/. Adds my_plugin-check and, with
# -DEU4_DIR=<game folder>, my_plugin-deploy.
anvil_add_plugin(my_plugin SOURCES src/plugin.cpp)
