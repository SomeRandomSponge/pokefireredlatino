#ifndef GUARD_FIELDMAP_H
#define GUARD_FIELDMAP_H

#include "global.h"

#define NUM_TILES_IN_PRIMARY 640
#define NUM_TILES_TOTAL 1024
#define NUM_METATILES_IN_PRIMARY 640
#define NUM_METATILES_TOTAL 1024
#define NUM_PALS_IN_PRIMARY 7
#define NUM_PALS_TOTAL 13
#define MAX_MAP_DATA_SIZE 0x2800
#define VIRTUAL_MAP_SIZE (MAX_MAP_DATA_SIZE)

extern struct BackupMapLayout VMap;
extern const struct MapLayout Route1_Layout;

u32 MapGridGetMetatileIdAt(s32, s32);
u32 MapGridGetMetatileBehaviorAt(s16, s16);
u8 MapGridGetMetatileLayerTypeAt(s16 x, s16 y);
void MapGridSetMetatileIdAt(s32, s32, u16);
void MapGridSetMetatileEntryAt(s32, s32, u16);
u8 MapGridGetZCoordAt(s32 x, s32 y);
void GetCameraCoords(u16*, u16*);
bool8 MapGridIsImpassableAt(s32, s32);
s32 GetMapBorderIdAt(s32, s32);
bool32 CanCameraMoveInDirection(s32);
const struct MapHeader * GetMapHeaderFromConnection(struct MapConnection * connection);
struct MapConnection * GetMapConnectionAtPos(s16 x, s16 y);
void Fieldmap_ApplyGlobalTintToPaletteSlot(u8 slot, u8 count);
void SaveMapView(void);
u32 ExtractMetatileAttribute(u32 attributes, u8 attributeType);
u32 MapGridGetMetatileAttributeAt(s16 x, s16 y, u8 attributeType);
void MapGridSetMetatileImpassabilityAt(s32 x, s32 y, bool32 arg2);
bool8 CameraMove(s32 x, s32 y);
void CopyMapTilesetsToVram(struct MapLayout const * mapLayout);
void LoadMapTilesetPalettes(struct MapLayout const * mapLayout);
void InitMap(void);
void CopySecondaryTilesetToVramUsingHeap(const struct MapLayout * mapLayout);
void LoadSecondaryTilesetPalette(const struct MapLayout * mapLayout);
void InitMapFromSavedGame(void);
void CopyPrimaryTilesetToVram(const struct MapLayout *mapLayout);
void CopySecondaryTilesetToVram(const struct MapLayout *mapLayout);
void GetCameraFocusCoords(u16 *x, u16 *y);
void SetCameraFocusCoords(u16 x, u16 y);

#endif //GUARD_FIELDMAP_H
