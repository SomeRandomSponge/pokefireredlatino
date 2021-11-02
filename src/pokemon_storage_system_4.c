#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "graphics.h"
#include "new_menu_helpers.h"
#include "pokemon_icon.h"
#include "pokemon_storage_system_internal.h"
#include "strings.h"
#include "task.h"
#include "trig.h"

static void sub_8090324(struct Sprite * sprite);
static void SetBoxSpeciesAndPersonalities(u8 boxId);
static void sub_8090A74(struct Sprite * sprite, u16 idx);
static void sub_8090AE0(struct Sprite * sprite);
static void DestroyBoxMonIcon(struct Sprite * sprite);
static void sub_80911B0(struct Sprite * sprite);
static void sub_8091420(u8 taskId);
static s8 sub_80916F4(u8 boxId);
static void LoadWallpaperGfx(u8 wallpaperId, s8 direction);
static bool32 WaitForWallpaperGfxLoad(void);
static void sub_8091984(void *buffer, const void *buffer2, s8 direction, u8 baseBlock);
static void sub_8091A24(void *buffer);
static void sub_8091A94(u8 wallpaperId);
static void sub_8091C48(u8 wallpaperId, s8 direction);
static void sub_8091E84(struct Sprite * sprite);
static void sub_8091EB8(struct Sprite * sprite);
static s16 sub_8091F60(const u8 *boxName);
static void sub_8091E34(void);
static void sub_8091EF0(void);
static void sub_8091F80(void);
static void sub_809200C(s8 direction);
static void sub_80920AC(void);
static void sub_8092164(struct Sprite * sprite);

static const struct OamData gUnknown_83CEC08;

static const struct SpriteTemplate gUnknown_83CEBF0 = {
    .tileTag = TAG_TILE_12,
    .paletteTag = TAG_PAL_DAC0,
    .oam = &gUnknown_83CEC08,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData gUnknown_83CEC08 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0
};

static const union AffineAnimCmd gUnknown_83CEC10[] = {
    AFFINEANIMCMD_FRAME(-2, -2, 0, 120),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd gUnknown_83CEC20[] = {
    AFFINEANIMCMD_FRAME(16, 16, 0,  0),
    AFFINEANIMCMD_FRAME(16, 16, 0, 15),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const gUnknown_83CEC38[] = {
    gUnknown_83CEC10,
    gUnknown_83CEC20
};

static const u16 gUnknown_83CEC40[] = INCBIN_U16("graphics/interface/pss_unk_83CEC40.gbapal");
static const u32 gUnknown_83CEC80[] = INCBIN_U32("graphics/interface/pss_unk_83CEC80.4bpp.lz");
static const u32 gUnknown_83CF050[] = INCBIN_U32("graphics/interface/pss_unk_83CF050.bin.lz");
static const u16 gUnknown_83CF12C[] = INCBIN_U16("graphics/interface/pss_unk_83CF12C.gbapal");
static const u32 gUnknown_83CF16C[] = INCBIN_U32("graphics/interface/pss_unk_83CF16C.4bpp.lz");
static const u32 gUnknown_83CF374[] = INCBIN_U32("graphics/interface/pss_unk_83CF374.bin.lz");
static const u16 gUnknown_83CF424[] = INCBIN_U16("graphics/interface/pss_unk_83CF424.gbapal");
static const u32 gUnknown_83CF464[] = INCBIN_U32("graphics/interface/pss_unk_83CF464.4bpp.lz");
static const u32 gUnknown_83CF750[] = INCBIN_U32("graphics/interface/pss_unk_83CF750.bin.lz");
static const u16 gUnknown_83CF834[] = INCBIN_U16("graphics/interface/pss_unk_83CF834.gbapal");
static const u32 gUnknown_83CF874[] = INCBIN_U32("graphics/interface/pss_unk_83CF874.4bpp.lz");
static const u32 gUnknown_83CFA94[] = INCBIN_U32("graphics/interface/pss_unk_83CFA94.bin.lz");
static const u16 gUnknown_83CFB60[] = INCBIN_U16("graphics/interface/pss_unk_83CFB60.gbapal");
static const u32 gUnknown_83CFBA0[] = INCBIN_U32("graphics/interface/pss_unk_83CFBA0.4bpp.lz");
static const u32 gUnknown_83CFEF0[] = INCBIN_U32("graphics/interface/pss_unk_83CFEF0.bin.lz");
static const u16 gUnknown_83CFFC8[] = INCBIN_U16("graphics/interface/pss_unk_83CFFC8.gbapal");
static const u32 gUnknown_83D0008[] = INCBIN_U32("graphics/interface/pss_unk_83D0008.4bpp.lz");
static const u8 sSpace_83D0338[4] = {};
static const u32 gUnknown_83D033C[] = INCBIN_U32("graphics/interface/pss_unk_83D033C.bin.lz");
static const u16 gUnknown_83D0414[] = INCBIN_U16("graphics/interface/pss_unk_83D0414.gbapal");
static const u32 gUnknown_83D0454[] = INCBIN_U32("graphics/interface/pss_unk_83D0454.4bpp.lz");
static const u32 gUnknown_83D070C[] = INCBIN_U32("graphics/interface/pss_unk_83D070C.bin.lz");
static const u16 gUnknown_83D07D8[] = INCBIN_U16("graphics/interface/pss_unk_83D07D8.gbapal");
static const u32 gUnknown_83D0818[] = INCBIN_U32("graphics/interface/pss_unk_83D0818.4bpp.lz");
static const u32 gUnknown_83D0B5C[] = INCBIN_U32("graphics/interface/pss_unk_83D0B5C.bin.lz");
static const u16 gUnknown_83D0C38[] = INCBIN_U16("graphics/interface/pss_unk_83D0C38.gbapal");
static const u32 gUnknown_83D0C78[] = INCBIN_U32("graphics/interface/pss_unk_83D0C78.4bpp.lz");
static const u32 gUnknown_83D0FFC[] = INCBIN_U32("graphics/interface/pss_unk_83D0FFC.bin.lz");
static const u16 gUnknown_83D10E4[] = INCBIN_U16("graphics/interface/pss_unk_83D10E4.gbapal");
static const u32 gUnknown_83D1124[] = INCBIN_U32("graphics/interface/pss_unk_83D1124.4bpp.lz");
static const u32 gUnknown_83D13D8[] = INCBIN_U32("graphics/interface/pss_unk_83D13D8.bin.lz");
static const u16 gUnknown_83D14B4[] = INCBIN_U16("graphics/interface/pss_unk_83D14B4.gbapal");
static const u32 gUnknown_83D14F4[] = INCBIN_U32("graphics/interface/pss_unk_83D14F4.4bpp.lz");
static const u32 gUnknown_83D1788[] = INCBIN_U32("graphics/interface/pss_unk_83D1788.bin.lz");
static const u16 gUnknown_83D1874[] = INCBIN_U16("graphics/interface/pss_unk_83D1874.gbapal");
static const u32 gUnknown_83D18B4[] = INCBIN_U32("graphics/interface/pss_unk_83D18B4.4bpp.lz");
static const u32 gUnknown_83D1B4C[] = INCBIN_U32("graphics/interface/pss_unk_83D1B4C.bin.lz");
static const u16 gUnknown_83D1C2C[] = INCBIN_U16("graphics/interface/pss_unk_83D1C2C.gbapal");
static const u8 sSpace_83D1C6C[32] = {};
static const u32 gUnknown_83D1C8C[] = INCBIN_U32("graphics/interface/pss_unk_83D1C8C.4bpp.lz");
static const u32 gUnknown_83D1EC4[] = INCBIN_U32("graphics/interface/pss_unk_83D1EC4.bin.lz");
static const u16 gUnknown_83D1F94[] = INCBIN_U16("graphics/interface/pss_unk_83D1F94.gbapal");
static const u32 gUnknown_83D1FD4[] = INCBIN_U32("graphics/interface/pss_unk_83D1FD4.4bpp.lz");
static const u32 gUnknown_83D22B8[] = INCBIN_U32("graphics/interface/pss_unk_83D22B8.bin.lz");
static const u16 gUnknown_83D239C[] = INCBIN_U16("graphics/interface/pss_unk_83D239C.gbapal");
static const u32 gUnknown_83D23DC[] = INCBIN_U32("graphics/interface/pss_unk_83D23DC.4bpp.lz");
static const u32 gUnknown_83D256C[] = INCBIN_U32("graphics/interface/pss_unk_83D256C.bin.lz");
static const u16 gUnknown_83D2614[] = INCBIN_U16("graphics/interface/pss_unk_83D2614.gbapal");
static const u32 gUnknown_83D2654[] = INCBIN_U32("graphics/interface/pss_unk_83D2654.4bpp.lz");
static const u32 gUnknown_83D277C[] = INCBIN_U32("graphics/interface/pss_unk_83D277C.bin.lz");
static const u16 gUnknown_83D2820[] = INCBIN_U16("graphics/interface/pss_unk_83D2820.bin");

static const u16 gUnknown_83D29D0[][2] = {
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)},
    {RGB( 7,  7,  7), RGB(31, 31, 31)}
};

