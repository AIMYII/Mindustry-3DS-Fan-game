#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

#include <map.cpp>
#include <ui.cpp>

int main(int argc, char* argv[]) {
    initMap();
    UI();


    // Deinitialize graphics in reverse order
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
