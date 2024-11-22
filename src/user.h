//TODO: All button related functions go here
//TODO: camera movement here
//TODO: user side Defining tile rects here
//TODO: user side defining bitmasks goes here
//TODO: user side creating/naming tile groups
//TODO: user side creating and naming layers
//TODO: user resize tile interface window
#undef GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"
#define GUI_WINDOW_FILE_DALOG_IMPLEMENTATION

#include "tile_gui.h"
#include "tilemap.h"

void UserInput(TileGuiState *tileGuiState, GuiWindowFileDialogState *fileDialogState, TileMap *tileMap);
