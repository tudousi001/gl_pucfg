/***********************************************************
*mng.h - manger handle head file
*
* Copyright(c) 2007~  , HCMIND
*
*
*modification history
*-----------------------
*
*01a, 07-01-01, Zhushuchao created
*
************************************************************/

#ifndef _MANGER_MSG_H_
#define _MANGER_MSG_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "defs.h"
#include "ioctrl.h"

#define MNG_CMD_FLAG 0xf0
#define MNG_CMD_SIZE 8192

#define MNG_CMD_FLAG 0xf0

#define MAX_PLATFORM_NUM 8
#define MAX_STORAGE_SCHEDULE 32
#define MAX_DISK_NUM 11

enum
{
    MNG_CDM_HEARTBEAT = 0,
    MNG_CMD_REGISTER = 0x10,
    MNS_CMD_REGISTER_BY_TICKET,
    MNG_CMD_GET_VER = 0x1000,
    MNG_CMD_GET_DEV_INFO,
    MNG_CMD_GET_BASE_INFO = 0x2000,
    MNG_CMD_GET_VIDEO_INFO,
    MNG_CMD_GET_OSD_INFO,
    MNG_CMD_GET_VIDEO_ALARM,
    MNG_CMD_GET_GPIN_ALARM,
    MNG_CMD_GET_ALARM_PARA,
    MNG_CMD_GET_DISPLAY,
    MNG_CMD_GET_HIDE_AREA,
    MNG_CMD_GET_MD,
    MNG_CMD_GET_EX_OSD_INFO,
    MNG_CMD_GET_PRESET_PARA,
    MNG_CMD_GET_CRUISE_PARA,
    MNG_CMD_GET_PTZ_PARA,
    MNG_CMD_GET_SYS_TIME,
    MNG_CMD_GET_ALARM_PLAN,
    MNG_CMD_GET_SWITCH_ALARM,
    MNG_CMD_GET_EX_VIDEO_INFO,
    MNG_CMD_GET_EX_GPIN_ALARM,
    MNG_CMD_GET_EX_SWITCH_ALARM,
    MNG_CMD_GET_YST_INFO,
    MNG_CMD_GET_IVP_RULE,
    MNG_CMD_GET_IVP_BASIC,
    MNG_CMD_GET_ENV,
    MNG_CMD_GET_IR,
    MNG_CMD_GET_SWITCH,
    MNG_CMD_GET_SUB_VIDEO_INFO = 0x20e0,
    MNG_CMD_GET_ALARM_BASIC = 0x20f0,
    MNG_CMD_GET_ALARM_SERVICE,
    MNG_CMD_GET_GPIN_STATE = 0x2100,
    MNG_CMD_GET_ACC = 0x2d00,
    MNG_CMD_GET_STREAM_INFO = 0x2e00,
    MNG_CMD_GET_DYNAMIC_NET_INFO,
    MNG_CMD_GET_WIFI_SEARCH,
    MNG_CMD_GET_DDNS = 0x2f00,    
    MNG_CMD_GET_LOG = 0x2ff0,    
    MNG_CMD_GET_LOG_NEXT,
    MNG_CMD_UPLOAD = 0x3000,
    MNG_CMD_UPLOAD_DATA,
    MNG_CMD_UPLOAD_OVER,
    MNG_CMD_DOWNLOAD = 0x3800,
    MNG_CMD_DOWNLOAD_DATA,
    MNG_CMD_DOWNLOAD_OVER,
    MNG_CMD_UPGRADE_FTP,
    MNG_CMD_SET_BASE_INFO = 0x8000,
    MNG_CMD_SET_VIDEO_INFO,
    MNG_CMD_SET_OSD_INFO,
    MNG_CMD_SET_VIDEO_ALARM,
    MNG_CMD_SET_GPIN_ALARM,
    MNG_CMD_SET_ALARM_PARA,
    MNG_CMD_SET_DISPLAY,
    MNG_CMD_SET_HIDE_AREA,
    MNG_CMD_SET_MD,
    MNG_CMD_SET_VIDOE_MIRROR,
    MNG_CMD_SET_VIDEO_FLIP,
    MNG_CMD_SET_VIDEO_ENCODE,
    MNG_CMD_SET_SIO_PARA,
    MNG_CMD_SET_EX_OSD_INFO,
    MNG_CMD_SET_PRESET_PARA,
    MNG_CMD_ADD_PRESET,
    MNG_CMD_DEL_PRESET,
    MNG_CMD_SET_CRUISE_PARA,
    MNG_CMD_MODIFY_CRUISE,
    MNG_CMD_ADD_CRUISE,
    MNG_CMD_DEL_CRUISE,
    MNG_CMD_SET_ALARM_PLAN,
    MNG_CMD_CTRL_GPOUT,
    MNG_CMD_SET_SWITCH_ALARM,
    MNG_CMD_SET_EX_VIDEO_INFO,
    MNG_CMD_SET_EX_GPIN_ALARM,
    MNG_CMD_SET_EX_SWITCH_ALARM,
    MNG_CMD_SET_YST_INFO,
    MNG_CMD_SET_IVP_RULE,
    MNG_CMD_SET_IVP_BASIC,
    MNG_CMD_SET_ENV,
    MNG_CMD_SET_IR,
    MNG_CMD_SET_SWITCH,
    MNG_CMD_SET_SUB_VIDEO_INFO = 0x80e0,
    MNG_CMD_SET_ALARM_BASIC = 0x80f0,
    MNG_CMD_SET_ALARM_SERVICE,
    MNG_CMD_SET_GPIN_STATE = 0x8100,
    MNG_CMD_SET_ACC = 0x8d00,
    MNG_CMD_ADD_ACC,
    MNG_CMD_DEL_ACC,
    MNG_CMD_SET_STREAM_INFO = 0x8e00,
    MNG_CMD_SET_DYNAMIC_NET_INFO,
    MNG_CMD_SET_DDNS = 0x8f00,   
    MNG_CMD_VIDEO_REQUEST = 0xa000,
    MNG_CMD_VIDEO_STOP,
    MNG_CMD_TALK_REQUEST,
    MNG_CMD_TALK_STOP,
    MNG_CMD_CTRL_PTZ = 0xa800,
    MNG_CMD_CTRL_DISPALY,
    MNG_CMD_CTRL_ALERT,
    MNG_CMD_CTRL_SNAP = 0xaf00,
    MNG_CMD_PLAYBACK = 0xb000,
    MNG_CMD_QUERY_STORAGE_FILES,
    MNG_CMD_PLAYBACK_STOP,
    MNG_CMD_PLAYBACK_BY_TIME,
    MNG_CMD_PIC_ADJ = 0xb100,
    MNG_CMD_GET_COLOR,
    MNG_CMD_SET_CVBS_CHN,
    MNG_CMD_VOLUME_SET = 0xb200,
    MNG_CMD_VOLUME_GET,
    MNG_CMD_GET_DISK_INFO = 0xc000,
    MNG_CMD_UMOUNT_DISK,
    MNG_CMD_FORMAT_DISK,
    MNG_CMD_FORMAT_PROGREES,
    MNG_CMD_GET_PLATFORM_INFO = 0xc100,
    MNG_CMD_SET_PLATFORM_INFO,
    MNG_CMD_GET_SYS_ADVANCED = 0xC200,
    MNG_CMD_SET_SYS_ADVANCED,
    MNG_CMD_GET_SYS_CPU_MEM,
    MNG_CMD_GET_STORAGE_INFO = 0xC300,
    MNG_CMD_SET_STORAGE_INFO,
    MNG_CMD_GET_STORAGE_NEW_INFO,
    MNG_CMD_SET_STORAGE_NEW_INFO,
    MNG_CMD_SET_STORAGE_CLEAR,
    MNG_CMD_SET_STORAGE_DEFAULT,
    MNG_CMD_SET_DVR_PARA = 0xD000,
    MNG_CMD_GET_DVR_PARA,  
    MNG_CMD_SET_NVR_PARA = 0xD010,
    MNG_CMD_GET_NVR_PARA,
    MNG_CMD_SET_DEV_ID = 0xD100,
    MNG_CMD_GET_DEV_ID,
    MNG_CMD_SET_PARA_ERR = 0xE000,    /*参数设置错误*/
    MNG_CMD_SET_TIME = 0xf000,
    MNG_CMD_REBOOT = 0xf080,
    MNG_CMD_SHUTDOWN,
    MNG_CMD_REBOOT_3GCARD,
    MNG_CMD_GET_PRODUCT_INFO = 0xfab0,
    MNG_CMD_SET_PRODUCT_INFO,      
    MNG_CMD_EARSE_PARA = 0xfabc,
    MNG_CMD_SET_DEFAULT_PARA,
    MNG_CMD_ERASE_DEFAULT_PARA,
    MNG_CMD_RUN_CMD = 0xfbbb,
};

