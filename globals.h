#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>

typedef struct Globals {
    Vector2 CameraPos;
    Vector2 MousePos;
    Rectangle TileTextureRect;
    int zoom;
    int selectedTile;
    int editingMode;
} Globals;

extern Globals globals;

#endif // GLOBALS_H
