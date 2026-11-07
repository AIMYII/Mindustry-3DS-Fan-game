#include "block.h"

ToolType activeTool = TOOL_CONVEYOR;
Category activeCategory = CAT_DISTRIBUTION;

int copperCount = 150;
int leadCount = 100;

int tileCurrentHealth = 85;
int tileMaxHealth = 100;
float healthPercent = 0.85f;
float stored = 50.0f;
float maxStored = 100.0f;
unsigned int buildingColor = C2D_Color32(90, 190, 90, 255);
