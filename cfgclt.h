//---------------------------------------------------------------------------

#ifndef cfgcltH
#define cfgcltH

#include "tcpclt.h"

#include "mngmsg.h"
//---------------------------------------------------------------------------

#define MAX_VIDEO_CHANNELS        4

typedef struct
{
    UINT16 result;
    UINT16 port;
    UINT32 addr;
    char callId[64];
}MNG_VIDEO_REQUEST_RSP;


typedef struct
{
    _UINT8 chn;
    _UINT8 linkType;
    _UINT8 encodeType;
    _UINT8 streamType;
}MNG_MDEIA_REQUEST;

typedef struct
{
    _UINT16 result;
    _UINT16 port;
    _UINT32 addr;
    char callId[64];
}MNG_MEDIA_REQUEST_RSP;

typedef struct
{
    UINT8 port;
    UINT8 mode;
    UINT8 parity;
    UINT8 dataBit;

    UINT8 stopBit;
    UINT8 flowControl;
    UINT8 speed;
    UINT8 addr;

    UINT32 baudRate;
    UINT32 timeout;
    char protocol[16];
    UINT8 displayMode;
    char res[3];
}SIO_PORT_PARA;

typedef struct
{
    UINT32 bitRate;
    UINT16 iFram;
    UINT8 norm;
    UINT8 audChn;
    
    UINT8 codec;
    UINT8 audCodec;
    UINT8 size;
    UINT8 frameRate;
       
    UINT8 vbr;    
    UINT8 qp;
    UINT8 strategy;
    UINT8 flipMode;

    UINT8 bright;
    UINT8 contrast;
    UINT8 saturation;
    UINT8 hue;   
    
    SIO_PORT_PARA sio;
}VIDEO_CHANNEL_PARA;

typedef struct
{
    _UINT32 bitRate;
    _UINT16 iFram;
    _UINT8 codec;
    _UINT8 size;
    
    _UINT8 frameRate;
    _UINT8 vbr;    
    _UINT8 qp;
    _UINT8 strategy;
}VIDEO_SUB_PARA;

typedef struct
{
    char chnName[32];
    char chnId[32];
    char res[64];    
}EX_VIDEO_CHANNEL_PARA;

typedef struct
{
    UINT16 timeX;   /*时间戳的坐标*/
    UINT16 timeY;
    UINT16 textX;    /*文字的坐标*/
    UINT16 textY;
    UINT8 timeEnable; /*是否显示时间 （0-hiden 1-show）*/
    UINT8 weekEnable;/*是否显示星期（0-hiden 1-show）*/
    UINT8 textEnable; /*是否显示文字 （0-hiden 1-show）*/
    UINT8 extOsdEnable;
    char text[64];    
    UINT32 timeClr;
    UINT32 textClr;
}OSD_PARA;

typedef struct
{
    _UINT16 textX;
    _UINT16 textY;
    _UINT8 exOsdEn;
    char res[3];
    _UINT32 textClr;
    char text[64];    
}EX_OSD_PARA;

typedef struct
{
    UINT8 alarmEn;
    UINT8 alarmOutChn;
    UINT8 res[2];
    int alarmOutSt;
}VIDEO_LOSE_PARA;

typedef struct
{
    UINT8 alarmEn;
    UINT8 sensitivity;
    UINT8 recordChn;
    UINT8 alarmOutChn;
    int alarmOutSt;

    UINT16 x;
    UINT16 y;
    UINT16 w;
    UINT16 h;
    
    UINT16 startTime;
    UINT16 stopTime;
}VIDEO_HD_PARA;

typedef struct
{
    UINT8 state;
    UINT8 sensitivity;
    UINT8 frequency;    
    UINT8 alarmChn;
    int alarmState;
    
    UINT8 recordChn;
    UINT8 shootChn;
    UINT16 res;
    
    UINT16 startTime;
    UINT16 stopTime;

    UINT8 blocks[400];
}VIDEO_MD_PARA;

typedef struct
{
    VIDEO_LOSE_PARA vLose;
    VIDEO_MD_PARA  vMd;
    VIDEO_HD_PARA vHd;
}VIDEO_ALARM;

/*
typedef struct
{
    UINT8 alarmEn;
    UINT8 alarmOutChn;
    UINT8 recordChn;
    UINT8 shootChn;
    int alarmOutSt;
}SWITCH_IN_INFO;     */

typedef struct
{
    UINT8 chn;
    UINT8 preset;
    UINT16 res;
}ALARM_RECORD_PARA;

typedef struct
{
    UINT16 startTime;
    UINT16 stopTime;

    UINT8 alarmEn;
    UINT8 alarmStatus;
    UINT8 alarmOutChn;
    _UINT8 alarmInMode;/*0: 常开, 1:常闭*/
    _UINT8 res[8];
    
    int alarmOutSt;

    ALARM_RECORD_PARA record[MAX_VIDEO_CHANNELS];
    ALARM_RECORD_PARA shoot[MAX_VIDEO_CHANNELS];
}GPIN_ALARM_PARA;


typedef struct
{
    _UINT8 mode; /*告警时: 1 常闭; 0:常开*/
    _UINT8 res[3];
}SWITCH_OUT_PARA;


