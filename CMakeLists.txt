cmake_minimum_required(VERSION 3.13)

# Project Name
project(Mindustry3DS CXX C)

# Check for devkitPro environment variable
if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "Please set DEVKITPRO in your environment (usually /opt/devkitpro)")
endif()

set(DEVKITPRO $ENV{DEVKITPRO})

# Standard 3DS include paths
include_directories(${DEVKITPRO}/libctru/include)
include_directories(${DEVKITPRO}/libcitro3d/include)
include_directories(${DEVKITPRO}/libcitro2d/include)

# Standard 3DS library paths
link_directories(${DEVKITPRO}/libctru/lib)
link_directories(${DEVKITPRO}/libcitro3d/lib)
link_directories(${DEVKITPRO}/libcitro2d/lib)

# Compiler flags for 3DS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -fpermissive")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -fpermissive")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the executable
add_executable(Mindustry3DS source/main.cpp README.md .gitignore)

# Link the mandatory 3DS libraries
# Note: The order matters. m should be last.
target_link_libraries(Mindustry3DS PRIVATE citro2d citro3d ctru m)

# Automatically generate the .3dsx file after compilation
find_program(3DSXTOOL 3dsxtool HINTS ${DEVKITPRO}/tools/bin)

if(3DSXTOOL)
    add_custom_command(
        TARGET Mindustry3DS POST_BUILD
        COMMAND ${3DSXTOOL} $<TARGET_FILE:Mindustry3DS> ${CMAKE_CURRENT_BINARY_DIR}/Mindustry3DS.3dsx
        COMMENT "Packing compiled binary into .3dsx..."
    )
else()
    message(WARNING "3dsxtool not found! Ensure 3ds-dev is installed via dkp-pakman.")
endif()
