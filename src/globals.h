#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>

#define PRINT_DEBUG(...) DrawText(TextFormat(__VA_ARGS__), 50, 50, 20, RED)

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
    Rectangle rectMouseHover;
} Globals;

extern Globals globals;

#endif // GLOBALS_H
