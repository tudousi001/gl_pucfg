//---------------------------------------------------------------------------

#ifndef datacltH
#define datacltH
//---------------------------------------------------------------------------

#include "tcpclt.h"
#include "display.h"

class CDataClient : public CTcpClient
{
private:
    HANDLE m_Thread;
    DWORD m_State;
    TDisplayForm *m_Displayer;
    char m_CallId[64];

    static DWORD WINAPI DThread(LPVOID Para);
public:
    CDataClient(TDisplayForm *Displayer, char *callId);
    ~CDataClient();

    int Connect(DWORD ServerIp, WORD ServerPort, WORD LocalPort = 0);
    void Stop();
};

#endif
 