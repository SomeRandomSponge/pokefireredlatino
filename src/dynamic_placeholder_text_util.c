#include "global.h"
#include "gflib.h"
#include "constants/event_objects.h"

static EWRAM_DATA const u8 *sStringPointers[8] = {0};

static const u8 sTextColorTable[] =
{
 // [LOW_NYBBLE / 2]                            = 0xXY, // HIGH_NYBBLE
    [OBJ_EVENT_GFX_RED_NORMAL / 2]              = 0x00, // OBJ_EVENT_GFX_RED_BIKE
    [OBJ_EVENT_GFX_RED_SURF / 2]                = 0x00, // OBJ_EVENT_GFX_RED_FIELD_MOVE
    [OBJ_EVENT_GFX_RED_FISH / 2]                = 0x00, // OBJ_EVENT_GFX_RED_VS_SEEKER
    [OBJ_EVENT_GFX_RED_VS_SEEKER_BIKE / 2]      = 0x10, // OBJ_EVENT_GFX_GREEN_NORMAL
    [OBJ_EVENT_GFX_GREEN_BIKE / 2]              = 0x11, // OBJ_EVENT_GFX_GREEN_SURF
    [OBJ_EVENT_GFX_GREEN_FIELD_MOVE / 2]        = 0x11, // OBJ_EVENT_GFX_GREEN_FISH
    [OBJ_EVENT_GFX_GREEN_VS_SEEKER / 2]         = 0x11, // OBJ_EVENT_GFX_GREEN_VS_SEEKER_BIKE
    [OBJ_EVENT_GFX_RS_BRENDAN / 2]              = 0x10, // OBJ_EVENT_GFX_RS_MAY
    [OBJ_EVENT_GFX_LITTLE_BOY / 2]              = 0x10, // OBJ_EVENT_GFX_LITTLE_GIRL
    [OBJ_EVENT_GFX_YOUNGSTER / 2]               = 0x00, // OBJ_EVENT_GFX_BOY
    [OBJ_EVENT_GFX_BUG_CATCHER / 2]             = 0x00, // OBJ_EVENT_GFX_SITTING_BOY
    [OBJ_EVENT_GFX_LASS / 2]                    = 0x11, // OBJ_EVENT_GFX_WOMAN_1
    [OBJ_EVENT_GFX_BATTLE_GIRL / 2]             = 0x01, // OBJ_EVENT_GFX_MAN
    [OBJ_EVENT_GFX_ROCKER / 2]                  = 0x00, // OBJ_EVENT_GFX_FAT_MAN
    [OBJ_EVENT_GFX_WOMAN_2 / 2]                 = 0x11, // OBJ_EVENT_GFX_BEAUTY
    [OBJ_EVENT_GFX_BALDING_MAN / 2]             = 0x10, // OBJ_EVENT_GFX_WOMAN_3
    [OBJ_EVENT_GFX_OLD_MAN_1 / 2]               = 0x00, // OBJ_EVENT_GFX_OLD_MAN_2
    [OBJ_EVENT_GFX_OLD_MAN_LYING_DOWN / 2]      = 0x10, // OBJ_EVENT_GFX_OLD_WOMAN
    [OBJ_EVENT_GFX_TUBER_M_WATER / 2]           = 0x10, // OBJ_EVENT_GFX_TUBER_F
    [OBJ_EVENT_GFX_TUBER_M_LAND / 2]            = 0x00, // OBJ_EVENT_GFX_CAMPER
    [OBJ_EVENT_GFX_PICNICKER / 2]               = 0x01, // OBJ_EVENT_GFX_COOLTRAINER_M
    [OBJ_EVENT_GFX_COOLTRAINER_F / 2]           = 0x01, // OBJ_EVENT_GFX_SWIMMER_M_WATER
    [OBJ_EVENT_GFX_SWIMMER_F_WATER / 2]         = 0x01, // OBJ_EVENT_GFX_SWIMMER_M_LAND
    [OBJ_EVENT_GFX_SWIMMER_F_LAND / 2]          = 0x01, // OBJ_EVENT_GFX_WORKER_M
    [OBJ_EVENT_GFX_WORKER_F / 2]                = 0x01, // OBJ_EVENT_GFX_ROCKET_M
    [OBJ_EVENT_GFX_ROCKET_F / 2]                = 0x01, // OBJ_EVENT_GFX_GBA_KID
    [OBJ_EVENT_GFX_SUPER_NERD / 2]              = 0x00, // OBJ_EVENT_GFX_BIKER
    [OBJ_EVENT_GFX_BLACKBELT / 2]               = 0x00, // OBJ_EVENT_GFX_SCIENTIST
    [OBJ_EVENT_GFX_HIKER / 2]                   = 0x00, // OBJ_EVENT_GFX_FISHER
    [OBJ_EVENT_GFX_CHANNELER / 2]               = 0x01, // OBJ_EVENT_GFX_CHEF
    [OBJ_EVENT_GFX_POLICEMAN / 2]               = 0x00, // OBJ_EVENT_GFX_GENTLEMAN
    [OBJ_EVENT_GFX_SAILOR / 2]                  = 0x00, // OBJ_EVENT_GFX_CAPTAIN
    [OBJ_EVENT_GFX_NURSE / 2]                   = 0x11, // OBJ_EVENT_GFX_CABLE_CLUB_RECEPTIONIST
    [OBJ_EVENT_GFX_UNION_ROOM_RECEPTIONIST / 2] = 0x01, // OBJ_EVENT_GFX_UNUSED_MALE_RECEPTIONIST
    [OBJ_EVENT_GFX_CLERK / 2]                   = 0x00, // OBJ_EVENT_GFX_MG_DELIVERYMAN
    [OBJ_EVENT_GFX_TRAINER_TOWER_DUDE / 2]      = 0x00, // OBJ_EVENT_GFX_PROF_OAK
    [OBJ_EVENT_GFX_BLUE / 2]                    = 0x00, // OBJ_EVENT_GFX_BILL
    [OBJ_EVENT_GFX_LANCE / 2]                   = 0x10, // OBJ_EVENT_GFX_AGATHA
    [OBJ_EVENT_GFX_DAISY / 2]                   = 0x11, // OBJ_EVENT_GFX_LORELEI
    [OBJ_EVENT_GFX_MR_FUJI / 2]                 = 0x00, // OBJ_EVENT_GFX_BRUNO
    [OBJ_EVENT_GFX_BROCK / 2]                   = 0x10, // OBJ_EVENT_GFX_MISTY
    [OBJ_EVENT_GFX_LT_SURGE / 2]                = 0x10, // OBJ_EVENT_GFX_ERIKA
    [OBJ_EVENT_GFX_KOGA / 2]                    = 0x10, // OBJ_EVENT_GFX_SABRINA
    [OBJ_EVENT_GFX_BLAINE / 2]                  = 0x00, // OBJ_EVENT_GFX_GIOVANNI
    [OBJ_EVENT_GFX_MOM / 2]                     = 0x01, // OBJ_EVENT_GFX_CELIO
    [OBJ_EVENT_GFX_TEACHY_TV_HOST / 2]          = 0x00, // OBJ_EVENT_GFX_GYM_GUY
    [OBJ_EVENT_GFX_ITEM_BALL / 2]               = 0x33, // OBJ_EVENT_GFX_TOWN_MAP
    [OBJ_EVENT_GFX_POKEDEX / 2]                 = 0x33, // OBJ_EVENT_GFX_CUT_TREE
    [OBJ_EVENT_GFX_ROCK_SMASH_ROCK / 2]         = 0x33, // OBJ_EVENT_GFX_PUSHABLE_BOULDER
    [OBJ_EVENT_GFX_FOSSIL / 2]                  = 0x33, // OBJ_EVENT_GFX_RUBY
    [OBJ_EVENT_GFX_SAPPHIRE / 2]                = 0x33, // OBJ_EVENT_GFX_OLD_AMBER
    [OBJ_EVENT_GFX_GYM_SIGN / 2]                = 0x33, // OBJ_EVENT_GFX_SIGN
    [OBJ_EVENT_GFX_TRAINER_TIPS / 2]            = 0x33, // OBJ_EVENT_GFX_CLIPBOARD
    [OBJ_EVENT_GFX_METEORITE / 2]               = 0x33, // OBJ_EVENT_GFX_LAPRAS_DOLL
    [OBJ_EVENT_GFX_SEAGALLOP / 2]               = 0x23, // OBJ_EVENT_GFX_SNORLAX
    [OBJ_EVENT_GFX_SPEAROW / 2]                 = 0x22, // OBJ_EVENT_GFX_CUBONE
    [OBJ_EVENT_GFX_POLIWRATH / 2]               = 0x22, // OBJ_EVENT_GFX_CLEFAIRY
    [OBJ_EVENT_GFX_PIDGEOT / 2]                 = 0x22, // OBJ_EVENT_GFX_JIGGLYPUFF
    [OBJ_EVENT_GFX_PIDGEY / 2]                  = 0x22, // OBJ_EVENT_GFX_CHANSEY
    [OBJ_EVENT_GFX_OMANYTE / 2]                 = 0x22, // OBJ_EVENT_GFX_KANGASKHAN
    [OBJ_EVENT_GFX_PIKACHU / 2]                 = 0x22, // OBJ_EVENT_GFX_PSYDUCK
    [OBJ_EVENT_GFX_NIDORAN_F / 2]               = 0x22, // OBJ_EVENT_GFX_NIDORAN_M
    [OBJ_EVENT_GFX_NIDORINO / 2]                = 0x22, // OBJ_EVENT_GFX_MEOWTH
    [OBJ_EVENT_GFX_SEEL / 2]                    = 0x22, // OBJ_EVENT_GFX_VOLTORB
    [OBJ_EVENT_GFX_SLOWPOKE / 2]                = 0x22, // OBJ_EVENT_GFX_SLOWBRO
    [OBJ_EVENT_GFX_MACHOP / 2]                  = 0x22, // OBJ_EVENT_GFX_WIGGLYTUFF
    [OBJ_EVENT_GFX_DODUO / 2]                   = 0x22, // OBJ_EVENT_GFX_FEAROW
    [OBJ_EVENT_GFX_MACHOKE / 2]                 = 0x22, // OBJ_EVENT_GFX_LAPRAS
    [OBJ_EVENT_GFX_ZAPDOS / 2]                  = 0x22, // OBJ_EVENT_GFX_MOLTRES
    [OBJ_EVENT_GFX_ARTICUNO / 2]                = 0x22, // OBJ_EVENT_GFX_MEWTWO
    [OBJ_EVENT_GFX_MEW / 2]                     = 0x22, // OBJ_EVENT_GFX_ENTEI
    [OBJ_EVENT_GFX_SUICUNE / 2]                 = 0x22, // OBJ_EVENT_GFX_RAIKOU
    [OBJ_EVENT_GFX_LUGIA / 2]                   = 0x22, // OBJ_EVENT_GFX_HO_OH
    [OBJ_EVENT_GFX_CELEBI / 2]                  = 0x22, // OBJ_EVENT_GFX_KABUTO
    [OBJ_EVENT_GFX_DEOXYS_D / 2]                = 0x22, // OBJ_EVENT_GFX_DEOXYS_A
    [OBJ_EVENT_GFX_DEOXYS_N / 2]                = 0x32, // OBJ_EVENT_GFX_SS_ANNE
};

