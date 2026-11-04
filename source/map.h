// map.h

#ifndef MINDUSTRY3DS_MAP_H
#define MINDUSTRY3DS_MAP_H

#define GRID_COLS 20
#define GRID_ROWS 15
#define TILE_SIZE 16


// 0 = Empty/Dirt, 1 = Copper Ore, 2 = Conveyor, 3 = Core
extern int mapGrid[GRID_COLS][GRID_ROWS];
extern int selectedTileX;
extern int selectedTileY;

#define SCREEN_TOP_WIDTH 400
#define SCREEN_TOP_HEIGHT 240
#define SCREEN_BOT_WIDTH 320
#define SCREEN_BOT_HEIGHT 240

void initMap(); // Just the name and type. No { curly braces } here!
void drawMap(); // Just the name and type. No { curly braces } here!

#endif //MINDUSTRY3DS_MAP_H
