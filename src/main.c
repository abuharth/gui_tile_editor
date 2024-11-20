// TODO: center the popup window
// TODO: prompt for a name when the create button is pressed
// TODO: prompt for a name when layer create or group create
// TODO: move initialization and button react and draw functions to more
// suitable places (MAYBE move button and user interaction stuff to one USER-SIDE FILE)
// TODO: global variables for the user (camera pos, etc.)

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

// TILE GUI
TileGuiState tileGuiState;
// FILE DIALOGUE
GuiWindowFileDialogState fileDialogState;
char fileNameToLoad[512];
static int screenWidth = 1280;
static int screenHeight = 720;

static TileMap tileMap;

// Module functions declaration
void UpdateDrawFrame(void);     // Update and Draw one frame

int main(void) {
    // initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    tileGuiState = InitTileGui();

    InitWindow(tileGuiState.screenWidth, tileGuiState.screenHeight, "tilemap editor");
    InitTileMap(&tileMap);
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

    // React to button presses
    if (tileGuiState.fullscreenPressed) {
        if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE)) {
            ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
            SetWindowSize(screenWidth, screenHeight);
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
    // File Dialogue
    if (fileDialogState.SelectFilePressed)
    { // Load image file (if supported extension)
        if (IsFileExtension(fileDialogState.fileNameText, ".png"))
        { strcpy(fileNameToLoad, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState.dirPathText, fileDialogState.fileNameText));
            TileMapLoadTexture(&tileMap, fileNameToLoad);
        }

        fileDialogState.SelectFilePressed = false;
    }
    if (tileGuiState.loadTexturePressed) {
        fileDialogState.windowActive = true;
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        DrawTileMap(&tileMap);
        // Draw gui
        TileGui(&tileGuiState);
        // draw texture in tile window
        if (tileGuiState.tileSetupWindowActive) {
            if (tileMap.textureLoaded) {
                Rectangle tmp = tileGuiState.layoutRecs[5];
                Rectangle dst;
                // resize
                float width, height, scale;
                if ((float)tileMap.texture.width/tileMap.texture.height > (float)tmp.width/tmp.height) {
                    width = tmp.width - 32;
                    scale = width / tileMap.texture.width;
                    height = tileMap.texture.height * scale;
                    dst = (Rectangle){ tmp.x + 16, tmp.y + (tmp.height - height)/2, width, height };
                }
                else {
                    height = tmp.height - 32;
                    scale = height / tileMap.texture.height;
                    width = tileMap.texture.width * scale;
                    dst = (Rectangle){ tmp.x + (tmp.width - width)/2, tmp.y + 16, width, height };
                }
                DrawTexturePro(tileMap.texture,
                        (Rectangle){ 0, 0, tileMap.texture.width, tileMap.texture.height },
                        dst, (Vector2){ 0, 0 }, 0, WHITE);
            }
        }
        // FILE DIALOG
        GuiUnlock();
        GuiWindowFileDialog(&fileDialogState);
        if (fileDialogState.windowActive) GuiLock();

        const char *messageBoxButtons = "##Yes;##No";
        const char *messageBoxText = "Are you Sure?";

        if (tileGuiState.messageBoxActive) {
            GuiUnlock();
            if ((tileGuiState.messageBoxSelection = GuiMessageBox(tileGuiState.layoutRecs[28], NULL, messageBoxText, messageBoxButtons)) >= 0) {
                tileGuiState.messageBoxActive = false;
            }
            GuiLock();
        }

    EndDrawing();
    //----------------------------------------------------------------------------------
}
