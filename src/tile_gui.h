#include "raylib.h"

#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_TILEGUI_H
#define GUI_TILEGUI_H

typedef struct {
    int screenWidth;
    int screenHeight;

    Vector2 bottomLeftAnchor;
    Vector2 tileWindowAnchor;
    Vector2 bottomRightAnchor;
    Vector2 topRightAnchor;
    Vector2 topLeftAnchor;

    bool messageBoxActive;
    int messageBoxSelection;
    
    int controlsButtonsActive;
    bool fullscreenPressed;
    bool tileMenuPressed;
    bool tileSetupWindowActive;
    bool tileSetupWindowMoving;
    bool tilingTypeEditMode;
    int tilingTypeActive;
    bool hideGridChecked;
    bool tileSizeEditMode;
    int tileSizeValue;
    bool loadTexturePressed;
    bool addTileGroupPressed;
    bool deleteTileGroupPressed;
    int tileGroupsScrollIndex;
    int tileGroupsActive;
    char *tileGroupsText;
    int editTilesActive;
    bool defineTilesPressed;
    bool layersLabelPressed;
    bool layerPressed;
    bool layerUpPressed;
    bool layerDownPressed;
    bool layerDeletePressed;
    bool layerCreatePressed;

    Rectangle layoutRecs[29];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} TileGuiState;

TileGuiState InitTileGui(void);
void TileGui(TileGuiState *state);
void UpdateGuiPositions(TileGuiState *state);

#endif // GUI_TILEGUI_H
