// TalkClt.cpp: implementation of the CTalkClt class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "TalkClt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTalkClt::CTalkClt()
{
    WSADATA wsad;
	WSAStartup(MAKEWORD(1, 1), &wsad);

    m_Socket = INVALID_SOCKET;
    m_PeerAddr = 0;
    m_LocalPort = 0;
    m_PeerPort = 0;

    WSACleanup();
}

CTalkClt::~CTalkClt()
{
    if(m_Socket != INVALID_SOCKET)
    {
        int t = 1;
        setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&t, sizeof(t));
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
}

int CTalkClt::Start(WORD Port)
{
    struct sockaddr_in addr;
    int len = sizeof(struct sockaddr_in);

    if((m_Socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        return -1;
    }

    ZeroMemory ((char *)&addr, len);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(Port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(m_Socket, (struct sockaddr *)&addr, len) < 0)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
        return -1;
    }

    m_LocalPort = Port;
    if(getsockname(m_Socket, (struct sockaddr FAR*)&addr, (int FAR*)&len) != SOCKET_ERROR)
    {
        m_LocalPort = ntohs(addr.sin_port);
    }

    return 0;
}

int CTalkClt::Send(char *Data, int Len)
{
    char buf[2048];
    struct sockaddr_in addr;
	int n, slen = sizeof(struct sockaddr_in);
	RTP_HEAD *pkt = (RTP_HEAD *)buf;

	if(!m_PeerPort)
		return 0;

    pkt->version= 2;
    pkt->padding = 0;
    pkt->extension = 0;
    pkt->csrcconut = 0;
    pkt->marker = 0;
    pkt->payload = 8;
    pkt->seqNo = htons(m_Seq);
    pkt->timestamp = htonl(m_Seq);
    pkt->ssrc = htonl(m_LocalPort);

	memcpy((char *)(pkt+1), Data, Len);
	m_Seq++;

	n = sizeof(RTP_HEAD) + Len;

	ZeroMemory(&addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(m_PeerPort);
    addr.sin_addr.s_addr = m_PeerAddr;
	if(n == sendto(m_Socket, buf, n, 0, (struct sockaddr *)&addr, slen))
		return 0;

	return -1;
}