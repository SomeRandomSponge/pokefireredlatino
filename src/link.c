#include "global.h"
#include "gflib.h"
#include "m4a.h"
#include "scanline_effect.h"
#include "bg_regs.h"
#include "decompress.h"
#include "save.h"
#include "battle.h"
#include "quest_log.h"
#include "link_rfu.h"
#include "librfu.h"
#include "random.h"
#include "task.h"
#include "event_data.h"
#include "item_menu.h"
#include "trade.h"
#include "menu.h"
#include "overworld.h"
#include "new_menu_helpers.h"
#include "link.h"
#include "graphics.h"
#include "strings.h"
#include "help_system.h"
#include "reset_save_heap.h"
#include "constants/battle.h"
#include "constants/songs.h"

extern u16 gHeldKeyCodeToSend;

struct BlockTransfer
{
    u16 pos;
    u16 size;
    const u8 *src;
    bool8 active;
    u8 multiplayerId;
};

struct LinkTestBGInfo
{
    u32 screenBaseBlock;
    u32 paletteNum;
    u32 dummy_8;
    u32 dummy_C;
};

#define SIO_MULTI_CNT ((struct SioMultiCnt *)REG_ADDR_SIOCNT)

static struct BlockTransfer sBlockSend;
ALIGNED(8) static struct BlockTransfer sBlockRecv[MAX_LINK_PLAYERS];
static u32 sBlockSendDelayCounter;
static u32 gUnknown_3000E4C;
static u8 gUnknown_3000E50;
static u32 sPlayerDataExchangeStatus;
static u32 gUnknown_3000E58;
static u8 sLinkTestLastBlockSendPos;
ALIGNED(8) static u8 sLinkTestLastBlockRecvPos[MAX_LINK_PLAYERS];
// File break?
static u8 sNumVBlanksWithoutSerialIntr;
static bool8 sSendBufferEmpty;
static u16 sSendNonzeroCheck;
static u16 sRecvNonzeroCheck;
static u8 sChecksumAvailable;
static u8 sHandshakePlayerCount;

u16 gLinkPartnersHeldKeys[6];
u32 gLinkDebugSeed;
struct LinkPlayerBlock gLocalLinkPlayerBlock;
bool8 gLinkErrorOccurred;
u32 gLinkDebugFlags;
u32 gFiller_3003EB4;
bool8 gRemoteLinkPlayersNotReceived[MAX_LINK_PLAYERS];
u8 gBlockReceivedStatus[MAX_LINK_PLAYERS];
u32 gFiller_3003EC0;
u16 gLinkHeldKeys;
u16 gRecvCmds[MAX_RFU_PLAYERS][CMD_LENGTH];
u32 gLinkStatus;
bool8 gLinkAllAcked5FFF;
bool8 gUnknown_3003F28;
bool8 gLinkCommand2FFEAck[MAX_LINK_PLAYERS];
bool8 gLinkCommand5FFFAck[MAX_LINK_PLAYERS];
u16 gLinkCmd5FFFparam;
u8 gSuppressLinkErrorMessage;
u8 gWirelessCommType;
u8 gSavedLinkPlayerCount;
u16 gSendCmd[CMD_LENGTH];
u8 gSavedMultiplayerId;
bool8 gReceivedRemoteLinkPlayers;
struct LinkTestBGInfo gLinkTestBGInfo;
void (*gLinkCallback)(void);
u8 gShouldAdvanceLinkState;
u16 gLinkTestBlockChecksums[MAX_LINK_PLAYERS];
u8 gBlockRequestType;
u32 gFiller_3003F94; // file
u32 gFiller_3003F98; // boundary
u32 gFiller_3003F9C; // here?
u8 gLastSendQueueCount;
struct Link gLink;
u8 gLastRecvQueueCount;
u16 gLinkSavedIme;

EWRAM_DATA bool8 gLinkTestDebugValuesEnabled = FALSE;
EWRAM_DATA bool8 gUnknown_2022111 = FALSE;
EWRAM_DATA u32 gUnknown_2022114 = 0;
EWRAM_DATA u16 gBlockRecvBuffer[MAX_RFU_PLAYERS][BLOCK_BUFFER_SIZE / 2] = {};
EWRAM_DATA u8 gBlockSendBuffer[BLOCK_BUFFER_SIZE] = {};
EWRAM_DATA bool8 gLinkOpen = FALSE;
EWRAM_DATA u16 gLinkType = 0;
EWRAM_DATA u16 gLinkTimeOutCounter = 0;
EWRAM_DATA struct LinkPlayer gLocalLinkPlayer = {};
EWRAM_DATA struct LinkPlayer gLinkPlayers[MAX_RFU_PLAYERS] = {};
EWRAM_DATA struct LinkPlayer gSavedLinkPlayers[MAX_RFU_PLAYERS] = {};
EWRAM_DATA struct {
    u32 status;
    u8 lastRecvQueueCount;
    u8 lastSendQueueCount;
    u8 unk_06;
} sLinkErrorBuffer = {};
static EWRAM_DATA u16 sStartSend5FFFfailures = 0;
static EWRAM_DATA void *sLinkErrorBgTilemapBuffer = NULL;

static void InitLocalLinkPlayer(void);
static void sub_800978C(void);
static void CB2_LinkTest(void);
static void ProcessRecvCmds(u8 id);
static void LinkCB_SendHeldKeys(void);
static void ResetBlockSend(void);
static bool32 InitBlockSend(const void *src, size_t size);
static void LinkCB_BlockSendBegin(void);
static void LinkCB_BlockSend(void);
static void LinkCB_BlockSendEnd(void);
static void sub_800A3CC(void);
static void SetBlockReceivedFlag(u8 id);
static u16 LinkTestCalcBlockChecksum(const u16 *src, u16 size);
static void LinkTest_prnthex(u32 pos, u8 a0, u8 a1, u8 a2);
static void LinkCB_RequestPlayerDataExchange(void);
static void Task_PrintTestData(u8 taskId);
static void LinkCB_BuildCommand5FFF(void);
static void LinkCB_WaitAckCommand5FFF(void);
static void LinkFunc_Send2FFE_1(void);
static void LinkFunc_Send2FFE_2(void);
static void CheckErrorStatus(void);
static void CB2_PrintErrorMessage(void);
static void SetWirelessCommType0(void);
static void DisableSerial(void);
static void EnableSerial(void);
static bool8 IsSioMultiMaster(void);
static void CheckMasterOrSlave(void);
static void InitTimer(void);
static void EnqueueSendCmd(u16 *sendCmd);
static void DequeueRecvCmds(u16 (*recvCmds)[CMD_LENGTH]);
static void StartTransfer(void);
static bool8 DoHandshake(void);
static void DoRecv(void);
static void DoSend(void);
static void StopTimer(void);
static void SendRecvDone(void);

static const u16 sWirelessLinkDisplayPal[] = INCBIN_U16("graphics/interface/wireless_link_display.gbapal");
static const u16 sWirelessLinkDisplay4bpp[] = INCBIN_U16("graphics/interface/wireless_link_display.4bpp.lz");
static const u16 sWirelessLinkDisplayBin[] = INCBIN_U16("graphics/interface/wireless_link_display.bin.lz");
static const u16 sLinkTestFontPal[] = INCBIN_U16("graphics/interface/link_test_font.gbapal");
static const u16 sLinkTestFontGfx[] = INCBIN_U16("graphics/interface/link_test_font.4bpp");

static const struct BlockRequest sBlockRequests[] = {
    {gBlockSendBuffer, 200},
    {gBlockSendBuffer, 200},
    {gBlockSendBuffer, 100},
    {gBlockSendBuffer, 220},
    {gBlockSendBuffer,  40}
};
static const char sASCIIGameFreakInc[] = "GameFreak inc.";
static const char sASCIITestPrint[] = "TEST PRINT\n"
                               "P0\n"
                               "P1\n"
                               "P2\n"
                               "P3";

static const struct BgTemplate sLinkErrorBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .priority = 0
    }, {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 8,
        .priority = 1
    }
};

static const struct WindowTemplate sLinkErrorWindowTemplates[] = {
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 0x002
    }, {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 6,
        .width = 30,
        .height = 7,
        .paletteNum = 15,
        .baseBlock = 0x098
    }, {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 13,
        .width = 30,
        .height = 7,
        .paletteNum = 15,
        .baseBlock = 0x16A
    }, DUMMY_WIN_TEMPLATE
};