static const struct WallpaperTable sWallpaperTable[] = {
    {gUnknown_83CEC80, gUnknown_83CF050, gUnknown_83CEC40},
    {gUnknown_83CF16C, gUnknown_83CF374, gUnknown_83CF12C},
    {gUnknown_83CF464, gUnknown_83CF750, gUnknown_83CF424},
    {gUnknown_83CF874, gUnknown_83CFA94, gUnknown_83CF834},
    {gUnknown_83CFBA0, gUnknown_83CFEF0, gUnknown_83CFB60},
    {gUnknown_83D0008, gUnknown_83D033C, gUnknown_83CFFC8},
    {gUnknown_83D0454, gUnknown_83D070C, gUnknown_83D0414},
    {gUnknown_83D0818, gUnknown_83D0B5C, gUnknown_83D07D8},
    {gUnknown_83D0C78, gUnknown_83D0FFC, gUnknown_83D0C38},
    {gUnknown_83D1124, gUnknown_83D13D8, gUnknown_83D10E4},
    {gUnknown_83D14F4, gUnknown_83D1788, gUnknown_83D14B4},
    {gUnknown_83D18B4, gUnknown_83D1B4C, gUnknown_83D1874},
    {gUnknown_83D1C8C, gUnknown_83D1EC4, gUnknown_83D1C2C},
    {gUnknown_83D1FD4, gUnknown_83D22B8, gUnknown_83D1F94},
    {gUnknown_83D23DC, gUnknown_83D256C, gUnknown_83D239C},
    {gUnknown_83D2654, gUnknown_83D277C, gUnknown_83D2614},
};

static const u16 gUnknown_83D2AD0[] = INCBIN_U16("graphics/interface/pss_unk_83D2AD0.4bpp");
static const u8 sUnref_83D2B50[] = {0xba, 0x23};

static const struct SpriteSheet gUnknown_83D2B54 = {
    gUnknown_83D2AD0, 0x0080, TAG_TILE_6
};

static const struct OamData gUnknown_83D2B5C = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0
};

static const union AnimCmd gUnknown_83D2B64[] = {
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END
};

static const union AnimCmd gUnknown_83D2B6C[] = {
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_END
};

static const union AnimCmd *const gUnknown_83D2B74[] = {
    gUnknown_83D2B64,
    gUnknown_83D2B6C
};

static const struct SpriteTemplate gUnknown_83D2B7C = {
    .tileTag = TAG_TILE_3,
    .paletteTag = TAG_PAL_DAC9,
    .oam = &gUnknown_83D2B5C,
    .anims = gUnknown_83D2B74,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData gUnknown_83D2B94 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0x000,
    .priority = 2,
    .paletteNum = 0
};

static const union AnimCmd gUnknown_83D2B9C[] = {
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END
};

static const union AnimCmd gUnknown_83D2BA4[] = {
    ANIMCMD_FRAME(2, 5),
    ANIMCMD_END
};

static const union AnimCmd *const gUnknown_83D2BAC[] = {
    gUnknown_83D2B9C,
    gUnknown_83D2BA4
};

static const struct SpriteTemplate gUnknown_83D2BB4 = {
    .tileTag = TAG_TILE_6,
    .paletteTag = TAG_PAL_WAVEFORM,
    .oam = &gUnknown_83D2B94,
    .anims = gUnknown_83D2BAC,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8092164
};

void sub_808FFAC(void)
{
    u16 i;

    LoadMonIconPalettes();
    for (i = 0; i < MAX_MON_ICONS; i++)
        gPSSData->field_B08[i] = 0;
    for (i = 0; i < MAX_MON_ICONS; i++)
        gPSSData->field_B58[i] = 0;
    for (i = 0; i < PARTY_SIZE; i++)
        gPSSData->partySprites[i] = NULL;
    for (i = 0; i < IN_BOX_COUNT; i++)
        gPSSData->boxMonsSprites[i] = NULL;

    gPSSData->movingMonSprite = NULL;
    gPSSData->field_78C = 0;
}

static u8 sub_8090058(void)
{
    return (IsCursorInBox() ? 2 : 1);
}

