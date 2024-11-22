#include <stdlib.h>
#include "raygui.h"
#include "tile_gui.h"
#include "globals.h"

TileGuiState InitTileGui(void) {
    TileGuiState state = { 0 };

    state.screenWidth = 1280;
    state.screenHeight = 720;

    state.bottomLeftAnchor = (Vector2){ 0, 720 };
    state.tileWindowAnchor = (Vector2){ 248, 192 };
    state.bottomRightAnchor = (Vector2){ 1280, 720 };
    state.topRightAnchor = (Vector2){ 1280, 0 };
    state.topLeftAnchor = (Vector2){ 0, 0 };

    state.messageBoxActive = false;
    state.messageBoxSelection = -1;

    state.controlsButtonsActive = 0;
    state.fullscreenPressed = false;
    state.tileMenuPressed = false;
    state.tileSetupWindowActive = false;
    state.tilingTypeEditMode = false;
    state.tilingTypeActive = 0;
    state.hideGridChecked = true;
    state.tileSizeEditMode = false;
    state.tileSizeValue = 16;
    state.loadTexturePressed = false;
    state.addTileGroupPressed = false;
    state.deleteTileGroupPressed = false;
    state.tileGroupsScrollIndex = 0;
    state.tileGroupsActive = -1;
    // TODO: handle dynamic memory allocation for this text
    state.tileGroupsText = (char*)malloc(512*sizeof(char));
    state.editTilesActive = -1;
    state.defineTilesPressed = false;
    state.layersLabelPressed = false;
    state.layerPressed = false;
    state.layerUpPressed = false;
    state.layerDownPressed = false;
    state.layerDeletePressed = false;
    state.layerCreatePressed = false;

    // Custom variables initialization

    return state;
}

void UpdateGuiPositions(TileGuiState *state) {
    state->screenWidth = GetScreenWidth();
    state->screenHeight = GetScreenHeight();

    state->topRightAnchor = (Vector2){ state->screenWidth, 0 };
    state->bottomLeftAnchor = (Vector2){ 0, state->screenHeight };
    state->bottomRightAnchor = (Vector2){ state->screenWidth, state->screenHeight };

    state->layoutRecs[0] = (Rectangle){ state->topLeftAnchor.x + 16, state->topLeftAnchor.y + 16, 40, 24 };
    state->layoutRecs[1] = (Rectangle){ state->topLeftAnchor.x + 16, state->topLeftAnchor.y + 80, 208, state->bottomLeftAnchor.y - 160 };
    state->layoutRecs[2] = (Rectangle){ state->topLeftAnchor.x + 16, state->topLeftAnchor.y + 48, state->topRightAnchor.x - 32, 16 };
    state->layoutRecs[3] = (Rectangle){ state->bottomLeftAnchor.x + 16, state->bottomLeftAnchor.y + -40, 208, 24 };
    state->layoutRecs[4] = (Rectangle){ state->tileWindowAnchor.x + 0, state->tileWindowAnchor.y + 0, 776, 464 };
    state->layoutRecs[5] = (Rectangle){ state->tileWindowAnchor.x + 264, state->tileWindowAnchor.y + 80, 488, 360 };
    state->layoutRecs[6] = (Rectangle){ state->tileWindowAnchor.x + 64, state->tileWindowAnchor.y + 376, 120, 24 };
    state->layoutRecs[7] = (Rectangle){ state->tileWindowAnchor.x + 64, state->tileWindowAnchor.y + 352, 120, 24 };
    state->layoutRecs[8] = (Rectangle){ state->tileWindowAnchor.x + 704, state->tileWindowAnchor.y + 40, 24, 24 };
    state->layoutRecs[9] = (Rectangle){ state->tileWindowAnchor.x + 336, state->tileWindowAnchor.y + 40, 48, 24 };
    state->layoutRecs[10] = (Rectangle){ state->tileWindowAnchor.x + 72, state->tileWindowAnchor.y + 96, 120, 24 };
    state->layoutRecs[11] = (Rectangle){ state->tileWindowAnchor.x + 24, state->tileWindowAnchor.y + 256, 120, 80 };
    state->layoutRecs[12] = (Rectangle){ state->tileWindowAnchor.x + 24, state->tileWindowAnchor.y + 232, 120, 24 };
    state->layoutRecs[13] = (Rectangle){ state->tileWindowAnchor.x + 160, state->tileWindowAnchor.y + 264, 88, 24 };
    state->layoutRecs[14] = (Rectangle){ state->tileWindowAnchor.x + 160, state->tileWindowAnchor.y + 312, 88, 24 };
    state->layoutRecs[15] = (Rectangle){ state->tileWindowAnchor.x + 160, state->tileWindowAnchor.y + 240, 88, 24 };
    state->layoutRecs[16] = (Rectangle){ state->tileWindowAnchor.x + 160, state->tileWindowAnchor.y + 288, 88, 24 };
    state->layoutRecs[17] = (Rectangle){ state->tileWindowAnchor.x + 72, state->tileWindowAnchor.y + 128, 120, 32 };
    state->layoutRecs[18] = (Rectangle){ state->tileWindowAnchor.x + 56, state->tileWindowAnchor.y + 168, 152, 24 };
    state->layoutRecs[19] = (Rectangle){ state->bottomRightAnchor.x + -224, state->bottomRightAnchor.y + -80, 120, 24 };
    state->layoutRecs[20] = (Rectangle){ state->bottomRightAnchor.x + -224, state->bottomRightAnchor.y + -48, 128, 32 };
    state->layoutRecs[21] = (Rectangle){ state->bottomRightAnchor.x + -88, state->bottomRightAnchor.y + -48, 32, 16 };
    state->layoutRecs[22] = (Rectangle){ state->bottomRightAnchor.x + -88, state->bottomRightAnchor.y + -32, 32, 16 };
    state->layoutRecs[23] = (Rectangle){ state->bottomRightAnchor.x + -48, state->bottomRightAnchor.y + -48, 32, 32 };
    state->layoutRecs[24] = (Rectangle){ state->bottomRightAnchor.x + -48, state->bottomRightAnchor.y + -88, 32, 32 };
    state->layoutRecs[25] = (Rectangle){ state->topRightAnchor.x + -40, state->topRightAnchor.y + 16, 24, 24 };
    state->layoutRecs[26] = (Rectangle){ state->tileWindowAnchor.x + 56, state->tileWindowAnchor.y + 208, 24, 24 };
    state->layoutRecs[27] = (Rectangle){ state->tileWindowAnchor.x + 88, state->tileWindowAnchor.y + 208, 24, 24 };
    state->layoutRecs[28] = (Rectangle){ state->topRightAnchor.x/2 - 200, state->bottomRightAnchor.y/2 - 100 , 400, 250};
}

