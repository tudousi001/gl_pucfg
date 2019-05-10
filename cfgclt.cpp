//---------------------------------------------------------------------------


#pragma hdrstop

#include <stdio.h>
#include "cfgclt.h"
#include "cmds.h"
#include "md5.h"
#include "main.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


CMClient::CMClient()
{
    m_Thread = NULL;
    m_State = 0;
    m_Seq = 0;
    m_RSeq = 0;
    m_Level = 0;

    m_Basic = 0;
    m_vChn = 0;
    m_vExVideo = 0;
    m_Osd = 0;
    m_OsdEx = 0;
    m_Alarm = 0;
    m_Gpin = 0;
    m_AlarmPara = 0;
    m_vSub = 0;
    m_PtInfo = 0;
    m_AdInfo = 0;
    m_Storage = 0;
    m_DVRInfo = 0;
    //m_DevTime = 0;
    m_sysInfo = 0;
    m_DevIdInfo = 0;
    m_nvr = 0;
    m_env = 0;
    m_switch = 0;
	
    m_vNum = 0;
    m_GpinNum = 0;
    m_file = NULL;
    m_fileSize = 0;
    m_fileRecved = 0;

    m_St = 0;
}

CMClient::~CMClient()
{
    Stop();
    if(m_Basic)
        delete m_Basic;
    if(m_vChn)
        delete m_vChn;
    if(m_vExVideo)
        delete m_vExVideo;     
    if(m_Osd)
        delete m_Osd;
    if(m_OsdEx)
        delete m_OsdEx;
    if(m_Alarm)
        delete m_Alarm;    
    if(m_Gpin)
        delete m_Gpin;
    if(m_AlarmPara)
        delete m_AlarmPara;
    if(m_vSub)
        delete m_vSub;
    if(m_PtInfo)
        delete m_PtInfo;
    if(m_AdInfo)
        delete m_AdInfo;
    if(m_Storage)
        delete []m_Storage;
    if(m_DVRInfo)
        delete m_DVRInfo;
    if(m_sysInfo)
        delete m_sysInfo;
    if(m_DevIdInfo)
        delete m_DevIdInfo;     
    if(m_nvr)
        delete m_nvr;
    if(m_switch)
        delete m_switch;
        	
    if(m_file)
    {
        CloseHandle(m_file);
        m_file = 0;
    }
}

void CMClient::Stop()
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

int CMClient::Connect(DWORD ServerIp, WORD ServerPort, WORD LocalPort)
{
    int ret;
    DWORD tid;

    ret = Start(LocalPort);
    if(ret != CLIENT_OK)
    {
        return ret;
    }

    ret = Link(ServerIp, ServerPort, 2000);
    if(ret != CLIENT_OK)
    {
        return ret;
    }

    m_State = 1;
    m_Thread = CreateThread(0, 0, MThread, this, 0, &tid);
    if(m_Thread == NULL)
    {
        return CLIENT_ERR_THREAD;
    }

    return CLIENT_OK;
}

int CMClient::SendMsg(int msg, void *para, int len)
{
    int tlen;
    char buf[MNG_CMD_SIZE];
    MNG_CMD_HEAD *cmd = (MNG_CMD_HEAD *)buf;
    tlen = len+sizeof(MNG_CMD_HEAD);
    if(tlen > MNG_CMD_SIZE)
        return -1;

    cmd->flag = CMD_FLAG;
    cmd->type = 0;
    cmd->cmd = htons(msg);
    cmd->seq = htons(m_Seq);
    m_Seq++;
    cmd->extlen = htons(len);

    if(len)
       memcpy((char *)(cmd+1), para, len);

    if(Send(buf, tlen))
        return -1;

    return 0;
}
int CMClient::SendRegister(char *name, char *pwd)
{
    char buf[128];

    strcpy(buf, name);
    md5Encrypt(pwd, buf+32);
    return SendMsg(MNG_CMD_REGISTER, buf, 64);
}

int CMClient::SendPtzCmd(int chn, int cmd, int mode, int speed)
{
    MNG_PTZ_CTRL tmp;
    char *cmds[15] = {"STOP", "TU", "TUPL", "PL", "TDPL", "TD", "TDPR", "PR", "TUPR",
                      "ZIN", "ZOUT", "FN", "FR", "IC", "IO"};

    tmp.chn = chn;
    tmp.speed = speed;
    if(!mode)
    {
        strcpy(tmp.cmd, cmds[cmd]);
		tmp.param[0] = 0;
    }
    else
    {
        strcpy(tmp.cmd, cmds[0]);
        strcpy(tmp.param, cmds[cmd]);
    }

    return SendMsg(MNG_CMD_CTRL_PTZ, &tmp, sizeof(tmp));
}

int CMClient::SendPresetCmd(int chn, int cmd, int presetId, int speed)
{               
    MNG_PTZ_CTRL tmp;
    char *cmds[3] = {"SET_PRESET", "DEL_PRESET", "GOTO_PRESET"};

    if(cmd >= 3)
        return -1;

    tmp.chn = chn;
    tmp.speed = speed;
    strcpy(tmp.cmd, cmds[cmd]);
    sprintf(tmp.param, "%d", presetId);
    
    return SendMsg(MNG_CMD_CTRL_PTZ, &tmp, sizeof(tmp));
}

int CMClient::SetBasic(MNG_BASIC_INFO *p)
{
    memcpy((char *)m_Basic, (char *)p, sizeof(MNG_BASIC_INFO));
    return SendMsg(MNG_CMD_SET_BASE_INFO, (char *)p, sizeof(MNG_BASIC_INFO));
}

int CMClient::SetAdvancedInfo(MNG_SYS_ADVANCED_INFO *p)
{
    if(m_AdInfo)
        memcpy((char *)m_AdInfo, (char *)p, sizeof(MNG_SYS_ADVANCED_INFO));
    return SendMsg(MNG_CMD_SET_SYS_ADVANCED, (char *)p, sizeof(MNG_SYS_ADVANCED_INFO));
}

