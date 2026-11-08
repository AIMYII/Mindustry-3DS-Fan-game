#include "ui.h"
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include "map.h"
#include "block.h"

// Scroll state for building options and categories
static int buildScrollOffset = 0;
static int catScrollOffset = 0;
#define MAX_BUILD_SCROLL 2
#define MAX_CAT_SCROLL 2

void UI()
{
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* topTarget = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* botTarget = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START) break;

        // D-Pad scrolling
        if (kDown & KEY_UP) {
            if (buildScrollOffset > 0) buildScrollOffset--;
        }
        if (kDown & KEY_DOWN) {
            if (buildScrollOffset < MAX_BUILD_SCROLL) buildScrollOffset++;
        }

        if (kHeld & KEY_TOUCH)
        {
            touchPosition touch;
            hidTouchRead(&touch);

            // Panel Area (X: 180 to 288, Y: 100 to 240)
            if (touch.px >= 180 && touch.px <= 288 && touch.py >= 100)
            {
                // Scrollbar touch interaction for building options
                if (touch.px >= 268 && touch.px <= 278 && touch.py >= 110 && touch.py <= 190)
                {
                    if (touch.py < 150) {
                        if (buildScrollOffset > 0) buildScrollOffset--;
                    } else {
                        if (buildScrollOffset < MAX_BUILD_SCROLL) buildScrollOffset++;
                    }
                }
                // Building options grid (10 buttons: 5 columns x 2 rows visible at a time)
                else if (touch.py >= 105 && touch.py <= 195)
                {
                    int col = (touch.px - 186) / 16;
                    int row = ((touch.py - 105) / 22) + buildScrollOffset;

                    if (col >= 0 && col < 5 && row >= 0 && row < (2 + MAX_BUILD_SCROLL))
                    {
                        if (activeCategory == CAT_DISTRIBUTION && row == 0 && col == 0) activeTool = TOOL_CONVEYOR;
                        else if (activeCategory == CAT_PRODUCTION && row == 0 && col == 0) activeTool = TOOL_DRILL;
                        else if (activeCategory == CAT_DEFENSE && row == 0 && col == 0) activeTool = TOOL_WALL;
                    }
                }
            }
            // Game Map interaction
            else 
            {
                int tx = touch.px / TILE_SIZE;
                int ty = touch.py / TILE_SIZE;

                if (tx >= 0 && tx < GRID_COLS && ty >= 0 && ty < GRID_ROWS)
                {
                    selectedTileX = tx;
                    selectedTileY = ty;

                    if (kDown & KEY_TOUCH)
                    {
                        if (activeTool == TOOL_CONVEYOR && mapGrid[tx][ty] == 0) mapGrid[tx][ty] = 2;
                        else if (activeTool == TOOL_DRILL && mapGrid[tx][ty] == 1) mapGrid[tx][ty] = 4;
                        else if (activeTool == TOOL_WALL && mapGrid[tx][ty] == 0) mapGrid[tx][ty] = 5;
                        else if (activeTool == TOOL_DEMOLISH) mapGrid[tx][ty] = 0;
                    }
                }
                else 
                {
                    selectedTileX = -1;
                    selectedTileY = -1;
                }
            }
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // --- TOP SCREEN ---
        C2D_TargetClear(topTarget, C2D_Color32(30, 30, 35, 255));
        C2D_SceneBegin(topTarget);

        C2D_DrawRectSolid(10, 10, 0, 380, 50, C2D_Color32(45, 45, 55, 255));
        C2D_DrawRectSolid(20, 20, 0, 60, 25, C2D_Color32(50, 50, 60, 255));
        C2D_DrawRectSolid(90, 20, 0, 60, 25, C2D_Color32(50, 50, 60, 255));

        if (selectedTileX != -1 && selectedTileY != -1)
        {
            C2D_DrawRectSolid(160, 20, 0, 90, 8, C2D_Color32(40, 20, 20, 255));
            C2D_DrawRectSolid(160, 20, 0, 90 * healthPercent, 8, C2D_Color32(230, 60, 60, 255));

            C2D_DrawRectSolid(160, 32, 0, 90, 8, C2D_Color32(20, 40, 20, 255));
            C2D_DrawRectSolid(160, 32, 0, 90 * (stored / maxStored), 8, C2D_Color32(80, 200, 80, 255));
        }

        // --- BOTTOM SCREEN ---
        C2D_TargetClear(botTarget, C2D_Color32(20, 20, 20, 255));
        C2D_SceneBegin(botTarget);
        
        drawMap();

        // Build Menu Panel at bottom-right (X: 180 to 288, Y: 100 to 240 -> width 108)
        C2D_DrawRectSolid(180, 100, 0, 108, 140, C2D_Color32(30, 30, 35, 255));
        
        // Borders
        C2D_DrawRectSolid(180, 100, 0, 108, 2, C2D_Color32(20, 20, 22, 255)); // Top
        C2D_DrawRectSolid(286, 100, 0, 2, 140, C2D_Color32(20, 20, 22, 255)); // Right
        C2D_DrawRectSolid(180, 238, 0, 108, 2, C2D_Color32(20, 20, 22, 255)); // Bottom
        C2D_DrawRectSolid(180, 100, 0, 2, 140, C2D_Color32(20, 20, 22, 255)); // Left

        // Building Options Grid (10 grey buttons: 5 columns x 2 rows visible square grid)
        for (int row = 0; row < 2; row++)
        {
            int dataRow = row + buildScrollOffset;
            for (int col = 0; col < 5; col++)
            {
                float bx = 186 + (col * 16);
                float by = 110 + (row * 22);

                bool isSelected = (col == 0 && dataRow == 0 && activeTool != TOOL_NONE);
                u32 slotColor = isSelected ? C2D_Color32(100, 220, 100, 255) : C2D_Color32(50, 50, 60, 255);

                C2D_DrawRectSolid(bx, by, 0, 14, 20, slotColor);
                C2D_DrawRectSolid(bx + 2, by + 3, 0, 10, 14, C2D_Color32(90, 90, 110, 255));
            }
        }

        // Working Vertical Scrollbar for Building Options
        float scrollX = 270;
        float scrollY = 110;
        float scrollH = 80;
        C2D_DrawRectSolid(scrollX, scrollY, 0, 6, scrollH, C2D_Color32(40, 40, 45, 255));
        float thumbH = scrollH / (1.0f + MAX_BUILD_SCROLL);
        float thumbY = scrollY + (scrollH - thumbH) * ((float)buildScrollOffset / MAX_BUILD_SCROLL);
        C2D_DrawRectSolid(scrollX, thumbY, 0, 6, thumbH, C2D_Color32(100, 100, 115, 255));

        // Bottom Action Bar (Clipboard icons inside the square panel)
        C2D_DrawRectSolid(182, 195, 0, 102, 40, C2D_Color32(25, 25, 28, 255));
        C2D_DrawRectSolid(190, 202, 0, 26, 26, C2D_Color32(45, 45, 52, 255));
        C2D_DrawRectSolid(222, 202, 0, 26, 26, C2D_Color32(45, 45, 52, 255));

        C3D_FrameEnd(0);
    }

    C2D_Fini();
    C3D_Fini();
    gfxExit();
}
