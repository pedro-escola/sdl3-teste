cmake_minimum_required(VERSION 3.10)
project(sdl3_teste)

# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" ON)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

# Create your game executable target as usual
if(WIN32)
    add_executable(sdl3_teste WIN32)

    add_custom_command(
        TARGET sdl3_teste POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:sdl3_teste>
        VERBATIM
    )
else()
    add_executable(sdl3_teste)
endif()

target_sources(sdl3_teste
  PRIVATE
    src/main.c
)

# Link to the actual SDL3 library.
target_link_libraries(sdl3_teste PRIVATE SDL3::SDL3)