int CMClient::SetStorageNewInfo(STORAGE_TEMP_PARA *p, int chn)
{
    char buf[sizeof(STORAGE_TEMP_PARA)+sizeof(int)];

    *(int *)buf = chn;
    memcpy(buf+sizeof(int), p, sizeof(STORAGE_TEMP_PARA));
    STORAGE_TEMP_PARA *pp = (STORAGE_TEMP_PARA *)(buf+sizeof(int));
    return SendMsg(MNG_CMD_SET_STORAGE_NEW_INFO, buf, sizeof(STORAGE_TEMP_PARA)+sizeof(int));
}

int CMClient::SetStorageClear()
{
    return SendMsg(MNG_CMD_SET_STORAGE_CLEAR, 0, 0);
}

int CMClient::SetStorageDefault()
{
    return SendMsg(MNG_CMD_SET_STORAGE_DEFAULT, 0, 0);    
}

int CMClient::SetStorageInfo(STORAGE_TEMP_PARA *p, int chn)
{
    char buf[sizeof(STORAGE_TEMP_PARA)+sizeof(int)];

    *(int *)buf = chn;
    memcpy(buf+sizeof(int), p, sizeof(STORAGE_TEMP_PARA));
    return SendMsg(MNG_CMD_SET_STORAGE_INFO, buf, sizeof(STORAGE_TEMP_PARA)+sizeof(int));
}

int CMClient::SetExVideoPara(int chn, EX_VIDEO_CHANNEL_PARA *p)
{
    if(!m_vExVideo)
        return -1;
        
    memcpy(&m_vExVideo[chn], p, sizeof(EX_VIDEO_CHANNEL_PARA));

    return SendMsg(MNG_CMD_SET_EX_VIDEO_INFO, (char *)m_vExVideo, m_vNum*sizeof(EX_VIDEO_CHANNEL_PARA));    
}

int CMClient::SetVideo(int chn, VIDEO_CHANNEL_PARA *p)
{
    if(!m_vChn)
        return -1;

    m_vChn[chn].norm = p->norm;
    m_vChn[chn].codec = p->codec;
    m_vChn[chn].bitRate = p->bitRate;
    m_vChn[chn].frameRate = p->frameRate;
    m_vChn[chn].iFram = p->iFram;
    m_vChn[chn].size = p->size;
    m_vChn[chn].qp = p->qp;
    m_vChn[chn].vbr = p->vbr;    
    m_vChn[chn].flipMode = p->flipMode;
    //m_vChn[chn].camType = p->camType;
    m_vChn[chn].sio.mode = p->sio.mode;
    m_vChn[chn].audCodec  = p->audCodec;
    m_vChn[chn].audChn = p->audChn;
    m_vChn[chn].sio.addr = p->sio.addr;
    m_vChn[chn].sio.port = p->sio.port;
    m_vChn[chn].sio.baudRate = p->sio.baudRate;
    m_vChn[chn].sio.mode = p->sio.mode;
    m_vChn[chn].sio.parity = p->sio.parity;
    m_vChn[chn].sio.dataBit = p->sio.dataBit;
    m_vChn[chn].sio.stopBit = p->sio.stopBit;
    m_vChn[chn].sio.flowControl = p->sio.flowControl;
    m_vChn[chn].sio.displayMode = p->sio.displayMode;
    strcpy(m_vChn[chn].sio.protocol, p->sio.protocol);

    return SendMsg(MNG_CMD_SET_VIDEO_INFO, (char *)m_vChn, m_vNum*sizeof(VIDEO_CHANNEL_PARA));
}

int CMClient::SetSubVideo(int chn, VIDEO_SUB_PARA *p)
{
    m_vSub[chn].bitRate = p->bitRate;
    m_vSub[chn].iFram  = p->iFram;
    m_vSub[chn].codec  = p->codec;
    m_vSub[chn].size  = p->size;
    m_vSub[chn].frameRate  = p->frameRate;
    m_vSub[chn].vbr  = p->vbr;
    m_vSub[chn].qp  = p->qp;

    return SendMsg(MNG_CMD_SET_SUB_VIDEO_INFO, (char *)m_vSub, m_vNum*sizeof(VIDEO_SUB_PARA));
}

int CMClient::SetOsd(int chn, OSD_PARA *p)
{
    if(!m_Osd)
        return -1;

    m_Osd[chn].timeX  = p->timeX;
    m_Osd[chn].timeY = p->timeY;
    m_Osd[chn].timeEnable = p->timeEnable;
    m_Osd[chn].weekEnable = p->weekEnable;
    m_Osd[chn].textX = p->textX;
    m_Osd[chn].textY = p->textY;
    m_Osd[chn].textEnable = p->textEnable;
    m_Osd[chn].textClr = p->textClr;
    strcpy(m_Osd[chn].text, p->text);
    return SendMsg(MNG_CMD_SET_OSD_INFO, (char *)m_Osd, m_vNum*sizeof(OSD_PARA));
}

int CMClient::SetOsdEx(int chn, EX_OSD_PARA *pEx)
{
    if(!m_OsdEx)
        return -1;

    m_OsdEx[chn].textX  = pEx->textX;
    m_OsdEx[chn].textY = pEx->textY;
    m_OsdEx[chn].exOsdEn = pEx->exOsdEn;
    m_OsdEx[chn].textClr = pEx->textClr;
    strcpy(m_OsdEx[chn].text, pEx->text);

    return SendMsg(MNG_CMD_SET_EX_OSD_INFO, (char *)m_OsdEx, m_vNum*sizeof(EX_OSD_PARA));
}
int CMClient::SetVideoAlarm(int chn, VIDEO_ALARM *p)
{
    if(!m_Alarm)
        return -1;

    m_Alarm[chn].vLose.alarmEn  = p->vLose.alarmEn;
    m_Alarm[chn].vLose.alarmOutChn = p->vLose.alarmOutChn;
    m_Alarm[chn].vLose.alarmOutSt = p->vLose.alarmOutSt;

    m_Alarm[chn].vMd.state = p->vMd.state;
    m_Alarm[chn].vMd.alarmChn = p->vMd.alarmChn;    
    m_Alarm[chn].vMd.alarmState = p->vMd.alarmState;
    m_Alarm[chn].vMd.startTime = p->vMd.startTime;
    m_Alarm[chn].vMd.stopTime = p->vMd.stopTime;
    m_Alarm[chn].vMd.recordChn = p->vMd.recordChn;
    m_Alarm[chn].vMd.shootChn = p->vMd.shootChn;

    return SendMsg(MNG_CMD_SET_VIDEO_ALARM, (char *)m_Alarm, m_vNum*sizeof(VIDEO_ALARM));
}

