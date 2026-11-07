#include "ui.h"
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include  "map.h"
#include  "block.h"

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
                if (touch.py >= 285)
                {
                    if (touch.py >= 90 && touch.py < 215)
                    {
                        int tabIndex = (touch.py - 90) / 25;
                        if (tabIndex >= 0 && tabIndex <= 4)
                        {
                            activeCategory = (Category)tabIndex;
                        }
                    }
                }
                // Did we tap a Building slot in the grid?
                else if (touch.py >= 90 && touch.py < 200)
                {
                    int col = (touch.px - 215) / 22;
                    int row = (touch.py - 90) / 22;
                    // Select active building tool...
                    if (col >= 0 && col < 3 && row >= 0 && row < 5)
                    {
                        if (activeCategory == CAT_DISTRIBUTION)
                        {
                            if (row == 0 && col == 0) activeTool = TOOL_CONVEYOR;
                        }
                        else if (activeCategory == CAT_PRODUCTION)
                        {
                            if (row == 0 && col == 0) activeTool = TOOL_DRILL;
                        }
                        else if (activeCategory == CAT_DEFENSE)
                        {
                            if (row == 0 && col == 0) activeTool = TOOL_WALL;
                        }
                    }
                }
            }
            // 2. Otherwise, the touch is on the GAME MAP!
            else
            {

                int tx = touch.px / TILE_SIZE;
                int ty = touch.py / TILE_SIZE;

                if (tx >= 0 && tx < GRID_COLS && ty >= 0 && ty < GRID_ROWS)
                {
                    selectedTileX = tx;
                    selectedTileY = ty;

                    // Tap to place a Conveyor (Type 2) on empty tiles
                    if (kDown & KEY_TOUCH)
                    {
                        if (activeTool == TOOL_CONVEYOR && mapGrid[tx][ty] == 0)
                        {
                            mapGrid[tx][ty] = 2; // Conveyor
                        }
                        else if (activeTool == TOOL_DRILL && mapGrid[tx][ty] == 1)
                        {
                            mapGrid[tx][ty] = 4; // Drill on ore
                        }
                        else if (activeTool == TOOL_WALL && mapGrid[tx][ty] == 0)
                        {
                            mapGrid[tx][ty] = 5; // Wall
                        }
                        else if (activeTool == TOOL_DEMOLISH)
                        {
                            mapGrid[tx][ty] = 0; // Clear tile
                        }
                    }
                } else
                {
                    selectedTileX = -1;
                    selectedTileY = -1;
                }
            }
        }

        // --- RENDER FRAME ---
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // --- DRAW TOP SCREEN (Status & Info HUD) ---
        C2D_TargetClear(topTarget, C2D_Color32(30, 30, 35, 255));
        C2D_SceneBegin(topTarget);

        // Draw HUD panel background on top screen
        C2D_DrawRectSolid(10, 10, 0, 380, 50, C2D_Color32(45, 45, 55, 255));


        // Top Resource Indicators (Copper / Lead counters)
        // Copper box & text area preview
        C2D_DrawRectSolid(215, 15, 0, 42, 18, C2D_Color32(50, 50, 60, 255));
        // Lead box
        C2D_DrawRectSolid(263, 15, 0, 42, 18, C2D_Color32(50, 50, 60, 255));

        // Selected Tile Health & Storage Bars (Hidden until a tile is selected/tapped)
        if (selectedTileX != -1 && selectedTileY != -1)
        {
            C2D_DrawRectSolid(160, 20, 0, 90, 8, C2D_Color32(40, 20, 20, 255)); // Health bg
            C2D_DrawRectSolid(160, 20, 0, 90 * healthPercent, 8, C2D_Color32(230, 60, 60, 255)); // Health fill
            C2D_DrawRectSolid(160, 20, 0, 90, 8, C2D_Color32(20, 40, 20, 255)); // Storage bg
            C2D_DrawRectSolid(160, 20, 0, 90 * (stored / maxStored), 8, C2D_Color32(80, 200, 80, 255)); // Storage fill
        }

        // --- DRAW BOTTOM SCREEN (Interactive Gameplay Grid & UI Panel) ---
        C2D_TargetClear(botTarget, C2D_Color32(20, 20, 20, 255));
        C2D_SceneBegin(botTarget);

        // Draw map tiles
        drawMap();

        // UI Panel background (Right side: X >= 210)
        u32 panelBgColor = C2D_Color32(30, 30, 35, 235);
        u32 panelBorderColor = C2D_Color32(60, 60, 70, 235);
        C2D_DrawRectSolid(209, 9, 0, 102, 222, panelBorderColor); // Outer border
        C2D_DrawRectSolid(210, 10, 0, 100, 220, panelBgColor); // Inner fill

        // Top Resource Indicators in UI Panel
        C2D_DrawRectSolid(215, 15, 0, 42, 18, C2D_Color32(50, 50, 60, 255));
        C2D_DrawRectSolid(263, 15, 0, 42, 18, C2D_Color32(50, 50, 60, 255));

        // Category Tabs (Vertical buttons on the right edge: 5 long. X >= 285)
        for (int i = 0; i < 5; i++)
        {
            float tabY = 90 + (i * 22);
            u32 tabColor = (activeCategory == i) ? C2D_Color32(240, 200, 80, 255) :
            C2D_Color32(60, 60, 75, 255);
            C2D_DrawRectSolid(285, tabY, 0, 20, 20, tabColor);

            if (activeCategory == i)
            {
                C2D_DrawRectSolid(284, tabY, 0, 2, 20, C2D_Color32(255, 255, 255, 255));
            }
        }

        // Building Selection Slots (3x5 grid inside panel)
        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                float bx = 215 + (col * 22);
                float by = 90 + (row * 22);

                bool isSelected = false;
                if (activeCategory == CAT_DISTRIBUTION && row == 0 && col == 0 && activeTool == TOOL_CONVEYOR)
                    isSelected = true;
                if (activeCategory == CAT_PRODUCTION && row == 0 && col == 0 && activeTool == TOOL_CONVEYOR)
                    isSelected = true;
                if (activeCategory == CAT_DEFENSE && row == 0 && col == 0 && activeTool == TOOL_CONVEYOR)
                    isSelected = true;

                u32 slotColor = isSelected ? C2D_Color32(100, 220, 100, 255) : C2D_Color32(50, 50, 60, 255);

                // Building preview cube aligned to the bottom of the grey button
                C2D_DrawRectSolid(bx, by, 0, 20, 20, slotColor);
                C2D_DrawRectSolid(bx + 2, by + 6, 0, 16, 12, C2D_Color32(90, 90, 110, 255));
            }
        }
        C3D_FrameEnd(0);
    }
    // Deinitiaize graphics in reverse order
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}
