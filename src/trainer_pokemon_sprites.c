#include "global.h"
#include "gflib.h"
#include "decompress.h"

extern const struct CompressedSpriteSheet gMonFrontPicTable[];
extern const struct CompressedSpriteSheet gMonBackPicTable[];
extern const struct CompressedSpriteSheet gTrainerFrontPicTable[];
extern const struct CompressedSpriteSheet gTrainerBackPicTable[];
extern const struct CompressedSpritePalette gTrainerFrontPicPaletteTable[];
extern const union AnimCmd *const gSpriteAnimTable_82349BC[];
extern const union AnimCmd *const *const gTrainerFrontAnimsPtrTable[];

// Static type declarations

struct PicData
{
    u8 *frames;
    struct SpriteFrameImage *images;
    u16 paletteTag;
    u8 spriteId;
    u8 active;
};

// Static RAM declarations
#define PICS_COUNT 8

static EWRAM_DATA struct SpriteTemplate sCreatingSpriteTemplate = {};
static EWRAM_DATA struct PicData sSpritePics[PICS_COUNT] = {};

// Static ROM declarations

// .rodata

static const struct PicData gUnknown_8453178 = {};

static const struct OamData gUnknown_8453184 =
{
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

// .text

void DummyPicSpriteCallback(struct Sprite *sprite)
{

}

bool16 ResetAllPicSprites(void)
{
    int i;

    for (i = 0; i < PICS_COUNT; i ++)
        sSpritePics[i] = gUnknown_8453178;

    return FALSE;
}

static bool16 DecompressPic(u16 species, u32 personality, bool8 isFrontPic, u8 *dest, bool8 isTrainer, bool8 ignoreDeoxys)
{
    if (!isTrainer)
    {
        if (isFrontPic)
        {
            if (!ignoreDeoxys)
                LoadSpecialPokePic(&gMonFrontPicTable[species], dest, species, personality, isFrontPic);
            else
                LoadSpecialPokePic_DontHandleDeoxys(&gMonFrontPicTable[species], dest, species, personality, isFrontPic);
        }
        else
        {
            if (!ignoreDeoxys)
                LoadSpecialPokePic(&gMonBackPicTable[species], dest, species, personality, isFrontPic);
            else
                LoadSpecialPokePic_DontHandleDeoxys(&gMonBackPicTable[species], dest, species, personality, isFrontPic);
        }
    }
    else
    {
        if (isFrontPic)
            DecompressPicFromTable(&gTrainerFrontPicTable[species], dest, species);
        else
            DecompressPicFromTable(&gTrainerBackPicTable[species], dest, species);
    }
    return FALSE;
}

static bool16 DecompressPic_HandleDeoxys(u16 species, u32 personality, bool8 isFrontPic, u8 *dest, bool8 isTrainer)
{
    return DecompressPic(species, personality, isFrontPic, dest, isTrainer, FALSE);
}

void LoadPicPaletteByTagOrSlot(u16 species, u32 otId, u32 personality, u8 paletteSlot, u16 paletteTag, bool8 isTrainer)
{
    if (!isTrainer)
    {
        if (paletteTag == 0xFFFF)
        {
            sCreatingSpriteTemplate.paletteTag = 0xFFFF;
            LoadCompressedPalette(GetMonSpritePalFromSpeciesAndPersonality(species, otId, personality), 0x100 + paletteSlot * 0x10, 0x20);
        }
        else
        {
            sCreatingSpriteTemplate.paletteTag = paletteTag;
            LoadCompressedSpritePalette(GetMonSpritePalStructFromOtIdPersonality(species, otId, personality));
        }
    }
    else
    {
        if (paletteTag == 0xFFFF)
        {
            sCreatingSpriteTemplate.paletteTag = 0xFFFF;
            LoadCompressedPalette(gTrainerFrontPicPaletteTable[species].data, 0x100 + paletteSlot * 0x10, 0x20);
        }
        else
        {
            sCreatingSpriteTemplate.paletteTag = paletteTag;
            LoadCompressedSpritePalette(&gTrainerFrontPicPaletteTable[species]);
        }
    }
}

void LoadPicPaletteBySlot(u16 species, u32 otId, u32 personality, u8 paletteSlot, bool8 isTrainer)
{
    if (!isTrainer)
        LoadCompressedPalette(GetMonSpritePalFromSpeciesAndPersonality(species, otId, personality), paletteSlot * 0x10, 0x20);
    else
        LoadCompressedPalette(gTrainerFrontPicPaletteTable[species].data, paletteSlot * 0x10, 0x20);
}

void AssignSpriteAnimsTable(bool8 isTrainer)
{
    if (!isTrainer)
        sCreatingSpriteTemplate.anims = gSpriteAnimTable_82349BC;
    else
        sCreatingSpriteTemplate.anims = gTrainerFrontAnimsPtrTable[0];
}

u16 CreatePicSprite(u16 species, u32 otId, u32 personality, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag, bool8 isTrainer, bool8 ignoreDeoxys)
{
    u8 i;
    u8 *framePics;
    struct SpriteFrameImage *images;
    int j;
    u8 spriteId;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (!sSpritePics[i].active)
        {
            break;
        }
    }
    if (i == PICS_COUNT)
    {
        return 0xFFFF;
    }
    framePics = Alloc(4 * 0x800);
    if (!framePics)
    {
        return 0xFFFF;
    }
    images = Alloc(4 * sizeof(struct SpriteFrameImage));
    if (!images)
    {
        Free(framePics);
        return 0xFFFF;
    }
    if (DecompressPic(species, personality, isFrontPic, framePics, isTrainer, ignoreDeoxys))
    {
        // debug trap?
        return 0xFFFF;
    }
    for (j = 0; j < 4; j ++)
    {
        images[j].data = framePics + 0x800 * j;
        images[j].size = 0x800;
    }
    sCreatingSpriteTemplate.tileTag = 0xFFFF;
    sCreatingSpriteTemplate.oam = &gUnknown_8453184;
    AssignSpriteAnimsTable(isTrainer);
    sCreatingSpriteTemplate.images = images;
    sCreatingSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sCreatingSpriteTemplate.callback = DummyPicSpriteCallback;
    LoadPicPaletteByTagOrSlot(species, otId, personality, paletteSlot, paletteTag, isTrainer);
    spriteId = CreateSprite(&sCreatingSpriteTemplate, x, y, 0);
    if (paletteTag == 0xFFFF)
    {
        gSprites[spriteId].oam.paletteNum = paletteSlot;
    }
    sSpritePics[i].frames = framePics;
    sSpritePics[i].images = images;
    sSpritePics[i].paletteTag = paletteTag;
    sSpritePics[i].spriteId = spriteId;
    sSpritePics[i].active = TRUE;
    return spriteId;
}