typedef struct
{
    char gpinId[32];
    char res[32];
}EX_GPIN_ALARM_PARA;

typedef struct
{
    char gpoutId[32];
    char res[32];
}EX_GPOUT_ALARM_PARA;

typedef struct
{    
    _UINT8 alarmSchType;
    _UINT8 res[3];
    _UINT32 daySch[7]; /*每一位表示每个小时是否存储共7天*/
}ALARM_PLAN_SCH;

typedef struct
{
    ALARM_PLAN_SCH alarmPlanSch[5]; 
    char res[8];
}ALARM_PLAN;

typedef enum
{
    DISK_STATE_NONE, /*无硬盘*/
    DISK_STATE_NORMAL, /*正常*/
    DISK_STATE_UNFORMAT, /*未格式化*/
    DISK_STATE_ALL,
}DISK_STATES_E;

typedef struct
{
    int chnId;
    int state;
}MNG_GPOUT_CTRL;
               
typedef struct
{
    _UINT32 total;
    _UINT32 free;    /*M*/
    int diskState;
}MNG_DISK_INFO_S;

typedef struct
{
    int cpuUsage; /*10000 = 100%*/
    long memTotal;
    long memFree;
    MNG_DISK_INFO_S disk[MAX_DISK_NUM];
}MNG_SYS_INFO_S;

