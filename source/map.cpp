#include "map.h"


#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

#define GRID_COLS 20
#define GRID_ROWS 15
#define TILE_SIZE 16


// 0 = Empty/Dirt, 1 = Copper Ore, 2 = Conveyor, 3 = Core
int mapGrid[GRID_COLS][GRID_ROWS];
int selectedTileX = -1;
int selectedTileY = -1;

void initMap() {
    for (int x = 0; x < GRID_COLS; x++) {
        for(int y = 0; y < GRID_ROWS; y++) {
            // Drop a small copper ore deposit near center
            if ((x >= 8 && x <= 10) && (y >= 6 && y <= 8)) {
                mapGrid[x][y] = 1;
            } else {
                mapGrid[x][y] = 0;
            }
        }
    }
    // Place starting Core
    mapGrid[4][6] = 3;


    // Draw Map Tiles
    for (int x = 0; x < GRID_COLS; x++) {
        for (int y = 0; y < GRID_ROWS; y++) {
            float px = (float)(x * TILE_SIZE);
            float py = (float)(y * TILE_SIZE);

            u32 color = C2D_Color32(40, 40, 40, 255); // Default ground

            if (mapGrid[x][y] == 1)      color = C2D_Color32(217, 145, 87, 255); // Copper Ore (Bronze)
            else if (mapGrid[x][y] == 2) color = C2D_Color32(90, 190, 90, 255);  // Conveyor (Green)
            else if (mapGrid[x][y] == 3) color = C2D_Color32(80, 130, 220, 255); // Core (Blue)

            // Fill Tile
            C2D_DrawRectSolid(px, py, 0, TILE_SIZE - 1, TILE_SIZE - 1, color);
        }
    }

    // Highlight Currently Touched Tile
    if (selectedTileX != -1 && selectedTileY != -1) {
        float hx = (float)(selectedTileX * TILE_SIZE);
        float hy = (float)(selectedTileY * TILE_SIZE);
        C2D_DrawRectSolid(hx, hy, 0, TILE_SIZE - 1, TILE_SIZE - 1, C2D_Color32(255, 255, 255, 120));
    }
}

enum Category { CAT_DISTRIBUTION, CAT_DEFENSE, CAT_POWER, CAT_PRODUCTION };
Category activeCategory = CAT_DISTRIBUTION;

// Draw vertical category tabs at X = 285
// Slot 0 (Distribution): Y = 90
// Slot 1 (Defense):      Y = 115
// Slot 2 (Power):        Y = 140
// Slot 3 (Production):   Y = 165
