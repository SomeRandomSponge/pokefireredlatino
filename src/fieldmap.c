#include "global.h"
#include "gflib.h"
#include "overworld.h"
#include "script.h"
#include "new_menu_helpers.h"
#include "quest_log.h"
#include "fieldmap.h"

struct ConnectionFlags
{
    u8 south:1;
    u8 north:1;
    u8 west:1;
    u8 east:1;
};

static void InitMapLayoutData(struct MapHeader *mapHeader);
static void map_copy_with_padding(u16 *map, u16 width, u16 height);
static void mapheader_copy_mapdata_of_adjacent_maps(struct MapHeader *mapHeader);
static void fillSouthConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset);
static void fillNorthConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset);
static void fillWestConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset);
static void fillEastConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset);
static void LoadSavedMapView(void);
static struct MapConnection *sub_8059600(u8 direction, s32 x, s32 y);
static bool8 sub_8059658(u8 direction, s32 x, s32 y, struct MapConnection *connection);
static bool8 sub_80596BC(s32 x, s32 src_width, s32 dest_width, s32 offset);

struct BackupMapLayout VMap;
EWRAM_DATA u16 gBackupMapData[VIRTUAL_MAP_SIZE] = {};
EWRAM_DATA struct MapHeader gMapHeader = {};
EWRAM_DATA struct Camera gCamera = {};
static EWRAM_DATA struct ConnectionFlags gMapConnectionFlags = {};
EWRAM_DATA u8 gGlobalFieldTintMode = QL_TINT_NONE;

static const struct ConnectionFlags sDummyConnectionFlags = {};

static const u32 sMetatileAttrMasks[METATILE_ATTRIBUTE_COUNT] = {
    [METATILE_ATTRIBUTE_BEHAVIOR]       = 0x000001ff,
    [METATILE_ATTRIBUTE_TERRAIN]        = 0x00003e00,
    [METATILE_ATTRIBUTE_2]              = 0x0003c000,
    [METATILE_ATTRIBUTE_3]              = 0x00fc0000,
    [METATILE_ATTRIBUTE_ENCOUNTER_TYPE] = 0x07000000,
    [METATILE_ATTRIBUTE_5]              = 0x18000000,
    [METATILE_ATTRIBUTE_LAYER_TYPE]     = 0x60000000,
    [METATILE_ATTRIBUTE_7]              = 0x80000000
};

static const u8 sMetatileAttrShifts[METATILE_ATTRIBUTE_COUNT] = {
    [METATILE_ATTRIBUTE_BEHAVIOR]       = 0,
    [METATILE_ATTRIBUTE_TERRAIN]        = 9,
    [METATILE_ATTRIBUTE_2]              = 14,
    [METATILE_ATTRIBUTE_3]              = 18,
    [METATILE_ATTRIBUTE_ENCOUNTER_TYPE] = 24,
    [METATILE_ATTRIBUTE_5]              = 27,
    [METATILE_ATTRIBUTE_LAYER_TYPE]     = 29,
    [METATILE_ATTRIBUTE_7]              = 31
};

const struct MapHeader * mapconnection_get_mapheader(struct MapConnection * connection)
{
    return Overworld_GetMapHeaderByGroupAndId(connection->mapGroup, connection->mapNum);
}

void InitMap(void)
{
    InitMapLayoutData(&gMapHeader);
    RunOnLoadMapScript();
}

void InitMapFromSavedGame(void)
{
    InitMapLayoutData(&gMapHeader);
    LoadSavedMapView();
    RunOnLoadMapScript();
}

static void InitMapLayoutData(struct MapHeader * mapHeader)
{
    const struct MapLayout * mapLayout = mapHeader->mapLayout;
    CpuFastFill(0x03FF03FF, gBackupMapData, sizeof(gBackupMapData));
    VMap.map = gBackupMapData;
    VMap.Xsize = mapLayout->width + 15;
    VMap.Ysize = mapLayout->height + 14;
    AGB_ASSERT_EX(VMap.Xsize * VMap.Ysize <= VIRTUAL_MAP_SIZE, ABSPATH("fieldmap.c"), 158);
    map_copy_with_padding(mapLayout->map, mapLayout->width, mapLayout->height);
    mapheader_copy_mapdata_of_adjacent_maps(mapHeader);
}