typedef struct
{
    UINT8 flag;
    UINT8 type;
    UINT16 cmd;
    UINT16 seq;
    UINT16 extlen;
}MNG_CMD_HEAD;

typedef struct
{
   char name[32];
   char regSrv[68];
   char id[32];
   char domainId[28];
   char pwd[36];
   _UINT32 ip;
   _UINT32 mask;
   _UINT32 gataway;
   _UINT32 dns[2];
   _UINT16 mngPort;
   _UINT16 sipPort;
   _UINT32 srvIp;
   _UINT16 srvPort;
   _UINT16 sipSrvPort;
   _UINT32 sipSrvIp;
   _UINT8 pppoeEn;
   _UINT8 sipHeartEn;
   _UINT8 httpHeartEn;
   _UINT8 linkMode;
   char pppoeName[36];
   _UINT32 multicastAddr;
   _UINT16 multicastPort;
   _UINT16 alarmSrvPort;
   _UINT32 alarmSrvAddr;
   _UINT16 httpHeartPeriod;
   _UINT16 sipHeartPeriod;
   _UINT8 protocol;
   _UINT8 norm;
   _UINT16 portBase;   
   _UINT32 pppIp;
   char pppoePwd[36];
   _UINT32 hwst;
   _UINT8 mac[6];
   _UINT16 portRange;
}MNG_BASIC_INFO;

typedef struct
{
    char name[256];
    int size;
}MNG_FILE;          

typedef struct
{
    int hour;
    int minute;
    int second;
    int month;
    int date;
    int year;
}MNG_TIME;

typedef struct
{
    int chn;
    int speed;
    char cmd[20];
    char param[20];
}MNG_PTZ_CTRL;

typedef struct
{
    int protocol;
    char acc[36];
    char pwd[36];
    char hostName[128];
}DDNS_INFO;

typedef struct
{
    int chn;
    int size;
    int bright;
    int contrast;
    int saturation;
    int hue;
}MNG_DISPALY_CTRL;

typedef struct
{
    char wifiSsid[32];
    char wifiPwd[64];
    int wifiAuth;
    int wifiEncryp;
    _UINT8 wifiIndex;
    _UINT8 wifiChannel;
    _UINT16 wifiRate;
}WIFI_INFO;


typedef struct
{
    _UINT8 dhcpEn;
    _UINT8 upnpEn;
    _UINT8 wifiEn;
    _UINT8 mobileEn;
    _UINT8 mobileType;
    _UINT8 mobileCard;
    _UINT8 portMapEn;  //20100427 added
    _UINT8 rstFialDialNum;
    char APN[36];
    _UINT32 netSt;  //0,1, 20131125 added
    _UINT32 wifiSt;  //0,-1 or ip, -1 means checking
    _UINT32 pppSt;  //0, or ip
    _UINT8 routerEn;
    _UINT8 dhcpdEn;
    _UINT8 routerRange; //20170705
    _UINT8 ipVer; //20181031
    _UINT8 mobileAuth;
    _UINT8 res[11]; //_UINT8 res[36];
    _UINT32 ipFlow;
    _UINT32 routerFlow;
    _UINT32 wifiIp;
    _UINT32 wifiMask;
    WIFI_INFO wifi;   
}MNG_DYNAMIC_NET_INFO;//20090806 added