void CreateMovingMonIcon(void)
{
    u32 personality = GetMonData(&gPSSData->movingMon, MON_DATA_PERSONALITY);
    u16 species = GetMonData(&gPSSData->movingMon, MON_DATA_SPECIES2);
    u8 priority = sub_8090058();

    gPSSData->movingMonSprite = CreateMonIconSprite(species, personality, 0, 0, priority, 7);
    gPSSData->movingMonSprite->callback = sub_80911B0;
}

static void sub_80900D4(u8 boxId)
{
    u8 boxPosition;
    u16 i, j, count;
    u16 species;
    u32 personality;

    count = 0;
    boxPosition = 0;
    for (i = 0; i < IN_BOX_COLUMNS; i++)
    {
        for (j = 0; j < IN_BOX_ROWS; j++)
        {
            species = GetBoxMonDataAt(boxId, boxPosition, MON_DATA_SPECIES2);
            if (species != SPECIES_NONE)
            {
                personality = GetBoxMonDataAt(boxId, boxPosition, MON_DATA_PERSONALITY);
                gPSSData->boxMonsSprites[count] = CreateMonIconSprite(species, personality, 8 * (3 * j) + 100, 8 * (3 * i) + 44, 2, 19 - j);
            }
            else
            {
                gPSSData->boxMonsSprites[count] = NULL;
            }
            boxPosition++;
            count++;
        }
    }

    if (gPSSData->boxOption == BOX_OPTION_MOVE_ITEMS)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++)
        {
            if (GetBoxMonDataAt(boxId, boxPosition, MON_DATA_HELD_ITEM) == 0)
                gPSSData->boxMonsSprites[boxPosition]->oam.objMode = ST_OAM_OBJ_BLEND;
        }
    }
}

void sub_80901EC(u8 boxPosition)
{
    u16 species = GetCurrentBoxMonData(boxPosition, MON_DATA_SPECIES2);

    if (species != SPECIES_NONE)
    {
        s16 x = 8 * (3 * (boxPosition % IN_BOX_ROWS)) + 100;
        s16 y = 8 * (3 * (boxPosition / IN_BOX_ROWS)) + 44;
        u32 personality = GetCurrentBoxMonData(boxPosition, MON_DATA_PERSONALITY);

        gPSSData->boxMonsSprites[boxPosition] = CreateMonIconSprite(species, personality, x, y, 2, 19 - (boxPosition % IN_BOX_ROWS));
        if (gPSSData->boxOption == BOX_OPTION_MOVE_ITEMS)
            gPSSData->boxMonsSprites[boxPosition]->oam.objMode = ST_OAM_OBJ_BLEND;
    }
}

static void sub_809029C(s16 arg0)
{
    u16 i;

    for (i = 0; i < IN_BOX_COUNT; i++)
    {
        if (gPSSData->boxMonsSprites[i] != NULL)
        {
            gPSSData->boxMonsSprites[i]->data[2] = arg0;
            gPSSData->boxMonsSprites[i]->data[4] = 1;
            gPSSData->boxMonsSprites[i]->callback = sub_8090324;
        }
    }
}

static void sub_80902E0(struct Sprite *sprite)
{
    if (sprite->data[1] != 0)
    {
        sprite->data[1]--;
        sprite->x += sprite->data[2];
    }
    else
    {
        gPSSData->field_C66--;
        sprite->x = sprite->data[3];
        sprite->callback = SpriteCallbackDummy;
    }
}

static void sub_8090324(struct Sprite *sprite)
{
    if (sprite->data[4] != 0)
    {
        sprite->data[4]--;
    }
    else
    {
        sprite->x += sprite->data[2];
        sprite->data[5] = sprite->x + sprite->x2;
        if (sprite->data[5] <= 68 || sprite->data[5] >= 252)
            sprite->callback = SpriteCallbackDummy;
    }
}

static void DestroyAllIconsInRow(u8 row)
{
    u16 column;
    u8 boxPosition = row;

    for (column = 0; column < IN_BOX_COLUMNS; column++)
    {
        if (gPSSData->boxMonsSprites[boxPosition] != NULL)
        {
            DestroyBoxMonIcon(gPSSData->boxMonsSprites[boxPosition]);
            gPSSData->boxMonsSprites[boxPosition] = NULL;
        }
        boxPosition += IN_BOX_ROWS;
    }
}