static void map_copy_with_padding(u16 *map, u16 width, u16 height)
{
    s32 y;
    u16 *dest = VMap.map;
    dest += VMap.Xsize * 7 + 7;

    for (y = 0; y < height; y++)
    {
        CpuCopy16(map, dest, width * sizeof(u16));
        dest += width + 15;
        map += width;
    }
}

static void mapheader_copy_mapdata_of_adjacent_maps(struct MapHeader *mapHeader)
{
    s32 count;
    struct MapConnection *connection;
    s32 i;

    gMapConnectionFlags = sDummyConnectionFlags;

    /*
     * This null pointer check is new to FireRed.  It was kept in
     * Emerald, with the above struct assignment moved to after
     * this check.
     */
    if (mapHeader->connections)
    {
        count = mapHeader->connections->count;
        connection = mapHeader->connections->connections;
        // Emerald puts this line here instead:
        // gMapConnectionFlags = sDummyConnectionFlags;
        for (i = 0; i < count; i++, connection++)
        {
            struct MapHeader const *cMap = mapconnection_get_mapheader(connection);
            u32 offset = connection->offset;
            switch (connection->direction)
            {
                case CONNECTION_SOUTH:
                    fillSouthConnection(mapHeader, cMap, offset);
                    gMapConnectionFlags.south = 1;
                    break;
                case CONNECTION_NORTH:
                    fillNorthConnection(mapHeader, cMap, offset);
                    gMapConnectionFlags.north = 1;
                    break;
                case CONNECTION_WEST:
                    fillWestConnection(mapHeader, cMap, offset);
                    gMapConnectionFlags.west = 1;
                    break;
                case CONNECTION_EAST:
                    fillEastConnection(mapHeader, cMap, offset);
                    gMapConnectionFlags.east = 1;
                    break;
            }
        }
    }
}

static void sub_8058B54(s32 x, s32 y, const struct MapHeader *connectedMapHeader, s32 x2, s32 y2, s32 width, s32 height)
{
    s32 i;
    u16 *src;
    u16 *dest;
    s32 mapWidth;

    mapWidth = connectedMapHeader->mapLayout->width;
    src = &connectedMapHeader->mapLayout->map[mapWidth * y2 + x2];
    dest = &VMap.map[VMap.Xsize * y + x];

    for (i = 0; i < height; i++)
    {
        CpuCopy16(src, dest, width * 2);
        dest += VMap.Xsize;
        src += mapWidth;
    }
}

static void fillSouthConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset)
{
    s32 x, y;
    s32 x2;
    s32 width;
    s32 cWidth;

    if (connectedMapHeader)
    {
        cWidth = connectedMapHeader->mapLayout->width;
        x = offset + 7;
        y = mapHeader->mapLayout->height + 7;
        if (x < 0)
        {
            x2 = -x;
            x += cWidth;
            if (x < VMap.Xsize)
            {
                width = x;
            }
            else
            {
                width = VMap.Xsize;
            }
            x = 0;
        }
        else
        {
            x2 = 0;
            if (x + cWidth < VMap.Xsize)
            {
                width = cWidth;
            }
            else
            {
                width = VMap.Xsize - x;
            }
        }

        sub_8058B54(
            x, y,
            connectedMapHeader,
            x2, /*y2*/ 0,
            width, /*height*/ 7);
    }
}

