/***********************************************************
*fileopt.h - file operation head file
*
* Copyright(c) 2007~  , HC
*
*$Date: 2009/02/03 06:52:23 $ 
*$Author: guoqingli $
*$Revision: 1.1.1.2 $
*
*-----------------------
*$Log: fileopt.h,v $
*Revision 1.1.1.2  2009/02/03 06:52:23  guoqingli
*no message
*
*Revision 1.1.1.1  2009/01/13 09:07:01  zhushuchao
*checkin
*
*
*01a, 07-11-26, Zhushuchao created
*
************************************************************/
#ifndef __FILE_OPT_H_
#define __FILE_OPT_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    FILE_OPEN_MODE_READ,
    FILE_OPEN_MODE_WRITE,
    FILE_OPEN_MODE_RW,
    FILE_OPEN_MODE_CREAT,
    FILE_OPEN_MODE_APPEND,
    FILE_OPEN_MODE_TRUNC,
    FILE_OPEN_MODE_EXE = 10,    
}FILE_OPEN_MODE;

int fileOpen(const char *name, int mode);
int fileWrite(int fd, char *data, int len);
int fileRead(int fd, char *buf, int len);
int fileClose(int fd);
int fileSize(int fd);
int fileSeek(int fd, int offset);

#ifdef __cplusplus
}
#endif

#endif


