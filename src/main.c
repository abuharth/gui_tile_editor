// TODO: center the popup window
// TODO: prompt for a name when the create button is pressed
// TODO: prompt for a name when layer create or group create
// TODO: move initialization draw functions to more suitable places

#define TILE_GUI_IMPLEMENTATION
#include "tile_gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"

//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include "tilemap.h"
#include "user.h"
#include "globals.h"

// TILE GUI
TileGuiState tileGuiState;
// FILE DIALOGUE
GuiWindowFileDialogState fileDialogState;
// Tilemap
static TileMap tileMap;

// Module functions declaration
void UpdateDrawFrame(void);     // Update and Draw one frame

#define PRINT_DEBUG(...) DrawText(TextFormat(__VA_ARGS__), 50, 50, 20, RED)

int main(void) {
    // initialization
    tileGuiState = InitTileGui();
    InitTileMap(&tileMap);
    CreateTileLayer(&tileMap, "TestLayer");
    tileMap.tileLayers[0].tiles[10][5] = 0;
    tileMap.tileLayers[0].tiles[10][6] = 0;
    tileMap.tileLayers[0].tiles[10][7] = 0;
    tileMap.tileLayers[0].tiles[10][8] = 0;
    tileMap.tileLayers[0].tiles[11][8] = 0;
    tileMap.tileLayers[0].tiles[12][8] = 0;
    tileMap.tileLayers[0].tiles[20][15] = 0;
    tileMap.tileLayers[0].tiles[21][15] = 0;
    tileMap.tileLayers[0].tiles[22][15] = 0;
    tileMap.tileLayers[0].tiles[21][16] = 0;
    tileMap.tileRects[0] = (Rectangle){ 0, 0, 16, 16 };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(tileGuiState.screenWidth, tileGuiState.screenHeight, "tilemap editor");
    // GUI
    // GuiLoadStyle("style_cyber.rgs");
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    // GUI WINDOW FILE DIALOGUE
    fileDialogState = InitGuiWindowFileDialog(TextFormat("%s/textures", GetWorkingDirectory()));

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
    UserInput(&tileGuiState, &fileDialogState, &tileMap);

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawTileMap(&tileMap);
        // Draw gui
        TileGui(&tileGuiState);
        // FILE DIALOG
        GuiUnlock();
        GuiWindowFileDialog(&fileDialogState);
        if (fileDialogState.windowActive)
            GuiLock();

        const char *messageBoxButtons = "##Yes;##No";
        const char *messageBoxText = "Are you Sure?";
        if (tileGuiState.messageBoxActive) {
            GuiUnlock();
            if ((tileGuiState.messageBoxSelection = GuiMessageBox(tileGuiState.layoutRecs[28], NULL, messageBoxText, messageBoxButtons)) >= 0) {
                tileGuiState.messageBoxActive = false;
            }
            GuiLock();
        }

        PRINT_DEBUG("current tilegroup: %d\nnumAutoTileGroups: %d", tileGuiState.tileGroupsActive, tileMap.numAutoTileGroups);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