static void fillNorthConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset)
{
    s32 x;
    s32 x2, y2;
    s32 width;
    s32 cWidth, cHeight;

    if (connectedMapHeader)
    {
        cWidth = connectedMapHeader->mapLayout->width;
        cHeight = connectedMapHeader->mapLayout->height;
        x = offset + 7;
        y2 = cHeight - 7;
        if (x < 0)
        {
            x2 = -x;
            x += cWidth;
            if (x < VMap.Xsize)
            {
                width = x;
            }
            else
            {
                width = VMap.Xsize;
            }
            x = 0;
        }
        else
        {
            x2 = 0;
            if (x + cWidth < VMap.Xsize)
            {
                width = cWidth;
            }
            else
            {
                width = VMap.Xsize - x;
            }
        }

        sub_8058B54(
            x, /*y*/ 0,
            connectedMapHeader,
            x2, y2,
            width, /*height*/ 7);

    }
}

static void fillWestConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset)
{
    s32 y;
    s32 x2, y2;
    s32 height;
    s32 cWidth, cHeight;
    if (connectedMapHeader)
    {
        cWidth = connectedMapHeader->mapLayout->width;
        cHeight = connectedMapHeader->mapLayout->height;
        y = offset + 7;
        x2 = cWidth - 7;
        if (y < 0)
        {
            y2 = -y;
            if (y + cHeight < VMap.Ysize)
            {
                height = y + cHeight;
            }
            else
            {
                height = VMap.Ysize;
            }
            y = 0;
        }
        else
        {
            y2 = 0;
            if (y + cHeight < VMap.Ysize)
            {
                height = cHeight;
            }
            else
            {
                height = VMap.Ysize - y;
            }
        }

        sub_8058B54(
            /*x*/ 0, y,
            connectedMapHeader,
            x2, y2,
            /*width*/ 7, height);
    }
}

static void fillEastConnection(struct MapHeader const *mapHeader, struct MapHeader const *connectedMapHeader, s32 offset)
{
    s32 x, y;
    s32 y2;
    s32 height;
    s32 cHeight;
    if (connectedMapHeader)
    {
        cHeight = connectedMapHeader->mapLayout->height;
        x = mapHeader->mapLayout->width + 7;
        y = offset + 7;
        if (y < 0)
        {
            y2 = -y;
            if (y + cHeight < VMap.Ysize)
            {
                height = y + cHeight;
            }
            else
            {
                height = VMap.Ysize;
            }
            y = 0;
        }
        else
        {
            y2 = 0;
            if (y + cHeight < VMap.Ysize)
            {
                height = cHeight;
            }
            else
            {
                height = VMap.Ysize - y;
            }
        }

        sub_8058B54(
            x, y,
            connectedMapHeader,
            /*x2*/ 0, y2,
            /*width*/ 8, height);
    }
}

union Block
{
    struct
    {
        u16 block:10;
        u16 collision:2;
        u16 elevation:4;
    } block;
    u16 value;
};

#define MapGridGetBorderTileAt(x, y) ({                                                            \
    u16 block;                                                                                     \
    s32 xprime;                                                                                    \
    s32 yprime;                                                                                    \
                                                                                                   \
    const struct MapLayout *mapLayout = gMapHeader.mapLayout;                                      \
                                                                                                   \
    xprime = x - 7;                                                                                \
    xprime += 8 * mapLayout->borderWidth;                                                          \
    xprime %= mapLayout->borderWidth;                                                              \
                                                                                                   \
    yprime = y - 7;                                                                                \
    yprime += 8 * mapLayout->borderHeight;                                                         \
    yprime %= mapLayout->borderHeight;                                                             \
                                                                                                   \
    block = mapLayout->border[xprime + yprime * mapLayout->borderWidth] | METATILE_COLLISION_MASK; \
})

#define AreCoordsWithinMapGridBounds(x, y) (x >= 0 && x < VMap.Xsize && y >= 0 && y < VMap.Ysize)

#define MapGridGetTileAt(x, y) (AreCoordsWithinMapGridBounds(x, y) ? VMap.map[x + VMap.Xsize * y] : MapGridGetBorderTileAt(x, y))

u8 MapGridGetZCoordAt(s32 x, s32 y)
{
    u16 block = MapGridGetTileAt(x, y);

    if (block == 0x3ff)
    {
        return 0;
    }

    return block >> 12;
}

