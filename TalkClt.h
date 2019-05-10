// TalkClt.h: interface for the CTalkClt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TALKCLT_H__0A2B7039_65CE_4FD0_A93A_AEE8C67B8E65__INCLUDED_)
#define AFX_TALKCLT_H__0A2B7039_65CE_4FD0_A93A_AEE8C67B8E65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>

typedef struct
{
    UINT8 csrcconut:4; /*csrc count*/
    UINT8 extension:1; /*Extension-X bit*/
    UINT8 padding:1; /*padding*/
    UINT8 version:2; /*version*/
    
    UINT8 payload:7; /*payload type*/
    UINT8 marker:1; /*��־λ��Marker-M�� bit*/
    UINT16 seqNo; /*Sequence Number*/

    UINT32 timestamp;
    UINT32 ssrc;   /*ͬ��Դ��ʶ*/
    /*_UINT32 csrcs[];*/ /*���ñ�ʶ�б�������ڻ��������ʱ,0~15��*/
    /*byte aligned RTP Payload*/
    /*16 bits optional RTP padding*/
}RTP_HEAD;

class CTalkClt  
{
protected:
    SOCKET m_Socket;
    DWORD m_PeerAddr;
    WORD m_PeerPort;
    WORD m_LocalPort;

	WORD m_Seq;

public:
	CTalkClt();
	virtual ~CTalkClt();

    int Start(WORD Port = 0);
    int GetPort(){return m_LocalPort;}
	void SetPeer(DWORD addr, WORD port){m_PeerAddr = addr; m_PeerPort = port;}
	int Send(char *Data, int Len);
};

#endif // !defined(AFX_TALKCLT_H__0A2B7039_65CE_4FD0_A93A_AEE8C67B8E65__INCLUDED_)
