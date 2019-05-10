//---------------------------------------------------------------------------


#pragma hdrstop

#include <stdio.h>
#include "dataclt.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CDataClient::CDataClient(TDisplayForm *Displayer, char *callId)
{
    m_Thread = NULL;
    m_State = 0;
    m_Displayer = Displayer;
    strcpy(m_CallId, callId);
}

CDataClient::~CDataClient()
{
    Stop();
}

void CDataClient::Stop()
{
    m_State = 0;
    if(m_Socket != INVALID_SOCKET)
    {
        //shutdown(m_Socket, SD_SEND);
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    } 

    if(m_Thread)
    {
        WaitForSingleObject(m_Thread, 2000);
        CloseHandle(m_Thread);
        m_Thread = NULL;
    }
}

int CDataClient::Connect(DWORD ServerIp, WORD ServerPort, WORD LocalPort)
{
    int ret;
    DWORD tid;
    char tmp[128];

    ret = Start(LocalPort);
    if(ret != CLIENT_OK)
    {
        return ret;
    }

    for(int i = 0; i < 3; i++)
    {
        ret = Link(ServerIp, ServerPort, 3000);
        if(ret == CLIENT_OK)
            break;
    }
    if(ret != CLIENT_OK)
    {
        return ret;
    }

    m_State = 1;
    m_Thread = CreateThread(0, 0, DThread, this, 0, &tid);
    if(m_Thread == NULL)
    {
        return CLIENT_ERR_THREAD;
    }

    sprintf(tmp, "INFO * SIP/2.0\r\nCall-ID: %s\r\n\r\n", m_CallId);

    Send(tmp, strlen(tmp));

    return CLIENT_OK;
}

DWORD WINAPI CDataClient::DThread(LPVOID Para)
{
    int i,rd, ret = 0;
    char buf[2048];
    CDataClient *This = (CDataClient *)Para;     
    DWORD tick, tick0 = GetTickCount(); 
    DWORD total, recved = 0, pkts = 0;
    AnsiString S;
    int timeOut = 0;

    while(This->m_State)
    {
        rd = This->WaitData(1000);
        if(rd == CLIENT_ERR_FAILED)
        {
            ret = -1;
            S = "Socket错误";
            break;
        }
        if(rd == CLIENT_ERR_TIMEOUT)
        {
            timeOut++;
            if(timeOut > 15)
            {
            S = "接收超时";
            //continue;
            break;
            }
        }

        rd = This->Read(buf, 2048);
        if(rd <= 0)
        {
            ret = -2;
            S = "接收错误";
            break;
        }
        This->m_Displayer->AddData(buf, rd);
        recved += rd;
        pkts++;
                //if(pkts >= 60)
                {
                    tick = GetTickCount();
                    if(tick < tick0)
                       tick = tick + (~tick0) + 1;
                    else
                       tick = tick - tick0;
                    if(tick >= 1000)
                    {
                    i = recved*8/tick;
                    This->m_Displayer->RefreshRate(i);
                    tick0 = GetTickCount();
                    recved = 0;
                    pkts = 0;
                    }
                }
    }
    if(This->m_State)
    {
        try
        {
        if(This->m_Displayer)This->m_Displayer->AddInfo(0, S);
        if(This->m_Displayer)This->m_Displayer->AddInfo(1, "停止");
        }
        catch(...)
        {
        }
    }
    return ret;
}
