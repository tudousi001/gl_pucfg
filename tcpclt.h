/***********************************************************
*tcpclt.h - tcp client head file
*
* Copyright(c) 2007, HCMind
*
*
*modification history
*-----------------------
*
*01a, 07-03-19, Zhushuchao created
*
************************************************************/

#ifndef __TCP_CLIENT_H_
#define __TCP_CLIENT_H_

#include <winsock2.h>

#define MAX_CMD_BODY_SIZE 4096

typedef enum
{
    CLIENT_OK = 0,
    CLEINT_ERR_SOCKET = -1,
    CLIENT_ERR_BIND = -2,   
    CLIENT_ERR_IOCTL = -3,
    CLIENT_ERR_CONNECT = -4,
    CLIENT_ERR_TIMEOUT = -5,
    CLIENT_ERR_FAILED = -6,
    CLIENT_ERR_RECV = -7,
    CLIENT_ERR_SEND = -8,
    CLIENT_ERR_THREAD = -9,
}CLIENT_ERROR;

class CTcpClient
{
protected:
    SOCKET m_Socket;
    DWORD m_LocalAddr;
    DWORD m_PeerAddr;
    WORD m_LocalPort;
    WORD m_PeerPort;

public:
    CTcpClient();
    ~CTcpClient();

    int Start(WORD Port = 0);
    int Link(DWORD ServerIp, WORD ServerPort, int TimeOut = -1/*ms*/);
    int WaitData(int TimeOut = -1);
    int Read(char *buf, int len);
    int Send(char *buf, int len);
    DWORD GetLocalIp();
    DWORD GetPeerIp();
    int GetLocalPort(){return m_LocalPort;}
    int GetPeerPort(){return m_PeerPort;}
};


#endif
 