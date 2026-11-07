#ifndef MINDUSTRY3DS_BLOCK_H
#define MINDUSTRY3DS_BLOCK_H

#include <citro2d.h>
// Tool / Building types for placement
enum  ToolType
{
    TOOL_NONE = 0,
    TOOL_CONVEYOR = 1,
    TOOL_DRILL = 2,
    TOOL_WALL = 3,
    TOOL_CORE = 4,
    TOOL_DEMOLISH = 99
};

enum Category
{
    CAT_DISTRIBUTION,
    CAT_DEFENSE,
    CAT_POWER,
    CAT_PRODUCTION
};

// Global state variables
extern ToolType activeTool;
extern  enum Category activeCategory;

extern int copperCount;
extern int leadCount;


extern int tileCurrentHealth;
extern int tileMaxHealth;
extern  float healthPercent;
extern  float stored;
extern  float maxStored;
extern  unsigned int buildingColor;

// Draw vertical category tabs at X = 285
// Slot 0 (Distribution): Y = 90
// Slot 1 (Defense):      Y = 115
// Slot 2 (Power):        Y = 140
// Slot 3 (Production):   Y = 165


#endif //MINDUSTRY3DS_BLOCK_H