void DynamicPlaceholderTextUtil_Reset(void)
{
    const u8 **ptr = sStringPointers;
    u8 *fillval = NULL;
    const u8 **ptr2 = ptr + (NELEMS(sStringPointers) - 1);
    
    do
    {
        *ptr2-- = fillval;
    }
    while ((intptr_t)ptr2 >= (intptr_t)ptr);
}

void DynamicPlaceholderTextUtil_SetPlaceholderPtr(u8 idx, const u8 *ptr)
{
    if (idx < NELEMS(sStringPointers))
        sStringPointers[idx] = ptr;
}

u8 *DynamicPlaceholderTextUtil_ExpandPlaceholders(u8 *dest, const u8 *src)
{
    while (*src != EOS)
    {
        if (*src != CHAR_DYNAMIC_PLACEHOLDER)
        {
            *dest++ = *src++;
        }
        else
        {
            src++;
            if (sStringPointers[*src] != NULL)
                dest = StringCopy(dest, sStringPointers[*src]);
            src++;
        }
    }
    *dest = EOS;
    return dest;
}

const u8 *DynamicPlaceholderTextUtil_GetPlaceholderPtr(u8 idx)
{
    return sStringPointers[idx];
}

u8 GetColorFromTextColorTable(u16 graphicId)
{
    u32 test = graphicId >> 1;
    u32 shift = (graphicId & 1) << 2;

    if (test >= NELEMS(sTextColorTable))
        return 3;
    else
        return (sTextColorTable[graphicId >> 1] >> shift) & 0xF;
}