typedef struct
{
    char ssid[32];
    int auth;
    int encryp;
    int net;
    int chn;
    int rssi;
}MNG_WIFI_NET;

typedef struct
{
    int count;
    MNG_WIFI_NET net[64];
}MNG_WIFI_NETs;

typedef struct
{
    _UINT8 diskFullAlarmEnabled;
    _UINT8 diskFullHighThreshold;/*硬盘满告警高阈值，0-100*/
    _UINT8 diskFullLowThreshold; /*硬盘满告警低阈值，0-100*/
    _UINT8 diskErrorAlarmEnabled;
    int alarmOutActiveTime;
    int shootTiming;  /*定时抓拍时间间隔*/
    _UINT16 ptzResetTime; //20100309 added
    _UINT16 rebootTime; //20100714 added
    _UINT8 ptzPreset[2];
    _UINT16 rebootDays; //20100714 added
    int alarmPreRecordTime; /*告警录像预录时间
                                                    0：不预录
                                                    1：5秒
                                                    2：10秒
                                                    3：15秒
                                                    4：20秒
                                                    5：25秒 
                                                    6：30秒
                                                    7：尽可能预录
                                                    */
    int alarmRecordTime;  /*发生告警后录像时长*/

    _UINT8 photoResolution;   /*抓图的分辨率： 0 . QCIF, 1 . CIF, 2 . D1, -1: DVS没有告警抓拍功能*/
    _UINT8 powerOffAlarmEn; /*断电告警使能:0. 禁止, 1. 使能*/
    _UINT8 encEn;           /*启用加密,最高位为1表示有这项功能*/
    _UINT8 alarmVoiceMode;
    _INT8 photoQuality;        /*抓图的质量：0 . 最好,1 . 较好, 2 . 一般,-1. DVS没有告警抓拍功能*/
    _UINT8 aac; /*aac时间*/
    _UINT8 wbMode; /*weighbridge mode*/
    _UINT8 TankH;
    short shootTimes;          /*每次触发抓拍次数：1-16,  -1. DVS没有告警抓拍功能*/
	_UINT8 TankH_water;
	_UINT8 res;
}ALARM_PARA;

typedef struct
{
    _UINT8 ftpEn;
    _UINT8 alarmEn;
    _UINT8 smtpEn;
    _UINT8 smtpEnc;
    int ftpPort;
    int alarmPort;
    int smtpPort;
    char ftpServer[32];
    char ftpAcc[32];
    char ftpPwd[32];
    char alarmServer[32];
    char smtpServer[32];
    char smtpAcc[32];
    char smtpPwd[32];
    char smtpEmail[32];
    char emailSender[32];
    char email[4][32];
}SERVICE_INFO;

#define MAX_MNG_NAME_LEN         16
#define MAX_MNG_DESCRIPTION_LEN 16   
#define MAX_MNG_PWD_LEN           16
typedef struct
{
    int level;
    char name[MAX_MNG_NAME_LEN];
    char description[MAX_MNG_DESCRIPTION_LEN];
    char pwd[MAX_MNG_PWD_LEN];
}MNG_ACC;

/*平台接入信息*/
typedef struct
{
    int platformId; /*平台号, 0:所有平台，1~8:平台1~8*/
    int protocol;
    char devId[32];
    char pwd[64];
    char regSrv[68]; /*接入服务器域名*/
    _UINT32 srvIp;
    int srvPort;
    _UINT32 alarmSrvIp;
    int alarmSrvPort;
    _UINT32 sipSrvIp; /*主sip地址*/
    int sipSrvPort;
    _UINT8 sipHeartEnabled;
    _UINT8 httpHeartEnabled;
    _UINT8 sipReisterEnabled;
    _UINT8 registerValidTime;
    _UINT16 sipHeartPeriod;
    _UINT16 httpHeartPeriod;
    int storageProtocol; /*存储文件格式(平台类型),保存用*/
    _UINT32 sipSrvIp2; /*辅sip地址*/
    int sipSrvPort2;
    char sipSrvAddr[64];
    char domainId[32];
    char serverId[32];
}PLATFORM_INFO_S;