u16 CreatePicSprite_HandleDeoxys(u16 species, u32 otId, u32 personality, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag, bool8 isTrainer)
{
    return CreatePicSprite(species, otId, personality, isFrontPic, x, y, paletteSlot, paletteTag, isTrainer, FALSE);
}

u16 FreeAndDestroyPicSpriteInternal(u16 spriteId)
{
    u8 i;
    u8 *framePics;
    struct SpriteFrameImage *images;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (sSpritePics[i].spriteId == spriteId)
        {
            break;
        }
    }
    if (i == PICS_COUNT)
    {
        return 0xFFFF;
    }
    framePics = sSpritePics[i].frames;
    images = sSpritePics[i].images;
    if (sSpritePics[i].paletteTag != 0xFFFF)
    {
        FreeSpritePaletteByTag(GetSpritePaletteTagByPaletteNum(gSprites[spriteId].oam.paletteNum));
    }
    DestroySprite(&gSprites[spriteId]);
    Free(framePics);
    Free(images);
    sSpritePics[i] = gUnknown_8453178;
    return 0;
}

u16 sub_810C050(u16 species, u32 otId, u32 personality, bool8 isFrontPic, u8 paletteSlot, u8 windowId, bool8 isTrainer)
{
    if (DecompressPic_HandleDeoxys(species, personality, isFrontPic, (u8 *)GetWindowAttribute(windowId, WINDOW_TILE_DATA), FALSE))
    {
        return 0xFFFF;
    }
    LoadPicPaletteBySlot(species, otId, personality, paletteSlot, isTrainer);
    return 0;
}