static const u8 sLinkErrorTextColor[] = { 0x00, 0x01, 0x02 };

bool8 IsWirelessAdapterConnected(void)
{
    if (QL_IS_PLAYBACK_STATE)
        return FALSE;

    SetWirelessCommType1();
    InitRFUAPI();
    sub_80FB128(TRUE);
    if (rfu_LMAN_REQBN_softReset_and_checkID() == RFU_ID)
    {
        rfu_REQ_stopMode();
        rfu_waitREQComplete();
        return TRUE;
    }
    SetWirelessCommType0();
    CloseLink();
    RestoreSerialTimer3IntrHandlers();
    return FALSE;
}

void Task_DestroySelf(u8 taskId)
{
    DestroyTask(taskId);
}

void InitLinkTestBG(u8 paletteNum, u8 bgNum, u8 screenBaseBlock, u8 charBaseBlock, u16 a4)
{
    LoadPalette(sLinkTestFontPal, paletteNum * 16, 0x20);
    DmaCopy16(3, sLinkTestFontGfx, (u16 *)BG_CHAR_ADDR(charBaseBlock) + (16 * a4), sizeof sLinkTestFontGfx);
    gLinkTestBGInfo.screenBaseBlock = screenBaseBlock;
    gLinkTestBGInfo.paletteNum = paletteNum;
    gLinkTestBGInfo.dummy_8 = a4;
    switch (bgNum)
    {
    case 1:
        SetGpuReg(REG_OFFSET_BG1CNT, BGCNT_SCREENBASE(screenBaseBlock) | BGCNT_PRIORITY(1) | BGCNT_CHARBASE(charBaseBlock));
        break;
    case 2:
        SetGpuReg(REG_OFFSET_BG2CNT, BGCNT_SCREENBASE(screenBaseBlock) | BGCNT_PRIORITY(1) | BGCNT_CHARBASE(charBaseBlock));
        break;
    case 3:
        SetGpuReg(REG_OFFSET_BG3CNT, BGCNT_SCREENBASE(screenBaseBlock) | BGCNT_PRIORITY(1) | BGCNT_CHARBASE(charBaseBlock));
        break;
    }
    SetGpuReg(REG_OFFSET_BG0HOFS + bgNum * 4, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS + bgNum * 4, 0);
}

void sub_80095BC(u8 paletteNum, u8 bgNum, u8 screenBaseBlock, u8 charBaseBlock)
{
    LoadPalette(sLinkTestFontPal, paletteNum * 16, 0x20);
    DmaCopy16(3, sLinkTestFontGfx, (u16 *)BG_CHAR_ADDR(charBaseBlock), sizeof sLinkTestFontGfx);
    gLinkTestBGInfo.screenBaseBlock = screenBaseBlock;
    gLinkTestBGInfo.paletteNum = paletteNum;
    gLinkTestBGInfo.dummy_8 = 0;
    SetGpuReg(gBGControlRegOffsets[bgNum], BGCNT_SCREENBASE(screenBaseBlock) | BGCNT_CHARBASE(charBaseBlock));
}

void LinkTestScreen(void)
{
    int i;

    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetTasks();
    SetVBlankCallback(sub_800978C);
    ResetBlockSend();
    gLinkType = 0x1111;
    OpenLink();
    SeedRng(gMain.vblankCounter2);
    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        gSaveBlock2Ptr->playerTrainerId[i] = Random() % 256;
    }
    InitLinkTestBG(0, 2, 4, 0, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON);
    CreateTask(Task_DestroySelf, 0);
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    gUnknown_3000E58 = 0;
    InitLocalLinkPlayer();
    CreateTask(Task_PrintTestData, 0);
    SetMainCallback2(CB2_LinkTest);
}

void SetLocalLinkPlayerId(u8 playerId)
{
    gLocalLinkPlayer.id = playerId;
}

static void InitLocalLinkPlayer(void)
{
    gLocalLinkPlayer.trainerId = gSaveBlock2Ptr->playerTrainerId[0] | (gSaveBlock2Ptr->playerTrainerId[1] << 8) | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[3] << 24);
    StringCopy(gLocalLinkPlayer.name, gSaveBlock2Ptr->playerName);
    gLocalLinkPlayer.gender = gSaveBlock2Ptr->playerGender;
    gLocalLinkPlayer.linkType = gLinkType;
    gLocalLinkPlayer.language = gGameLanguage;
    gLocalLinkPlayer.version = gGameVersion + 0x4000;
    gLocalLinkPlayer.lp_field_2 = 0x8000;
    gLocalLinkPlayer.progressFlags = IsNationalPokedexEnabled();
    if (FlagGet(FLAG_SYS_CAN_LINK_WITH_RS))
    {
        gLocalLinkPlayer.progressFlags |= 0x10;
    }
}

static void sub_800978C(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void InitLink(void)
{
    int i;

    for (i = 0; i < 8; i++)
    {
        gSendCmd[i] = 0xEFFF;
    }
    gLinkOpen = TRUE;
    EnableSerial();
}

void Task_TriggerHandshake(u8 taskId)
{
    if (++gTasks[taskId].data[0] == 5)
    {
        gShouldAdvanceLinkState = 1;
        DestroyTask(taskId);
    }
}

void OpenLink(void)
{
    int i;

    if (!gWirelessCommType)
    {
        ResetSerial();
        InitLink();
        gLinkCallback = LinkCB_RequestPlayerDataExchange;
        gLinkVSyncDisabled = FALSE;
        gLinkErrorOccurred = FALSE;
        gSuppressLinkErrorMessage = FALSE;
        ResetBlockReceivedFlags();
        ResetBlockSend();
        gUnknown_3000E4C = 0;
        gUnknown_3003F28 = FALSE;
        gLinkAllAcked5FFF = FALSE;
        gLinkCmd5FFFparam = 0;
        CreateTask(Task_TriggerHandshake, 2);
    }
    else
    {
        InitRFUAPI();
    }
    gReceivedRemoteLinkPlayers = FALSE;
    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        gRemoteLinkPlayersNotReceived[i] = TRUE;
        gLinkCommand5FFFAck[i] = FALSE;
        gLinkCommand2FFEAck[i] = FALSE;
    }
}

void CloseLink(void)
{
    gReceivedRemoteLinkPlayers = FALSE;
    if (gWirelessCommType)
    {
        LinkRfu_Shutdown();
    }
    gLinkOpen = FALSE;
    DisableSerial();
}

void TestBlockTransfer(u8 nothing, u8 is, u8 used)
{
    u8 i;
    u8 status;

    if (sLinkTestLastBlockSendPos != sBlockSend.pos)
    {
        LinkTest_prnthex(sBlockSend.pos, 2, 3, 2);
        sLinkTestLastBlockSendPos = sBlockSend.pos;
    }
    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        if (sLinkTestLastBlockRecvPos[i] != sBlockRecv[i].pos)
        {
            LinkTest_prnthex(sBlockRecv[i].pos, 2, i + 4, 2);
            sLinkTestLastBlockRecvPos[i] = sBlockRecv[i].pos;
        }
    }
    status = GetBlockReceivedStatus();
    if (status == 0xF) // 0b1111
    {
        for (i = 0; i < MAX_LINK_PLAYERS; i++)
        {
            if ((status >> i) & 1)
            {
                gLinkTestBlockChecksums[i] = LinkTestCalcBlockChecksum(gBlockRecvBuffer[i], sBlockRecv[i].size);
                ResetBlockReceivedFlag(i);
                if (gLinkTestBlockChecksums[i] != 0x0342)
                {
                    gLinkTestDebugValuesEnabled = FALSE;
                    gUnknown_2022111 = FALSE;
                }
            }
        }
    }
}

void LinkTestProcessKeyInput(void)
{
    if (JOY_NEW(A_BUTTON))
    {
        gShouldAdvanceLinkState = 1;
    }
    if (JOY_HELD(B_BUTTON))
    {
        InitBlockSend(gHeap + 0x4000, 0x2004);
    }
    if (JOY_NEW(L_BUTTON))
    {
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(2, 0, 0));
    }
    if (JOY_NEW(START_BUTTON))
    {
        SetSuppressLinkErrorMessage(TRUE);
    }
    if (JOY_NEW(R_BUTTON))
    {
        TrySavingData(SAVE_LINK);
    }
    if (JOY_NEW(SELECT_BUTTON))
    {
        SetCloseLinkCallback();
    }
    if (gLinkTestDebugValuesEnabled)
    {
        SetLinkDebugValues(gMain.vblankCounter2, gLinkCallback ? gLinkVSyncDisabled : gLinkVSyncDisabled | 0x10);
    }
}