void UpdateTileWindow(TileGuiState *state) {
    // move tile setup window
    Rectangle title_collision_rect = {
        state->tileWindowAnchor.x,
        state->tileWindowAnchor.y,
        776 - 18,
        24 // raygui windowbox statusbar height
    };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !state->tileSetupWindowMoving) {
        if(CheckCollisionPointRec(GetMousePosition(), title_collision_rect)) {
            state->tileSetupWindowMoving = true;
        }
    }
    if (state->tileSetupWindowMoving | IsWindowResized()) {
        Vector2 mouse_delta = GetMouseDelta();
        state->tileWindowAnchor.x += mouse_delta.x;
        state->tileWindowAnchor.y += mouse_delta.y;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            state->tileSetupWindowMoving = false;
        }
        if (state->tileWindowAnchor.x + state->layoutRecs[4].width > state->screenWidth ) {
            state->tileWindowAnchor.x = state->screenWidth - state->layoutRecs[4].width;
        }
        if (state->tileWindowAnchor.x < 0) {
            state->tileWindowAnchor.x = 0;
        }
        if (state->tileWindowAnchor.y + state->layoutRecs[4].height > state->screenHeight ) {
            state->tileWindowAnchor.y = state->screenHeight - state->layoutRecs[4].height;
        }
        if (state->tileWindowAnchor.y < 0) {
            state->tileWindowAnchor.y = 0;
        }
    }
}
// draw texture in tile window
Rectangle TextureUIDimensions(TileGuiState *state) {
    Rectangle dst = { 0 };
    if (!state->tileSetupWindowActive) {
        return dst;
    }
    if (globals.tx.id <= 0) {
        return dst;
    }

    Rectangle tmp = state->layoutRecs[5];
    // resize
    float width, height, scale;
    if ((float)globals.tx.width/globals.tx.height > (float)tmp.width/tmp.height) {
        width = tmp.width - 32;
        scale = width / globals.tx.width;
        height = globals.tx.height * scale;
        dst = (Rectangle){ tmp.x + 16, tmp.y + (tmp.height - height)/2, width, height };
    }
    else {
        height = tmp.height - 32;
        scale = height / globals.tx.height;
        width = globals.tx.width * scale;
        dst = (Rectangle){ tmp.x + (tmp.width - width)/2, tmp.y + 16, width, height };
    }
    return dst;
}