static u8 sub_80903A4(u8 row, u16 times, s16 xDelta)
{
    s32 i;
    u16 y = 44;
    s16 xDest = 8 * (3 * row) + 100;
    u16 x = xDest - ((times + 1) * xDelta);
    u8 subpriority = 19 - row;
    u8 count = 0;
    u8 boxPosition = row;

    if (gPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
    {
        for (i = 0; i < IN_BOX_COLUMNS; i++)
        {
            if (gPSSData->boxSpecies[boxPosition] != SPECIES_NONE)
            {
                gPSSData->boxMonsSprites[boxPosition] = CreateMonIconSprite(gPSSData->boxSpecies[boxPosition],
                                                                            gPSSData->boxPersonalities[boxPosition],
                                                                            x, y, 2, subpriority);
                if (gPSSData->boxMonsSprites[boxPosition] != NULL)
                {
                    gPSSData->boxMonsSprites[boxPosition]->data[1] = times;
                    gPSSData->boxMonsSprites[boxPosition]->data[2] = xDelta;
                    gPSSData->boxMonsSprites[boxPosition]->data[3] = xDest;
                    gPSSData->boxMonsSprites[boxPosition]->callback = sub_80902E0;
                    count++;
                }
            }
            boxPosition += IN_BOX_ROWS;
            y += 24;
        }
    }
    else
    {
        for (i = 0; i < IN_BOX_COLUMNS; i++)
        {
            if (gPSSData->boxSpecies[boxPosition] != SPECIES_NONE)
            {
                gPSSData->boxMonsSprites[boxPosition] = CreateMonIconSprite(gPSSData->boxSpecies[boxPosition],
                                                                            gPSSData->boxPersonalities[boxPosition],
                                                                            x, y, 2, subpriority);
                if (gPSSData->boxMonsSprites[boxPosition] != NULL)
                {
                    gPSSData->boxMonsSprites[boxPosition]->data[1] = times;
                    gPSSData->boxMonsSprites[boxPosition]->data[2] = xDelta;
                    gPSSData->boxMonsSprites[boxPosition]->data[3] = xDest;
                    gPSSData->boxMonsSprites[boxPosition]->callback = sub_80902E0;
                    if (GetBoxMonDataAt(gPSSData->field_C5C, boxPosition, MON_DATA_HELD_ITEM) == 0)
                        gPSSData->boxMonsSprites[boxPosition]->oam.objMode = ST_OAM_OBJ_BLEND;
                    count++;
                }
            }
            boxPosition += IN_BOX_ROWS;
            y += 24;
        }
    }

    return count;
}

static void sub_8090574(u8 boxId, s8 direction)
{
    gPSSData->field_C6A = 0;
    gPSSData->field_C6B = boxId;
    gPSSData->field_C69 = direction;
    gPSSData->field_C60 = 32;
    gPSSData->field_C64 = -(6 * direction);
    gPSSData->field_C66 = 0;
    SetBoxSpeciesAndPersonalities(boxId);
    if (direction > 0)
        gPSSData->field_C68 = 0;
    else
        gPSSData->field_C68 = IN_BOX_ROWS - 1;

    gPSSData->field_C62 = (24 * gPSSData->field_C68) + 100;
    sub_809029C(gPSSData->field_C64);
}

static bool8 sub_809062C(void)
{
    if (gPSSData->field_C60 != 0)
        gPSSData->field_C60--;

    switch (gPSSData->field_C6A)
    {
    case 0:
        gPSSData->field_C62 += gPSSData->field_C64;
        if (gPSSData->field_C62 <= 64 || gPSSData->field_C62 >= 252)
        {
            DestroyAllIconsInRow(gPSSData->field_C68);
            gPSSData->field_C62 += gPSSData->field_C69 * 24;
            gPSSData->field_C6A++;
        }
        break;
    case 1:
        gPSSData->field_C62 += gPSSData->field_C64;
        gPSSData->field_C66 += sub_80903A4(gPSSData->field_C68, gPSSData->field_C60, gPSSData->field_C64);
        if ((gPSSData->field_C69 > 0 && gPSSData->field_C68 == IN_BOX_ROWS - 1)
            || (gPSSData->field_C69 < 0 && gPSSData->field_C68 == 0))
        {
            gPSSData->field_C6A++;
        }
        else
        {
            gPSSData->field_C68 += gPSSData->field_C69;
            gPSSData->field_C6A = 0;
        }
        break;
    case 2:
        if (gPSSData->field_C66 == 0)
        {
            gPSSData->field_C60++;
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

static void SetBoxSpeciesAndPersonalities(u8 boxId)
{
    s32 i, j, boxPosition;

    boxPosition = 0;
    for (i = 0; i < IN_BOX_COLUMNS; i++)
    {
        for (j = 0; j < IN_BOX_ROWS; j++)
        {
            gPSSData->boxSpecies[boxPosition] = GetBoxMonDataAt(boxId, boxPosition, MON_DATA_SPECIES2);
            if (gPSSData->boxSpecies[boxPosition] != SPECIES_NONE)
                gPSSData->boxPersonalities[boxPosition] = GetBoxMonDataAt(boxId, boxPosition, MON_DATA_PERSONALITY);
            boxPosition++;
        }
    }

    gPSSData->field_C5C = boxId;
}

void DestroyBoxMonIconAtPosition(u8 boxPosition)
{
    if (gPSSData->boxMonsSprites[boxPosition] != NULL)
    {
        DestroyBoxMonIcon(gPSSData->boxMonsSprites[boxPosition]);
        gPSSData->boxMonsSprites[boxPosition] = NULL;
    }
}

void SetBoxMonIconObjMode(u8 boxPosition, u8 objMode)
{
    if (gPSSData->boxMonsSprites[boxPosition] != NULL)
    {
        gPSSData->boxMonsSprites[boxPosition]->oam.objMode = objMode;
    }
}

void CreatePartyMonsSprites(bool8 arg0)
{
    u16 i, count;
    u16 species = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES2);
    u32 personality = GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY);

    gPSSData->partySprites[0] = CreateMonIconSprite(species, personality, 104, 64, 1, 12);
    count = 1;
    for (i = 1; i < PARTY_SIZE; i++)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
        if (species != SPECIES_NONE)
        {
            personality = GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY);
            gPSSData->partySprites[i] = CreateMonIconSprite(species, personality, 152,  8 * (3 * (i - 1)) + 16, 1, 12);
            count++;
        }
        else
        {
            gPSSData->partySprites[i] = NULL;
        }
    }

    if (!arg0)
    {
        for (i = 0; i < count; i++)
        {
            gPSSData->partySprites[i]->y -= 160;
            gPSSData->partySprites[i]->invisible = TRUE;
        }
    }

    if (gPSSData->boxOption == BOX_OPTION_MOVE_ITEMS)
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (gPSSData->partySprites[i] != NULL && GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM) == 0)
                gPSSData->partySprites[i]->oam.objMode = ST_OAM_OBJ_BLEND;
        }
    }
}

void sub_80909F4(void)
{
    u16 i, count;

    gPSSData->field_C5E = 0;
    for (i = 0, count = 0; i < PARTY_SIZE; i++)
    {
        if (gPSSData->partySprites[i] != NULL)
        {
            if (i != count)
            {
                sub_8090A74(gPSSData->partySprites[i], count);
                gPSSData->partySprites[i] = NULL;
                gPSSData->field_C5E++;
            }
            count++;
        }
    }
}

u8 sub_8090A60(void)
{
    return gPSSData->field_C5E;
}

static void sub_8090A74(struct Sprite *sprite, u16 partyId)
{
    s16 x, y;

    sprite->data[1] = partyId;
    if (partyId == 0)
        x = 104, y = 64;
    else
        x = 152, y = 8 * (3 * (partyId - 1)) + 16;

    sprite->data[2] = (u16)(sprite->x) * 8;
    sprite->data[3] = (u16)(sprite->y) * 8;
    sprite->data[4] = ((x * 8) - sprite->data[2]) / 8;
    sprite->data[5] = ((y * 8) - sprite->data[3]) / 8;
    sprite->data[6] = 8;
    sprite->callback = sub_8090AE0;
}

static void sub_8090AE0(struct Sprite *sprite)
{
    if (sprite->data[6] != 0)
    {
        s16 x = sprite->data[2] += sprite->data[4];
        s16 y = sprite->data[3] += sprite->data[5];
        sprite->x = x / 8u;
        sprite->y = y / 8u;
        sprite->data[6]--;
    }
    else
    {
        if (sprite->data[1] == 0)
        {
            sprite->x = 104;
            sprite->y = 64;
        }
        else
        {
            sprite->x = 152;
            sprite->y = 8 * (3 * (sprite->data[1] - 1)) + 16;
        }
        sprite->callback = SpriteCallbackDummy;
        gPSSData->partySprites[sprite->data[1]] = sprite;
        gPSSData->field_C5E--;
    }
}

