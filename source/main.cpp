#include <3ds.h>
#include <citro2d.h>

int main(int argc, char* argv[]) {
    // Initialize services
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_TOP, NULL);

    C3D_RenderTarget* topTarget = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* botTarget = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Main program loop
    while (aptMainLoop()) {
        gspWaitForVBlank();
        gfxFlushBuffers();
        gfxSwapBuffers();

        // Your game update and draw logic here

        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; // Exit back to homebrew launcher on START

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // Top screen render
        C2D_SceneBegin(topTarget);
        C2D_TargetClear(topTarget, C2D_Color32(35, 35, 40, 255)); // Dark background

        // Bottom screen render
        C2D_SceneBegin(botTarget);
        C2D_TargetClear(botTarget, C2D_Color32(20, 20, 25, 255));

        C3D_FrameEnd(0);
        // OR if you want standard command flushing:
        // C3D_FrameEnd(C3D_DEFAULT_FLUSH_FLAG_CMD);
    }

    gfxExit();
    return 0;
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}