static void CB2_LinkTest(void)
{
    LinkTestProcessKeyInput();
    TestBlockTransfer(1, 1, 0);
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

u16 LinkMain2(const u16 *heldKeys)
{
    u8 i;

    if (!gLinkOpen)
    {
        return 0;
    }
    for (i = 0; i < 8; i++)
    {
        gSendCmd[i] = 0;
    }
    gLinkHeldKeys = *heldKeys;
    if (gLinkStatus & LINK_STAT_CONN_ESTABLISHED)
    {
        ProcessRecvCmds(SIO_MULTI_CNT->id);
        if (gLinkCallback != NULL)
        {
            gLinkCallback();
        }
        CheckErrorStatus();
    }
    return gLinkStatus;
}

void HandleReceiveRemoteLinkPlayer(u8 who)
{
    int i;
    int count;

    count = 0;
    gRemoteLinkPlayersNotReceived[who] = FALSE;
    for (i = 0; i < GetLinkPlayerCount_2(); i++)
    {
        count += gRemoteLinkPlayersNotReceived[i];
    }
    if (count == 0 && !gReceivedRemoteLinkPlayers)
    {
        gReceivedRemoteLinkPlayers = TRUE;
    }
}

void ProcessRecvCmds(u8 unused)
{
    u16 i;

    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        gLinkPartnersHeldKeys[i] = 0;
        if (gRecvCmds[i][0] == 0)
        {
            continue;
        }
        switch (gRecvCmds[i][0])
        {
        case LINKCMD_SEND_LINK_TYPE:
        {
            struct LinkPlayerBlock * block;

            InitLocalLinkPlayer();
            block = &gLocalLinkPlayerBlock;
            block->linkPlayer = gLocalLinkPlayer;
            memcpy(block->magic1, sASCIIGameFreakInc, sizeof(block->magic1) - 1);
            memcpy(block->magic2, sASCIIGameFreakInc, sizeof(block->magic2) - 1);
            InitBlockSend(block, sizeof(*block));
            break;
        }
        case LINKCMD_SEND_HELD_KEYS:
            gLinkPartnersHeldKeys[i] = gRecvCmds[i][1];
            break;
        case LINKCMD_0x5555:
            gUnknown_3003F28 = TRUE;
            break;
        case LINKCMD_0x5566:
            gUnknown_3003F28 = TRUE;
            break;
        case LINKCMD_INIT_BLOCK:
        {
            struct BlockTransfer * blockRecv;

            blockRecv = &sBlockRecv[i];
            blockRecv->pos = 0;
            blockRecv->size = gRecvCmds[i][1];
            blockRecv->multiplayerId = gRecvCmds[i][2];
            break;
        }
        case LINKCMD_CONT_BLOCK:
        {
            if (sBlockRecv[i].size > BLOCK_BUFFER_SIZE)
            {
                u16 *buffer;
                u16 j;

                buffer = (u16 *)gDecompressionBuffer;
                for (j = 0; j < CMD_LENGTH - 1; j++)
                {
                    buffer[(sBlockRecv[i].pos / 2) + j] = gRecvCmds[i][j + 1];
                }
            }
            else
            {
                u16 j;

                for (j = 0; j < CMD_LENGTH - 1; j++)
                {
                    gBlockRecvBuffer[i][(sBlockRecv[i].pos / 2) + j] = gRecvCmds[i][j + 1];
                }
            }

            sBlockRecv[i].pos += (CMD_LENGTH - 1) * 2;

            if (sBlockRecv[i].pos >= sBlockRecv[i].size)
            {
                if (gRemoteLinkPlayersNotReceived[i] == TRUE)
                {
                    struct LinkPlayerBlock * block;
                    struct LinkPlayer * linkPlayer;

                    block = (struct LinkPlayerBlock *)&gBlockRecvBuffer[i];
                    linkPlayer = &gLinkPlayers[i];
                    *linkPlayer = block->linkPlayer;
                    if ((linkPlayer->version & 0xFF) == VERSION_RUBY || (linkPlayer->version & 0xFF) == VERSION_SAPPHIRE)
                    {
                        linkPlayer->progressFlagsCopy = 0;
                        linkPlayer->neverRead = 0;
                        linkPlayer->progressFlags = 0;
                    }
                    ConvertLinkPlayerName(linkPlayer);
                    if (strcmp(block->magic1, sASCIIGameFreakInc) != 0
                        || strcmp(block->magic2, sASCIIGameFreakInc) != 0)
                    {
                        SetMainCallback2(CB2_LinkError);
                    }
                    else
                    {
                        HandleReceiveRemoteLinkPlayer(i);
                    }
                }
                else
                {
                    SetBlockReceivedFlag(i);
                }
            }
        }
            break;
        case LINKCMD_0x5FFF:
            gLinkCommand5FFFAck[i] = TRUE;
            break;
        case LINKCMD_0x2FFE:
            gLinkCommand2FFEAck[i] = TRUE;
            break;
        case LINKCMD_0xAAAA:
            sub_800A3CC();
            break;
        case LINKCMD_0xCCCC:
            SendBlock(0, sBlockRequests[gRecvCmds[i][1]].address, sBlockRequests[gRecvCmds[i][1]].size);
            break;
        case LINKCMD_SEND_HELD_KEYS_2:
            gLinkPartnersHeldKeys[i] = gRecvCmds[i][1];
            break;
        }
    }
}

void BuildSendCmd(u16 command)
{
    switch (command)
    {
    case LINKCMD_SEND_LINK_TYPE:
        gSendCmd[0] = LINKCMD_SEND_LINK_TYPE;
        gSendCmd[1] = gLinkType;
        break;
    case LINKCMD_0x2FFE:
        gSendCmd[0] = LINKCMD_0x2FFE;
        break;
    case LINKCMD_SEND_HELD_KEYS:
        gSendCmd[0] = LINKCMD_SEND_HELD_KEYS;
        gSendCmd[1] = gMain.heldKeys;
        break;
    case LINKCMD_0x5555:
        gSendCmd[0] = LINKCMD_0x5555;
        break;
    case LINKCMD_0x6666:
        gSendCmd[0] = LINKCMD_0x6666;
        gSendCmd[1] = 0;
        break;
    case LINKCMD_0x7777:
    {
        u8 i;

        gSendCmd[0] = LINKCMD_0x7777;
        for (i = 0; i < 5; i++)
        {
            gSendCmd[i + 1] = 0xEE;
        }
        break;
    }
    case LINKCMD_INIT_BLOCK:
        gSendCmd[0] = LINKCMD_INIT_BLOCK;
        gSendCmd[1] = sBlockSend.size;
        gSendCmd[2] = sBlockSend.multiplayerId + 0x80;
        break;
    case LINKCMD_0xAAAA:
        gSendCmd[0] = LINKCMD_0xAAAA;
        break;
    case LINKCMD_0xAAAB:
        gSendCmd[0] = LINKCMD_0xAAAB;
        gSendCmd[1] = gSpecialVar_ItemId;
        break;
    case LINKCMD_0xCCCC:
        gSendCmd[0] = LINKCMD_0xCCCC;
        gSendCmd[1] = gBlockRequestType;
        break;
    case LINKCMD_0x5FFF:
        gSendCmd[0] = LINKCMD_0x5FFF;
        gSendCmd[1] = gLinkCmd5FFFparam;
        break;
    case LINKCMD_0x5566:
        gSendCmd[0] = LINKCMD_0x5566;
        break;
    case LINKCMD_SEND_HELD_KEYS_2:
        if (gHeldKeyCodeToSend == 0 || gLinkTransferringData)
        {
            break;
        }
        gSendCmd[0] = LINKCMD_SEND_HELD_KEYS_2;
        gSendCmd[1] = gHeldKeyCodeToSend;
        break;
    }
}

void StartSendingKeysToLink(void)
{
    if (gWirelessCommType)
    {
        StartSendingKeysToRfu();
    }
    gLinkCallback = LinkCB_SendHeldKeys;
}

bool32 IsSendingKeysToLink(void)
{
    if (gWirelessCommType)
    {
        return IsSendingKeysToRfu();
    }
    if (gLinkCallback == LinkCB_SendHeldKeys)
    {
        return TRUE;
    }
    return FALSE;
}