void DestroyMovingMonIcon(void)
{
    if (gPSSData->movingMonSprite != NULL)
    {
        DestroyBoxMonIcon(gPSSData->movingMonSprite);
        gPSSData->movingMonSprite = NULL;
    }
}

void sub_8090B98(s16 yDelta)
{
    u16 i, posY;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (gPSSData->partySprites[i] != NULL)
        {
            gPSSData->partySprites[i]->y += yDelta;
            posY = gPSSData->partySprites[i]->y + gPSSData->partySprites[i]->y2 + gPSSData->partySprites[i]->centerToCornerVecY;
            posY += 16;
            if (posY > 192)
                gPSSData->partySprites[i]->invisible = TRUE;
            else
                gPSSData->partySprites[i]->invisible = FALSE;
        }
    }
}

void DestroyPartyMonIcon(u8 partyId)
{
    if (gPSSData->partySprites[partyId] != NULL)
    {
        DestroyBoxMonIcon(gPSSData->partySprites[partyId]);
        gPSSData->partySprites[partyId] = NULL;
    }
}

void DestroyAllPartyMonIcons(void)
{
    u16 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (gPSSData->partySprites[i] != NULL)
        {
            DestroyBoxMonIcon(gPSSData->partySprites[i]);
            gPSSData->partySprites[i] = NULL;
        }
    }
}

void SetPartyMonIconObjMode(u8 partyId, u8 objMode)
{
    if (gPSSData->partySprites[partyId] != NULL)
    {
        gPSSData->partySprites[partyId]->oam.objMode = objMode;
    }
}

void sub_8090CC0(u8 mode, u8 id)
{
    if (mode == MODE_PARTY)
    {
        gPSSData->movingMonSprite = gPSSData->partySprites[id];
        gPSSData->partySprites[id] = NULL;
    }
    else if (mode == MODE_BOX)
    {
        gPSSData->movingMonSprite = gPSSData->boxMonsSprites[id];
        gPSSData->boxMonsSprites[id] = NULL;
    }
    else
    {
        return;
    }

    gPSSData->movingMonSprite->callback = sub_80911B0;
    gPSSData->movingMonSprite->oam.priority = sub_8090058();
    gPSSData->movingMonSprite->subpriority = 7;
}

void sub_8090D58(u8 boxId, u8 position)
{
    if (boxId == TOTAL_BOXES_COUNT) // party mon
    {
        gPSSData->partySprites[position] = gPSSData->movingMonSprite;
        gPSSData->partySprites[position]->oam.priority = 1;
        gPSSData->partySprites[position]->subpriority = 12;
    }
    else
    {
        gPSSData->boxMonsSprites[position] = gPSSData->movingMonSprite;
        gPSSData->boxMonsSprites[position]->oam.priority = 2;
        gPSSData->boxMonsSprites[position]->subpriority = 19 - (position % IN_BOX_ROWS);
    }
    gPSSData->movingMonSprite->callback = SpriteCallbackDummy;
    gPSSData->movingMonSprite = NULL;
}

void sub_8090E08(u8 boxId, u8 position)
{
    if (boxId == TOTAL_BOXES_COUNT) // party mon
        gPSSData->field_B00 = &gPSSData->partySprites[position];
    else
        gPSSData->field_B00 = &gPSSData->boxMonsSprites[position];

    gPSSData->movingMonSprite->callback = SpriteCallbackDummy;
    gPSSData->field_C5D = 0;
}

bool8 sub_8090E74(void)
{
    if (gPSSData->field_C5D == 16)
        return FALSE;

    gPSSData->field_C5D++;
    if (gPSSData->field_C5D & 1)
    {
        (*gPSSData->field_B00)->y--;
        gPSSData->movingMonSprite->y++;
    }

    (*gPSSData->field_B00)->x2 = gSineTable[gPSSData->field_C5D * 8] / 16;
    gPSSData->movingMonSprite->x2 = -(gSineTable[gPSSData->field_C5D * 8] / 16);
    if (gPSSData->field_C5D == 8)
    {
        gPSSData->movingMonSprite->oam.priority = (*gPSSData->field_B00)->oam.priority;
        gPSSData->movingMonSprite->subpriority = (*gPSSData->field_B00)->subpriority;
        (*gPSSData->field_B00)->oam.priority = sub_8090058();
        (*gPSSData->field_B00)->subpriority = 7;
    }

    if (gPSSData->field_C5D == 16)
    {
        struct Sprite *sprite = gPSSData->movingMonSprite;
        gPSSData->movingMonSprite = (*gPSSData->field_B00);
        *gPSSData->field_B00 = sprite;

        gPSSData->movingMonSprite->callback = sub_80911B0;
        (*gPSSData->field_B00)->callback = SpriteCallbackDummy;
    }

    return TRUE;
}

void sub_8090FC4(u8 mode, u8 position)
{
    switch (mode)
    {
    case MODE_PARTY:
        gPSSData->field_B04 = &gPSSData->partySprites[position];
        break;
    case MODE_BOX:
        gPSSData->field_B04 = &gPSSData->boxMonsSprites[position];
        break;
    case MODE_2:
        gPSSData->field_B04 = &gPSSData->movingMonSprite;
        break;
    default:
        return;
    }

    if (*gPSSData->field_B04 != NULL)
    {
        InitSpriteAffineAnim(*gPSSData->field_B04);
        (*gPSSData->field_B04)->oam.affineMode = ST_OAM_AFFINE_NORMAL;
        (*gPSSData->field_B04)->affineAnims = gUnknown_83CEC38;
        StartSpriteAffineAnim(*gPSSData->field_B04, 0);
    }
}

bool8 sub_8091084(void)
{
    if (*gPSSData->field_B04 == NULL || (*gPSSData->field_B04)->invisible)
        return FALSE;

    if ((*gPSSData->field_B04)->affineAnimEnded)
        (*gPSSData->field_B04)->invisible = TRUE;

    return TRUE;
}

void sub_80910CC(void)
{
    if (*gPSSData->field_B04 != NULL)
    {
        FreeOamMatrix((*gPSSData->field_B04)->oam.matrixNum);
        DestroyBoxMonIcon(*gPSSData->field_B04);
        *gPSSData->field_B04 = NULL;
    }
}

void sub_8091114(void)
{
    if (*gPSSData->field_B04 != NULL)
    {
        (*gPSSData->field_B04)->invisible = FALSE;
        StartSpriteAffineAnim(*gPSSData->field_B04, 1);
    }
}

