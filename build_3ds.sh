#!/bin/bash
set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

echo "=== Building Mindustry3DS for Nintendo 3DS ==="

USER_ID="${SUDO_UID:-$(id -u)}"
GROUP_ID="${SUDO_GID:-$(id -g)}"

# Check if Docker is available
if command -v docker &> /dev/null; then
    # Determine whether sudo is needed for docker
    if docker info >/dev/null 2>&1; then
        DOCKER_BIN="docker"
    else
        # Try starting service if stopped
        sudo service docker start >/dev/null 2>&1 || true
        DOCKER_BIN="sudo docker"
    fi

    echo "Running build inside devkitPro container..."
    $DOCKER_BIN run --rm -u "$USER_ID:$GROUP_ID" -v "$PROJECT_DIR":/project -w /project devkitpro/devkitarm cmake -B build -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/3DS.cmake
    $DOCKER_BIN run --rm -u "$USER_ID:$GROUP_ID" -v "$PROJECT_DIR":/project -w /project devkitpro/devkitarm cmake --build build
    echo ""
    echo "==============================================="
    echo "SUCCESS! Your 3DS binary is ready at:"
    echo "$PROJECT_DIR/build/Mindustry3DS.3dsx"
    echo "==============================================="
else
    echo "Running native CMake build with 3DS toolchain..."
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/3DS.cmake
    cmake --build build
    echo ""
    echo "==============================================="
    echo "SUCCESS! Your 3DS binary is ready at:"
    echo "$PROJECT_DIR/build/Mindustry3DS.3dsx"
    echo "==============================================="
fi
