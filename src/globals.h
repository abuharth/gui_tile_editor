#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>

typedef struct Globals {
    Vector2 CameraPos;
    Vector2 MousePos;
    float scale;
    int selectedTile;
    int selectedAutoGroup;
    int currentLayer;
    int editingMode;
    int screenWidth;
    int screenHeight;
    Texture2D tx;
    Rectangle txUIRect;
} Globals;

extern Globals globals;

#endif // GLOBALS_H