u8 MapGridIsImpassableAt(s32 x, s32 y)
{

    u16 block = MapGridGetTileAt(x, y);

    if (block == 0x3ff)
    {
        return 1;
    }

    return (block & 0xc00) >> 10;
}

u32 MapGridGetMetatileIdAt(s32 x, s32 y)
{
    u16 block = MapGridGetTileAt(x, y);

    if (block == 0x3FF)
    {
        return MapGridGetBorderTileAt(x, y) & 0x3FF;
    }
    return block & 0x3FF;
}

u32 GetMetatileAttributeFromRawMetatileBehavior(u32 original, u8 bit)
{
    if (bit >= METATILE_ATTRIBUTE_COUNT)
        return original;

    return (original & sMetatileAttrMasks[bit]) >> sMetatileAttrShifts[bit];
}

u32 MapGridGetMetatileAttributeAt(s16 x, s16 y, u8 attr)
{
    u16 metatileId = MapGridGetMetatileIdAt(x, y);
    return GetBehaviorByMetatileIdAndMapLayout(gMapHeader.mapLayout, metatileId, attr);
}

u32 MapGridGetMetatileBehaviorAt(s16 x, s16 y)
{
    return MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_BEHAVIOR);
}

u8 MapGridGetMetatileLayerTypeAt(s16 x, s16 y)
{
    return MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_LAYER_TYPE);
}

void MapGridSetMetatileIdAt(s32 x, s32 y, u16 metatile)
{
    s32 i;
    if (x >= 0 && x < VMap.Xsize
        && y >= 0 && y < VMap.Ysize)
    {
        i = x + y * VMap.Xsize;
        VMap.map[i] = (VMap.map[i] & 0xf000) | (metatile & 0xfff);
    }
}

void MapGridSetMetatileEntryAt(s32 x, s32 y, u16 metatile)
{
    s32 i;
    if (x >= 0 && x < VMap.Xsize
        && y >= 0 && y < VMap.Ysize)
    {
        i = x + VMap.Xsize * y;
        VMap.map[i] = metatile;
    }
}

void MapGridSetMetatileImpassabilityAt(s32 x, s32 y, bool32 arg2)
{
    if (x >= 0 && x < VMap.Xsize
        && y >= 0 && y < VMap.Ysize)
    {
        if (arg2)
        {
            VMap.map[x + VMap.Xsize * y] |= 0x0C00;
        }
        else
        {
            VMap.map[x + VMap.Xsize * y] &= ~0x0C00;
        }
    }
}

u32 GetBehaviorByMetatileIdAndMapLayout(const struct MapLayout *mapLayout, u16 metatile, u8 attr)
{
    u32 * attributes;

    if (metatile < NUM_METATILES_IN_PRIMARY)
    {
        attributes = mapLayout->primaryTileset->metatileAttributes;
        return GetMetatileAttributeFromRawMetatileBehavior(attributes[metatile], attr);
    }
    else if (metatile < 0x400)
    {
        attributes = mapLayout->secondaryTileset->metatileAttributes;
        return GetMetatileAttributeFromRawMetatileBehavior(attributes[metatile - NUM_METATILES_IN_PRIMARY], attr);
    }
    else
    {
        return 0xFF;
    }
}

void save_serialize_map(void)
{
    s32 i, j;
    s32 x, y;
    u16 *mapView;
    s32 width;
    mapView = gSaveBlock2Ptr->mapView;
    width = VMap.Xsize;
    x = gSaveBlock1Ptr->pos.x;
    y = gSaveBlock1Ptr->pos.y;
    for (i = y; i < y + 14; i++)
    {
        for (j = x; j < x + 15; j++)
        {
            *mapView++ = gBackupMapData[width * i + j];
        }
    }
}

static bool32 SavedMapViewIsEmpty(void)
{
    u16 i;
    u32 marker = 0;

#ifndef UBFIX
    // BUG: This loop extends past the bounds of the mapView array. Its size is only 0x100.
    for (i = 0; i < 0x200; i++)
        marker |= gSaveBlock2Ptr->mapView[i];
#else
    for (i = 0; i < NELEMS(gSaveBlock2Ptr->mapView); i++)
        marker |= gSaveBlock2Ptr->mapView[i];
#endif
    if (marker == 0)
        return TRUE;
    else
        return FALSE;
}

