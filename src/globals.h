#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>

typedef struct Globals {
    Vector2 CameraPos;
    Vector2 MousePos;
    Rectangle TileTextureRect;
    float scale;
    int selectedTile;
    int editingMode;
    int screenWidth;
    int screenHeight;
} Globals;

extern Globals globals;

#endif // GLOBALS_H
