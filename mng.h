/***********************************************************
*mng.h - manger handle head file
*
* Copyright(c) 2007~  , HCMIND
*
*
*modification history
*-----------------------
*
*01a, 07-01-01, Zhushuchao created
*
************************************************************/

#ifndef _MANGER_H_
#define _MANGER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "defs.h"

#define MNG_CMD_FLAG 0xf0
#define MNG_CMD_SIZE 2048

enum
{
    MNG_CMD_REGISTER = 0x10,
    MNG_CMD_GET_INFO = 0x20,
    MNG_CMD_GET_FILE = 0x30,
};

enum
{
    TCP_BUF_SIZE_LEVEL_DEFAULT,
    TCP_BUF_SIZE_LEVEL_512,
    TCP_BUF_SIZE_LEVEL_1K,
    TCP_BUF_SIZE_LEVEL_2K,
    TCP_BUF_SIZE_LEVEL_4K,
    TCP_BUF_SIZE_LEVEL_8K,
    TCP_BUF_SIZE_LEVEL_16K,
    TCP_BUF_SIZE_LEVEL_32K,
    TCP_BUF_SIZE_LEVEL_64K,
};

typedef struct
{
    UINT8 flag;
    UINT8 type;
    UINT8 cmd;
    UINT8 subCmd;
    UINT16 seq;
    UINT16 extlen;
}MNG_CMD_HEAD;

typedef struct _MNG_CLIENT
{
    int state;
    int error;
    int sock;
    UINT32 localIP;
    UINT32 remoteIP;
    int localPort;
    int remotePort;
    THREAD_ID threadId;
    UINT8 Msg;
    UINT8 SubMsg;
    UINT16 Seq;
    int Recved;
    int Length;
    UINT linkTime;
    void *data;
    char ContectBuf[MNG_CMD_SIZE];
}MNG_CLIENT;

#ifdef __cplusplus
}
#endif

#endif

