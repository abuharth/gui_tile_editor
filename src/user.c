#undef TILE_GUI_IMPLEMENTATION
#include "tile_gui.h"
#define TILE_GUI_IMPLEMENTATION
#undef GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION

#include "tilemap.h"
#include "globals.h"

#ifdef _WIN32
#define PATH_SEPERATOR "\\"
#else
#define PATH_SEPERATOR "/"
#endif

char fileNameToLoad[512];

Globals globals = {
    .scale = 1.0f
};

static int screenWidth = 1280;
static int screenHeight = 720;

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

void UserInput(TileGuiState *tileGuiState, GuiWindowFileDialogState *fileDialogState, TileMap *tileMap) {
    globals.screenWidth = GetScreenWidth();
    globals.screenHeight = GetScreenHeight();
    globals.MousePos = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)
            // TODO: CHECK that the mouse is not overlapping with the tile window
            || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_SPACE))) {
        MousePanCamera();
    }
    ZoomCamera();
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
    // File Dialogue
    if (fileDialogState->SelectFilePressed) { // Load image file (if supported extension)
        FDLoadTexture(fileDialogState, tileMap);
    }
}
