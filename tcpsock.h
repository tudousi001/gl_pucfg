//---------------------------------------------------------------------------

#ifndef tcpsockH
#define tcpsockH
//---------------------------------------------------------------------------

#include <winsock2.h>

typedef enum
{
    TCPSOCK_OK = 0,
    TCPSOCK_ERR_SOCKET = -1,
    TCPSOCK_ERR_BIND = -2,
    TCPSOCK_ERR_LISTEN = -3,
    TCPSOCK_ERR_ACCEPT = -4,
    TCPSOCK_ERR_IOCTL = -5,
    TCPSOCK_ERR_CONNECT = -6,
    TCPSOCK_ERR_TIMEOUT = -7,
    TCPSOCK_ERR_FAILED = -8,
    TCPSOCK_ERR_RECV = -9,
    TCPSOCK_ERR_SEND = -10,
    TCPSOCK_THREAD = -11,
}TCPSOCK_ERROR;

class CTcpSock
{
protected:
    SOCKET m_Socket;
    DWORD m_LocalAddr;
    DWORD m_PeerAddr;
    WORD m_LocalPort;
    WORD m_PeerPort;

public:
    CTcpSock();
    ~CTcpSock();

    int SetSocket(SOCKET Socket);
    int Start(WORD Port = 0);
    int Close();
    int Listen(int backlog = 1);
    int Accept();
    int Link(DWORD ServerIp, WORD ServerPort, int TimeOut = -1/*ms*/);
    int Link(char *ServerIp, WORD ServerPort, int TimeOut = -1/*ms*/);
    int Block(int Mode);
    int SetSendBuff(int Size);
    int SetRecvBuff(int Size);
    int WaitData(int TimeOut = -1);
    int Read(char *buf, int len);
    int Peek(char *buf, int len);
    int Send(char *buf, int len);
    DWORD GetLocalIp();
    DWORD GetPeerIp();
    int GetLocalPort(){return m_LocalPort;}
    int GetPeerPort(){return m_PeerPort;}
    SOCKET Socket(){return m_Socket;}
};

#endif