bool8 sub_8091150(void)
{
    if (gPSSData->field_B04 == NULL)
        return FALSE;

    if ((*gPSSData->field_B04)->affineAnimEnded)
        gPSSData->field_B04 = NULL;

    return TRUE;
}

void SetMovingMonPriority(u8 priority)
{
    gPSSData->movingMonSprite->oam.priority = priority;
}

static void sub_80911B0(struct Sprite *sprite)
{
    sprite->x = gPSSData->field_CB4->x;
    sprite->y = gPSSData->field_CB4->y + gPSSData->field_CB4->y2 + 4;
}

static u16 sub_80911D4(u16 species)
{
    u16 i, var;

    // Find the currently-allocated slot
    for (i = 0; i < MAX_MON_ICONS; i++)
    {
        if (gPSSData->field_B58[i] == species)
            break;
    }

    if (i == MAX_MON_ICONS)
    {
        // Find the first empty slot
        for (i = 0; i < MAX_MON_ICONS; i++)
        {
            if (gPSSData->field_B58[i] == SPECIES_NONE)
                break;
        }
        if (i == MAX_MON_ICONS)
            return 0xFFFF;
    }

    gPSSData->field_B58[i] = species;
    gPSSData->field_B08[i]++;
    var = 16 * i;
    CpuCopy32(GetMonIconTiles(species, TRUE), (void*)(OBJ_VRAM0) + var * 32, 0x200);

    return var;
}

static void sub_8091290(u16 species)
{
    u16 i;

    for (i = 0; i < MAX_MON_ICONS; i++)
    {
        if (gPSSData->field_B58[i] == species)
        {
            if (--gPSSData->field_B08[i] == 0)
                gPSSData->field_B58[i] = 0;
            break;
        }
    }
}

struct Sprite *CreateMonIconSprite(u16 species, u32 personality, s16 x, s16 y, u8 oamPriority, u8 subpriority)
{
    u16 tileNum;
    u8 spriteId;
    struct SpriteTemplate template = gUnknown_83CEBF0;

    species = GetIconSpecies(species, personality);
    template.paletteTag = 0xDAC0 + gMonIconPaletteIndices[species];
    tileNum = sub_80911D4(species);
    if (tileNum == 0xFFFF)
        return NULL;

    spriteId = CreateSprite(&template, x, y, subpriority);
    if (spriteId == MAX_SPRITES)
    {
        sub_8091290(species);
        return NULL;
    }

    gSprites[spriteId].oam.tileNum = tileNum;
    gSprites[spriteId].oam.priority = oamPriority;
    gSprites[spriteId].data[0] = species;
    return &gSprites[spriteId];
}

static void DestroyBoxMonIcon(struct Sprite *sprite)
{
    sub_8091290(sprite->data[0]);
    DestroySprite(sprite);
}

void sub_80913DC(u8 boxId)
{
    u8 taskId = CreateTask(sub_8091420, 2);

    gTasks[taskId].data[2] = boxId;
}

bool8 sub_809140C(void)
{
    return FuncIsActiveTask(sub_8091420);
}

static void sub_8091420(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        gPSSData->field_2D2 = 0;
        gPSSData->bg2_X = 0;
        task->data[1] = RequestDma3Fill(0, gPSSData->field_4AC4, 0x1000, DMA3_32BIT);
        break;
    case 1:
        if (WaitDma3Request(task->data[1]) == -1)
            return;

        SetBgTilemapBuffer(2, gPSSData->field_4AC4);
        ShowBg(2);
        break;
    case 2:
        LoadWallpaperGfx(task->data[2], 0);
        break;
    case 3:
        if (!WaitForWallpaperGfxLoad())
            return;

        sub_8091A94(task->data[2]);
        sub_8091F80();
        sub_80900D4(task->data[2]);
        SetGpuReg(REG_OFFSET_BG2CNT, BGCNT_PRIORITY(2) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(27) | BGCNT_TXT512x256);
        break;
    case 4:
        DestroyTask(taskId);
        break;
    default:
        task->data[0] = 0;
        return;
    }

    task->data[0]++;
}

void SetUpScrollToBox(u8 boxId)
{
    s8 direction = sub_80916F4(boxId);

    gPSSData->field_2CE = (direction > 0) ? 6 : -6;
    gPSSData->field_2D3 = (direction > 0) ? 1 : 2;
    gPSSData->field_2D0 = 32;
    gPSSData->field_2D4 = boxId;
    gPSSData->field_2D6 = (direction <= 0) ? 5 : 0;
    gPSSData->field_2D8 = direction;
    gPSSData->field_2DA = (direction > 0) ? 264 : 56;
    gPSSData->field_2DC = (direction <= 0) ? 5 : 0;
    gPSSData->field_2DE = 0;
    gPSSData->field_2E0 = 2;
    gPSSData->field_A64 = boxId;
    gPSSData->field_A65 = direction;
    gPSSData->field_A63 = 0;
}

bool8 ScrollToBox(void)
{
    bool8 var;

    switch (gPSSData->field_A63)
    {
    case 0:
        LoadWallpaperGfx(gPSSData->field_A64, gPSSData->field_A65);
        gPSSData->field_A63++;
    case 1:
        if (!WaitForWallpaperGfxLoad())
            return TRUE;

        sub_8090574(gPSSData->field_A64, gPSSData->field_A65);
        sub_8091C48(gPSSData->field_A64, gPSSData->field_A65);
        sub_809200C(gPSSData->field_A65);
        break;
    case 2:
        var = sub_809062C();
        if (gPSSData->field_2D0 != 0)
        {
            gPSSData->bg2_X += gPSSData->field_2CE;
            if (--gPSSData->field_2D0 != 0)
                return TRUE;
            sub_8091E34();
            sub_80920AC();
        }
        return var;
    }

    gPSSData->field_A63++;
    return TRUE;
}

static s8 sub_80916F4(u8 boxId)
{
    u8 i;
    u8 currentBox = StorageGetCurrentBox();

    for (i = 0; currentBox != boxId; i++)
    {
        currentBox++;
        if (currentBox >= TOTAL_BOXES_COUNT)
            currentBox = 0;
    }

    return (i < TOTAL_BOXES_COUNT / 2) ? 1 : -1;
}

void SetWallpaperForCurrentBox(u8 wallpaperId)
{
    u8 boxId = StorageGetCurrentBox();
    SetBoxWallpaper(boxId, wallpaperId);
    gPSSData->wallpaperChangeState = 0;
}