typedef struct
{
    _UINT8 tcpSbufLevel;    
    _UINT8 udpDelayPacks; /*udp延时间隔(/包)*/
    _UINT16 naluSize; /*nalu分割大小*/
    _UINT16 udpDelayUs; /*udp发送包延时us*/
    _UINT16 packbufNum;
    _UINT8 subNaluEnable; /*子码流nalu分割*/
    _UINT8 carType;
    _UINT8 carColor;
    char res1;
    int timOffset;/*时间偏移*/
    char plate[10];
    char sim[14];
    int gnssTim;
    int gnssDis;
    char gnssType;
    char gnssMode;
    int  gpsUploadIntval;
    int  alarmSpeed;
    int  tireTime;
    int  restTime;
    char rollOverSen;
    char collisionSen;
    char res[188];
}MNG_SYS_ADVANCED_INFO;

typedef struct
{
    _UINT8 weekday;/*0:None, 1:SUN, 2:Mon, 3:Tue, 4:Wed..., 7:Sat, 8:EveryDay*/
    _UINT8 startHour;
    _UINT8 startMin;
    _UINT8 stopHour;
    _UINT8 stopMin;
    _UINT8 recordType;
    _UINT8 res[2];
}STORAGE_SCHEDULE;

typedef struct
{
    _UINT8 recordEnable;
    _UINT8 recycleRecEnable;
    _UINT8 recordPolicy;
    _UINT8 frameInterval;
    _UINT32 splitTime;
    char filenameSuffix[32];
}STORAGE_BASIC;
             
typedef struct
{
    _UINT8 recordType;
    _UINT8 res[3];
    _UINT32 daySch[7]; /*每一位表示每个小时是否存储共7天*/
}STORAGE_SCHEDULE_EX;

typedef struct
{
    STORAGE_BASIC basic;
    STORAGE_SCHEDULE schedule[MAX_STORAGE_SCHEDULE]; 
    STORAGE_SCHEDULE_EX scheduleEx[5]; /*5种类型*/
    _UINT8 streamType; /*存储码流类型:0 主码流,1 子码流*/
    _UINT8 recycleMode;
    char res[7]; 
}STORAGE_TEMP_PARA;

typedef enum
{
    STREAM_MAIN,
    STREAM_MINOR,
    STREAM_SNAP,
}STREAM_MODE_E;

/*分割模式*/
/*typedef enum
{
    DEV_PREVIEW_ONE,
    DEV_PREVIEW_FOUR = 4,
    DEV_PREVIEW_SIX = 6,
    DEV_PREVIEW_EIGHT = 8,
    DEV_PREVIEW_NINE = 9,
    DEV_PREVIEW_SIXTEEN = 16,
}DEV_PREVIEW_MODE_E; */
typedef enum 
{
    DISPLAY_MODE_1,
    DISPLAY_MODE_1_1INCLUDE,
    DISPLAY_MODE_1BIG_1SMALL,
    DISPLAY_MODE_2_MIDDLE,
    DISPLAY_MODE_3,
    DISPLAY_MODE_1_2RIGHT,
    DISPLAY_MODE_4,
    DISPLAY_MODE_1_3BOTTOM,
    DISPLAY_MODE_1_3RIGHT,
    DISPLAY_MODE_1_3LEFT,
    DISPLAY_MODE_4_SMALL,
    DISPLAY_MODE_6_2ROW,
    DISPLAY_MODE_6_3ROW,
    DISPLAY_MODE_6,
    DISPLAY_MODE_8,
    DISPLAY_MODE_9,
    DISPLAY_MODE_10,
    DISPLAY_MODE_13,
    DISPLAY_MODE_16,
    DISPLAY_MODE_TOTAL,
}DISPLAY_MODE;
#define BIT_SET(st, bit)  ((st) = (st)|(bit))
#define BIT_CLR(st, bit) ((st) = (st)&~(bit))
#define HAS_BIT(st, bit) ((st)&(bit))

typedef enum hiVO_INTF_SYNC_E
{
    VO_OUTPUT_PAL           = 0,
    VO_OUTPUT_NTSC          = 1,

    VO_OUTPUT_720P60        = 2,
    VO_OUTPUT_1080I50       = 3,
    VO_OUTPUT_1080I60       = 4,
    VO_OUTPUT_1080P25       = 5,
    VO_OUTPUT_1080P30       = 6,

    VO_OUTPUT_800x600_60    = 7,			/* VESA 800 x 600 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1024x768_60   = 8,			/* VESA 1024 x 768 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1280x1024_60  = 9,			/* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1366x768_60   = 10,			/* VESA 1366 x 768 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1440x900_60   = 11,			/* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT Compliant */

    VO_OUTPUT_800x600_75    = 12,			/* VESA 800 x 600 at 75 Hz (non-interlaced) */
    VO_OUTPUT_1024x768_75   = 13,			/* VESA 1024 x 768 at 75 Hz (non-interlaced) */

    VO_OUTPUT_USER			= 14,
    VO_OUTPUT_BUTT

} VO_INTF_SYNC_E;

