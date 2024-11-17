// TODO: center the popup window
// TODO: prompt for a name when the create button is pressed
// TODO: prompt for a name when layer create

#define TILE_GUI_IMPLEMENTATION
#include "tile_gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

TileGuiState tileGuiState;

// Module functions declaration
void UpdateDrawFrame(void);     // Update and Draw one frame

int main(void) {
    // initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    tileGuiState = InitTileGui();
    InitWindow(tileGuiState.screenWidth, tileGuiState.screenHeight, "tilemap editor");
    // GUI
    GuiLoadStyle("style_cyber.rgs");
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        UpdateDrawFrame();
    }
#endif

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(void)
{
    UpdateGuiPositions(&tileGuiState);

    // React to button presses
    if (tileGuiState.fullscreenPressed) {
        if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE)) {
            ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
            UpdateGuiPositions(&tileGuiState);
        }
        else {
            SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
        }
    }
    if (tileGuiState.tileMenuPressed) {
        tileGuiState.tileSetupWindowActive = !tileGuiState.tileSetupWindowActive;
    }
    if (tileGuiState.deleteTileGroupPressed) {
        tileGuiState.messageBoxActive = true;
    }
    if (tileGuiState.layerDeletePressed) {
        tileGuiState.messageBoxActive = true;
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        // Draw gui
        TileGui(&tileGuiState);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