static void ClearSavedMapView(void)
{
    CpuFill16(0, gSaveBlock2Ptr->mapView, sizeof(gSaveBlock2Ptr->mapView));
}

static void LoadSavedMapView(void)
{
    s32 i, j;
    s32 x, y;
    u16 *mapView;
    s32 width;
    mapView = gSaveBlock2Ptr->mapView;
    if (!SavedMapViewIsEmpty())
    {
        width = VMap.Xsize;
        x = gSaveBlock1Ptr->pos.x;
        y = gSaveBlock1Ptr->pos.y;
        for (i = y; i < y + 14; i++)
        {
            for (j = x; j < x + 15; j++)
            {
                gBackupMapData[j + width * i] = *mapView;
                mapView++;
            }
        }
        ClearSavedMapView();
    }
}

static void sub_8059250(u8 a1)
{
    s32 width;
    u16 *mapView;
    s32 x0, y0;
    s32 x2, y2;
    u16 *src, *dest;
    s32 srci, desti;
    s32 r9, r8;
    s32 x, y;
    s32 i, j;
    mapView = gSaveBlock2Ptr->mapView;
    width = VMap.Xsize;
    r9 = 0;
    r8 = 0;
    x0 = gSaveBlock1Ptr->pos.x;
    y0 = gSaveBlock1Ptr->pos.y;
    x2 = 15;
    y2 = 14;
    switch (a1)
    {
        case CONNECTION_NORTH:
            y0 += 1;
            y2 = 13;
            break;
        case CONNECTION_SOUTH:
            r8 = 1;
            y2 = 13;
            break;
        case CONNECTION_WEST:
            x0 += 1;
            x2 = 14;
            break;
        case CONNECTION_EAST:
            r9 = 1;
            x2 = 14;
            break;
    }
    for (y = 0; y < y2; y++)
    {
        i = 0;
        j = 0;
        for (x = 0; x < x2; x++)
        {
            desti = width * (y + y0);
            srci = (y + r8) * 15 + r9;
            src = &mapView[srci + i];
            dest = &gBackupMapData[x0 + desti + j];
            *dest = *src;
            i++;
            j++;
        }
    }
    ClearSavedMapView();
}

s32 GetMapBorderIdAt(s32 x, s32 y)
{
    if (MapGridGetTileAt(x, y) == 0x3FF)
    {
        return -1;
    }

    if (x >= VMap.Xsize - 8)
    {
        if (!gMapConnectionFlags.east)
        {
            return -1;
        }
        return CONNECTION_EAST;
    }

    if (x < 7)
    {
        if (!gMapConnectionFlags.west)
        {
            return -1;
        }
        return CONNECTION_WEST;
    }

    if (y >= VMap.Ysize - 7)
    {
        if (!gMapConnectionFlags.south)
        {
            return -1;
        }
        return CONNECTION_SOUTH;
    }

    if (y < 7)
    {
        if (!gMapConnectionFlags.north)
        {
            return -1;
        }
        return CONNECTION_NORTH;
    }

    return 0;
}

static s32 GetPostCameraMoveMapBorderId(s32 x, s32 y)
{
    return GetMapBorderIdAt(7 + gSaveBlock1Ptr->pos.x + x, 7 + gSaveBlock1Ptr->pos.y + y);
}

bool32 CanCameraMoveInDirection(s32 direction)
{
    s32 x, y;
    
    x = gSaveBlock1Ptr->pos.x + 7 + gDirectionToVectors[direction].x;
    y = gSaveBlock1Ptr->pos.y + 7 + gDirectionToVectors[direction].y;
    if (GetMapBorderIdAt(x, y) == -1)
    {
        return FALSE;
    }
    return TRUE;
}