int CMClient::SetAlarmBasic(ALARM_PARA *p)
{
    return SendMsg(MNG_CMD_SET_ALARM_BASIC, (char *)p, sizeof(ALARM_PARA));
}

int CMClient::SendGetLog(int mode)
{
    if(mode)
        return SendMsg(MNG_CMD_GET_LOG_NEXT, 0, 0);
    else
        return SendMsg(MNG_CMD_GET_LOG, 0, 0);
}

/*
int CMClient::SetGpinAlarm(int num, UINT8 *st)
{
    if(num > m_GpinNum)
        num = m_GpinNum;
    for(int i = 0; i < num; i++)
    {
        m_Gpin[i].alarmEn = st[i];
    }
    return SendMsg(MNG_CMD_SET_GPIN_ALARM, (char *)m_Gpin, m_GpinNum*sizeof(SWITCH_IN_INFO));
}  */

int CMClient::SetGpinAlarm(int num, GPIN_ALARM_PARA *st)
{
/*    if(num > m_GpinNum)
        num = m_GpinNum;

    for(int i = 0; i < num; i++)
    {
        m_Gpin[i].alarmEn = st[i].alarmEn;     
        m_Gpin[i].alarmOutChn = st[i].alarmOutChn;
        m_Gpin[i].record[0].chn = st[i].record[0].chn;
        m_Gpin[i].record[0].preset = st[i].record[0].preset;
        m_Gpin[i].alarmOutSt = st[i].alarmOutSt;
    }*/

    if(num >= m_GpinNum)
        return -1;

    m_Gpin[num].alarmEn = st->alarmEn;
    m_Gpin[num].alarmOutChn = st->alarmOutChn;
    m_Gpin[num].record[0].chn = st->record[0].chn;
    m_Gpin[num].record[0].preset = st->record[0].preset;
    m_Gpin[num].alarmOutSt = st->alarmOutSt;
    m_Gpin[num].alarmInMode = st->alarmInMode;

    return SendMsg(MNG_CMD_SET_GPIN_ALARM, (char *)m_Gpin, m_GpinNum*sizeof(GPIN_ALARM_PARA));
}

int CMClient::SetDdns(DDNS_INFO *info)
{
    return SendMsg(MNG_CMD_SET_DDNS, (char *)info, sizeof(DDNS_INFO));
}

int CMClient::SetDynamicNetInfo(MNG_DYNAMIC_NET_INFO *info)
{
    return SendMsg(MNG_CMD_SET_DYNAMIC_NET_INFO, (char *)info, sizeof(MNG_DYNAMIC_NET_INFO));
}

int CMClient::SetPlatFormInfo(PLATFORM_INFO_S *info)
{
    char buf[sizeof(PLATFORM_INFO_S)+sizeof(int)];
    *((int *)buf) = info->platformId;
    memcpy(buf+sizeof(int), info, sizeof(PLATFORM_INFO_S));
    return SendMsg(MNG_CMD_SET_PLATFORM_INFO, (char *)buf, sizeof(PLATFORM_INFO_S)+sizeof(int));
}

int CMClient::SetDVRSet(MNG_DVR_S *p)
{
    if(m_DVRInfo)
        memcpy((char *)m_DVRInfo, (char *)p, sizeof(MNG_DVR_S));
    return SendMsg(MNG_CMD_SET_DVR_PARA, (char *)p, sizeof(MNG_DVR_S));
}

int CMClient::SendGetNVRInfo()
{
    return SendMsg(MNG_CMD_GET_NVR_PARA, 0, 0);
}

int CMClient::SetDevId(MNG_DEV_ID *p)
{                        
    if(m_DevIdInfo)
        memcpy((char *)m_DevIdInfo, (char *)p, sizeof(MNG_DEV_ID));
    return SendMsg(MNG_CMD_SET_DEV_ID, (char *)m_DevIdInfo, sizeof(MNG_DEV_ID));
}

int CMClient::SendReboot()
{
    return SendMsg(MNG_CMD_REBOOT, 0, 0);
}

int CMClient::SendGetVideoInfo()
{
    //if(m_St & ST_VIDEO)
    //    return 0;

    SendMsg(MNG_CMD_GET_VIDEO_INFO, 0, 0);
    SendMsg(MNG_CMD_GET_EX_VIDEO_INFO, 0, 0);
    return SendMsg(MNG_CMD_GET_SUB_VIDEO_INFO, 0, 0);
}

int CMClient::SendGetOsdInfo()
{
    //if(m_St & ST_OSD)
    //    return 0;

    return SendMsg(MNG_CMD_GET_OSD_INFO, 0, 0);
}
  
int CMClient::SendGetExOsdInfo()
{
    //if(m_St & ST_OSD_EX)
    //    return 0;

    return SendMsg(MNG_CMD_GET_EX_OSD_INFO, 0, 0);
}

int CMClient::SendGetVolume()
{
    return SendMsg(MNG_CMD_VOLUME_GET, 0, 0);
}

int CMClient::SendGetAlarmBasicInfo()
{
    return SendMsg(MNG_CMD_GET_ALARM_BASIC, 0, 0);
}

int CMClient::SendGetAlarmVideoInfo()
{
    return SendMsg(MNG_CMD_GET_VIDEO_ALARM, 0, 0);
}

int CMClient::SendGetAlarmGpinInfo()
{
    return SendMsg(MNG_CMD_GET_GPIN_ALARM, 0, 0);
}

