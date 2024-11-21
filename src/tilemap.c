#include <stdlib.h>
#include <raylib.h>
#include "tilemap.h"
#include "globals.h"

void CreateTileLayer(TileMap *tileMap) {
    // TODO: allocate memory if numLayers is above the current allocation
    tileMap->numLayers += 1;
}

void InitTileMap(TileMap *tileMap) {
    tileMap->textureLoaded = false;
    tileMap->layerAlloc = 3;
    tileMap->tileGroupAlloc = 3;
    tileMap->rectAlloc = 32;
    tileMap->WidthAlloc = 128;
    tileMap->HeightAlloc = 64;
    tileMap->tileRuleAlloc = 32;

    // initial memory allocation
    tileMap->tileLayers = (TileLayer*)malloc(tileMap->layerAlloc*sizeof(TileLayer));
    tileMap->numLayers = tileMap->layerAlloc;
    tileMap->autoTileGroups = (AutoTileGroup*)malloc(tileMap->tileGroupAlloc*sizeof(AutoTileGroup));
    tileMap->numAutoTileGroups = 0;
    tileMap->tileRects = (Rectangle*)malloc(tileMap->rectAlloc*sizeof(Rectangle));
    tileMap->numTileRects = 0;
    tileMap->tileSize = 16;
    // tileLayers
    tileMap->width = tileMap->WidthAlloc;
    tileMap->height = tileMap->HeightAlloc;
    for (int i = 0; i < tileMap->layerAlloc; i++) {
        tileMap->tileLayers[i].tiles = (int**)malloc(tileMap->HeightAlloc*sizeof(int*));
        for (int j = 0; j < tileMap->HeightAlloc; j++) {
                tileMap->tileLayers[i].tiles[j] = (int*)malloc(tileMap->WidthAlloc*sizeof(int));
        }
    }
    // autoTileGroups
    for (int i = 0; i < tileMap->layerAlloc; i++) {
        tileMap->autoTileGroups[i].tileRules = (TileRule*)malloc(tileMap->tileRuleAlloc*sizeof(TileRule));
        tileMap->autoTileGroups[i].numRules = 0;
    }
    // zero out tile array
    for (int i = 0; i < tileMap->layerAlloc; i++) {
        for (int j = 0; j < tileMap->height; j++) {
            for (int k = 0; k < tileMap->width; k++) {
                tileMap->tileLayers[i].tiles[j][k] = -1;
            }
        }
    }
}

void TileMapLoadTexture(TileMap *tileMap, const char *filename) {
    tileMap->texture = LoadTexture(filename);
    tileMap->textureLoaded = true;
}

#include <stdio.h>
void DrawTileMap(TileMap *tileMap) {
    if (!tileMap->textureLoaded) {
        return;
    }
    for (int i = 0; i < tileMap->numLayers; i++) {
        for (int j = 0; j < tileMap->height; j++) {
            for (int k = 0; k < tileMap->width; k++) {
                int tile = tileMap->tileLayers[i].tiles[j][k];
                if (tile < 0) continue;
                Rectangle dst = {
                    tileMap->tileSize * k * globals.scale - globals.CameraPos.x,
                    tileMap->tileSize * j * globals.scale - globals.CameraPos.y,
                    tileMap->tileSize * globals.scale,
                    tileMap->tileSize * globals.scale
                };
                DrawTexturePro(
                        tileMap->texture,
                        tileMap->tileRects[tile],
                        dst,
                        (Vector2){ 0, 0 }, 0, WHITE
                );
            }
        }
    }
}

void PlaceTile(TileMap *tileMap, int layer, int posX, int posY, int tile) {
    if (posX < tileMap->width && posY < tileMap->height) {
        tileMap->tileLayers[layer].tiles[posY][posX] = tile;
    }
    // TODO: must allocate more memory
    else {
    }
}

void RemoveTile(TileMap *tileMap, int layer, int posX, int posY) {
    if (posX < tileMap->width && posY < tileMap->height) {
        tileMap->tileLayers[layer].tiles[posY][posX] = -1;
    }
}

void ApplyTileRules(TileMap tileMap, int layer, int posX, int posY, AutoTileType type) {
    // check if tile position is occupied
    int **tiles = tileMap.tileLayers[layer].tiles;
    int tile;
    if ((tile = tiles[posY][posX]) == -1) {
        return;
    }
    // check if tile belongs to any tile groups
    AutoTileGroup *tileGroup = NULL;
    if ((tileGroup = SearchAllTileRules(tile, tileMap)) == NULL) {
        return;
    }

    int bitmask = GenerateBitMask(tiles, posX, posY, tileGroup, type);
    int newTile;
    if ((newTile = SearchGroupMask(bitmask, tileGroup)) >= 0) {
        tiles[posY][posX] = newTile;
    }
}

// search through all auto-tile groups to see if the tile
// is in it. Returns the auto-tile group it resides in
AutoTileGroup *SearchAllTileRules(int tile, TileMap tileMap) {
    int numGroups = tileMap.numAutoTileGroups;
    for (int i = 0; i < numGroups; i++) {
        if (SearchGroupTile(tile, &tileMap.autoTileGroups[i]) >= 0) {
            return &tileMap.autoTileGroups[i];
        }
    }
    return NULL;
}

int SearchGroupTile(int tile, AutoTileGroup *tileGroup) {
    for (int i = 0; i < tileGroup->numRules; i++) {
        if (tileGroup->tileRules[i].tile == tile) {
            return i;
        }
    }
    return -1;
}

int SearchGroupMask(int bitmask, AutoTileGroup *tileGroup) {
    for (int i = 0; i < tileGroup->numRules; i++) {
        if (tileGroup->tileRules[i].bitmask == bitmask) {
            return i;
        }
    }
    return -1;
}

// create the bitmask for the current tile
int GenerateBitMask(int **tiles, int posX, int posY, AutoTileGroup *tileGroup, int type) {
    if (type == TWOxTWO) {
        int matchMask = 0b0000;
        if ((SearchGroupTile(tiles[posY - 1][posX - 1], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY - 1][posX], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY][posX - 1], tileGroup) >= 0)) {
            matchMask = matchMask | 0b1000;
        }
        if ((SearchGroupTile(tiles[posY - 1][posX + 1], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY - 1][posX], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY][posX + 1], tileGroup) >= 0)) {
            matchMask = matchMask | 0b0100;
        }
        if ((SearchGroupTile(tiles[posY + 1][posX - 1], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY + 1][posX], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY][posX - 1], tileGroup) >= 0)) {
            matchMask = matchMask | 0b0010;
        }
        if ((SearchGroupTile(tiles[posY + 1][posX + 1], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY + 1][posX], tileGroup) >= 0)
                && (SearchGroupTile(tiles[posY][posX + 1], tileGroup) >= 0)) {
            matchMask = matchMask | 0b0001;
        }
        return matchMask;
    }
    else if (type == THREExTHREE) {
        // not implemented
        return -1;
    }
    return -1;
}