static void LinkCB_SendHeldKeys(void)
{
    if (gReceivedRemoteLinkPlayers == TRUE)
    {
        BuildSendCmd(LINKCMD_SEND_HELD_KEYS_2);
    }
}

void ClearLinkCallback(void)
{
    gLinkCallback = NULL;
}

void ClearLinkCallback_2(void)
{
    if (gWirelessCommType)
    {
        ClearLinkRfuCallback();
    }
    else
    {
        gLinkCallback = NULL;
    }
}

u8 GetLinkPlayerCount(void)
{
    if (gWirelessCommType)
    {
        return GetRfuPlayerCount();
    }
    return EXTRACT_PLAYER_COUNT(gLinkStatus);
}

void OpenLinkTimed(void)
{
    sPlayerDataExchangeStatus = 0;
    gLinkTimeOutCounter = 0;
    OpenLink();
}

u8 GetLinkPlayerDataExchangeStatusTimed(int lower, int upper)
{
    int i;
    int count;
    u32 index;
    u8 cmpVal;
    u32 linkType1;
    u32 linkType2;

    count = 0;
    if (gReceivedRemoteLinkPlayers == TRUE)
    {
        cmpVal = GetLinkPlayerCount_2();
        if (lower > cmpVal || cmpVal > upper)
        {
            sPlayerDataExchangeStatus = EXCHANGE_STAT_6;
            return EXCHANGE_STAT_6;
        }
        else
        {
            if (GetLinkPlayerCount() == 0)
            {
                gLinkErrorOccurred = TRUE;
                CloseLink();
            }
            for (i = 0, index = 0; i < GetLinkPlayerCount(); index++, i++)
            {
                if (gLinkPlayers[index].linkType == gLinkPlayers[0].linkType)
                {
                    count++;
                }
            }
            if (count == GetLinkPlayerCount())
            {
                if (gLinkPlayers[0].linkType == 0x1133)
                {
                    switch (Trade_CalcLinkPlayerCompatibilityParam())
                    {
                    case 0:
                        sPlayerDataExchangeStatus = EXCHANGE_COMPLETE;
                        break;
                    case 1:
                        sPlayerDataExchangeStatus = EXCHANGE_STAT_4;
                        break;
                    case 2:
                        sPlayerDataExchangeStatus = EXCHANGE_STAT_5;
                        break;
                    }
                }
                else
                {
                    sPlayerDataExchangeStatus = EXCHANGE_COMPLETE;
                }
            }
            else
            {
                sPlayerDataExchangeStatus = EXCHANGE_IN_PROGRESS;
            }
        }
    }
    else if (++gLinkTimeOutCounter > 600)
    {
        sPlayerDataExchangeStatus = EXCHANGE_TIMED_OUT;
    }
    return sPlayerDataExchangeStatus;
}

bool8 IsLinkPlayerDataExchangeComplete(void)
{
    u8 i;
    u8 count;
    bool8 retval;

    count = 0;
    for (i = 0; i < GetLinkPlayerCount(); i++)
    {
        if (gLinkPlayers[i].linkType == gLinkPlayers[0].linkType)
        {
            count++;
        }
    }
    if (count == GetLinkPlayerCount())
    {
        retval = TRUE;
        sPlayerDataExchangeStatus = EXCHANGE_COMPLETE;
    }
    else
    {
        retval = FALSE;
        sPlayerDataExchangeStatus = EXCHANGE_IN_PROGRESS;
    }
    return retval;
}

u32 GetLinkPlayerTrainerId(u8 who)
{
    return gLinkPlayers[who].trainerId;
}

void ResetLinkPlayers(void)
{
    int i;

    for (i = 0; i <= MAX_LINK_PLAYERS; i++)
    {
        gLinkPlayers[i] = (struct LinkPlayer){};
    }
}

static void ResetBlockSend(void)
{
    sBlockSend.active = FALSE;
    sBlockSend.pos = 0;
    sBlockSend.size = 0;
    sBlockSend.src = NULL;
}

static bool32 InitBlockSend(const void *src, size_t size)
{
    if (sBlockSend.active)
    {
        return FALSE;
    }
    sBlockSend.multiplayerId = GetMultiplayerId();
    sBlockSend.active = TRUE;
    sBlockSend.size = size;
    sBlockSend.pos = 0;
    if (size > 0x100)
    {
        sBlockSend.src = src;
    }
    else
    {
        if (src != gBlockSendBuffer)
        {
            memcpy(gBlockSendBuffer, src, size);
        }
        sBlockSend.src = gBlockSendBuffer;
    }
    BuildSendCmd(LINKCMD_INIT_BLOCK);
    gLinkCallback = LinkCB_BlockSendBegin;
    sBlockSendDelayCounter = 0;
    return TRUE;
}

static void LinkCB_BlockSendBegin(void)
{
    if (++sBlockSendDelayCounter > 2)
    {
        gLinkCallback = LinkCB_BlockSend;
    }
}

static void LinkCB_BlockSend(void)
{
    int i;
    const u8 *src;

    src = sBlockSend.src;
    gSendCmd[0] = LINKCMD_CONT_BLOCK;
    for (i = 0; i < 7; i++)
    {
        gSendCmd[i + 1] = (src[sBlockSend.pos + i * 2 + 1] << 8) | src[sBlockSend.pos + i * 2];
    }
    sBlockSend.pos += 14;
    if (sBlockSend.size <= sBlockSend.pos)
    {
        sBlockSend.active = FALSE;
        gLinkCallback = LinkCB_BlockSendEnd;
    }
}

static void LinkCB_BlockSendEnd(void)
{
    gLinkCallback = NULL;
}
void sub_800A3AC(void)
{
    GetMultiplayerId();
    BuildSendCmd(LINKCMD_SEND_HELD_KEYS);
    gUnknown_2022114++;
}

static void sub_800A3CC(void)
{
    gUnknown_2022114 = 0;
    gLinkCallback = sub_800A3AC;
}


u32 sub_800A3E8(void)
{
    return gUnknown_2022114;
}

void sub_800A3F4(void)
{
    BuildSendCmd(LINKCMD_0xAAAA);
}

u8 GetMultiplayerId(void)
{
    if (gWirelessCommType == 1)
    {
        return LinkRfu_GetMultiplayerId();
    }
    return SIO_MULTI_CNT->id;
}

u8 bitmask_all_link_players_but_self(void)
{
    u8 mpId;

    mpId = GetMultiplayerId();
    return ((1 << MAX_LINK_PLAYERS) - 1) ^ (1 << mpId);
}

bool8 SendBlock(u8 unused, const void *src, u16 size)
{
    if (gWirelessCommType == 1)
    {
        return Rfu_InitBlockSend(src, size);
    }
    return InitBlockSend(src, size);
}

bool8 Link_PrepareCmd0xCCCC_Rfu0xA100(u8 blockRequestType)
{
    if (gWirelessCommType == 1)
    {
        return Rfu_SendBlockRequest(blockRequestType);
    }
    if (gLinkCallback == NULL)
    {
        gBlockRequestType = blockRequestType;
        BuildSendCmd(LINKCMD_0xCCCC);
        return TRUE;
    }
    return FALSE;
}

bool8 IsLinkTaskFinished(void)
{
    if (gWirelessCommType == 1)
    {
        return IsLinkRfuTaskFinished();
    }
    return gLinkCallback == NULL;
}

u8 GetBlockReceivedStatus(void)
{
    if (gWirelessCommType == 1)
    {
        return Rfu_GetBlockReceivedStatus();
    }
    return (gBlockReceivedStatus[3] << 3) | (gBlockReceivedStatus[2] << 2) | (gBlockReceivedStatus[1] << 1) | (gBlockReceivedStatus[0] << 0);
}

void SetBlockReceivedFlag(u8 who)
{
    if (gWirelessCommType == 1)
    {
        Rfu_SetBlockReceivedFlag(who);
    }
    else
    {
        gBlockReceivedStatus[who] = TRUE;
    }
}

void ResetBlockReceivedFlags(void)
{
    int i;

    if (gWirelessCommType == 1)
    {
        for (i = 0; i < MAX_RFU_PLAYERS; i++)
        {
            Rfu_ResetBlockReceivedFlag(i);
        }
    }
    else
    {
        for (i = 0; i < MAX_LINK_PLAYERS; i++)
        {
            gBlockReceivedStatus[i] = FALSE;
        }
    }
}

