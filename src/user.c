#undef GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION

#include "tilemap.h"
#include "tile_gui.h"
#include "globals.h"

#ifdef _WIN32
#define PATH_SEPERATOR "\\"
#else
#define PATH_SEPERATOR "/"
#endif

char fileNameToLoad[512];

Globals globals = {
    .scale = 32.0f,
    .currentLayer = 0,
    .rectMouseHover = { 0 },
};

static int screenWidth = 1280;
static int screenHeight = 720;

static bool definingTile = false;
static Rectangle startRect;

void UpdateGlobals(TileGuiState *tileGuiState, TileMap *tileMap) {
     if (tileGuiState->tileSizeValue > 0)
         tileMap->tileSize = tileGuiState->tileSizeValue;
     else
         tileMap->tileSize = 1;

}

void FullScreenToggle(TileGuiState *tileGuiState) {
    if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE)) {
        ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
        SetWindowSize(screenWidth, screenHeight);
        UpdateGuiPositions(tileGuiState);
    }
    else {
        SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
    }
}

void FDLoadTexture(GuiWindowFileDialogState *fileDialogState, TileMap *tileMap) {
    if (IsFileExtension(fileDialogState->fileNameText, ".png")) {
        strcpy(fileNameToLoad, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState->dirPathText, fileDialogState->fileNameText));
        TileMapLoadTexture(tileMap, fileNameToLoad);
    }
    fileDialogState->SelectFilePressed = false;
}

void MousePanCamera() {
    Vector2 delta = GetMouseDelta();
    globals.CameraPos.x -= delta.x;
    globals.CameraPos.y -= delta.y;
}

void ZoomCamera() {
    float scroll = GetMouseWheelMove() / 3.0;
    if (scroll > 0.0001 || scroll < -0.0001) {
        // TODO: make camera zoom towards mouse
        globals.scale += scroll;
    }
}

void DefineTileRect(TileMap *tileMap) {
    Rectangle curRect = globals.rectMouseHover;
    Rectangle rect;
    if (startRect.x <= curRect.x && startRect.y <= curRect.y) {
        rect = (Rectangle){ startRect.x, startRect.y, curRect.x - startRect.x + curRect.width, curRect.y - startRect.y + curRect.height };
    }
    else {
        return;
    }
    // TODO: dont place tiles that are outside of the texture
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        definingTile = false;
        bool overlap = false;
        for (int i = 0; i < tileMap->numTileRects; i++) {
            float scale = globals.txUIRect.width / globals.tx.width;
            Rectangle tile = tileMap->tileRects[i];
            Rectangle rec = (Rectangle){ tile.x*scale, tile.y*scale,
                tile.width*scale, tile.height*scale };
            Rectangle curRect = (Rectangle){ rect.x*scale, rect.y*scale, rect.width*scale, rect.height*scale};
            // make sure the new rect doesnt overlap any current recs
            if (CheckCollisionRecs(curRect, rec)) {
                overlap = true;
            }
        }
        if (!overlap) {
            // TODO: check if need to realloc for more tiles
            if (tileMap->numTileRects < tileMap->rectAlloc) {
                tileMap->tileRects[tileMap->numTileRects] = rect;
                tileMap->numTileRects++;
            }
        }
    }
}

void DeleteRect(TileMap *tileMap, int pos) {
    Rectangle *recs = tileMap->tileRects;
    recs[pos] = recs[tileMap->numTileRects - 1];
    // adjust tile ids to match what was just done
    for (int i = 0; i < tileMap->numLayers; i++) {
        for (int j = 0; j < tileMap->height; j++) {
            for (int k = 0; k < tileMap->width; k++) {
                int tile = tileMap->tileLayers[i].tiles[j][k];
                if (tile < 0) continue;

                if (tile == pos) {
                    // TODO: check this position
                    tileMap->tileLayers[i].tiles[j][k] = tileMap->numTileRects - 1;
                }
                else if (tile == tileMap->numTileRects - 1) {
                    tileMap->tileLayers[i].tiles[j][k] = pos;
                }

            }
        }
    }
    tileMap->numTileRects--;
}

void RemoveTileRect(TileMap *tileMap) {
    // TODO: implement this
    for (int i = 0; i < tileMap->numTileRects; i++) {
        float scale = globals.txUIRect.width / globals.tx.width;
        Vector2 mp = { globals.MousePos.x - globals.txUIRect.x,
            globals.MousePos.y - globals.txUIRect.y };
        Rectangle tile = tileMap->tileRects[i];
        Rectangle rec = (Rectangle){ tile.x*scale, tile.y*scale,
            tile.width*scale, tile.height*scale };
        if (CheckCollisionPointRec(mp, rec)) {
            DeleteRect(tileMap, i);
        }
    }
}

void UserInput(TileGuiState *tileGuiState, GuiWindowFileDialogState *fileDialogState, TileMap *tileMap) {
    globals.screenWidth = GetScreenWidth();
    globals.screenHeight = GetScreenHeight();
    globals.MousePos = GetMousePosition();

    if (!CheckCollisionPointRec(globals.MousePos, tileGuiState->layoutRecs[4]) || !tileGuiState->tileSetupWindowActive) {
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)
                || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_SPACE))) {
            MousePanCamera();
        }

        ZoomCamera();
    }

    if (!fileDialogState->windowActive) {
        if (tileGuiState->tileSetupWindowActive) {
            if (CheckCollisionPointRec(globals.MousePos, globals.txUIRect)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    bool overlap = false;
                    for (int i = 0; i < tileMap->numTileRects; i++) {
                        float scale = globals.txUIRect.width / globals.tx.width;
                        Vector2 mp = { globals.MousePos.x - globals.txUIRect.x,
                            globals.MousePos.y - globals.txUIRect.y };
                        Rectangle tile = tileMap->tileRects[i];
                        Rectangle rec = (Rectangle){ tile.x*scale, tile.y*scale,
                            tile.width*scale, tile.height*scale };
                        if (CheckCollisionPointRec(mp, rec)) {
                            overlap = true;
                        }
                    }
                    if (!overlap) {
                        startRect = globals.rectMouseHover;
                        definingTile = true;
                    }
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    RemoveTileRect(tileMap);
                }
            }
        }
        else {
            // Place a tile
        }

    if (definingTile) DefineTileRect(tileMap);

    if (tileGuiState->fullscreenPressed) {
        FullScreenToggle(tileGuiState);
    }
    if (tileGuiState->tileMenuPressed) {
        tileGuiState->tileSetupWindowActive = !tileGuiState->tileSetupWindowActive;
    }
    if (tileGuiState->deleteTileGroupPressed) {
        tileGuiState->messageBoxActive = true;
    }
    if (tileGuiState->layerDeletePressed) {
        tileGuiState->messageBoxActive = true;
    }
    if (tileGuiState->loadTexturePressed) {
        fileDialogState->windowActive = true;
    }
    // create tile group
    if (tileGuiState->addTileGroupPressed) {
        CreateTileGroup(tileMap, "Testing 123");
        strcpy(tileGuiState->tileGroupsText, "");
        for (int i = 0; i < tileMap->numAutoTileGroups; i++) {
            if (i > 0) {
                strcat(tileGuiState->tileGroupsText, ";");
            }
            strcat(tileGuiState->tileGroupsText, TextFormat("Testing %d", i));
        }
    }
    // File Dialogue
    if (fileDialogState->SelectFilePressed) { // Load image file (if supported extension)
        FDLoadTexture(fileDialogState, tileMap);
    }
    UpdateGlobals(tileGuiState, tileMap);
}