void TileGui(TileGuiState *state) {
    const char *controlsButtonsText = "#021#;#108#;#68#;#50#;#100#";
    const char *fullscreenText = "#107#";
    const char *tilePaneText = "#100#Tiles";
    const char *tileMenuText = "#101#Tile Menu";
    const char *tileSetupWindowText = "Tile Setup";
    const char *tileSizeText = "Tile Size ";
    const char *textureBoxText = "Texture.png";
    const char *tilingTypeText = "#100#2x2;#100#3x3";
    const char *tilingTypeLabelText = "Auto-tiling Type";
    const char *hideGridText = "#097#";
    const char *loadTextureText = "Load Texture";
    const char *tileGroupsLabelText = "Auto-Tile Groups";
    const char *tileEditLabelText = "Mode";
    const char *tileEditText = "#022#\n#84#";
    const char *defineTilesText = "#099#Define Tiles";
    const char *layersLabelText = "#016#Layers";
    const char *layerText = "layer 1";
    const char *layerUpText = "#121#";
    const char *layerDownText = "#120#";
    const char *layerDeleteText = "#113#";
    const char *layerCreateText = "+";

    if (state->tilingTypeEditMode) GuiLock();
    if (state->messageBoxActive) GuiLock();

    if (!GuiIsLocked()) UpdateTileWindow(state);
    GuiToggleGroup(state->layoutRecs[0], controlsButtonsText, &state->controlsButtonsActive);
    state->fullscreenPressed = GuiButton(state->layoutRecs[25], fullscreenText);
    GuiGroupBox(state->layoutRecs[1], tilePaneText);
    GuiLine(state->layoutRecs[2], NULL);
    state->tileMenuPressed = GuiButton(state->layoutRecs[3], tileMenuText); 

    int lastStyle = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabel(state->layoutRecs[19], layersLabelText);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, lastStyle);

    state->layerPressed = GuiButton(state->layoutRecs[20], layerText); 
    state->layerUpPressed = GuiButton(state->layoutRecs[21], layerUpText); 
    state->layerDownPressed = GuiButton(state->layoutRecs[22], layerDownText); 
    state->layerDeletePressed = GuiButton(state->layoutRecs[23], layerDeleteText); 
    state->layerCreatePressed = GuiButton(state->layoutRecs[24], layerCreateText); 

    if (state->tileSetupWindowActive)
    {
        state->tileSetupWindowActive = !GuiWindowBox(state->layoutRecs[4], tileSetupWindowText);
        GuiGroupBox(state->layoutRecs[5], textureBoxText);
        GuiLabel(state->layoutRecs[7], tilingTypeLabelText);
        GuiCheckBox(state->layoutRecs[8], hideGridText, &state->hideGridChecked);
        if (GuiValueBox(state->layoutRecs[9], tileSizeText, &state->tileSizeValue, 0, 256, state->tileSizeEditMode)) state->tileSizeEditMode = !state->tileSizeEditMode;
        state->loadTexturePressed = GuiButton(state->layoutRecs[10], loadTextureText); 
        GuiListView(state->layoutRecs[11], !strcmp(state->tileGroupsText, "")? NULL: state->tileGroupsText, &state->tileGroupsScrollIndex, &state->tileGroupsActive);
        GuiLabel(state->layoutRecs[12], tileGroupsLabelText);
        GuiToggleGroup(state->layoutRecs[13], tileEditText, &state->editTilesActive);
        GuiLabel(state->layoutRecs[15], tileEditLabelText);
        state->defineTilesPressed = GuiButton(state->layoutRecs[17], defineTilesText); 
        state->addTileGroupPressed = GuiButton(state->layoutRecs[26], "+");
        state->deleteTileGroupPressed = GuiButton(state->layoutRecs[27], "-");
        GuiLine(state->layoutRecs[18], NULL);
        if (GuiDropdownBox(state->layoutRecs[6], tilingTypeText, &state->tilingTypeActive, state->tilingTypeEditMode)) state->tilingTypeEditMode = !state->tilingTypeEditMode;
    }

    globals.txUIRect = TextureUIDimensions(state);
    if (globals.tx.id > 0) {
        // Draw texture on tile window
        DrawTexturePro(globals.tx,
                (Rectangle){ 0, 0, globals.tx.width, globals.tx.height },
                globals.txUIRect, (Vector2){ 0, 0 }, 0, WHITE);
        // draw grid over texture
        if (state->hideGridChecked) {
            int size = state->tileSizeValue;
            int col = globals.tx.width/size;
            int row = globals.tx.height/size;
            float scale = globals.txUIRect.width / globals.tx.width;
            for (int i = 0; i <= row; i++) {
                DrawLine(globals.txUIRect.x - 8,
                        globals.txUIRect.y + size*scale*i,
                        globals.txUIRect.x + globals.txUIRect.width + 8,
                        globals.txUIRect.y + size*scale*i,
                        GREEN);
            }
            for (int i = 0; i <= col; i++) {
                DrawLine(globals.txUIRect.x + size*scale*i,
                        globals.txUIRect.y - 8,
                        globals.txUIRect.x + size*scale*i,
                        globals.txUIRect.y + globals.txUIRect.height + 8,
                        GREEN);
            }
        }
    }

    GuiUnlock();
}