void ResetBlockReceivedFlag(u8 who)
{
    if (gWirelessCommType == 1)
    {
        Rfu_ResetBlockReceivedFlag(who);
    }
    else if (gBlockReceivedStatus[who])
    {
        gBlockReceivedStatus[who] = FALSE;
    }
}

void CheckShouldAdvanceLinkState(void)
{
    if ((gLinkStatus & LINK_STAT_MASTER) && EXTRACT_PLAYER_COUNT(gLinkStatus) > 1)
    {
        gShouldAdvanceLinkState = 1;
    }
}

static u16 LinkTestCalcBlockChecksum(const u16 *src, u16 size)
{
    u16 chksum;
    u16 i;

    chksum = 0;
    for (i = 0; i < size / 2; i++)
    {
        chksum += src[i];
    }
    return chksum;
}

void LinkTest_prnthexchar(char a0, u8 a1, u8 a2)
{
    u16 *vAddr;

    vAddr = (u16 *)BG_SCREEN_ADDR(gLinkTestBGInfo.screenBaseBlock);
    vAddr[a2 * 32 + a1] = (gLinkTestBGInfo.paletteNum << 12) | (a0 + 1 + gLinkTestBGInfo.dummy_8);
}

void LinkTest_prntchar(char a0, u8 a1, u8 a2)
{
    u16 *vAddr;

    vAddr = (u16 *)BG_SCREEN_ADDR(gLinkTestBGInfo.screenBaseBlock);
    vAddr[a2 * 32 + a1] = (gLinkTestBGInfo.paletteNum << 12) | (a0 + gLinkTestBGInfo.dummy_8);
}

static void LinkTest_prnthex(u32 pos, u8 a0, u8 a1, u8 a2)
{
    char sp[32 / 2];
    int i;

    for (i = 0; i < a2; i++)
    {
        sp[i] = pos & 0xf;
        pos >>= 4;
    }
    for (i = a2 - 1; i >= 0; i--)
    {
        LinkTest_prnthexchar(sp[i], a0, a1);
        a0++;
    }
}

void LinkTest_prntstr(const char *a0, u8 a1, u8 a2)
{
    int r6;
    int i;
    int r5;

    r5 = 0;
    r6 = 0;
    for (i = 0; a0[i] != 0; a0++)
    {
        if (a0[i] == *"\n")
        {
            r5++;
            r6 = 0;
        }
        else
        {
            LinkTest_prntchar(a0[i], a1 + r6, a2 + r5);
            r6++;
        }
    }
}

static void LinkCB_RequestPlayerDataExchange(void)
{
    if (gLinkStatus & LINK_STAT_MASTER)
    {
        BuildSendCmd(LINKCMD_SEND_LINK_TYPE);
    }
    gLinkCallback = NULL;
}

static void Task_PrintTestData(u8 taskId)
{
    char sp[32];
    int i;

    strcpy(sp, sASCIITestPrint);
    LinkTest_prntstr(sp, 5, 2);
    LinkTest_prnthex(gShouldAdvanceLinkState, 2, 1, 2);
    LinkTest_prnthex(gLinkStatus, 15, 1, 8);
    LinkTest_prnthex(gLink.state, 2, 10, 2);
    LinkTest_prnthex(EXTRACT_PLAYER_COUNT(gLinkStatus), 15, 10, 2);
    LinkTest_prnthex(GetMultiplayerId(), 15, 12, 2);
    LinkTest_prnthex(gLastSendQueueCount, 25, 1, 2);
    LinkTest_prnthex(gLastRecvQueueCount, 25, 2, 2);
    LinkTest_prnthex(GetBlockReceivedStatus(), 15, 5, 2);
    LinkTest_prnthex(gLinkDebugSeed, 2, 12, 8);
    LinkTest_prnthex(gLinkDebugFlags, 2, 13, 8);
    LinkTest_prnthex(GetSioMultiSI(), 25, 5, 1);
    LinkTest_prnthex(IsSioMultiMaster(), 25, 6, 1);
    LinkTest_prnthex(IsLinkConnectionEstablished(), 25, 7, 1);
    LinkTest_prnthex(HasLinkErrorOccurred(), 25, 8, 1);
    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        LinkTest_prnthex(gLinkTestBlockChecksums[i], 10, 4 + i, 4);
    }
}

void SetLinkDebugValues(u32 seed, u32 flags)
{
    gLinkDebugSeed = seed;
    gLinkDebugFlags = flags;
}

u8 sub_800A8A4(void)
{
    int i;
    u8 flags;

    flags = 0;
    for (i = 0; i < gSavedLinkPlayerCount; i++)
    {
        flags |= (1 << i);
    }
    return flags;
}

u8 sub_800A8D4(void)
{
    int i;
    u8 flags;

    flags = 0;
    for (i = 0; i < GetLinkPlayerCount(); i++)
    {
        flags |= (1 << i);
    }
    return flags;
}

void sub_800A900(u8 a0)
{
    int i;

    gSavedLinkPlayerCount = a0;
    gSavedMultiplayerId = GetMultiplayerId();
    for (i = 0; i < MAX_RFU_PLAYERS; i++)
    {
        gSavedLinkPlayers[i] = gLinkPlayers[i];
    }
}

// The number of players when trading began. This is frequently compared against the
// current number of connected players to check if anyone dropped out.
u8 GetSavedPlayerCount(void)
{
    return gSavedLinkPlayerCount;
}

u8 GetSavedMultiplayerId(void)
{
    return gSavedMultiplayerId;
}

bool8 sub_800A95C(void)
{
    int i;
    unsigned count;

    count = 0;
    for (i = 0; i < gSavedLinkPlayerCount; i++)
    {
        if (gLinkPlayers[i].trainerId == gSavedLinkPlayers[i].trainerId)
        {
            count++;
        }
    }
    if (count == gSavedLinkPlayerCount)
    {
        return TRUE;
    }
    return FALSE;
}

void sub_800A9A4(void)
{
    u8 i;

    for (i = 0; i < gSavedLinkPlayerCount; i++)
    {
        if (gSavedLinkPlayers[i].trainerId != gLinkPlayers[i].trainerId || StringCompare(gSavedLinkPlayers[i].name, gLinkPlayers[i].name) != 0)
        {
            gLinkErrorOccurred = TRUE;
            CloseLink();
            SetMainCallback2(CB2_LinkError);
        }
    }
}

void sub_800AA24(void)
{
    gSavedLinkPlayerCount = 0;
    gSavedMultiplayerId = 0;
}

u8 GetLinkPlayerCount_2(void)
{
    return EXTRACT_PLAYER_COUNT(gLinkStatus);
}

bool8 IsLinkMaster(void)
{
    if (gWirelessCommType)
    {
        return Rfu_IsMaster();
    }
    return EXTRACT_MASTER(gLinkStatus);
}

u8 sub_800AA74(void)
{
    return gUnknown_3000E50;
}

void Link_StartSend5FFFwithParam(u16 a0)
{
    if (gWirelessCommType == 1)
    {
        Rfu_SetCloseLinkCallback();
    }
    else
    {
        if (gLinkCallback == NULL)
        {
            gLinkCallback = LinkCB_BuildCommand5FFF;
            gLinkAllAcked5FFF = FALSE;
            gLinkCmd5FFFparam = a0;
        }
    }
}

void SetCloseLinkCallback(void)
{
    if (gWirelessCommType == 1)
    {
        Rfu_SetCloseLinkCallback();
    }
    else
    {
        if (gLinkCallback != NULL)
        {
            sStartSend5FFFfailures++;
        }
        else
        {
            gLinkCallback = LinkCB_BuildCommand5FFF;
            gLinkAllAcked5FFF = FALSE;
            gLinkCmd5FFFparam = 0;
        }
    }
}

static void LinkCB_BuildCommand5FFF(void)
{
    if (gLastRecvQueueCount == 0)
    {
        BuildSendCmd(LINKCMD_0x5FFF);
        gLinkCallback = LinkCB_WaitAckCommand5FFF;
    }
}

static void LinkCB_WaitAckCommand5FFF(void)
{
    int i;
    unsigned count;
    u8 linkPlayerCount;

    linkPlayerCount = GetLinkPlayerCount();
    count = 0;
    for (i = 0; i < linkPlayerCount; i++)
    {
        if (gLinkCommand5FFFAck[i])
        {
            count++;
        }
    }
    if (count == linkPlayerCount)
    {
        gBattleTypeFlags &= ~(BATTLE_TYPE_LINK_ESTABLISHED | 0xFFFF0000);
        gLinkVSyncDisabled = TRUE;
        CloseLink();
        gLinkCallback = NULL;
        gLinkAllAcked5FFF = TRUE;
    }
}

