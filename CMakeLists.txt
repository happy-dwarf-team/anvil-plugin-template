cmake_minimum_required(VERSION 3.25)
project(my_plugin LANGUAGES CXX)

include(FetchContent)

# The Anvil SDK: the AnvilPlugin CMake package (anvil_add_plugin) and anvil.exe,
# from the SDK archive of an Anvil release. URL_HASH is the archive's line in
# the release's SHA256SUMS.txt.
FetchContent_Declare(anvil_sdk
    URL https://github.com/happy-dwarf-team/anvil/releases/download/v1.6.0/anvil-sdk-windows-x64.zip
    URL_HASH SHA256=a17bef66de0be9d1832c837ec2509e3e0070b13eeb012a6ba0504b8a694bfad8)
FetchContent_MakeAvailable(anvil_sdk)

# Reads plugin.toml, runs `anvil gen` on symbols.toml and settings.toml, builds
# the DLL and lays out build/plugin/<id>/. Adds my_plugin-check and, with
# -DEU4_DIR=<game folder>, my_plugin-deploy.
anvil_add_plugin(my_plugin SOURCES src/plugin.cpp)