enum
{
    ST_VER = 0x1,
    ST_BASIC = 0x2,
    ST_VIDEO = 0x4,
    ST_OSD = 0x8,
    ST_ALARM = 0x10,
    ST_SWITCH = 0x20,
    ST_ALARM_BASIC = 0x40,
    ST_PLATFORM = 0x80,
    ST_ADVANCED = 0x100,
    ST_STORAGE = 0x200,
    ST_DVR = 0x400,
    ST_OSD_EX = 0x800,
    ST_NVR = 0x1000,
};

class CMClient : public CTcpClient
{
private:
    HANDLE m_Thread;
    DWORD m_State;

    char m_Ver[256];

    HANDLE m_file;
    DWORD m_fileSize;
    DWORD m_fileRecved;
    DWORD m_St;
    int m_vNum;
    int m_GpinNum;
    int m_SwitchNum;
    int m_NVRNum;
    int m_regState;
    WORD m_Seq;
    WORD m_RSeq;
    int MsgHandle(BYTE *MsgBuf, int Length);
    static DWORD WINAPI MThread(LPVOID Para);
    int SendFile();
public:
    CMClient();
    ~CMClient();

    int Connect(DWORD ServerIp, WORD ServerPort, WORD LocalPort = 0);
    void Stop();
    int SendMsg(int msg, void *para, int len);
    int SendRegister(char *name, char *pwd);
    int SetBasic(MNG_BASIC_INFO *p);
    int SetAdvancedInfo(MNG_SYS_ADVANCED_INFO *p);
    int SetStorageNewInfo(STORAGE_TEMP_PARA *p, int chn);
    int SetStorageInfo(STORAGE_TEMP_PARA *p, int chn);
    int SetStorageClear();
    int SetStorageDefault();
    int SendGetFile(char *fileName);
    int SendReboot();
    int SendHeartBeat();
    int SendGetVideoInfo();
    int SendGetOsdInfo();
    int SendGetExOsdInfo();
    int SendGetAlarmInfo();
    int SendGetAlarmBasicInfo();
    int SendGetAlarmVideoInfo();
    int SendGetAlarmGpinInfo();
    int SendGetAlarmPlan();
    int SendSetAlarmPlan(ALARM_PLAN *alarmPlan);
    int SendGetDdnsInfo();
    int SendGetDynamicNetInfo();
    int SendGetPlatFormInfo(int Id);
    int SendGetBaseInfo();
    int SendGetAdvancedInfo();
    int SendGetStorageNewInfo(int Id);
    int SendGetStorageInfo(int Id);
    int SendGetDVRSet();
    int SendGetDiskInfo();
    int SendGetPreset();
    int SendGetCurise();
    int SendGetPtzInfo();
    int SendGetDevTime();
    int SendGetSysInfo();
    int SendFormatDisk(int diskNum);
    int SendGetDevId(MNG_DEV_ID *p);
    int SendReboot3gCard();
    int SendVideoRequest(int chn, int streamtype);
    int SendVideoStop(int chn);  
    int SendTalkRequest();
    int SendTime();
    int SetTime(SYSTEMTIME *t);
    int SendErase();
    int SetDefaultPara();
    int SendEraseDefaultPara();
    int SetVideo(int chn, VIDEO_CHANNEL_PARA *p);
    int SetExVideoPara(int chn, EX_VIDEO_CHANNEL_PARA *p);
    int SetSubVideo(int chn, VIDEO_SUB_PARA *p);
    int SetOsd(int chn, OSD_PARA *p);       
    int SetOsdEx(int chn, EX_OSD_PARA *pEx);
    int SetVideoAlarm(int chn, VIDEO_ALARM *p);
    int SetGpinAlarm(int num, GPIN_ALARM_PARA *st);
    int SetDdns(DDNS_INFO *info);
    int SetDynamicNetInfo(MNG_DYNAMIC_NET_INFO *Info);
    int SetPlatFormInfo(PLATFORM_INFO_S *info);
    int SetDVRSet(MNG_DVR_S *p);
    int SetDevId(MNG_DEV_ID *p);
    int StartDownload(char * fileName);
    int SendGetLog(int mode);
    int SendPtzCmd(int chn, int cmd, int mode, int speed);
    int SendPresetCmd(int chn, int cmd, int presetId, int speed);
    int SetDisplay(int chn, int bright, int contrast, int saturation, int hue);
    int SendParaGetCmd(int chn, int cmd);
    int SetAlarmBasic(ALARM_PARA *p);
    int SendGetVolume();
    int SetVolume(VOLUME *volume);
    int SendGetNVRInfo();    
    int SetNvr(int chn, MNG_NVR *p);
    
    MNG_BASIC_INFO *m_Basic;
    VIDEO_CHANNEL_PARA *m_vChn;
    EX_VIDEO_CHANNEL_PARA *m_vExVideo;
    OSD_PARA *m_Osd;
    EX_OSD_PARA *m_OsdEx;
    VIDEO_ALARM *m_Alarm;
    GPIN_ALARM_PARA *m_Gpin;
    ALARM_PARA *m_AlarmPara;
    VIDEO_SUB_PARA *m_vSub;
    PLATFORM_INFO_S *m_PtInfo;
    MNG_SYS_ADVANCED_INFO *m_AdInfo;
    STORAGE_TEMP_PARA *m_Storage;
    MNG_DVR_S *m_DVRInfo;
    MNG_DEV_ID *m_DevIdInfo;
    MNG_SYS_INFO_S *m_sysInfo;
    MNG_NVR *m_nvr;
    DEV_ENV_SET *m_env;
    SWITCH_INFO *m_switch;
    char m_progrees[128];
    int m_chn;
    int m_Level;
};
#endif

