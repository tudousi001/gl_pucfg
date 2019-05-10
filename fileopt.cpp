/***********************************************************
*fileopt.c - file operation source file
*
* Copyright(c) 2007~  , HC
*
*$Date: 2009/02/03 06:52:24 $ 
*$Author: guoqingli $
*$Revision: 1.1.1.2 $
*
*-----------------------
*$Log: fileopt.c,v $
*Revision 1.1.1.2  2009/02/03 06:52:24  guoqingli
*no message
*
*Revision 1.1.1.1  2009/01/13 09:07:01  zhushuchao
*checkin
*
*
*01a, 07-11-26, Zhushuchao created
*
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <io.h>
//#include <unistd.h>
#include "defs.h"
#include "fileopt.h"

#ifdef _DEBUG
#define DBG_INFO(x) printf x
#else
#define DBG_INFO(x)
#endif

int fileOpen(const char *name, int mode)
{
    switch(mode)
    {
        case FILE_OPEN_MODE_READ:
            return open(name, O_RDONLY, 0666);
        case FILE_OPEN_MODE_WRITE:
            return open(name, O_WRONLY, 0666);    
        case FILE_OPEN_MODE_RW:
            return open(name, O_RDWR, 0666);    
        case FILE_OPEN_MODE_CREAT:
            return open(name, O_RDWR|O_CREAT, 0666); 
        case FILE_OPEN_MODE_APPEND:
            return open(name, O_RDWR|O_CREAT|O_APPEND, 0666);    
        case FILE_OPEN_MODE_TRUNC:
            return open(name, O_RDWR|O_CREAT|O_TRUNC, 0666);   
        case FILE_OPEN_MODE_EXE:
            return open(name, O_RDWR|O_CREAT|O_TRUNC, 0777);   
        default:
            return ERROR;
    }
}

int fileWrite(int fd, char *data, int len)
{
    int n = write(fd, data, len);

    if(n <= 0)
    {
        DBG_INFO(("fileWrite:write failed %d(%s)\n", n, strerror(errno)));
        return ERROR;
    }
    if(n != len)
    {
        DBG_INFO(("fileWrite:write %d-%d\n", len, n));
        return ERROR;        
    }
    return OK;
}

int fileRead(int fd, char *buf, int len)
{
    return read(fd, buf, len);
}

int fileClose(int fd)
{
    return close(fd);
}

int fileSize(int fd)
{
    return lseek(fd, 0, SEEK_END);
}

int fileSeek(int fd, int offset)
{
    return lseek(fd, offset, SEEK_SET);
}