static void sub_80594AC(struct MapConnection *connection, int direction, s32 x, s32 y)
{
    struct MapHeader const *mapHeader;
    mapHeader = mapconnection_get_mapheader(connection);
    switch (direction)
    {
        case CONNECTION_EAST:
            gSaveBlock1Ptr->pos.x = -x;
            gSaveBlock1Ptr->pos.y -= connection->offset;
            break;
        case CONNECTION_WEST:
            gSaveBlock1Ptr->pos.x = mapHeader->mapLayout->width;
            gSaveBlock1Ptr->pos.y -= connection->offset;
            break;
        case CONNECTION_SOUTH:
            gSaveBlock1Ptr->pos.x -= connection->offset;
            gSaveBlock1Ptr->pos.y = -y;
            break;
        case CONNECTION_NORTH:
            gSaveBlock1Ptr->pos.x -= connection->offset;
            gSaveBlock1Ptr->pos.y = mapHeader->mapLayout->height;
            break;
    }
}

bool8 CameraMove(s32 x, s32 y)
{
    u32 direction;
    struct MapConnection *connection;
    s32 old_x, old_y;
    gCamera.active = FALSE;
    direction = GetPostCameraMoveMapBorderId(x, y);
    if (direction + 1 <= 1)
    {
        gSaveBlock1Ptr->pos.x += x;
        gSaveBlock1Ptr->pos.y += y;
    }
    else
    {
        save_serialize_map();
        old_x = gSaveBlock1Ptr->pos.x;
        old_y = gSaveBlock1Ptr->pos.y;
        connection = sub_8059600(direction, gSaveBlock1Ptr->pos.x, gSaveBlock1Ptr->pos.y);
        sub_80594AC(connection, direction, x, y);
        LoadMapFromCameraTransition(connection->mapGroup, connection->mapNum);
        gCamera.active = TRUE;
        gCamera.x = old_x - gSaveBlock1Ptr->pos.x;
        gCamera.y = old_y - gSaveBlock1Ptr->pos.y;
        gSaveBlock1Ptr->pos.x += x;
        gSaveBlock1Ptr->pos.y += y;
        sub_8059250(direction);
    }
    return gCamera.active;
}

struct MapConnection *sub_8059600(u8 direction, s32 x, s32 y)
{
    s32 count;
    struct MapConnection *connection;
    const struct MapConnections *connections = gMapHeader.connections;
    s32 i;
    // UB: Multiple possible null dereferences
#ifdef UBFIX
    if (connections != NULL)
    {
        count = connections->count;
        connection = connections->connections;
        if (connection != NULL)
        {
            for (i = 0; i < count; i++, connection++)
            {
                if (connection->direction == direction && sub_8059658(direction, x, y, connection) == TRUE)
                    return connection;
            }
        }
    }
#else
    count = connections->count;
    connection = connections->connections;
    for (i = 0; i < count; i++, connection++)
    {
        if (connection->direction == direction && sub_8059658(direction, x, y, connection) == TRUE)
            return connection;
    }
#endif
    return NULL;

}

static bool8 sub_8059658(u8 direction, s32 x, s32 y, struct MapConnection *connection)
{
    struct MapHeader const *mapHeader;
    mapHeader = mapconnection_get_mapheader(connection);
    switch (direction)
    {
        case CONNECTION_SOUTH:
        case CONNECTION_NORTH:
            return sub_80596BC(x, gMapHeader.mapLayout->width, mapHeader->mapLayout->width, connection->offset);
        case CONNECTION_WEST:
        case CONNECTION_EAST:
            return sub_80596BC(y, gMapHeader.mapLayout->height, mapHeader->mapLayout->height, connection->offset);
    }
    return FALSE;
}

static bool8 sub_80596BC(s32 x, s32 src_width, s32 dest_width, s32 offset)
{
    s32 offset2 = max(offset, 0);

    if (dest_width + offset < src_width)
        src_width = dest_width + offset;

    if (offset2 <= x && x <= src_width)
        return TRUE;

    return FALSE;
}

static bool32 sub_80596E8(s32 x, s32 width)
{
    if (x >= 0 && x < width)
        return TRUE;

    return FALSE;
}