void SetLinkStandbyCallback(void)
{
    if (gWirelessCommType == 1)
    {
        Rfu_SetLinkStandbyCallback();
    }
    else
    {
        if (gLinkCallback == NULL)
        {
            gLinkCallback = LinkFunc_Send2FFE_1;
        }
        gLinkAllAcked5FFF = FALSE;
    }
}

static void LinkFunc_Send2FFE_1(void)
{
    if (gLastRecvQueueCount == 0)
    {
        BuildSendCmd(LINKCMD_0x2FFE);
        gLinkCallback = LinkFunc_Send2FFE_2;
    }
}

static void LinkFunc_Send2FFE_2(void)
{
    u8 i;
    u8 linkPlayerCount;

    linkPlayerCount = GetLinkPlayerCount();
    for (i = 0; i < linkPlayerCount; i++)
    {
        if (!gLinkCommand2FFEAck[i])
        {
            break;
        }
    }
    if (i == linkPlayerCount)
    {
        for (i = 0; i < MAX_LINK_PLAYERS; i++)
        {
            gLinkCommand2FFEAck[i] = FALSE;
        }
        gLinkCallback = NULL;
    }
}

static void CheckErrorStatus(void)
{
    if (gLinkOpen && EXTRACT_LINK_ERRORS(gLinkStatus))
    {
        if (!gSuppressLinkErrorMessage)
        {
            sLinkErrorBuffer.status = gLinkStatus;
            sLinkErrorBuffer.lastRecvQueueCount = gLastRecvQueueCount;
            sLinkErrorBuffer.lastSendQueueCount = gLastSendQueueCount;
            SetMainCallback2(CB2_LinkError);
        }
        gLinkErrorOccurred = TRUE;
        CloseLink();
    }
}

void SetLinkErrorFromRfu(u32 status, u8 lastSendQueueCount, u8 lastRecvQueueCount, u8 isConnectionError)
{
    sLinkErrorBuffer.status = status;
    sLinkErrorBuffer.lastSendQueueCount = lastSendQueueCount;
    sLinkErrorBuffer.lastRecvQueueCount = lastRecvQueueCount;
    sLinkErrorBuffer.unk_06 = isConnectionError;
}

void CB2_LinkError(void)
{
    u8 *tilemapBuffer;

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    m4aMPlayStop(&gMPlayInfo_SE1);
    m4aMPlayStop(&gMPlayInfo_SE2);
    m4aMPlayStop(&gMPlayInfo_SE3);
    InitHeap(gHeap, HEAP_SIZE);
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFadeControl();
    FillPalette(0, 0, 2);
    ResetTasks();
    ScanlineEffect_Stop();
    if (gWirelessCommType)
    {
        if (!sLinkErrorBuffer.unk_06)
        {
            gWirelessCommType = 3;
        }
        ResetLinkRfuGFLayer();
    }
    SetVBlankCallback(sub_800978C);
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sLinkErrorBgTemplates, 2);
    sLinkErrorBgTilemapBuffer = tilemapBuffer = malloc(0x800);
    SetBgTilemapBuffer(1, tilemapBuffer);
    if (InitWindows(sLinkErrorWindowTemplates))
    {
        DeactivateAllTextPrinters();
        ResetTempTileDataBuffers();
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON);
        LoadPalette(gTMCaseMainWindowPalette, 0xf0, 0x20);
        gSoftResetDisabled = FALSE;
        CreateTask(Task_DestroySelf, 0);
        StopMapMusic();
        gMain.callback1 = NULL;
        RunTasks();
        AnimateSprites();
        BuildOamBuffer();
        UpdatePaletteFade();
        SetMainCallback2(CB2_PrintErrorMessage);
    }
}

void sub_800AE1C(void)
{
    DecompressAndLoadBgGfxUsingHeap(1, sWirelessLinkDisplay4bpp, FALSE, 0, 0);
    CopyToBgTilemapBuffer(1, sWirelessLinkDisplayBin, 0, 0);
    CopyBgTilemapBufferToVram(1);
    LoadPalette(sWirelessLinkDisplayPal, 0, 0x20);
    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    AddTextPrinterParameterized3(0, 3, 2, 5, sLinkErrorTextColor, 0, gText_CommErrorEllipsis);
    AddTextPrinterParameterized3(2, 3, 2, 2, sLinkErrorTextColor, 0, gText_MoveCloserToLinkPartner);
    PutWindowTilemap(0);
    PutWindowTilemap(2);
    CopyWindowToVram(0, 0);
    CopyWindowToVram(2, COPYWIN_BOTH);
    ShowBg(0);
    ShowBg(1);
}

void sub_800AED0(void)
{
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    AddTextPrinterParameterized3(1, 3, 2, 0, sLinkErrorTextColor, 0, gText_CommErrorCheckConnections);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
    CopyWindowToVram(1, 0);
    CopyWindowToVram(2, COPYWIN_BOTH);
    ShowBg(0);
}

static void CB2_PrintErrorMessage(void)
{
    switch (gMain.state)
    {
    case  00:
        if (sLinkErrorBuffer.unk_06)
        {
            sub_800AE1C();
        }
        else
        {
            sub_800AED0();
        }
        break;
    case  30:
        PlaySE(SE_BOO);
        break;
    case  60:
        PlaySE(SE_BOO);
        break;
    case  90:
        PlaySE(SE_BOO);
        break;
    case 130:
        if (gWirelessCommType == 2)
        {
            AddTextPrinterParameterized3(0, 3, 2, 20, sLinkErrorTextColor, 0, gText_ABtnTitleScreen);
        }
        else if (gWirelessCommType == 1)
        {
            AddTextPrinterParameterized3(0, 3, 2, 20, sLinkErrorTextColor, 0, gText_ABtnRegistrationCounter);
        }
        break;
    }
    if (gMain.state == 160)
    {
        if (gWirelessCommType == 1)
        {
            if (JOY_NEW(A_BUTTON))
            {
                HelpSystem_Enable();
                PlaySE(SE_PIN);
                gWirelessCommType = 0;
                sLinkErrorBuffer.unk_06 = 0;
                ResetSaveHeap();
            }
        }
        else if (gWirelessCommType == 2)
        {
            if (JOY_NEW(A_BUTTON))
            {
                HelpSystem_Enable();
                rfu_REQ_stopMode();
                rfu_waitREQComplete();
                DoSoftReset();
            }
        }
    }
    if (gMain.state != 160)
    {
        gMain.state++;
    }
}

bool8 GetSioMultiSI(void)
{
    return (REG_SIOCNT & SIO_MULTI_SI) != 0;
}

static bool8 IsSioMultiMaster(void)
{
    return (REG_SIOCNT & SIO_MULTI_SD) && !(REG_SIOCNT & SIO_MULTI_SI);
}

bool8 IsLinkConnectionEstablished(void)
{
    return EXTRACT_CONN_ESTABLISHED(gLinkStatus);
}

void SetSuppressLinkErrorMessage(bool8 flag)
{
    gSuppressLinkErrorMessage = flag;
}

bool8 HasLinkErrorOccurred(void)
{
    return gLinkErrorOccurred;
}

void LocalLinkPlayerToBlock(void)
{
    struct LinkPlayerBlock * block;

    InitLocalLinkPlayer();
    block = &gLocalLinkPlayerBlock;
    block->linkPlayer = gLocalLinkPlayer;
    memcpy(block->magic1, sASCIIGameFreakInc, sizeof(block->magic1) - 1);
    memcpy(block->magic2, sASCIIGameFreakInc, sizeof(block->magic2) - 1);
    memcpy(gBlockSendBuffer, block, sizeof(*block));
}

void LinkPlayerFromBlock(u32 who)
{
    u8 who_ = who;
    struct LinkPlayerBlock * block;
    struct LinkPlayer * player;

    block = (struct LinkPlayerBlock *)gBlockRecvBuffer[who_];
    player = &gLinkPlayers[who_];
    *player = block->linkPlayer;
    ConvertLinkPlayerName(player);
    if (strcmp(block->magic1, sASCIIGameFreakInc) != 0 || strcmp(block->magic2, sASCIIGameFreakInc) != 0)
    {
        SetMainCallback2(CB2_LinkError);
    }
}

