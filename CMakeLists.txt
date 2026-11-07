cmake_minimum_required(VERSION 3.13)

# Automatically handle 3DS toolchain if DEVKITPRO is present
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (DEFINED ENV{DEVKITPRO} AND EXISTS "$ENV{DEVKITPRO}/cmake/3DS.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/cmake/3DS.cmake" CACHE FILEPATH "3DS Toolchain" FORCE)
    endif ()
endif ()

# Project Name
project(Mindustry3DS CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the executable
add_executable(Mindustry3DS source/main.cpp
        source/ui.cpp
        source/ui.h
        source/map.cpp
        source/map.h
        source/block.cpp
        source/block.h
        source/tile.cpp
        source/tile.h
)

# Link the mandatory 3DS libraries
# Note: The order matters. m should be last.
target_link_libraries(Mindustry3DS citro2d citro3d ctru m)

# Automatically generate the .3dsx file after compilation
find_program(3DSXTOOL 3dsxtool HINTS ${DEVKITPRO}/tools/bin)

if (3DSXTOOL)
    add_custom_command(
            TARGET Mindustry3DS POST_BUILD
            COMMAND ${3DSXTOOL} $<TARGET_FILE:Mindustry3DS> ${CMAKE_CURRENT_BINARY_DIR}/Mindustry3DS.3dsx
            COMMENT "Packing compiled binary into .3dsx..."
    )
else ()
    message(WARNING "3dsxtool not found! Ensure 3ds-dev is installed via dkp-pacman.")
endif ()
