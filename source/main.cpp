#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

#include "map.h"
#include "ui.h"
#include  "block.h"

int main(int argc, char* argv[]) {
    initMap(); // The compiler knows this exists because of map.h Initialize map data

    UI();// Start UI loop and rendering

    // Deinitiaize graphics in reverse order
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