int CMClient::SendGetAlarmInfo()
{
    //if(m_St & ST_ALARM)
        //return 0;

    SendMsg(MNG_CMD_GET_VIDEO_ALARM, 0, 0);  
    SendMsg(MNG_CMD_GET_ALARM_BASIC, 0, 0);
    SendMsg(MNG_CMD_GET_SWITCH_ALARM, 0, 0);
    SendMsg(MNG_CMD_GET_EX_GPIN_ALARM, 0, 0);
    SendMsg(MNG_CMD_GET_EX_SWITCH_ALARM, 0, 0);
    SendMsg(MNG_CMD_GET_GPIN_ALARM, 0, 0);
    return SendMsg(MNG_CMD_GET_ALARM_SERVICE, 0, 0);
}

int CMClient::SendGetDdnsInfo()
{
    return SendMsg(MNG_CMD_GET_DDNS, 0, 0);
}

int CMClient::SendGetDynamicNetInfo()
{
    return SendMsg(MNG_CMD_GET_DYNAMIC_NET_INFO, 0, 0);
}

int CMClient::SendGetPlatFormInfo(int Id)
{
    return SendMsg(MNG_CMD_GET_PLATFORM_INFO, &Id, sizeof(Id));
}

int CMClient::SendGetBaseInfo()
{
    return SendMsg(MNG_CMD_GET_BASE_INFO, 0, 0);
}

int CMClient::SendGetAdvancedInfo()
{
    return SendMsg(MNG_CMD_GET_SYS_ADVANCED, 0, 0);
}

int CMClient::SendGetStorageNewInfo(int Id)
{
    return SendMsg(MNG_CMD_GET_STORAGE_NEW_INFO, &Id, sizeof(Id));
}

int CMClient::SendGetStorageInfo(int Id)
{
    return SendMsg(MNG_CMD_GET_STORAGE_INFO, &Id, sizeof(Id));
}

int CMClient::SendGetDVRSet()
{
	return SendMsg(MNG_CMD_GET_DVR_PARA, 0, 0);
}

int CMClient::SendGetDiskInfo()
{
    return SendMsg(MNG_CMD_GET_DISK_INFO, 0, 0);
}

int CMClient::SendGetPreset()
{
    return SendMsg(MNG_CMD_GET_PRESET_PARA, 0, 0);
}

int CMClient::SendGetCurise()
{
    return SendMsg(MNG_CMD_GET_CRUISE_PARA, 0, 0);
}

int CMClient::SendGetPtzInfo()
{
    return SendMsg(MNG_CMD_GET_PTZ_PARA, 0, 0);
}

int CMClient::SendGetDevTime()
{
    return SendMsg(MNG_CMD_GET_SYS_TIME, 0, 0);
}

int CMClient::SendGetSysInfo()
{
    return SendMsg(MNG_CMD_GET_SYS_CPU_MEM, 0, 0);
}

int CMClient::SendFormatDisk(int diskNum)
{
    return SendMsg(MNG_CMD_FORMAT_DISK, &diskNum, sizeof(diskNum));
}

int CMClient::SendReboot3gCard()
{
    return SendMsg(MNG_CMD_REBOOT_3GCARD, 0, 0);
}

int CMClient::SendGetDevId(MNG_DEV_ID *p)
{
    return SendMsg(MNG_CMD_GET_DEV_ID, p, sizeof(MNG_DEV_ID));
}

int CMClient::SendGetFile(char *fileName)
{
    return SendMsg(MNG_CMD_UPLOAD, fileName, 256);
}

int CMClient::StartDownload(char * fileName)
{
    MNG_FILE p;
    AnsiString S = fileName;
    m_file = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL,
             OPEN_EXISTING, 0, 0);
    if (m_file == INVALID_HANDLE_VALUE)
    {
        m_file = 0;
        return -1;
    }
    int n = S.LastDelimiter("\\");
    strcpy(p.name, fileName+n);
    p.size = GetFileSize(m_file, 0);
    PostMessage(MainForm->Handle, WM_MNG_DOWNLOAD_POS, 0, (unsigned int)p.size);
    return SendMsg(MNG_CMD_DOWNLOAD, &p, sizeof(p));
}

int CMClient::SendFile()
{
    char buf[2048];
    DWORD x, n = 0, size = GetFileSize(m_file, 0);
    DWORD rd = 2048-sizeof(MNG_CMD_HEAD);
    while(n < size)
    {
        x = size-n;
        if(x > rd)
            x = rd;
        if(!ReadFile(m_file, buf, x, &x, 0))
        {
            break;
        }
        if(SendMsg(MNG_CMD_DOWNLOAD_DATA, buf, x) < 0)
        {
            break;
        }
        n += x;
        PostMessage(MainForm->Handle, WM_MNG_DOWNLOAD_POS, (unsigned int)n, 0);
    }
    CloseHandle(m_file);
    m_file = 0;
    if(n == size)
    {
        //Sleep(100);
        SendMsg(MNG_CMD_DOWNLOAD_OVER, 0, 0);
        return 0;
    }
    return -1;
}

int CMClient::SendVideoRequest(int chn, int streamtype)
{
    MNG_MDEIA_REQUEST t;

    t.chn = chn;
    t.linkType = 0;
    t.encodeType = 0;
    t.streamType = streamtype;
    return SendMsg(MNG_CMD_VIDEO_REQUEST, &t, sizeof(t));
}

int CMClient::SendVideoStop(int chn)
{
    return SendMsg(MNG_CMD_VIDEO_STOP, &chn, sizeof(int));
}

int CMClient::SendTalkRequest()
{
    MNG_MDEIA_REQUEST t;
    memset(&t, 0, sizeof(t));
    t.linkType = 2; //0 = tcp
    t.encodeType = 1;//AC_G711A;

    return SendMsg(MNG_CMD_TALK_REQUEST, &t, sizeof(t));
}

int CMClient::SetTime(SYSTEMTIME *t)
{
     return SendMsg(MNG_CMD_SET_TIME, (char *)t, sizeof(SYSTEMTIME));
}

int CMClient::SendTime()
{
    SYSTEMTIME t;
    GetLocalTime(&t);
    return SendMsg(MNG_CMD_SET_TIME, (char *)&t, sizeof(t));
}