bool8 DoWallpaperGfxChange(void)
{
    switch (gPSSData->wallpaperChangeState)
    {
    case 0:
        BeginNormalPaletteFade(gPSSData->field_738, 1, 0, 16, RGB_WHITEALPHA);
        gPSSData->wallpaperChangeState++;
        break;
    case 1:
        if (!UpdatePaletteFade())
        {
            u8 curBox = StorageGetCurrentBox();
            LoadWallpaperGfx(curBox, 0);
            gPSSData->wallpaperChangeState++;
        }
        break;
    case 2:
        if (WaitForWallpaperGfxLoad() == TRUE)
        {
            sub_8091EF0();
            BeginNormalPaletteFade(gPSSData->field_738, 1, 16, 0, RGB_WHITEALPHA);
            gPSSData->wallpaperChangeState++;
        }
        break;
    case 3:
        if (!UpdatePaletteFade())
            gPSSData->wallpaperChangeState++;
        break;
    case 4:
        return FALSE;
    }

    return TRUE;
}

static void LoadWallpaperGfx(u8 boxId, s8 direction)
{
    u8 wallpaperId;
    const struct WallpaperTable *wallpaperGfx;
    void *iconGfx;
    u32 size1, size2;

    gPSSData->field_6F9 = 0;
    gPSSData->field_6FA = boxId;
    gPSSData->field_6FB = direction;
    if (gPSSData->field_6FB != 0)
    {
        gPSSData->field_2D2 = (gPSSData->field_2D2 == 0);
        sub_8091A24(gPSSData->field_4AC4);
    }

    wallpaperId = GetBoxWallpaper(gPSSData->field_6FA);
    wallpaperGfx = &sWallpaperTable[wallpaperId];
    LZ77UnCompWram(wallpaperGfx->tileMap, gPSSData->field_792);
    sub_8091984(gPSSData->field_4AC4, gPSSData->field_792, gPSSData->field_6FB, gPSSData->field_2D2);

    if (gPSSData->field_6FB != 0)
        LoadPalette(wallpaperGfx->palettes, (gPSSData->field_2D2 * 32) + 0x40, 0x40);
    else
        CpuCopy16(wallpaperGfx->palettes, &gPlttBufferUnfaded[(gPSSData->field_2D2 * 32) + 0x40], 0x40);

    DecompressAndLoadBgGfxUsingHeap(2, wallpaperGfx->tiles, 0, 256 * gPSSData->field_2D2, 0);

    CopyBgTilemapBufferToVram(2);
}

static bool32 WaitForWallpaperGfxLoad(void)
{
    if (IsDma3ManagerBusyWithBgCopy() == TRUE)
        return FALSE;

    return TRUE;
}

static void sub_8091984(void *buffer, const void *tilemap, s8 direction, u8 arg2)
{
    s16 var = (arg2 * 2) + 3;
    s16 x = ((gPSSData->bg2_X / 8 + 10) + (direction * 24)) & 0x3F;

    CopyRectToBgTilemapBufferRect(2, tilemap, 0, 0, 0x14, 0x12, x, 2, 0x14, 0x12, 0x11, arg2 << 8, var);

    if (direction == 0)
        return;
    else if (direction > 0)
        x *= 1, x += 0x14; // x * 1 is needed to match, but can be safely removed as it makes no functional difference
    else
        x -= 4;

    FillBgTilemapBufferRect(2, 0, x, 2, 4, 0x12, 0x11);
}

static void sub_8091A24(void *arg0)
{
    u16 i;
    u16 *dest = arg0;
    s16 r3 = ((gPSSData->bg2_X / 8) + 30) & 0x3F;

    if (r3 <= 31)
        dest += r3 + 0x260;
    else
        dest += r3 + 0x640;

    for (i = 0; i < 0x2C; i++)
    {
        *dest++ = 0;
        r3 = (r3 + 1) & 0x3F;
        if (r3 == 0)
            dest -= 0x420;
        if (r3 == 0x20)
            dest += 0x3e0;
    }
}

static void sub_8091A94(u8 boxId)
{
    u8 tagIndex;
    s16 r6;
    u16 i;

    struct SpriteSheet spriteSheet = {gPSSData->field_2F8, 0x200, TAG_TILE_3};
    struct SpritePalette palettes[] = {
        {gPSSData->field_6FC, TAG_PAL_DAC9},
        {}
    };

    u16 wallpaperId = GetBoxWallpaper(boxId);

    gPSSData->field_6FC[14] = gUnknown_83D29D0[wallpaperId][0];
    gPSSData->field_6FC[15] = gUnknown_83D29D0[wallpaperId][1];
    LoadSpritePalettes(palettes);
    gPSSData->field_738 = 0x3f0;

    tagIndex = IndexOfSpritePaletteTag(TAG_PAL_DAC9);
    gPSSData->field_71C = 0x10e + 16 * tagIndex;
    gPSSData->field_738 |= 0x10000 << tagIndex;

    tagIndex = IndexOfSpritePaletteTag(TAG_PAL_DAC9);
    gPSSData->field_71E = 0x10e + 16 * tagIndex;
    gPSSData->field_738 |= 0x10000 << tagIndex;

    StringCopyPadded(gPSSData->field_21B8, GetBoxNamePtr(boxId), 0, 8);
    DrawTextWindowAndBufferTiles(gPSSData->field_21B8, gPSSData->field_2F8, 0, 0, gPSSData->field_4F8, 2);
    LoadSpriteSheet(&spriteSheet);
    r6 = sub_8091F60(GetBoxNamePtr(boxId));

    for (i = 0; i < 2; i++)
    {
        u8 spriteId = CreateSprite(&gUnknown_83D2B7C, r6 + i * 32, 28, 24);
        gPSSData->field_720[i] = &gSprites[spriteId];
        StartSpriteAnim(gPSSData->field_720[i], i);
    }
    gPSSData->field_6F8 = 0;
}

