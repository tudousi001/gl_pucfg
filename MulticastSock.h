//////////////////////////////////////////////////////////////////////
// MulticastSock.h
// multicast socket source head file
// RAS service should be open
//////////////////////////////////////////////////////////////////////

#if !defined(_MULICAST_H)
#define _MULICAST_H

#include <winsock.h>
//#incldue <ws2tcpip.h>

class CMulticastSock
{
public:	
	SOCKET	m_socket;
	int	m_error;
	
private:
	struct sockaddr_in m_local;
        struct ip_mreq m_mcast; //save the last leaf

public:
	CMulticastSock();
	virtual ~CMulticastSock();

        int GetError();
	int Create();
	int Close();
	int Bind(unsigned short port = 0, char *localIp = NULL);
	int Join(char *dstIp = NULL, char *localIp = NULL);
	int Leave(char *dstIp = NULL, char *localIp = NULL);
        int SetLoopbackMode(bool mode = 1);  /*default loopback enable*/
        int SetBlockMode(bool mode = 1);     /*default block mode*/
        int SetTTL(int ttl = 1);
        int SetInterface(char * localIp = NULL);
        int Sendto(char * dstIp, unsigned short port, char *buf, int len);
        int RecvFrom(char * buf, int len, struct sockaddr_in *fromAddr);
};

#endif 