static s32 sub_80596FC(struct MapConnection *connection, s32 x, s32 y)
{
    struct MapHeader const *mapHeader;
    mapHeader = mapconnection_get_mapheader(connection);
    switch (connection->direction)
    {
        case CONNECTION_SOUTH:
        case CONNECTION_NORTH:
            return sub_80596E8(x - connection->offset, mapHeader->mapLayout->width);
        case CONNECTION_WEST:
        case CONNECTION_EAST:
            return sub_80596E8(y - connection->offset, mapHeader->mapLayout->height);
    }
    return FALSE;
}

struct MapConnection *GetMapConnectionAtPos(s16 x, s16 y)
{
    s32 count;
    struct MapConnection *connection;
    s32 i;
    u8 direction;
    if (!gMapHeader.connections)
    {
        return NULL;
    }
    else
    {
        count = gMapHeader.connections->count;
        connection = gMapHeader.connections->connections;
        for (i = 0; i < count; i++, connection++)
        {
            direction = connection->direction;
            if ((direction == CONNECTION_DIVE || direction == CONNECTION_EMERGE)
                || (direction == CONNECTION_NORTH && y > 6)
                || (direction == CONNECTION_SOUTH && y < gMapHeader.mapLayout->height + 7)
                || (direction == CONNECTION_WEST && x > 6)
                || (direction == CONNECTION_EAST && x < gMapHeader.mapLayout->width + 7))
            {
                continue;
            }
            if (sub_80596FC(connection, x - 7, y - 7) == TRUE)
            {
                return connection;
            }
        }
    }
    return NULL;
}

void SetCameraFocusCoords(u16 x, u16 y)
{
    gSaveBlock1Ptr->pos.x = x - 7;
    gSaveBlock1Ptr->pos.y = y - 7;
}

void GetCameraFocusCoords(u16 *x, u16 *y)
{
    *x = gSaveBlock1Ptr->pos.x + 7;
    *y = gSaveBlock1Ptr->pos.y + 7;
}

static void SetCameraCoords(u16 x, u16 y)
{
    gSaveBlock1Ptr->pos.x = x;
    gSaveBlock1Ptr->pos.y = y;
}

void GetCameraCoords(u16 *x, u16 *y)
{
    *x = gSaveBlock1Ptr->pos.x;
    *y = gSaveBlock1Ptr->pos.y;
}
static void copy_tileset_patterns_to_vram(struct Tileset const *tileset, u16 numTiles, u16 offset)
{
    if (tileset)
    {
        if (!tileset->isCompressed)
            LoadBgTiles(2, tileset->tiles, numTiles * 32, offset);
        else
            DecompressAndCopyTileDataToVram2(2, tileset->tiles, numTiles * 32, offset, 0);
    }
}

static void copy_tileset_patterns_to_vram2(struct Tileset const *tileset, u16 numTiles, u16 offset)
{
    if (tileset)
    {
        if (!tileset->isCompressed)
            LoadBgTiles(2, tileset->tiles, numTiles * 32, offset);
        else
            DecompressAndLoadBgGfxUsingHeap2(2, tileset->tiles, numTiles * 32, offset, 0);
    }
}

static void Fieldmap_ApplyGlobalTintToPaletteEntries(u16 offset, u16 size)
{
    switch (gGlobalFieldTintMode)
    {
        case QL_TINT_NONE:
            return;
        case QL_TINT_GRAYSCALE:
            TintPalette_GrayScale(gPlttBufferUnfaded + offset, size);
            break;
        case QL_TINT_SEPIA:
            TintPalette_SepiaTone(gPlttBufferUnfaded + offset, size);
            break;
        case QL_TINT_BACKUP_GRAYSCALE:
            QuestLog_BackUpPalette(offset, size);
            TintPalette_GrayScale(gPlttBufferUnfaded + offset, size);
            break;
        default:
            return;
    }
    CpuCopy16(gPlttBufferUnfaded + offset, gPlttBufferFaded + offset, size * sizeof(u16));
}

