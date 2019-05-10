/***********************************************************
*tcpsock.cpp - tcp socket source file
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
#pragma hdrstop

#include "tcpsock.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CTcpSock::CTcpSock()
{
    WSADATA wsaData;

    m_Socket = INVALID_SOCKET;
    m_LocalPort = 0;
    m_LocalAddr = 0;
    m_PeerPort = 0;
    m_PeerAddr = 0;

    WSAStartup(MAKEWORD(1, 1), &wsaData);
}

CTcpSock::~CTcpSock()
{
    Close();

    WSACleanup();
}

int CTcpSock::SetSocket(SOCKET Socket)
{
    struct sockaddr_in addr;
    int len = sizeof(struct sockaddr_in);

    m_Socket = Socket;

    if(getsockname(m_Socket, (struct sockaddr FAR*)&addr, (int FAR*)&len) != SOCKET_ERROR)
    {
        m_LocalPort = ntohs(addr.sin_port);  
        m_LocalAddr = addr.sin_addr.s_addr;
        GetPeerIp();
        return TCPSOCK_OK;
    }       

    return TCPSOCK_ERR_SOCKET;
}

int CTcpSock::Start(WORD Port)
{
    struct sockaddr_in addr;
    int t, len = sizeof(struct sockaddr_in);

    if((m_Socket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        return TCPSOCK_ERR_BIND;
    }

    ZeroMemory ((char *)&addr, len);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(Port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(m_Socket, (struct sockaddr *)&addr, len) < 0)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
        return TCPSOCK_ERR_BIND;
    }

    t = 1;
    if(setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&t, sizeof(t))
       == SOCKET_ERROR)
    {
    }

    m_LocalPort= Port;
    if(getsockname(m_Socket, (struct sockaddr FAR*)&addr, (int FAR*)&len) != SOCKET_ERROR)
    {
        m_LocalPort = ntohs(addr.sin_port);
    }

    return TCPSOCK_OK;
}

int CTcpSock::Close()
{
    if(m_Socket != INVALID_SOCKET)
    {
        //shutdown(m_Socket, SD_SEND);
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
    return TCPSOCK_OK;
}

int CTcpSock::Listen(int backlog)
{
    return listen(m_Socket, backlog);
}

int CTcpSock::Accept()
{
    struct sockaddr_in addr;
    int len = sizeof(addr);

    SOCKET sock = accept(m_Socket, (struct sockaddr *)&addr, &len);
    if(INVALID_SOCKET == sock)
        return TCPSOCK_ERR_ACCEPT;

    m_PeerAddr = addr.sin_addr.s_addr;
    m_PeerPort = ntohs(addr.sin_port);
    closesocket(m_Socket);
    m_Socket = sock;

    return TCPSOCK_OK;
}

int CTcpSock::Link(DWORD ServerIp, WORD ServerPort, int TimeOut)
{
    struct sockaddr_in addr;
    int len = sizeof(struct sockaddr_in);

    if(m_Socket == INVALID_SOCKET)
    {
        return TCPSOCK_ERR_SOCKET;
    }

    ZeroMemory ((char *)&addr, len);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(ServerPort);
    addr.sin_addr.s_addr = ServerIp;

    if(TimeOut != -1)
    {
        unsigned long flag = 1;
        fd_set rFds;
        struct timeval tv;

        if(ioctlsocket(m_Socket, FIONBIO, &flag) < 0)
        {
            return TCPSOCK_ERR_IOCTL;
        }

        FD_ZERO(&rFds);
        FD_SET(m_Socket, &rFds);
        if(connect(m_Socket, (struct sockaddr *)&addr, len) < 0)
        {
            //return TCPSOCK_ERR_CONNECT;
        }

        tv.tv_sec = TimeOut/1000;
        tv.tv_usec = (TimeOut%1000)*1000;
        if(select(0, 0, &rFds, 0, &tv) <= 0)
        {
            return TCPSOCK_ERR_FAILED;
        }

        flag = 0;
        if(ioctlsocket(m_Socket, FIONBIO, &flag) < 0)
        {
            return TCPSOCK_ERR_IOCTL;
        }
    }
    else
    {
        if(connect(m_Socket, (struct sockaddr *)&addr, len) < 0)
        {
            //int n = WSAGetLastError();
            return TCPSOCK_ERR_CONNECT;
        }
    }

    m_PeerAddr = ServerIp;
    m_PeerPort = ServerPort;
    if(getsockname(m_Socket, (struct sockaddr FAR*)&addr, (int FAR*)&len) != SOCKET_ERROR)
    {
        m_LocalPort = ntohs(addr.sin_port);
        m_LocalAddr = addr.sin_addr.s_addr;
    }

    return TCPSOCK_OK;
}

int CTcpSock::Link(char *ServerIp, WORD ServerPort, int TimeOut)
{
    int ret, n;
    struct in_addr *p;
    struct hostent *ht = gethostbyname(ServerIp);
    
    if(ht)
    {
        n = 0;
        while((p = (struct in_addr *)ht->h_addr_list[n]) != NULL)
        {
            ret = Link(p->s_addr, ServerPort, TimeOut);
            if(ret == TCPSOCK_OK)
                return ret;
            n++;
        }
    }

    return TCPSOCK_ERR_FAILED;
}

int CTcpSock::Block(int Mode)
{   
    unsigned long flag;
    flag = Mode ? 0:1;

    if(ioctlsocket(m_Socket, FIONBIO, &flag) < 0)
    {
        return TCPSOCK_ERR_IOCTL;
    }

    return TCPSOCK_OK;
}

int CTcpSock::SetSendBuff(int Size)
{
    if(setsockopt (m_Socket, SOL_SOCKET, SO_SNDBUF, (char *)&Size, sizeof (int)) == SOCKET_ERROR)
    {
        return TCPSOCK_ERR_FAILED;
    }
    
    return TCPSOCK_OK;
}

int CTcpSock::SetRecvBuff(int Size)
{
    if(setsockopt (m_Socket, SOL_SOCKET, SO_RCVBUF, (char *)&Size, sizeof (int)) == SOCKET_ERROR)
    {
        return TCPSOCK_ERR_FAILED;
    }

    return TCPSOCK_OK;
}

DWORD CTcpSock::GetLocalIp()
{
    struct sockaddr_in Addr;
    int len = sizeof(struct sockaddr_in);

    if(m_Socket == INVALID_SOCKET)
    {
        return 0;
    }

    if(getsockname(m_Socket, (struct sockaddr FAR*)&Addr, (int FAR*)&len) == SOCKET_ERROR)
        return 0;
    else
        return Addr.sin_addr.s_addr;
}

DWORD CTcpSock::GetPeerIp()
{
    struct sockaddr_in Addr;
    int len = sizeof(struct sockaddr_in);

    if(m_Socket == INVALID_SOCKET)
    {
        return 0;
    }

    if(getpeername(m_Socket, (struct sockaddr FAR*)&Addr, (int FAR*)&len) == SOCKET_ERROR)
        return 0;
    else
    {
        m_PeerPort = ntohs(Addr.sin_port);
        return Addr.sin_addr.s_addr;
    }
}

int CTcpSock::WaitData(int TimeOut)
{
    fd_set rFds;
    int ret;

    FD_ZERO(&rFds);
    FD_SET(m_Socket, &rFds);
    if(TimeOut != -1)
    {
        struct timeval tv;
        tv.tv_sec = TimeOut/1000;
        tv.tv_usec = (TimeOut%1000)*1000;
        ret = select(FD_SETSIZE, &rFds, 0, 0, &tv);
    }
    else
    {
        ret = select(FD_SETSIZE, &rFds, 0, 0, 0);
    }

    if(ret < 0)
    {
        return TCPSOCK_ERR_FAILED;
    }

    if(ret == 0)
    {
        return TCPSOCK_ERR_TIMEOUT;
    }

    return TCPSOCK_OK;
}

int CTcpSock::Read(char *buf, int len)
{
    if(m_Socket == INVALID_SOCKET)
        return -1;
    
    return recv(m_Socket, buf, len, 0);
}

int CTcpSock::Peek(char *buf, int len)
{
    return recv(m_Socket, buf, len, MSG_PEEK);
}

int CTcpSock::Send(char *buf, int len)
{
    int x, n = 0;

    while(n < len)
    {
        x = send(m_Socket, buf+n, len-n, 0);
        if(x <= 0)
        {
            return TCPSOCK_ERR_SEND;
        }

        n += x;
    }
    return TCPSOCK_OK;
}
