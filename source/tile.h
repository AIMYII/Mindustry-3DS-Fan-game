#ifndef MINDUSTRY3DS_TILE_H
#define MINDUSTRY3DS_TILE_H


struct Tile
{
    int floorType; // e.g., 0 = Grass/Dirt, 1 = Metal Floor
    int oreType; // e.g., 0 = None, 1 = Copper, 2 = Lead
    int buildingType; // e.g., 0 = None, 1 = Conveyor, 2 = Mechanical Drill, 3 = Core
    int health; // Building health
    int rotation; // Direction conveyor/drill is facing (0=Up, 1=Right, 2=Down, 3=Left)
};


#endif //MINDUSTRY3DS_TILE_H
