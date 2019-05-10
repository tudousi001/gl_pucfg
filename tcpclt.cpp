/***********************************************************
*tcpclt.cpp - tcp client source file
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

#include "tcpclt.h"

CTcpClient::CTcpClient()
{
    WSADATA wsaData;

    m_Socket = INVALID_SOCKET;
    m_LocalPort = 0;
    m_LocalAddr = 0;
    m_PeerPort = 0;
    m_PeerAddr = 0;

    WSAStartup(MAKEWORD(1, 1), &wsaData);
}

CTcpClient::~CTcpClient()
{
    if(m_Socket != INVALID_SOCKET)
    {
        //shutdown(m_Socket, SD_SEND);
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }

    WSACleanup();
}

int CTcpClient::Start(WORD Port)
{
    struct sockaddr_in addr;
    int t, len = sizeof(struct sockaddr_in);

    if((m_Socket = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        return CLEINT_ERR_SOCKET;
    }

    ZeroMemory ((char *)&addr, len);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(Port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(m_Socket, (struct sockaddr *)&addr, len) < 0)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
        return CLIENT_ERR_BIND;
    }

    t = 1;
    if(setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&t, sizeof(t))
       == SOCKET_ERROR)
    {
    }

    t = 1;
    if(setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (char *)&t, sizeof(t))
       == SOCKET_ERROR)
    {
    }

    m_LocalPort= Port;
    if(getsockname(m_Socket, (struct sockaddr FAR*)&addr, (int FAR*)&len) != SOCKET_ERROR)
    {
        m_LocalPort = ntohs(addr.sin_port);
    }

    return CLIENT_OK;
}

int CTcpClient::Link(DWORD ServerIp, WORD ServerPort, int TimeOut)
{
    struct sockaddr_in addr;
    int ret, len = sizeof(struct sockaddr_in);

    if(m_Socket == INVALID_SOCKET)
    {
        ret = Start();
        if(ret != CLIENT_OK)
        return ret;
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
            return CLIENT_ERR_IOCTL;
        }

        FD_ZERO(&rFds);
        FD_SET(m_Socket, &rFds);
        if(connect(m_Socket, (struct sockaddr *)&addr, len) < 0)
        {
            //return CLIENT_ERR_CONNECT;
        }

        tv.tv_sec = TimeOut/1000;
        tv.tv_usec = TimeOut%1000;
        if(select(0, 0, &rFds, 0, &tv) <= 0)
        {
            return CLIENT_ERR_FAILED;
        }

        flag = 0;
        if(ioctlsocket(m_Socket, FIONBIO, &flag) < 0)
        {
            return CLIENT_ERR_IOCTL;
        }
    }
    else
    {
        if(connect(m_Socket, (struct sockaddr *)&addr, len) < 0)
        {
            int n = WSAGetLastError();
            return CLIENT_ERR_CONNECT;
        }
    }

    m_PeerAddr = ServerIp;
    m_PeerPort = ServerPort;
    if(getsockname(m_Socket, (struct sockaddr FAR*)&addr, (int FAR*)&len) != SOCKET_ERROR)
    {
        m_LocalPort = ntohs(addr.sin_port);
        m_LocalAddr = addr.sin_addr.s_addr;
    }

    return CLIENT_OK;
}

DWORD CTcpClient::GetLocalIp()
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

DWORD CTcpClient::GetPeerIp()
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
        return Addr.sin_addr.s_addr;
}

int CTcpClient::WaitData(int TimeOut)
{
    fd_set rFds;
    int ret;

    FD_ZERO(&rFds);
    FD_SET(m_Socket, &rFds);
    if(TimeOut != -1)
    {
        struct timeval tv;
        tv.tv_sec = TimeOut/1000;
        tv.tv_usec = TimeOut%1000;
        ret = select(FD_SETSIZE, &rFds, 0, 0, &tv);
    }
    else
    {
        ret = select(FD_SETSIZE, &rFds, 0, 0, 0);
    }

    if(ret < 0)
    {
        return CLIENT_ERR_FAILED;
    }

    if(ret == 0)
    {
        return CLIENT_ERR_TIMEOUT;
    }

    return CLIENT_OK;
}

int CTcpClient::Read(char *buf, int len)
{
    return recv(m_Socket, buf, len, 0);
}

int CTcpClient::Send(char *buf, int len)
{
    int x, n = 0;

    while(n < len)
    {
        x = send(m_Socket, buf+n, len-n, 0);
        if(x <= 0)
        {
            return CLIENT_ERR_SEND;
        }

        n += x;
    }
    return CLIENT_OK;
    //return send(m_Socket, buf, len, 0);
}