int CMClient::SendErase()
{
    return SendMsg(MNG_CMD_EARSE_PARA, 0, 0);
}

int CMClient::SetDefaultPara()
{  
    return SendMsg(MNG_CMD_SET_DEFAULT_PARA, 0, 0);    
}

int CMClient::SendEraseDefaultPara()
{  
    return SendMsg(MNG_CMD_ERASE_DEFAULT_PARA, 0, 0);    
}

int CMClient::SetVolume(VOLUME *volume)
{
    return SendMsg(MNG_CMD_VOLUME_SET, (char *)volume, sizeof(VOLUME));
}

int CMClient::SendHeartBeat()
{
    return SendMsg(MNG_CDM_HEARTBEAT, 0, 0);
}

int CMClient::SetDisplay(int chn, int bright, int contrast, int saturation, int hue)
{
    MNG_DISPALY_CTRL tmp;
    
    tmp.chn = chn;
    tmp.bright = bright;
    tmp.contrast = contrast;
    tmp.saturation = saturation;
    tmp.hue = hue;
    return SendMsg(MNG_CMD_CTRL_DISPALY, (char *)&tmp, sizeof(tmp));
}

int CMClient::SetNvr(int chn, MNG_NVR *p)
{
    if(m_nvr)
    {
    m_nvr[chn].type = p->type;
    m_nvr[chn].port  = p->port;
    m_nvr[chn].stroage  = p->stroage;
    strcpy(m_nvr[chn].ip, p->ip);
    strcpy(m_nvr[chn].acc, p->acc);
    strcpy(m_nvr[chn].pwd, p->pwd);
    }
    return SendMsg(MNG_CMD_SET_NVR_PARA, (char *)p, sizeof(MNG_NVR));
}

int CMClient::SendParaGetCmd(int chn, int cmd)
{
    return SendMsg(cmd, &chn, sizeof(int));
}

