//////////////////////////////////////////////////////////////////////
// MulticastSock.cpp
// multicast socket source file
// RAS service should be open
//////////////////////////////////////////////////////////////////////

#include "MulticastSock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMulticastSock::CMulticastSock()
{
    WSADATA wsad;

    m_socket = INVALID_SOCKET;
    m_error  = 0;
    ZeroMemory((char *)&m_mcast, sizeof(m_mcast));

    if(WSAStartup(MAKEWORD(1,1), &wsad) != 0 )
    {
    	m_error = WSAGetLastError();
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
CMulticastSock::~CMulticastSock()
{
    Close();
    if(WSACleanup() != 0)
    {
    	m_error = WSAGetLastError();
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::GetError()
{
    return m_error;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::Create()
{

    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_socket== INVALID_SOCKET)
    {
 	m_error = WSAGetLastError();
        return 0;
    }

    return 1;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::Close()
{
    int ret = 1;
    if(m_socket != INVALID_SOCKET)
    {
  	if(closesocket(m_socket) != 0)
    	{
             m_error = WSAGetLastError();
	     ret = 0;
        }
    }

    m_socket = INVALID_SOCKET;

    return ret;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::Bind(unsigned short port, char *localIp)
{

    ZeroMemory(&m_local, sizeof(m_local));
    m_local.sin_family = AF_INET;
    m_local.sin_port = htons(port);
    if(localIp == NULL)
        m_local.sin_addr.s_addr = htonl (INADDR_ANY);
    else
        m_local.sin_addr.s_addr = inet_addr(localIp);
    if(bind(m_socket, (struct sockaddr *)&m_local, sizeof(m_local)) == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
        Close();
        return 0;
    }
    return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::Join(char *dstIp, char *localIp)
{
    if(dstIp == NULL)
        return 0;

    m_mcast.imr_multiaddr.s_addr = inet_addr(dstIp);
    if(localIp == NULL)
        m_mcast.imr_interface.s_addr = htonl(INADDR_ANY);
    else
        m_mcast.imr_interface.s_addr = inet_addr(localIp);
    if(setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
          (char *)&m_mcast, sizeof(m_mcast)) == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
        return 0;
    }
    return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::Leave(char *dstIp, char *localIp)
{
    struct ip_mreq mcast;

    if(dstIp == NULL)
    {
        mcast.imr_multiaddr = m_mcast.imr_multiaddr;
        ZeroMemory((char *)&m_mcast, sizeof(m_mcast));
    }
    else
    {
        mcast.imr_multiaddr.s_addr = inet_addr(dstIp);
    }
    if(localIp == NULL)
        mcast.imr_interface.s_addr = htonl(INADDR_ANY);
    else
        mcast.imr_interface.s_addr = inet_addr(localIp);

    if(setsockopt(m_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP,
           (char *)&mcast, sizeof(mcast)) == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
        return 0;
    }
    return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::SetLoopbackMode(bool mode)
{
    int optval = mode;
    if(setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_LOOP,
           (char *)&optval, sizeof(optval)) == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
        return 0;
    }

    return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::SetBlockMode(bool mode)
{
    int optval = mode;
    if ( ioctlsocket(m_socket, FIONBIO, (unsigned long *)&optval) == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
        return 0;
    }

    return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::SetTTL(int ttl)
{
    int optval = ttl;
    if(setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_TTL,
           (char *)&optval, sizeof(optval)) == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
        return 0;
    }
    return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::SetInterface(char * localIp)
{
    DWORD face;
    if(localIp == NULL)
        return 0;

    face = inet_addr(localIp);
    if(setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_IF,
             (char *)&face, sizeof(face)) == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
        return 0;
    }
    return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::Sendto(char * dstIp, unsigned short port, char *buf, int len)
{
    int ret;
    struct sockaddr_in toAddr;
    ZeroMemory(&toAddr, sizeof(toAddr));
    toAddr.sin_family = AF_INET;
    toAddr.sin_port = htons(port);
    toAddr.sin_addr.s_addr = inet_addr(dstIp);
    ret = sendto(m_socket, buf, len, 0,
        (struct sockaddr *)&toAddr,
        sizeof(struct sockaddr_in));
    if(ret == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
    }
    return ret;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int CMulticastSock::RecvFrom(char * buf, int len, struct sockaddr_in *fromAddr)
{
    int ret, addrLen = sizeof(struct sockaddr_in);
    if((buf == NULL )||(fromAddr == NULL))
        return 0;
    //ZeroMemory(fromAddr, addrLen);
    ret = recvfrom(m_socket, (char FAR*)buf, len, 0, (struct sockaddr FAR*)fromAddr, &addrLen);
    if(ret == SOCKET_ERROR)
    {
        m_error = WSAGetLastError();
    }
    return ret;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
