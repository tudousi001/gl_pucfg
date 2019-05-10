//---------------------------------------------------------------------------
#include "QCMngClt.h"

//---------------------------------------------------------------------------

CMClient::CMClient()
{
    m_Seq = 0;
    m_RSeq = 0;
}

CMClient::~CMClient()
{
    Stop();
}

void CCfgClient::Stop()
{
    if(m_Socket != INVALID_SOCKET)
    {
        //shutdown(m_Socket, SD_SEND);
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
}

int CCfgClient::Connect(char *Host, WORD ServerPort, WORD LocalPort)
{
    int i, ret;
    DWORD tid;

    ret = Start(LocalPort);
    if(ret != TCPSOCK_OK)
    {
        return ret;
    }

    for(i = 0; i < 2; i++)
    {
        ret = Link(Host, ServerPort, 2000);
        if(ret == TCPSOCK_OK)
        {
            break;
        }
    }
    if(i >= 2)
        return ret;

    return TCPSOCK_OK;
}

int CCfgClient::SendMsg(int msg, void *para, int len)
{
    MNG_CMD_HEAD cmd;

    cmd.flag = MNG_CMD_FLAG;
    cmd.type = 0;
    cmd.cmd = htons(msg);
    cmd.seq = htons(m_Seq);
    m_Seq++;
    cmd.extlen = htons(len);

    if(Send(&cmd, sizeof(MNG_CMD_HEAD)) != TCPSOCK_OK)
        return -1;

    if(Send(para, len) != TCPSOCK_OK)
        return -2;

    return 0;
}

int CCfgClient::GetData(char *buf, int len)
{
    int rd, size = 0;
    while(size < len)
    {
        rd = Read(buf+size, len-size);
        if(rd <= 0)
            return -1;
        size += rd;
    }
    return 0;
}

enum
{
    RSP_ERR_NONE = 0,
    RSP_ERR_BUF = 1,    //缓冲区太小
    RSP_ERR_WAIT,       //等待数据失败
    RSP_ERR_TIMEOUT,    //等待数据超时
    RSP_ERR_READ,       //读数据失败
    RSP_ERR_FLAG,       //数据非法
    RSP_ERR_CMD,        //命令非法
};

int CCfgClient::GetRsp(WORD msg, WORD seq, void *buf, int *len, int timeout)
{
    MNG_CMD_HEAD cmd;
    int rd;
    short rspLen;

    rd = WaitData(timeout);
    if(rd == CLIENT_ERR_FAILED)
    {
        return RSP_ERR_WAIT;
    }
    if(rd == CLIENT_ERR_TIMEOUT)
    {
        return RSP_ERR_TIMEOUT;
    }

    rd = GetData(&cmd, sizeof(cmd));
    if(rd <= 0)
    {
        return RSP_ERR_READ;
    }

    if(msg.flag != MNG_CMD_FLAG)
    {
        return RSP_ERR_FLAG;
    }

    if((msg != ntohs(msg->cmd))||(ntohs(msg->seq) != seq))
    {
        return RSP_ERR_CMD;
    }

    rspLen = ntohs(msg->extlen);
    if(rspLen <= 0)
    {
        return rspLen;
    }

    if(rspLen > *len)
    {
        return RSP_ERR_BUF;
    }

    if(GetData((char *)buf, rspLen) <= 0)
    {
        return RSP_ERR_READ;
    }

    *len = rspLen;

    return 0;
}

int CCfgClient::SetPara(int msg, int chn, void *para, int len)
{
    int ret;
    if(ret = SendMsg())
    {
        
    }
}

int CCfgClient::GetPara(int msg, int chn, void *para, int len)
{

}