u16 sub_810C0C0(u16 species, u32 otId, u32 personality, bool8 isFrontPic, u16 destX, u16 destY, u8 paletteSlot, u8 windowId, bool8 isTrainer)
{
    u8 *framePics;

    framePics = Alloc(4 * 0x800);
    if (framePics && !DecompressPic_HandleDeoxys(species, personality, isFrontPic, framePics, isTrainer))
    {
        BlitBitmapRectToWindow(windowId, framePics, 0, 0, 0x40, 0x40, destX, destY, 0x40, 0x40);
        LoadPicPaletteBySlot(species, otId, personality, paletteSlot, isTrainer);
        Free(framePics);
        return 0;
    }
    return 0xFFFF;
}

u16 CreateMonPicSprite(u16 species, u32 otId, u32 personality, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag, bool8 ignoreDeoxys)
{
    return CreatePicSprite(species, otId, personality, isFrontPic, x, y, paletteSlot, paletteTag, FALSE, ignoreDeoxys);
}

u16 CreateMonPicSprite_HandleDeoxys(u16 species, u32 otId, u32 personality, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag)
{
    return CreateMonPicSprite(species, otId, personality, isFrontPic, x, y, paletteSlot, paletteTag, FALSE);
}

u16 FreeAndDestroyMonPicSprite(u16 spriteId)
{
    return FreeAndDestroyPicSpriteInternal(spriteId);
}

u16 LoadMonPicInWindow(u16 species, u32 otId, u32 personality, bool8 isFrontPic, u8 paletteSlot, u8 windowId)
{
    return sub_810C0C0(species, otId, personality, isFrontPic, 0, 0, paletteSlot, windowId, FALSE);
}

u16 sub_810C25C(u16 species, u32 otId, u32 personality, bool8 isFrontPic, u16 destX, u16 destY, u8 paletteSlot, u8 windowId)
{
    return sub_810C0C0(species, otId, personality, isFrontPic, destX, destY, paletteSlot, windowId, FALSE);
}

u16 CreateTrainerPicSprite(u16 species, bool8 isFrontPic, s16 x, s16 y, u8 paletteSlot, u16 paletteTag)
{
    return CreatePicSprite_HandleDeoxys(species, 0, 0, isFrontPic, x, y, paletteSlot, paletteTag, TRUE);
}

u16 FreeAndDestroyTrainerPicSprite(u16 spriteId)
{
    return FreeAndDestroyPicSpriteInternal(spriteId);
}

u16 sub_810C2FC(u16 species, bool8 isFrontPic, u8 paletteSlot, u8 windowId)
{
    return sub_810C050(species, 0, 0, isFrontPic, paletteSlot, windowId, TRUE);
}

u16 CreateTrainerCardTrainerPicSprite(u16 species, bool8 isFrontPic, u16 destX, u16 destY, u8 paletteSlot, u8 windowId)
{
    return sub_810C0C0(species, 0, 0, isFrontPic, destX, destY, paletteSlot, windowId, TRUE);
}

u16 PlayerGenderToFrontTrainerPicId_Debug(u8 gender, bool8 getClass)
{
    if (getClass == TRUE)
    {
        switch (gender)
        {
        default:
            return gFacilityClassToPicIndex[0x87];
        case MALE:
            return gFacilityClassToPicIndex[0x86];
        }
    }
    return gender;
}
