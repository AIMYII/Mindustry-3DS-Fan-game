#include <3ds.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    // Initialize graphics on the top screen
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    printf("Mindustry 3DS Initialized!\n");
    printf("Press START to exit back to the Homebrew.\n");

    // Main execution loop required by 3DS homebrew
    while (aptMainLoop())
    {
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break; // Breaks loop to exit app

        // Flush and swap video buffers, then wait for screen refresh
        gfxFlushBuffers();
        gfxFlushBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