void Fieldmap_ApplyGlobalTintToPaletteSlot(u8 slot, u8 count)
{
    switch (gGlobalFieldTintMode)
    {
        case QL_TINT_NONE:
            return;
        case QL_TINT_GRAYSCALE:
            TintPalette_GrayScale(gPlttBufferUnfaded + slot * 16, count * 16);
            break;
        case QL_TINT_SEPIA:
            TintPalette_SepiaTone(gPlttBufferUnfaded + slot * 16, count * 16);
            break;
        case QL_TINT_BACKUP_GRAYSCALE:
            QuestLog_BackUpPalette(slot * 16, count * 16);
            TintPalette_GrayScale(gPlttBufferUnfaded + slot * 16, count * 16);
            break;
        default:
            return;
    }
    CpuFastCopy(gPlttBufferUnfaded + slot * 16, gPlttBufferFaded + slot * 16, count * 16 * sizeof(u16));
}

static void apply_map_tileset_palette(struct Tileset const *tileset, u16 destOffset, u16 size)
{
    u16 black = RGB_BLACK;

    if (tileset)
    {
        if (tileset->isSecondary == FALSE)
        {
            LoadPalette(&black, destOffset, 2);
            LoadPalette(((u16*)tileset->palettes) + 1, destOffset + 1, size - 2);
            Fieldmap_ApplyGlobalTintToPaletteEntries(destOffset + 1, (size - 2) >> 1);
        }
        else if (tileset->isSecondary == TRUE)
        {
            LoadPalette(((u16*)tileset->palettes) + (NUM_PALS_IN_PRIMARY * 16), destOffset, size);
            Fieldmap_ApplyGlobalTintToPaletteEntries(destOffset, size >> 1);
        }
        else
        {
            LoadCompressedPalette((u32*)tileset->palettes, destOffset, size);
            Fieldmap_ApplyGlobalTintToPaletteEntries(destOffset, size >> 1);
        }
    }
}

void copy_map_tileset1_to_vram(const struct MapLayout *mapLayout)
{
    copy_tileset_patterns_to_vram(mapLayout->primaryTileset, NUM_TILES_IN_PRIMARY, 0);
}

void copy_map_tileset2_to_vram(const struct MapLayout *mapLayout)
{
    copy_tileset_patterns_to_vram(mapLayout->secondaryTileset, NUM_TILES_TOTAL - NUM_TILES_IN_PRIMARY, NUM_TILES_IN_PRIMARY);
}

void copy_map_tileset2_to_vram_2(const struct MapLayout *mapLayout)
{
    copy_tileset_patterns_to_vram2(mapLayout->secondaryTileset, NUM_TILES_TOTAL - NUM_TILES_IN_PRIMARY, NUM_TILES_IN_PRIMARY);
}

void apply_map_tileset1_palette(const struct MapLayout *mapLayout)
{
    apply_map_tileset_palette(mapLayout->primaryTileset, 0, NUM_PALS_IN_PRIMARY * 16 * 2);
}

void apply_map_tileset2_palette(const struct MapLayout *mapLayout)
{
    apply_map_tileset_palette(mapLayout->secondaryTileset, NUM_PALS_IN_PRIMARY * 16, (NUM_PALS_TOTAL - NUM_PALS_IN_PRIMARY) * 16 * 2);
}

void copy_map_tileset1_tileset2_to_vram(struct MapLayout const *mapLayout)
{
    if (mapLayout)
    {
        copy_tileset_patterns_to_vram2(mapLayout->primaryTileset, NUM_TILES_IN_PRIMARY, 0);
        copy_tileset_patterns_to_vram2(mapLayout->secondaryTileset, NUM_TILES_TOTAL - NUM_TILES_IN_PRIMARY, NUM_TILES_IN_PRIMARY);
    }
}

void apply_map_tileset1_tileset2_palette(struct MapLayout const *mapLayout)
{
    if (mapLayout)
    {
        apply_map_tileset1_palette(mapLayout);
        apply_map_tileset2_palette(mapLayout);
    }
}