bool8 HandleLinkConnection(void)
{
    bool32 r4;
    bool32 r5;

    if (gWirelessCommType == 0)
    {
        gLinkStatus = LinkMain1(&gShouldAdvanceLinkState, gSendCmd, gRecvCmds);
        LinkMain2(&gMain.heldKeys);
        if ((gLinkStatus & LINK_STAT_RECEIVED_NOTHING) && IsSendingKeysOverCable() == TRUE)
        {
            return TRUE;
        }
    }
    else
    {
        r4 = LinkRfuMain1();
        r5 = LinkRfuMain2();
        if (IsSendingKeysOverCable() == TRUE)
        {
            if (r4 == TRUE || IsRfuRecvQueueEmpty() || r5)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void SetWirelessCommType1(void)
{
    if (!gReceivedRemoteLinkPlayers)
    {
        gWirelessCommType = 1;
    }
}

static void SetWirelessCommType0(void)
{
    if (!gReceivedRemoteLinkPlayers)
    {
        gWirelessCommType = 0;
    }
}

void SetWirelessCommType0_UnusedCopy(void)
{
    if (!gReceivedRemoteLinkPlayers)
    {
        gWirelessCommType = 0;
    }
}

u32 GetLinkRecvQueueLength(void)
{
    if (gWirelessCommType != 0)
    {
        return GetRfuRecvQueueLength();
    }
    return gLink.recvQueue.count;
}

bool32 LinkRecvQueueLengthMoreThan2(void)
{
    if (GetLinkRecvQueueLength() > 2)
    {
        return TRUE;
    }
    return FALSE;
}

void ConvertLinkPlayerName(struct LinkPlayer * player)
{
    player->progressFlagsCopy = player->progressFlags; // ? Perhaps relocating for a longer name field
    ConvertInternationalString(player->name, player->language);
}

// File break?

static void DisableSerial(void)
{
    DisableInterrupts(INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
    REG_SIOCNT = SIO_MULTI_MODE;
    REG_TM3CNT_H = 0;
    REG_IF = INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL;
    REG_SIOMLT_SEND = 0;
    REG_SIOMLT_RECV = 0;
    CpuFill32(0, &gLink, sizeof(gLink));
}

static void EnableSerial(void)
{
    DisableInterrupts(INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
    REG_RCNT = 0;
    REG_SIOCNT = SIO_MULTI_MODE;
    REG_SIOCNT |= SIO_115200_BPS | SIO_INTR_ENABLE;
    EnableInterrupts(INTR_FLAG_SERIAL);
    REG_SIOMLT_SEND = 0;
    CpuFill32(0, &gLink, sizeof(gLink));
    sNumVBlanksWithoutSerialIntr = 0;
    sSendNonzeroCheck = 0;
    sRecvNonzeroCheck = 0;
    sChecksumAvailable = 0;
    sHandshakePlayerCount = 0;
    gLastSendQueueCount = 0;
    gLastRecvQueueCount = 0;
}

void ResetSerial(void)
{
    EnableSerial();
    DisableSerial();
}

u32 LinkMain1(u8 *shouldAdvanceLinkState, u16 *sendCmd, u16 (*recvCmds)[CMD_LENGTH])
{
    u32 retVal;
    u32 retVal2;

    switch (gLink.state)
    {
    case LINK_STATE_START0:
        DisableSerial();
        gLink.state = LINK_STATE_START1;
        break;
    case LINK_STATE_START1:
        if (*shouldAdvanceLinkState == 1)
        {
            EnableSerial();
            gLink.state = LINK_STATE_HANDSHAKE;
        }
        break;
    case LINK_STATE_HANDSHAKE:
        switch (*shouldAdvanceLinkState)
        {
        default:
            CheckMasterOrSlave();
            break;
        case 1:
            if (gLink.isMaster == LINK_MASTER && gLink.playerCount > 1)
            {
                gLink.handshakeAsMaster = TRUE;
            }
            break;
        case 2:
            gLink.state = LINK_STATE_START0;
            REG_SIOMLT_SEND = 0;
            break;
        }
        break;
    case LINK_STATE_INIT_TIMER:
        InitTimer();
        gLink.state = LINK_STATE_CONN_ESTABLISHED;
        // fallthrough
    case LINK_STATE_CONN_ESTABLISHED:
        EnqueueSendCmd(sendCmd);
        DequeueRecvCmds(recvCmds);
        break;
    }
    *shouldAdvanceLinkState = 0;
    retVal = gLink.localId;
    retVal |= (gLink.playerCount << LINK_STAT_PLAYER_COUNT_SHIFT);
    if (gLink.isMaster == LINK_MASTER)
    {
        retVal |= LINK_STAT_MASTER;
    }
    {
        u32 receivedNothing = gLink.receivedNothing << LINK_STAT_RECEIVED_NOTHING_SHIFT;
        u32 link_field_F = gLink.link_field_F << LINK_STAT_UNK_FLAG_9_SHIFT;
        u32 hardwareError = gLink.hardwareError << LINK_STAT_ERROR_HARDWARE_SHIFT;
        u32 badChecksum = gLink.badChecksum << LINK_STAT_ERROR_CHECKSUM_SHIFT;
        u32 queueFull = gLink.queueFull << LINK_STAT_ERROR_QUEUE_FULL_SHIFT;
        u32 val;

        if (gLink.state == LINK_STATE_CONN_ESTABLISHED)
        {
            val = LINK_STAT_CONN_ESTABLISHED;
            val |= receivedNothing;
            val |= retVal;
            val |= link_field_F;
            val |= hardwareError;
            val |= badChecksum;
            val |= queueFull;
        }
        else
        {
            val = retVal;
            val |= receivedNothing;
            val |= link_field_F;
            val |= hardwareError;
            val |= badChecksum;
            val |= queueFull;
        }

        retVal = val;
    }

    if (gLink.lag == LAG_MASTER)
    {
        retVal |= LINK_STAT_ERROR_LAG_MASTER;
    }

    if (gLink.localId >= MAX_LINK_PLAYERS)
    {
        retVal |= LINK_STAT_ERROR_INVALID_ID;
    }

    retVal2 = retVal;
    if (gLink.lag == LAG_SLAVE)
    {
        retVal2 |= LINK_STAT_ERROR_LAG_SLAVE;
    }

    return retVal2;
}

static void CheckMasterOrSlave(void)
{
    u32 terminals;

    terminals = *(vu32 *)REG_ADDR_SIOCNT & (SIO_MULTI_SD | SIO_MULTI_SI);
    if (terminals == SIO_MULTI_SD && gLink.localId == 0)
    {
        gLink.isMaster = LINK_MASTER;
    }
    else
    {
        gLink.isMaster = LINK_SLAVE;
    }
}

static void InitTimer(void)
{
    if (gLink.isMaster)
    {
        REG_TM3CNT_L = -197;
        REG_TM3CNT_H = TIMER_64CLK | TIMER_INTR_ENABLE;
        EnableInterrupts(INTR_FLAG_TIMER3);
    }
}

static void EnqueueSendCmd(u16 *sendCmd)
{
    u8 i;
    u8 offset;

    gLinkSavedIme = REG_IME;
    REG_IME = 0;
    if (gLink.sendQueue.count < QUEUE_CAPACITY)
    {
        offset = gLink.sendQueue.pos + gLink.sendQueue.count;
        if (offset >= QUEUE_CAPACITY)
        {
            offset -= QUEUE_CAPACITY;
        }
        for (i = 0; i < CMD_LENGTH; i++)
        {
            sSendNonzeroCheck |= *sendCmd;
            gLink.sendQueue.data[i][offset] = *sendCmd;
            *sendCmd = 0;
            sendCmd++;
        }
    }
    else
    {
        gLink.queueFull = QUEUE_FULL_SEND;
    }
    if (sSendNonzeroCheck)
    {
        gLink.sendQueue.count++;
        sSendNonzeroCheck = 0;
    }
    REG_IME = gLinkSavedIme;
    gLastSendQueueCount = gLink.sendQueue.count;
}

static void DequeueRecvCmds(u16 (*recvCmds)[CMD_LENGTH])
{
    u8 i;
    u8 j;

    gLinkSavedIme = REG_IME;
    REG_IME = 0;
    if (gLink.recvQueue.count == 0)
    {
        for (i = 0; i < gLink.playerCount; i++)
        {
            for (j = 0; j < CMD_LENGTH; j++)
            {
                recvCmds[i][j] = 0;
            }
        }

        gLink.receivedNothing = TRUE;
    }
    else
    {
        for (i = 0; i < gLink.playerCount; i++)
        {
            for (j = 0; j < CMD_LENGTH; j++)
            {
                recvCmds[i][j] = gLink.recvQueue.data[i][j][gLink.recvQueue.pos];
            }
        }
        gLink.recvQueue.count--;
        gLink.recvQueue.pos++;
        if (gLink.recvQueue.pos >= QUEUE_CAPACITY)
        {
            gLink.recvQueue.pos = 0;
        }
        gLink.receivedNothing = FALSE;
    }
    REG_IME = gLinkSavedIme;
}

void LinkVSync(void)
{
    if (gLink.isMaster)
    {
        switch (gLink.state)
        {
        case LINK_STATE_CONN_ESTABLISHED:
            if (gLink.serialIntrCounter < 9)
            {
                if (gLink.hardwareError != TRUE)
                {
                    gLink.lag = LAG_MASTER;
                }
                else
                {
                    StartTransfer();
                }
            }
            else if (gLink.lag != LAG_MASTER)
            {
                gLink.serialIntrCounter = 0;
                StartTransfer();
            }
            break;
        case LINK_STATE_HANDSHAKE:
            StartTransfer();
            break;
        }
    }
    else if (gLink.state == LINK_STATE_CONN_ESTABLISHED || gLink.state == LINK_STATE_HANDSHAKE)
    {
        if (++sNumVBlanksWithoutSerialIntr > 10)
        {
            if (gLink.state == LINK_STATE_CONN_ESTABLISHED)
            {
                gLink.lag = LAG_SLAVE;
            }
            if (gLink.state == LINK_STATE_HANDSHAKE)
            {
                gLink.playerCount = 0;
                gLink.link_field_F = FALSE;
            }
        }
    }
}

void Timer3Intr(void)
{
    StopTimer();
    StartTransfer();
}

void SerialCB(void)
{
    gLink.localId = SIO_MULTI_CNT->id;
    switch (gLink.state)
    {
    case LINK_STATE_CONN_ESTABLISHED:
        gLink.hardwareError = SIO_MULTI_CNT->error;
        DoRecv();
        DoSend();
        SendRecvDone();
        break;
    case LINK_STATE_HANDSHAKE:
        if (DoHandshake())
        {
            if (gLink.isMaster)
            {
                gLink.state = LINK_STATE_INIT_TIMER;
                gLink.serialIntrCounter = 8;
            }
            else
            {
                gLink.state = LINK_STATE_CONN_ESTABLISHED;
            }
        }
        break;
    }
    gLink.serialIntrCounter++;
    sNumVBlanksWithoutSerialIntr = 0;
    if (gLink.serialIntrCounter == 8)
    {
        gLastRecvQueueCount = gLink.recvQueue.count;
    }
}

static void StartTransfer(void)
{
    REG_SIOCNT |= SIO_START;
}

static bool8 DoHandshake(void)
{
    u8 i;
    u8 playerCount;
    u16 minRecv;

    playerCount = 0;
    minRecv = 0xFFFF;
    if (gLink.handshakeAsMaster == TRUE)
    {
        REG_SIOMLT_SEND = MASTER_HANDSHAKE;
    }
    else
    {
        REG_SIOMLT_SEND = SLAVE_HANDSHAKE;
    }
    *(u64 *)gLink.tempRecvBuffer = REG_SIOMLT_RECV;
    REG_SIOMLT_RECV = 0;
    gLink.handshakeAsMaster = FALSE;
    for (i = 0; i < 4; i++)
    {
        if ((gLink.tempRecvBuffer[i] & ~0x3) == SLAVE_HANDSHAKE || gLink.tempRecvBuffer[i] == MASTER_HANDSHAKE)
        {
            playerCount++;
            if (minRecv > gLink.tempRecvBuffer[i] && gLink.tempRecvBuffer[i] != 0)
            {
                minRecv = gLink.tempRecvBuffer[i];
            }
        }
        else
        {
            if (gLink.tempRecvBuffer[i] != 0xFFFF)
            {
                playerCount = 0;
            }
            break;
        }
    }
    gLink.playerCount = playerCount;
    if (gLink.playerCount > 1 && gLink.playerCount == sHandshakePlayerCount && gLink.tempRecvBuffer[0] == MASTER_HANDSHAKE)
    {
        return TRUE;
    }
    if (gLink.playerCount > 1)
    {
        gLink.link_field_F = (minRecv & 3) + 1;
    }
    else
    {
        gLink.link_field_F = 0;
    }
    sHandshakePlayerCount = gLink.playerCount;
    return FALSE;
}

static void DoRecv(void)
{
    u16 recv[4];
    u8 i;
    u8 index;

    *(u64 *)recv = REG_SIOMLT_RECV;
    if (gLink.sendCmdIndex == 0)
    {
        for (i = 0; i < gLink.playerCount; i++)
        {
            if (gLink.checksum != recv[i] && sChecksumAvailable)
            {
                gLink.badChecksum = TRUE;
            }
        }
        gLink.checksum = 0;
        sChecksumAvailable = TRUE;
    }
    else
    {
        index = gLink.recvQueue.pos + gLink.recvQueue.count;
        if (index >= QUEUE_CAPACITY)
        {
            index -= QUEUE_CAPACITY;
        }
        if (gLink.recvQueue.count < QUEUE_CAPACITY)
        {
            for (i = 0; i < gLink.playerCount; i++)
            {
                gLink.checksum += recv[i];
                sRecvNonzeroCheck |= recv[i];
                gLink.recvQueue.data[i][gLink.recvCmdIndex][index] = recv[i];
            }
        }
        else
        {
            gLink.queueFull = QUEUE_FULL_RECV;
        }
        gLink.recvCmdIndex++;
        if (gLink.recvCmdIndex == CMD_LENGTH && sRecvNonzeroCheck)
        {
            gLink.recvQueue.count++;
            sRecvNonzeroCheck = 0;
        }
    }
}

static void DoSend(void)
{
    if (gLink.sendCmdIndex == CMD_LENGTH)
    {
        REG_SIOMLT_SEND = gLink.checksum;
        if (!sSendBufferEmpty)
        {
            gLink.sendQueue.count--;
            gLink.sendQueue.pos++;
            if (gLink.sendQueue.pos >= QUEUE_CAPACITY)
            {
                gLink.sendQueue.pos = 0;
            }
        }
        else
        {
            sSendBufferEmpty = FALSE;
        }
    }
    else
    {
        if (!sSendBufferEmpty && gLink.sendQueue.count == 0)
        {
            sSendBufferEmpty = TRUE;
        }
        if (sSendBufferEmpty)
        {
            REG_SIOMLT_SEND = 0;
        }
        else
        {
            REG_SIOMLT_SEND = gLink.sendQueue.data[gLink.sendCmdIndex][gLink.sendQueue.pos];
        }
        gLink.sendCmdIndex++;
    }
}

static void StopTimer(void)
{
    if (gLink.isMaster)
    {
        REG_TM3CNT_H &= ~TIMER_ENABLE;
        REG_TM3CNT_L = -197;
    }
}

static void SendRecvDone(void)
{
    if (gLink.recvCmdIndex == CMD_LENGTH)
    {
        gLink.sendCmdIndex = 0;
        gLink.recvCmdIndex = 0;
    }
    else if (gLink.isMaster)
    {
        REG_TM3CNT_H |= TIMER_ENABLE;
    }
}

void ResetSendBuffer(void)
{
    u8 i;
    u8 j;

    gLink.sendQueue.count = 0;
    gLink.sendQueue.pos = 0;
    for (i = 0; i < CMD_LENGTH; i++)
    {
        for (j = 0; j < QUEUE_CAPACITY; j++)
        {
            gLink.sendQueue.data[i][j] = 0xEFFF;
        }
    }
}

void ResetRecvBuffer(void)
{
    u8 i;
    u8 j;
    u8 k;

    gLink.recvQueue.count = 0;
    gLink.recvQueue.pos = 0;
    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        for (j = 0; j < CMD_LENGTH; j++)
        {
            for (k = 0; k < QUEUE_CAPACITY; k++)
            {
                gLink.recvQueue.data[i][j][k] = 0xEFFF;
            }
        }
    }
}
