//---------------------------------------------------------------------------

#ifndef QCMngCltH
#define QCMngCltH
//---------------------------------------------------------------------------
#include "tcpclt.h"
#include "mngmsg.h"

class CQCMClt : public CTcpClient
{
private:
    WORD m_Seq;
    WORD m_RSeq;

    int ReadData(void *buf, int len);       
    int GetRsp(void *buf, int len, int timeout = 10000);
public:
    CQCMClt();
    ~CQCMClt();
    int Connect(char *Host, WORD ServerPort, WORD LocalPort = 0);
    int SendMsg(int msg, void *para, int len);
    int SetPara(int chn, int msg, void *para, int len);
    int GetPara(int chn, int msg, void *para, int len);
};

#endif
