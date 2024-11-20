#ifndef TILEMAP_H
#define TILEMAP_H

#include <raylib.h>

#define STRING_LENGTH 64

typedef struct Vec2i {
    int x;
    int y;
} Vec2i;

typedef struct TileLayer {
    char name[64];
    int **tiles;        // 2D array of tiles (can be grown)
} TileLayer;

typedef struct TileRule {
    int tile;
    int bitmask;
} TileRule;

typedef enum AutoTileType {
    TWOxTWO = 0,
    THREExTHREE
} AutoTileType;

typedef struct AutoTileGroup {
    char name[64];
    TileRule *tileRules;
    int numRules;
    AutoTileType ruleType;           // 0 = 2x2, 1 = 3x3, ...
} AutoTileGroup;

typedef struct TileMap {
    Texture2D texture;
    bool textureLoaded;
    TileLayer *tileLayers;
    int numLayers;
    AutoTileGroup *autoTileGroups;
    int numAutoTileGroups;
    Rectangle *tileRects;   // Rectangles that define tiles
    int numTileRects;
    int tileSize;
    int width;
    int height;
    // current memory allocations
    int layerAlloc;
    int tileGroupAlloc;
    int rectAlloc;
    int layerWidthAlloc;
    int layerHeightAlloc;
    int tileRuleAlloc;
} TileMap;

// functions
void InitTileMap(TileMap *tileMap);
void TileMapLoadTexture(TileMap *tileMap, const char *filename);
void DrawTileMap(TileMap *tileMap);
AutoTileGroup *SearchAllTileRules(int tile, TileMap tileMap);
int SearchGroupTile(int tile, AutoTileGroup *tileGroup);
int SearchGroupMask(int bitmask, AutoTileGroup *tileGroup);
int GenerateBitMask(int **tiles, int posX, int posY, AutoTileGroup *tileGroup, int type);
void ApplyTileRules(TileMap tileMap, int layer, int posX, int posY, AutoTileType type);

#endif // TILEMAP_H
