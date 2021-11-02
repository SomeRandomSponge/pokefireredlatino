#ifndef GUARD_EREADER_HELPERS_H
#define GUARD_EREADER_HELPERS_H

enum {
    EREADER_XFR_STATE_INIT = 0,
    EREADER_XFR_STATE_HANDSHAKE,
    EREADER_XFR_STATE_START,
    EREADER_XFR_STATE_TRANSFER,
    EREADER_XFR_STATE_TRANSFER_DONE,
    EREADER_XFR_STATE_CHECKSUM,
    EREADER_XFR_STATE_DONE
};

#define EREADER_XFER_EXE 1
#define EREADER_XFER_CHK 2
#define EREADER_XFER_SHIFT 0
#define EREADER_XFER_MASK  3

#define EREADER_CANCEL_TIMEOUT 1
#define EREADER_CANCEL_KEY     2
#define EREADER_CANCEL_MASK  0xC
#define EREADER_CANCEL_SHIFT 2

#define EREADER_CHECKSUM_OK  1
#define EREADER_CHECKSUM_ERR 2
#define EREADER_CHECKSUM_MASK  0x30
#define EREADER_CHECKSUM_SHIFT 4

void EReaderHelper_SerialCallback(void);
void EReaderHelper_Timer3Callback(void);
void EReaderHelper_SaveRegsState(void);
void EReaderHelper_ClearsSendRecvMgr(void);
void EReaderHelper_RestoreRegsState(void);
u16 EReaderHandleTransfer(u8 mode, size_t size, const void * src, void * dest);

#endif //GUARD_EREADER_HELPERS_H
