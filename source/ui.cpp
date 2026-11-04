#include "ui.h"

#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

#include  "ui.h"
#include  "map.h" // Including this gives UI access to mapGrid and selectedTileX/Y

void UI()
{

    // Initialize services
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* topTarget = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* botTarget = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Main program loop
    while (aptMainLoop())
    {
        // Handle Hardware Controls
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START) break; // Exit back to homebrew launcher on START

        // Handle Stylus Touch Input on Bottom Screen
        if (kHeld & KEY_TOUCH)
        {
            touchPosition touch;
            hidTouchRead(&touch);

            // 1. Did the touch land inside the UI panel? (X >= 210)
            if (touch.px >= 210)
            {
                // Did we tap a Category tab on the right? (X >= 285)
                if (touch.px >= 285)
                {
                    int tabIndex = (touch.py - 90) / 25;
                    // Switch activeCategory...
                }
                // Did we tap a Building slot in the grid?
                else if (touch.py >= 90 && touch.py <= 160)
                {
                    int col = (touch.px - 215) / 22;
                    int row = (touch.py - 90) / 22;
                    // Select active building tool...
                }
            }
            // 2. Otherwise, the touch is on the GAME MAP!
            else {

                int tx = touch.px / TILE_SIZE;
                int ty = touch.py / TILE_SIZE;

                // Inspect the tapped tile for the top info box
                selectedTileX = tx;
                selectedTileY = ty;

                // Place selected building...

                if (tx >= 0 && tx < GRID_COLS && ty >= 0 && ty < GRID_ROWS)
                {
                    selectedTileX = tx;
                    selectedTileY = ty;

                    // Tap to place a Conveyor (Type 2) on empty tiles
                    if ((kDown & KEY_TOUCH) && mapGrid[tx][ty] == 0) {
                        mapGrid[tx][ty] = 2;
                    }
                } else {
                    selectedTileX = -1;
                    selectedTileY = -1;
                }
            }
        }

        // --- RENDER FRAME ---
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // --- DRAW TOP SCREEN (Status & Info) ---
        C2D_TargetClear(topTarget, C2D_Color32(30, 30, 35, 255));
        C2D_SceneBegin(topTarget);

        // Draw HUD panel background on top
        C2D_DrawRectSolid(10, 10, 0, 380, 50, C2D_Color32(45, 45, 55, 255));

        // --- DRAW BOTTOM SCREEN (Interactive Gameplay Grid) ---
        C2D_TargetClear(botTarget, C2D_Color32(20, 20, 20, 255));
        C2D_SceneBegin(botTarget);
        drawMap();

        // Panel background (dark grey with slight transparency)
        u32 panelBgColor = C2D_Color32(30, 30, 35, 235);
        u32 panelBorderColor = C2D_Color32(60, 60, 70, 235);

        // Outer border
        C2D_DrawRectSolid(209, 9, 0, 102, 222, panelBorderColor);
        // Inner fill
        C2D_DrawRectSolid(210, 10, 0, 100, 220, panelBgColor);

        // Example: Health Bar at (X: 215, Y: 40, Width: 90, Height: 8)
        float healthPercent = (float)tileCurrentHealth / tileMaxHealth; // e.g. 0.85

        // 1. Dark background bar
        C2D_DrawRectSolid(215, 40, 0, 90, 8, C2D_Color32(40, 20, 20, 255));

        // 2. Bright red filled bar scaled by health percentage
        C2D_DrawRectSolid(215, 40, 0, 90 * healthPercent, 8, C2D_Color32(230, 60, 60, 255));

        // Example: Resource / Stored Bar (Orange or Green)
        C2D_DrawRectSolid(215, 52, 0, 90, 8, C2D_Color32(20, 40, 20, 255)); // Background
        C2D_DrawRectSolid(215, 52, 0, 90 * (stored / maxstored), 8, C2D_Color32(80, 200, 80,
            255)); // Fill

        if (activeCategory == CAT_DEFENSE)
        {
            // Draw gold accent border around this category button
            C2D_DrawRectSolid(284, 114, 0, 22, 22, C2D_Color32(240, 200, 80, 255));
        }

        // Slot spacing: 20px per item
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                float bx = 215 + (col * 22);
                float by = 90 + (row * 22);

                // Draw slot background
                C2D_DrawRectSolid(bx, by, 0, 20, 20, C2D_Color32(50, 50, 60, 255));

                // Draw building color preview inside
                C2D_DrawRectSolid(bx + 2, by + 2, 0, 16, 16, buildingColor);
            }
        }
        C3D_FrameEnd(0);
    }
}
