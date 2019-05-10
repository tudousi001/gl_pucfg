#ifndef _CMD_H_
#define _CMD_H_

#define CMD_FLAG 0xf0

enum
{
    CMD_REGISTER = 0x10,
    CMD_GET_INFO = 0x20,
    CMF_GET_FILE = 0x30,
};

typedef struct
{
    UINT8 flag;
    UINT8 cmd;
    UINT8 len[2];
}CMD_HEAD;

#endif
 