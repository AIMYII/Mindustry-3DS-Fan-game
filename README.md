# Mindustry3DS

Credit to Anuken / Mindustry for original assets and concept.

## How to Build

Requires Docker with the `devkitpro/devkitarm` image.

```bash
docker run --rm -v "$PWD":/project -w /project devkitpro/devkitarm cmake -B build -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/3DS.cmake
docker run --rm -v "$PWD":/project -w /project devkitpro/devkitarm cmake --build build