/*输出设备3515支持*/
typedef enum
{
    VO_MST_DEV_HD = 0, /*高清显示设备*/
    VO_MST_DEV_AD = 1, /*辅助显示设备3515不支持*/
    VO_MST_DEV_SD = 2, /*标清显示设备*/
    VO_MST_DEV_ALL = 3, /*所有显示设备*/ 
    VO_MST_DEV_BUTT,
}VO_MST_DEV_E;

typedef enum
{
    ALARM_SCH_GPIN,
    ALARM_SCH_VIDEOLOSE,
    ALARM_SCH_HIDEDETECT,
    ALARM_SCH_MOTIONDETECT,
    ALARM_SCH_RES, /*保留*/
    ALARM_SCH_TOTAL,
}ALARM_SCH_TYPE_E;
              
typedef struct
{
    _UINT8 splitMod; /*分割模式*/
    _UINT8 language;
    _UINT8 skin;
    _UINT8 displayChn;/*单画面显示的通道*/
    _UINT8 intfSync[4]; /*分辨率*/
    int voDev; /*输出设备:HD, AD, SD*/
    int ctrlMode;
    char displayLocal;
    char displayIcon;
    char screenSame;
    char loopMute;
    char irDisable;
    char pktDropMode;
    char showMtName;
    _UINT8 alpha;
    _UINT32 tipClr;
    _UINT32 screenBkClr;    
    _UINT8 ledType;
    char res[31]; /*保留*/
}MNG_DVR_S;

typedef struct
{
    char devId[32];
    _UINT8 devMac[6];
    char devName[32];
    char devPwd[32];
    _UINT32 devCap;
    int chnNum;
    _UINT32 sipRegIp;
    _UINT32 sipRegPort;
    char sipSrvAddr[64];
    char res[128];
}MNG_DEV_ID;

typedef struct
{
    int phoneVol;
    int micVol;
    char res[32];
}VOLUME;


typedef struct
{
    char productType[8];
    char productNo[16];
    unsigned int date;
    unsigned char mac[6];
    unsigned short regPort;
    char id[20];
    char regsrv[32];
    char user[16];
    char pwd[16];
	char imei[16];
    char res[16];
}PRODUCT_INFO;

typedef struct
{
    int type; //图片类型：1定时抓拍、2超速拍照、3超时停车拍照、4油量异常拍照、5考核拍照、6GPS无效拍照
    int chn;//通道数：代表装了几路摄像头，装了几路拍照时就同时触发几路拍照
    char date[12];//日期
    char tim[12];//图片名(服务器时间)
}MNG_PTCMD; //PTCMD,000000000001,1,3,20160512,080515,OK
                   
typedef struct
{
    int pos;
    _UINT8 type;
    _UINT8 stroage;
    _UINT16 port;
    char ip[32];
    char acc[32];
    char pwd[32];
    char timeZone;
    char res[23];
}MNG_NVR;

/*
typedef struct
{
    int type;
    int lowLimit;
    int highLimit;
    int lowAction;
    int highAction;
    int res[3];
}DEV_ENV_SET;    */

typedef struct
{
    int type;
    int lowLimit;
    int highLimit;
    _UINT32 action;
    _UINT32 haction;
    _UINT16 timing;
    _UINT16 timing2;
    _UINT8 days;
    _UINT8 chnNum;
    _UINT16 maxTime;
    _INT8 actionSt;
    _INT8 actionNow;
    _UINT16 count;
}DEV_ENV_SET;

typedef struct
{
	int type;
	int low_limit;
	int high_limit;
}DEV_ENV_LLIMIT;

enum
{
    e_IR_TYPE_Air_Conditioner,
    e_IR_TYPE_Unknow
};
   
typedef struct
{
    _UINT8 mode;  //0=控制,1=学习
    _UINT8 res;
    _UINT16 type;   //控制类型
    _UINT8 action[60]; //控制命令
}MNG_IR;

#ifdef __cplusplus
}
#endif

#endif