static void sub_8091C48(u8 boxId, s8 direction)
{
    u16 r8;
    s16 x, x2;
    u16 i;
    struct SpriteSheet spriteSheet = {gPSSData->field_2F8, 0x200, TAG_TILE_3};
    struct SpriteTemplate template = gUnknown_83D2B7C;

    gPSSData->field_6F8 = (gPSSData->field_6F8 == 0);
    if (gPSSData->field_6F8 == 0)
    {
        spriteSheet.tag = TAG_TILE_3;
        r8 = gPSSData->field_71C;
    }
    else
    {
        spriteSheet.tag = TAG_TILE_4;
        r8 = gPSSData->field_71C;
        template.tileTag = TAG_TILE_4;
        template.paletteTag = TAG_PAL_DAC9;
    }

    StringCopyPadded(gPSSData->field_21B8, GetBoxNamePtr(boxId), 0, 8);
    DrawTextWindowAndBufferTiles(gPSSData->field_21B8, gPSSData->field_2F8, 0, 0, gPSSData->field_4F8, 2);
    LoadSpriteSheet(&spriteSheet);
    LoadPalette(gUnknown_83D29D0[GetBoxWallpaper(boxId)], r8, 4);
    x = sub_8091F60(GetBoxNamePtr(boxId));
    x2 = x;
    x2 += direction * 192;

    for (i = 0; i < 2; i++)
    {
        u8 spriteId = CreateSprite(&template, i * 32 + x2, 28, 24);

        gPSSData->field_728[i] = &gSprites[spriteId];
        gPSSData->field_728[i]->data[0] = (-direction) * 6;
        gPSSData->field_728[i]->data[1] = i * 32 + x;
        gPSSData->field_728[i]->data[2] = 0;
        gPSSData->field_728[i]->callback = sub_8091E84;
        StartSpriteAnim(gPSSData->field_728[i], i);

        gPSSData->field_720[i]->data[0] = (-direction) * 6;
        gPSSData->field_720[i]->data[1] = 1;
        gPSSData->field_720[i]->callback = sub_8091EB8;
    }
}

static void sub_8091E34(void)
{
    if (gPSSData->field_6F8 == 0)
        FreeSpriteTilesByTag(TAG_TILE_4);
    else
        FreeSpriteTilesByTag(TAG_TILE_3);

    gPSSData->field_720[0] = gPSSData->field_728[0];
    gPSSData->field_720[1] = gPSSData->field_728[1];
}

static void sub_8091E84(struct Sprite *sprite)
{
    if (sprite->data[2] != 0)
        sprite->data[2]--;
    else if ((sprite->x += sprite->data[0]) == sprite->data[1])
        sprite->callback = SpriteCallbackDummy;
}

static void sub_8091EB8(struct Sprite *sprite)
{
    if (sprite->data[1] != 0)
    {
        sprite->data[1]--;
    }
    else
    {
        sprite->x += sprite->data[0];
        sprite->data[2] = sprite->x + sprite->x2;
        if (sprite->data[2] < 0x40 || sprite->data[2] > 0x100)
            DestroySprite(sprite);
    }
}

static void sub_8091EF0(void)
{
    u8 boxId = StorageGetCurrentBox();
    u8 wallpaperId = GetBoxWallpaper(boxId);
    if (gPSSData->field_6F8 == 0)
        CpuCopy16(gUnknown_83D29D0[wallpaperId], gPlttBufferUnfaded + gPSSData->field_71C, 4);
    else
        CpuCopy16(gUnknown_83D29D0[wallpaperId], gPlttBufferUnfaded + gPSSData->field_71E, 4);
}

static s16 sub_8091F60(const u8 *string)
{
    return 0xB0 - GetStringWidth(1, string, 0) / 2;
}

static void sub_8091F80(void)
{
    u16 i;

    LoadSpriteSheet(&gUnknown_83D2B54);
    for (i = 0; i < 2; i++)
    {
        u8 spriteId = CreateSprite(&gUnknown_83D2BB4, 0x5c + i * 0x88, 28, 22);
        if (spriteId != MAX_SPRITES)
        {
            struct Sprite *sprite = &gSprites[spriteId];
            StartSpriteAnim(sprite, i);
            sprite->data[3] = (i == 0) ? -1 : 1;
            gPSSData->field_730[i] = sprite;
        }
    }
    if (IsCursorOnBox())
        sub_80920FC(TRUE);
}

static void sub_809200C(s8 direction)
{
    u16 i;

    for (i = 0; i < 2; i++)
    {
        gPSSData->field_730[i]->x2 = 0;
        gPSSData->field_730[i]->data[0] = 2;
    }
    if (direction < 0)
    {
        gPSSData->field_730[0]->data[1] = 29;
        gPSSData->field_730[1]->data[1] = 5;
        gPSSData->field_730[0]->data[2] = 0x48;
        gPSSData->field_730[1]->data[2] = 0x48;
    }
    else
    {
        gPSSData->field_730[0]->data[1] = 5;
        gPSSData->field_730[1]->data[1] = 29;
        gPSSData->field_730[0]->data[2] = 0xF8;
        gPSSData->field_730[1]->data[2] = 0xF8;
    }
    gPSSData->field_730[0]->data[7] = 0;
    gPSSData->field_730[1]->data[7] = 1;
}

static void sub_80920AC(void)
{
    u16 i;

    for (i = 0; i < 2; i++)
    {
        gPSSData->field_730[i]->x = 0x88 * i + 0x5c;
        gPSSData->field_730[i]->x2 = 0;
        gPSSData->field_730[i]->invisible = FALSE;
    }
    sub_80920FC(TRUE);
}

void sub_80920FC(bool8 a0)
{
    u16 i;

    if (a0)
    {
        for (i = 0; i < 2; i++)
        {
            gPSSData->field_730[i]->data[0] = 1;
            gPSSData->field_730[i]->data[1] = 0;
            gPSSData->field_730[i]->data[2] = 0;
            gPSSData->field_730[i]->data[4] = 0;
        }
    }
    else
    {
        for (i = 0; i < 2; i++)
        {
            gPSSData->field_730[i]->data[0] = 0;
        }
    }
}

static void sub_8092164(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->x2 = 0;
        break;
    case 1:
        if (++sprite->data[1] > 3)
        {
            sprite->data[1] = 0;
            sprite->x2 += sprite->data[3];
            if (++sprite->data[2] > 5)
            {
                sprite->data[2] = 0;
                sprite->x2 = 0;
            }
        }
        break;
    case 2:
        sprite->data[0] = 3;
        break;
    case 3:
        sprite->x -= gPSSData->field_2CE;
        if (sprite->x < 73 || sprite->x > 247)
            sprite->invisible = TRUE;
        if (--sprite->data[1] == 0)
        {
            sprite->x = sprite->data[2];
            sprite->invisible = FALSE;
            sprite->data[0] = 4;
        }
        break;
    case 4:
        sprite->x -= gPSSData->field_2CE;
        break;
    }
}

struct Sprite *sub_809223C(u16 x, u16 y, u8 animId, u8 priority, u8 subpriority)
{
    u8 spriteId = CreateSprite(&gUnknown_83D2BB4, x, y, subpriority);
    if (spriteId == MAX_SPRITES)
        return NULL;

    animId %= 2;
    StartSpriteAnim(&gSprites[spriteId], animId);
    gSprites[spriteId].oam.priority = priority;
    gSprites[spriteId].callback = SpriteCallbackDummy;
    return &gSprites[spriteId];
}
