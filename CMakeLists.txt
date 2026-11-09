cmake_minimum_required(VERSION 3.25)
project(my_plugin LANGUAGES CXX)

if(MSVC)
    # Compile a target's sources on every core; the Visual Studio generator runs cl one file at a time.
    add_compile_options(/MP)
endif()

include(FetchContent)

# The Anvil SDK: the AnvilPlugin CMake package (anvil_add_plugin) and anvil.exe,
# from the SDK archive of an Anvil release. URL_HASH is the archive's line in
# the release's SHA256SUMS.txt.
FetchContent_Declare(anvil_sdk
    URL https://github.com/happy-dwarf-team/anvil/releases/download/v1.13.0/anvil-sdk-windows-x64.zip
    URL_HASH SHA256=99a8517fcfd8a00b9579206c53c222c3edc3fcaabb2654cba6d6b700399eea33)
FetchContent_MakeAvailable(anvil_sdk)

# Reads plugin.toml, runs `anvil gen` on symbols.toml and settings.toml, builds
# the DLL and lays out build/plugin/<id>/. Adds <project>-check and, with
# -DEU4_DIR=<game folder>, <project>-deploy.
anvil_add_plugin(${PROJECT_NAME} SOURCES src/plugin.cpp)

# The licence asks to go with every copy: the release zip and the Workshop mod are
# this folder.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/LICENSE")