int CMClient::MsgHandle(BYTE *MsgBuf, int Length)
{
    MNG_CMD_HEAD *hd;
    char *body;
    int len, cmd, n;

    if(Length < sizeof(MNG_CMD_HEAD))
        return 0;

    hd = (MNG_CMD_HEAD *)MsgBuf;
    if(hd->flag != CMD_FLAG)
    {
        return ERROR;
    }
    if(!hd->type)
    {
        return ERROR;
    }

    cmd = htons(hd->cmd);

    if(hd->extlen == 0xffff)
    {
        if(cmd == MNG_CMD_FORMAT_PROGREES)  
            PostMessage(MainForm->Handle, WM_MNG_FORMAT_PROGRESS, 0, -1);
        else
            PostMessage(MainForm->Handle, WM_MNG_UNSUPPORT, cmd, 0);
        return sizeof(MNG_CMD_HEAD);
    }
    len = ntohs(hd->extlen);
    if(len < 0)
    {
        PostMessage(MainForm->Handle, WM_MNG_CMD_RET, cmd, len);
        return sizeof(MNG_CMD_HEAD);
    }
    if((len + sizeof(MNG_CMD_HEAD)) > MNG_CMD_SIZE)
    {
        return ERROR;
    }
    if(len + sizeof(MNG_CMD_HEAD) > Length)
    {
        return 0;
    }

    body = MsgBuf+sizeof(MNG_CMD_HEAD);

    switch(cmd)
    {
        case MNG_CMD_REGISTER:
        {
            m_regState = 1;
            m_Level = *(int *)body;
            SendMsg(MNG_CMD_GET_BASE_INFO, 0, 0);     
            SendMsg(MNG_CMD_GET_VER, 0, 0);
            break;
        }
        case MNG_CMD_GET_VER:
        {
            body[len] = 0;
            strcpy(m_Ver, body);
            PostMessage(MainForm->Handle, WM_MNG_VERSION, (unsigned int)m_Ver, 0);
            m_St |= ST_VER;
            break;
        }
        case MNG_CMD_GET_BASE_INFO:
        {
            MNG_BASIC_INFO *p = (MNG_BASIC_INFO *)body;
            if(!m_Basic)
                m_Basic = new MNG_BASIC_INFO;
            memcpy((char *)m_Basic, (char *)p, sizeof(MNG_BASIC_INFO));
            PostMessage(MainForm->Handle, WM_MNG_BASE_INFO, (unsigned int)m_Basic, 0);
            m_St |= ST_BASIC;
            break;
        }
        case MNG_CMD_GET_STORAGE_NEW_INFO:
        {
            STORAGE_TEMP_PARA *p = (STORAGE_TEMP_PARA *)body;
            n = len/(sizeof(STORAGE_TEMP_PARA));
            if(m_Storage)
                delete []m_Storage;
            m_Storage = new STORAGE_TEMP_PARA[n];
            m_vNum = n;
            memcpy((char *)m_Storage, (char *)p, n*sizeof(STORAGE_TEMP_PARA));
            PostMessage(MainForm->Handle, WM_STORAGE_NEW_INFO, (unsigned int)m_Storage, n);
            m_St |= ST_STORAGE;
            break;
        }
        case MNG_CMD_FORMAT_PROGREES:
        {
            if(strlen(body) > 4)
            {
            strcpy(m_progrees, body);
            PostMessage(MainForm->Handle, WM_MNG_FORMAT_PROGRESS, (unsigned int)m_progrees, strlen(m_progrees));
            }
            else
            {           
            PostMessage(MainForm->Handle, WM_MNG_FORMAT_PROGRESS, *(unsigned int*)body, 0);
            }
            break;
        }
        case MNG_CMD_GET_STORAGE_INFO:
        {
            STORAGE_TEMP_PARA *p = (STORAGE_TEMP_PARA *)body;
            n = len/(sizeof(STORAGE_TEMP_PARA));
            if(m_Storage)
                delete []m_Storage;
            m_Storage = new STORAGE_TEMP_PARA[n];
            m_vNum = n;
            memcpy((char *)m_Storage, (char *)p, n*sizeof(STORAGE_TEMP_PARA));
            PostMessage(MainForm->Handle, WM_STORAGE_INFO, (unsigned int)m_Storage, n);
            m_St |= ST_STORAGE;
            break;
        }
        case MNG_CMD_GET_SYS_ADVANCED:
        {
            MNG_SYS_ADVANCED_INFO *p = (MNG_SYS_ADVANCED_INFO *)body;
            if(!m_AdInfo)
                m_AdInfo = new MNG_SYS_ADVANCED_INFO;
            memcpy((char *)m_AdInfo, (char *)p, sizeof(MNG_SYS_ADVANCED_INFO));
            PostMessage(MainForm->Handle, WM_MNG_ADVANCED_INFO, (unsigned int)m_AdInfo, 0);
	     m_St |= ST_ADVANCED;
            break;
        }
        case MNG_CMD_GET_DVR_PARA:
        {
            MNG_DVR_S  *p = (MNG_DVR_S *)body;
            if(!m_DVRInfo)
                m_DVRInfo = new MNG_DVR_S;
            memcpy((char *)m_DVRInfo, (char *)p, sizeof(MNG_DVR_S));
            PostMessage(MainForm->Handle, WM_MNG_DVR_INFO, (unsigned int)m_DVRInfo, 0);
            m_St |= ST_DVR;
            break;
        }
        case MNG_CMD_GET_DISK_INFO:
        {
            if(len > 0)
            {
            MNG_SYS_INFO_S *p = (MNG_SYS_INFO_S *)body;
            if(!m_sysInfo)
                m_sysInfo = new MNG_SYS_INFO_S;
            memcpy(m_sysInfo, p, sizeof(MNG_SYS_INFO_S));                    
            PostMessage(MainForm->Handle, WM_MNG_GET_DISK_INFO, (unsigned int)m_sysInfo, 0);            
            }
            break;
        }
        case MNG_CMD_GET_DEV_ID:
        {
            if(!m_DevIdInfo)
                m_DevIdInfo = new MNG_DEV_ID;
            memcpy(m_DevIdInfo, body, sizeof(MNG_DEV_ID));
            PostMessage(MainForm->Handle, WM_MNG_DEV_ID_INFO, (unsigned int)m_DevIdInfo, 0);
            break;
        }
        case MNG_CMD_GET_PLATFORM_INFO:
        {
            PLATFORM_INFO_S *p = (PLATFORM_INFO_S *)body;
            if(!m_PtInfo)
                m_PtInfo = new PLATFORM_INFO_S;
            memcpy((char *)m_PtInfo, (char *)p, sizeof(PLATFORM_INFO_S));
            PostMessage(MainForm->Handle, WM_MNG_PLATFORM_INFO, (unsigned int)m_PtInfo, 0);
            m_St |= ST_PLATFORM;
            break;
        }
        case MNG_CMD_GET_VIDEO_INFO:
        {
            VIDEO_CHANNEL_PARA *p = (VIDEO_CHANNEL_PARA *)body;
            n = len/sizeof(VIDEO_CHANNEL_PARA);
            if(m_vChn)
            {
                 delete m_vChn;
            }
            m_vChn = new VIDEO_CHANNEL_PARA[n];
            m_vNum = n;
            memcpy((char *)m_vChn, (char *)p, n*sizeof(VIDEO_CHANNEL_PARA));
            PostMessage(MainForm->Handle, WM_MNG_VIDEO_INFO, (unsigned int)m_vChn, n);
            m_St |= ST_VIDEO;
            break;
        }
        case MNG_CMD_VOLUME_GET:
        {
            MainForm->SetPara(MNG_CMD_VOLUME_GET, body);
            break;
        }
        case MNG_CMD_GET_SUB_VIDEO_INFO:
        {
            n = len / sizeof(VIDEO_SUB_PARA);
            if(n == m_vNum)
            {
                if(m_vSub)
                    delete m_vSub;
                m_vSub = new VIDEO_SUB_PARA[m_vNum];
                memcpy((char *)m_vSub, (char *)body, n*sizeof(VIDEO_SUB_PARA));
                PostMessage(MainForm->Handle, WM_MNG_VIDEO_SUB_INFO, (unsigned int)m_vSub, n);
            }
            break;
        }
        case MNG_CMD_GET_EX_VIDEO_INFO:
        {
            n = len/sizeof(EX_VIDEO_CHANNEL_PARA);
            if(n == m_vNum)
            {
                if(m_vExVideo)
                    delete m_vExVideo;
                m_vExVideo = new EX_VIDEO_CHANNEL_PARA[m_vNum];
                memcpy((void *)m_vExVideo, (void *)body, n*sizeof(EX_VIDEO_CHANNEL_PARA));
                PostMessage(MainForm->Handle, WM_MNG_GET_EX_VIDEO_PARA, (unsigned int)m_vExVideo, n);
            }
            break;
        }     
        case MNG_CMD_GET_ENV:
        {
            DEV_ENV_SET *p = (DEV_ENV_SET *)body;
            n = len/sizeof(DEV_ENV_SET);
            if(m_env)
                delete m_env;
            m_env = new DEV_ENV_SET[n];
            memcpy((char *)m_env, (char *)p, n*sizeof(DEV_ENV_SET));
            PostMessage(MainForm->Handle, WM_MNG_CMD_GET_ENV, (unsigned int)m_env, n);
            break;
        }
        case MNG_CMD_GET_SWITCH:
        {
            if(m_switch)
            {
                delete m_switch;
                m_switch = 0;
            }
            if(len)
            {
                m_switch = (SWITCH_INFO *)(new BYTE[len]);
                memcpy(m_switch, body, len);
            }
            PostMessage(MainForm->Handle, WM_MNG_CMD_GET_SWITCH, (unsigned int)m_switch, 0);
            break;
        }
        case MNG_CMD_GET_NVR_PARA:
        {
            MNG_NVR *p = (MNG_NVR *)body;
            n = len/sizeof(MNG_NVR);
            if(m_nvr)
            {
                 delete m_nvr;
            }
            m_nvr = new MNG_NVR[n];
            m_NVRNum = n;
            memcpy((char *)m_nvr, (char *)p, n*sizeof(MNG_NVR));
            PostMessage(MainForm->Handle, WM_MNG_GET_NVR_PARA, (unsigned int)m_nvr, n);
            m_St |= ST_NVR;
            break;
        }
        case MNG_CMD_GET_OSD_INFO:
        {
            OSD_PARA *p = (OSD_PARA *)body;
            n = len/sizeof(OSD_PARA);
            if(m_Osd)
            {
                 delete m_Osd;
            }
            m_Osd = new OSD_PARA[n];
            m_vNum = n;
            memcpy((char *)m_Osd, (char *)p, n*sizeof(OSD_PARA));
            PostMessage(MainForm->Handle, WM_MNG_OSD_INFO, (unsigned int)m_Osd, n);
            m_St |= ST_OSD;
            break;
        }
        case MNG_CMD_GET_EX_OSD_INFO:
        {
            EX_OSD_PARA *p = (EX_OSD_PARA *)body;
            n = len/sizeof(EX_OSD_PARA);
            if(m_OsdEx)
            {
                 delete m_OsdEx;
            }
            m_OsdEx = new EX_OSD_PARA[n];
            m_vNum = n;
            memcpy((char *)m_OsdEx, (char *)p, n*sizeof(EX_OSD_PARA));
            PostMessage(MainForm->Handle, WM_MNG_OSD_EX_INFO, (unsigned int)m_OsdEx, n);
            m_St |= ST_OSD_EX;
            break;
        }
        case MNG_CMD_GET_VIDEO_ALARM:
        {
            VIDEO_ALARM *p = (VIDEO_ALARM *)body;
            n = len/sizeof(VIDEO_ALARM);
            if(m_Alarm)
            {
                 delete m_Alarm;
            }
            m_Alarm = new VIDEO_ALARM[n];
            m_vNum = n;
            memcpy((char *)m_Alarm, (char *)p, n*sizeof(VIDEO_ALARM));
            PostMessage(MainForm->Handle, WM_MNG_VIDEO_ALARM, (unsigned int)m_Alarm, n);
            m_St |= ST_ALARM;
            break;
        }
        case MNG_CMD_GET_GPIN_ALARM:
        {
            GPIN_ALARM_PARA *p = (GPIN_ALARM_PARA *)body;
            n = len/sizeof(GPIN_ALARM_PARA);
            if(m_Gpin)
            {
                 delete m_Gpin;
            }
            m_Gpin = new GPIN_ALARM_PARA[n];
            m_GpinNum = n;
            memcpy((char *)m_Gpin, (char *)p, n*sizeof(GPIN_ALARM_PARA));
            PostMessage(MainForm->Handle, WM_MNG_GPIN_ALARM, (unsigned int)m_Gpin, n);
            m_St |= ST_SWITCH;
            break;
        }
        case MNG_CMD_GET_ALARM_BASIC:
        {
            if(!m_AlarmPara)
                m_AlarmPara = new ALARM_PARA;
            memcpy((char *)m_AlarmPara, (char *)body, sizeof(ALARM_PARA));
            PostMessage(MainForm->Handle, WM_MNG_ALARM_BASIC, (unsigned int)m_AlarmPara, 0);
            m_St |= ST_ALARM_BASIC;
            break;
        }
        case MNG_CMD_GET_ALARM_SERVICE:
        {
            MainForm->SetPara(MNG_CMD_GET_ALARM_SERVICE, body);
            break;
        }
        case MNG_CMD_GET_DDNS:
        {
            MainForm->SetPara(MNG_CMD_GET_DDNS, body);
            break;
        }
        case MNG_CMD_GET_DYNAMIC_NET_INFO:
        {
            MainForm->SetPara(MNG_CMD_GET_DYNAMIC_NET_INFO, body);
            break;
        }
        case MNG_CMD_GET_WIFI_SEARCH:
        {
            MainForm->SetPara(MNG_CMD_GET_WIFI_SEARCH, body);
            break;
        }
        case MNG_CMD_GET_ACC:
        {
            n = len/sizeof(MNG_ACC);
            if(n > 16)
                n /= sizeof(MNG_ACC);
            MainForm->OnAccList((MNG_ACC *)body, n);
            break;
        }
        case MNG_CMD_GET_DISPLAY:
        {
            MainForm->SetPara(MNG_CMD_GET_DISPLAY, body);
            break;
        }
        case MNG_CMD_GET_PRODUCT_INFO:
        {
            MainForm->SetPara(MNG_CMD_GET_PRODUCT_INFO, body);
            break;
        }
        case MNG_CMD_GET_LOG:
        {
            if(Length > sizeof(MNG_CMD_HEAD))
                MainForm->LogShow->Text = body;
            else
                MainForm->LogShow->Text = "";
            break;
        }
        case MNG_CMD_GET_LOG_NEXT:
        {
            if(Length > sizeof(MNG_CMD_HEAD))
                MainForm->LogShow->Lines->Add(body);
            break;
        }
        case MNG_CMD_SET_BASE_INFO:
        {
            PostMessage(MainForm->Handle, WM_MNG_SET_BASE_OK, 0, 0);
            break;
        }
        case MNG_CMD_SET_ENV:
        case MNG_CMD_SET_OSD_INFO:
        case MNG_CMD_SET_VIDEO_ALARM:
        case MNG_CMD_SET_GPIN_ALARM:
        case MNG_CMD_SET_VIDEO_INFO:  
        case MNG_CMD_SET_SUB_VIDEO_INFO: 
        case MNG_CMD_SET_DDNS:
        case MNG_CMD_EARSE_PARA:
        case MNG_CMD_SET_DEFAULT_PARA:
        case MNG_CMD_ERASE_DEFAULT_PARA:
        case MNG_CMD_SET_DYNAMIC_NET_INFO:
        case MNG_CMD_SET_ALARM_BASIC:
        case MNG_CMD_SET_PLATFORM_INFO:
        case MNG_CMD_SET_SYS_ADVANCED:
        case MNG_CMD_SET_STORAGE_NEW_INFO:
        case MNG_CMD_SET_STORAGE_INFO:
        case MNG_CMD_SET_STORAGE_CLEAR:
        case MNG_CMD_SET_STORAGE_DEFAULT:
        case MNG_CMD_SET_DVR_PARA:
        case MNG_CMD_REBOOT_3GCARD:
        case MNG_CMD_SET_ALARM_PLAN:
        case MNG_CMD_SET_SWITCH_ALARM:
        case MNG_CMD_SET_DEV_ID:
        case MNG_CMD_UPGRADE_FTP:
        case MNG_CMD_CTRL_GPOUT:
        case MNG_CMD_SET_PRODUCT_INFO:
        case MNG_CMD_SET_ALARM_SERVICE:
        case MNG_CMD_SET_NVR_PARA:  
        case MNG_CMD_SET_SWITCH:
        {
            PostMessage(MainForm->Handle, WM_MNG_SET_VIDEO_INFO, 0, 0);
            break;
        }                                
        case MNG_CMD_FORMAT_DISK:
        {
            int result = *(int *)body;
            if(result)
                PostMessage(MainForm->Handle, WM_MNG_FORMAT_DISK, result, 0);
            break;
        }
        case MNG_CMD_REBOOT:
        {
            PostMessage(MainForm->Handle, WM_MNG_REBOOTING, 0, 0);
            break;
        }
        case MNG_CMD_UPLOAD:
        {
            MNG_FILE *p = (MNG_FILE *)body;
            AnsiString S = p->name;
            if(p->size < 0)
            {
                PostMessage(MainForm->Handle, WM_MNG_UPLOAD_ERROR, 0, 0);
                break;
            }
            int n = S.LastDelimiter("/");
            S = (p->name+n);
            m_file = CreateFile(S.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
                 CREATE_ALWAYS, 0, 0);
            if (m_file == INVALID_HANDLE_VALUE)
            {
                m_file = 0;
            }
            m_fileRecved = 0;
            m_fileSize = p->size;
            break;
        }
        case MNG_CMD_UPLOAD_DATA:
        {
            DWORD n;
            if(m_file)
            {
                if(m_fileRecved+len <= m_fileSize)
                {
                    WriteFile(m_file, MsgBuf+sizeof(MNG_CMD_HEAD), len, &n, NULL);
                    m_fileRecved += len;
                }
                else
                {
                    PostMessage(MainForm->Handle, WM_MNG_UPLOAD_ERROR, 1, 0);
                }
            }
            break;
        }
        case MNG_CMD_UPLOAD_OVER:
        {
            if(m_file)
            {
                CloseHandle(m_file);
                m_file = 0;
				ShowMessage("ÏÂÔØÍê³É!");
            }
            if(m_fileRecved != m_fileSize)
            {
                PostMessage(MainForm->Handle, WM_MNG_UPLOAD_ERROR, 2, 0);
            }
            break;
        }
        case MNG_CMD_DOWNLOAD:
        {
            if(Length-sizeof(MNG_CMD_HEAD))
            {
                int ret = *(int *)body;
                PostMessage(MainForm->Handle, WM_MNG_DOWNLOAD_ERROR, ret, 0);
            }
            else
            {
                SendFile();
            }
            break;
        }
        case MNG_CMD_DOWNLOAD_OVER:
        {
            int ret = *(int *)body;
            PostMessage(MainForm->Handle, WM_MNG_DOWNLOAD_OVER, ret, 0);
            break;
        }

        case MNG_CMD_VIDEO_REQUEST:
        {
            MNG_VIDEO_REQUEST_RSP *p = (MNG_VIDEO_REQUEST_RSP *)body;
            if(!p->result)
            {
                p->callId[63] = 0;
                MainForm->StartVideo(p->addr, p->port, p->callId);
                //PostMessage(MainForm->Handle, WM_MNG_DISPLAY_START, p->addr, p->port);
            }
            else
            {
                PostMessage(MainForm->Handle, WM_MNG_UNSUPPORT, 0, 0);
            }
            break;
        }
        case MNG_CMD_VIDEO_STOP:
        {
            break;
        }
        case MNG_CMD_TALK_REQUEST:
        {
            MNG_MEDIA_REQUEST_RSP *p = (MNG_MEDIA_REQUEST_RSP *)body;
            if(!p->result)
            {
                MainForm->StartTalk(p->addr, p->port, p->callId);
            }
            else
            {
                PostMessage(MainForm->Handle, WM_MNG_CMD_RET, cmd, p->result);
            }
            break;
        }
        case MNG_CMD_SET_TIME:
        {
            //PostMessage(MainForm->Handle, WM_MNG_DISPLAY_START, 0, 0);
            break;
        }
        default:
        {
            break;
        }
    }

    return len+sizeof(MNG_CMD_HEAD);
}

