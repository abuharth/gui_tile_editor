#include <stdlib.h>
#include <raylib.h>
#include "tilemap.h"

void CreateTileLayer(TileMap *tileMap) {
    tileMap->numLayers += 1;
}

void InitTileMap(TileMap *tileMap) {
    tileMap->textureLoaded = false;
    tileMap->layerAlloc = 3;
    tileMap->tileGroupAlloc = 3;
    tileMap->rectAlloc = 32;
    tileMap->layerWidthAlloc = 128;
    tileMap->layerHeightAlloc = 64;
    tileMap->tileRuleAlloc = 32;

    tileMap->textureLoaded = false;
    // initial memory allocation
    tileMap->tileLayers = (TileLayer*)malloc(tileMap->layerAlloc*sizeof(TileLayer));
    tileMap->numLayers = 0;
    tileMap->autoTileGroups = (AutoTileGroup*)malloc(tileMap->tileGroupAlloc*sizeof(AutoTileGroup));
    tileMap->numAutoTileGroups = 0;
    tileMap->tileRects = (Rectangle*)malloc(tileMap->rectAlloc*sizeof(Rectangle));
    tileMap->numTileRects = 0;
    tileMap->tileSize = 16;
    // tileLayers
    tileMap->width = tileMap->layerWidthAlloc;
    tileMap->height = tileMap->layerHeightAlloc;
    for (int i = 0; i < tileMap->layerAlloc; i++) {
        tileMap->tileLayers[i].tiles = (int**)malloc(tileMap->layerHeightAlloc*sizeof(int*));
        for (int j = 0; j < tileMap->layerWidthAlloc; j++) {
                tileMap->tileLayers[i].tiles[j] = (int*)malloc(tileMap->layerWidthAlloc*sizeof(int));
        }
    }
    // autoTileGroups
    for (int i = 0; i < tileMap->layerAlloc; i++) {
        tileMap->autoTileGroups[i].tileRules = (TileRule*)malloc(tileMap->tileRuleAlloc*sizeof(TileRule));
        tileMap->autoTileGroups[i].numRules = 0;
    }
}

void TileMapLoadTexture(TileMap *tileMap, const char *filename) {
    tileMap->texture = LoadTexture(filename);
    tileMap->textureLoaded = true;
}

void DrawTileMap(TileMap *tileMap) {
    for (int i = 0; i < tileMap->numLayers; i++) {
        for (int j = 0; j < tileMap->height; j++) {
            for (int k = 0; k < tileMap->width; k++) {
                int tile = tileMap->tileLayers[i].tiles[j][k];
                // TODO: global camera position and zoom
                // these attributes will affect the dst rect
                Rectangle dst = {
                    k*tileMap->tileSize,
                    j*tileMap->tileSize,
                    tileMap->tileSize,
                    tileMap->tileSize
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

// search through all tilemap rules to see if the tile
// is in it. Returns the rule and the auto-tile group it resides in
AutoTileGroup *SearchAllTileRules(int tile, TileMap tileMap) {
    int numGroups = tileMap.numAutoTileGroups;
    AutoTileGroup *tileGroup = (AutoTileGroup*)malloc(sizeof(AutoTileGroup));
    for (int i = 0; i < numGroups; i++) {
        int numRules = tileMap.autoTileGroups[i].numRules;
        for (int j = 0; j < numRules; j++) {
            TileRule rule = tileMap.autoTileGroups[i].tileRules[j];
            if (rule.tile == tile) {
                tileGroup = &tileMap.autoTileGroups[i];
                return tileGroup;
            }
        }
    }
    free(tileGroup);
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