DWORD WINAPI CMClient::MThread(LPVOID Para)
{
    int rd, x, n = 0, ret = 0;
    char buf[MNG_CMD_SIZE];
    CMClient *This = (CMClient *)Para;

    while(This->m_State)
    {
        rd = This->WaitData(1000);
        if(rd == CLIENT_ERR_FAILED)
        {
            ret = -1;
            break;
        }
        if(rd == CLIENT_ERR_TIMEOUT)
        {
            continue;
        }

        rd = This->Read(buf+n, MNG_CMD_SIZE-n);
        if(rd <= 0)
        {
            ret = -2;
            break;
        }

        rd += n;
        n = 0;
        while(n < rd)
        {
            x = This->MsgHandle(buf+n, rd-n);
            if(x > 0)
            {
                n += x;
            }
            else
            {
                if(x < 0)
                    ret = -3;
                break;
            }
        }
        if(ret)
            break;

        if(!n)
        {
            n = rd;
            continue;
        }
        if(rd > n)
        {
            memmove(buf, buf+n, rd-n);
            n = rd - n;
        }
        else
        {
            n = 0;
        }
    }

    if(This->m_State)
    PostMessage(MainForm->Handle, WM_CLIENT_DOWN, (unsigned int)This, ret);

    return 0;
}

