//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>

#include "display.h"
#include "cfgclt.h"
#include "dataclt.h"
#include <Dialogs.hpp>
#include "pies.h"
#include "CGAUGES.h"
#include "CSPIN.h"
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include "WinSkinData.hpp"
#include "dateplan.h"
#include <Buttons.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include "printset.h"
#include "udp.h"             
#include "cfgclt.h"
#include <CheckLst.hpp>
//#include "WinSkinData.hpp"

/*//////////////////////
版本说明:
v1.0.0.1,初始版本
v1.0.0.2,增加告警设置
v1.0.0.3,增加日志查询，增加皮肤
v1.0.0.4,一次升级多个文件
v1.0.0.5,多通道设置
v1.0.0.6,更改硬件状态
v1.0.0.7,增加ip地址和端口号查询    
v1.0.0.8,增加数据端口设置，点击图象显式窗口可控制云台    
v1.0.0.8,增加DDNS设置,
v1.0.0.9,增加恢复显示参数
V1.0.0.10,VCLskin5.03，增加显示参数设置，增加拉近拉远、以及焦距调节
V1.0.0.11，增加动态网络设置
V1.0.0.12, 支持对讲(为了兼容hx平台，使用rtp方式)  
V1.0.0.13, 增加基本告警设置、子码流设置
v1.0.0.14, 增加预置点设置
v1.0.0.15, 增加端口映射设置
V1.0.0.16, 修改开关量告警设置    
V1.0.0.17, 云台自动回位白天、夜间模式设置
V1.0.0.18, 增加自动重启设置
V1.0.0.38, 增加通道ID/名称设置
V1.0.0.40, 增加存为缺省，清除缺省，一键设ip        
V1.0.0.50, 增加报警服务设置，增加账号设置
V1.0.0.60, 增加开关量触发模式设置
V1.0.0.61, 修改显示设置
V1.0.0.62, 增加启用路由、dhcpserver功能
v1.0.0.63, 增加3g apn设置
v1.0.0.64, 增加router flow设置       
v1.0.0.65, 格式化进度修改
v1.0.0.66, 增加DF协议
V1.0.0.67,增加AAC设置
V1.0.0.68,增加地磅设置
V1.0.0.69,增加油箱高度设置
V1.0.0.70,增加nvr设置
V1.0.0.71,增加镜像设置
V1.0.0.72,修改平台设置none问题
V1.0.0.73,增加配置文件加载
V1.0.0.74,增加IP限速
V1.0.0.75,增加环境参数设置     
V1.0.0.76,增加T808,T212协议 .增加外接led设置
V1.0.0.77,增加路由区域限定
V1.0.0.78,增加时间偏移设置
V1.0.0.79,增加车辆信息设置
V1.0.0.80,增加NVR通道时区设置     
V1.0.0.81,增加ENV类型
V1.0.0.82,增加红外配置
v1.0.0.83,增加开关配置
v1.0.0.84,增加环境数据分区数量配置
v1.0.0.85,增加O2等配置          
v1.0.0.86,增加倒车映像配置等配置    
v1.0.0.87,增加几种控制器
V1.0.0.89,增加GNSS设置        
V1.0.0.90,增加硬盘正在格式化状态          
V1.0.0.92,增加无线拨号认证方式配置
*/

#define WM_CLIENT_DOWN (WM_USER+100)
#define WM_MNG_BASE_INFO (WM_USER+101)
#define WM_MNG_SET_BASE_OK (WM_USER+102)
#define WM_MNG_REBOOTING (WM_USER+103)
#define WM_MNG_VIDEO_INFO (WM_USER+104)
#define WM_MNG_SET_VIDEO_INFO (WM_USER+105)
#define WM_MNG_UPLOAD_ERROR (WM_USER+106)
#define WM_MNG_DOWNLOAD_ERROR (WM_USER+107)
#define WM_MNG_DISPLAY_END (WM_USER+108)
#define WM_MNG_DISPLAY_START (WM_USER+109)
#define WM_MNG_DOWNLOAD_POS (WM_USER+110)
#define WM_MNG_DOWNLOAD_OVER (WM_USER+111)
#define WM_MNG_OSD_INFO  (WM_USER+112)
#define WM_MNG_VIDEO_ALARM  (WM_USER+113)
#define WM_MNG_GPIN_ALARM  (WM_USER+114)
#define WM_MNG_UNSUPPORT   (WM_USER+115)
#define WM_MNG_VERSION   (WM_USER+116) 
#define WM_MNG_CMD_ERR   (WM_USER+117)
#define WM_MNG_CMD_RET   (WM_USER+200)   
#define WM_MNG_ALARM_BASIC   (WM_USER+201)
#define WM_MNG_VIDEO_SUB_INFO  (WM_USER+202)
#define WM_MNG_PLATFORM_INFO  (WM_USER+203)
#define WM_MNG_ADVANCED_INFO (WM_USER+204)
#define WM_STORAGE_NEW_INFO (WM_USER+205)
#define WM_STORAGE_INFO (WM_USER+206)
#define WM_MNG_DVR_INFO (WM_USER+207)
#define WM_MNG_DEV_ID_INFO (WM_USER+208)   
#define WM_MNG_SET_VPARA_INFO (WM_USER+209)
#define WM_MNG_GET_VOLUME (WM_USER+210)  
#define WM_MNG_OSD_EX_INFO  (WM_USER+211) 
#define WM_MNG_PTZ_INFO (WM_USER+212)
#define WM_MNG_DEV_TIME (WM_USER+213)
#define WM_MNG_DEV_ALARM_PLAN (WM_USER+214)
#define WM_MNG_SYS_INFO (WM_USER+215)
#define WM_MNG_FORMAT_PROGRESS (WM_USER+216)
#define WM_MNG_FORMAT_DISK (WM_USER+217)
#define WM_MNG_GET_DISK_INFO (WM_USER+218)
#define WM_MNG_SWITCH_ALARM (WM_USER+219)
#define WM_MNG_GET_EX_VIDEO_PARA (WM_USER+220) 
#define WM_MNG_EX_GPIN_ALARM  (WM_USER+221)
#define WM_MNG_EX_SWITCH_OUT_ALARM  (WM_USER+222)
#define WM_MNG_GET_NVR_PARA (WM_USER+223)
#define WM_MNG_CMD_GET_ENV  (WM_USER+224)
#define WM_MNG_CMD_GET_SWITCH (WM_USER+225)


#define WM_MNG_SET_PARA_ERROR (WM_USER+500)

typedef struct
{
    long ip;
    int port;
    unsigned char mac[6];
    char macSt;
    char ver;
    char ethSt;
}DEVICE_ADDRESS;

typedef enum
{
    DVS_PROTOCOL_HC,
    DVS_PROTOCOL_CNC,
    DVS_PROTOCOL_HX,
    DVS_PROTOCOL_AL,
    DVS_PROTOCOL_ZTE,
    DVS_PROTOCOL_KEDCOM,      
    DVS_PROTOCOL_HW,
    DVS_PROTOCOL_DF,
    DVS_PROTOCOL_T808,
    DVS_PROTOCOL_T212,
    DVS_PROTOCOL_GB28181,    
    DVS_PROTOCOL_RM,
    DVS_PROTOCOL_DTU,
    DVS_PROTOCOL_ONENET,
    DVS_PROTOCOL_DM,
    DVS_PROTOCOL_NONE = 16,
}DVS_PROTOCOL;

typedef struct
{
    char sign[16];
    char devType[64];
    char devVer[32];
    char devBatch[64];
    char devName[128];
    char userAddr[128];
}PRINTER_SAVE_S;

typedef struct
{
    DWORD macH;
    DWORD macL;
    DWORD sNoH;
    DWORD sNoL;
    DWORD lastTime;
    char productType[8];
    char regsrv[32];
    int regPort;
    DWORD idH;
    DWORD idL;
    int hexMode;
}PRODUCT_INFO_ALL;

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *BasicSheet;
        TTabSheet *VideoSheet;
        TEdit *Name;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *Id;
        TLabel *Label3;
        TEdit *IP;
        TLabel *Label4;
        TEdit *RegIp;
        TLabel *Label6;
        TEdit *IPMask;
        TLabel *Label7;
        TLabel *Label8;
        TButton *Button1;
        TLabel *Label9;
        TEdit *PWD;
        TButton *Button2;
        TTabSheet *LogSheet;
        TButton *Button5;
        TLabel *Label16;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TEdit *LogPort;
        TEdit *LogName;
        TEdit *LogPwd;
        TTabSheet *UpgradeSheet;
        TEdit *Edit1;
        TButton *Button6;
        TLabel *Label24;
        TButton *ButtShowVid;
        TButton *Button8;
        TEdit *Gataway;
        TLabel *Label25;
        TEdit *DNS1;
        TLabel *Label26;
        TEdit *DNS2;
        TLabel *Label27;
        TButton *Button9;
        TOpenDialog *OpenDialog1;
        TCGauge *CGauge1;
        TLabel *Label29;
        TButton *Button10;
        TButton *Button11;
        TCheckBox *PPPoEEn;
        TLabel *Label30;
        TLabel *Label31;
        TEdit *PPPoEName;
        TEdit *PPPoEPWD;
        TEdit *SipSrvIp;
        TLabel *Label33;
        TLabel *Label36;
        TEdit *MAC;
        TLabel *Label37;
        TEdit *HWSt;
        TTabSheet *OsdSheet;
        TTabSheet *AlarmSheet;
        TButton *Button12;
        TButton *Button4;
        TButton *Button3;
        TLabel *Label44;
        TEdit *Ver;
        TLabel *Label45;
        TComboBox *LinkMode;
        TLabel *Label46;
        TEdit *MulticastAddr;
        TTabSheet *LogInfoSheet;
        TMemo *LogShow;
        TButton *Button15;
        TButton *Button16;
        TButton *Button17;
        TLabel *Label71;
        TComboBox *Protocol;
        TLabel *Label72;
        TEdit *DomainId;
        TLabel *Label5;
        TEdit *AlarmSrvAddr;
        TCSpinEdit *MngPort;
        TCSpinEdit *SipPort;
        TCSpinEdit *RegPort;
        TCSpinEdit *SipSrvPort;
        TCSpinEdit *AlarmSrvPort;
        TCSpinEdit *HttpPeriod;
        TCSpinEdit *SipPeriod;
        TCSpinEdit *MulticastPort;
        TCheckBox *HttpHeartEn;
        TCheckBox *SipHeartEn;
        TButton *Button18;
        TSaveDialog *SaveDialog1;
        TMemo *Files;
        TLabel *Label32;
        TMemo *Result;
        TTabControl *TabControl2;
        TGroupBox *GroupBox1;
        TLabel *Label38;
        TLabel *Label39;
        TCSpinEdit *TimeX;
        TCSpinEdit *TimeY;
        TCheckBox *ShowTime;
        TCheckBox *ShowDate;
        TGroupBox *GroupBox2;
        TLabel *Label40;
        TLabel *Label41;
        TCSpinEdit *TextX;
        TCSpinEdit *TextY;
        TCheckBox *ShowText;
        TEdit *Text;
    TEdit *PPPIp;
    TLabel *Label34;
    TButton *Button7;
    TComboBox *LogIp;
    TLabel *Label47;
    TLabel *Num;
    TLabel *Label73;
    TCSpinEdit *PortBase;
    TLabel *Label74;
    TCSpinEdit *PortRange;
        TTabSheet *IOSheet;
    TButton *Button20;
        TSkinData *SkinData1;
        TTabSheet *DynamicSheet;
        TCheckBox *DhcpEn;
        TCheckBox *UpnpEn;
        TGroupBox *GroupBox7;
        TLabel *Label80;
        TComboBox *MobileType;
        TButton *Button21;
        TLabel *Label85;
        TComboBox *MobileCard;
        TButton *Button22;
        TCheckBox *PortMapEn;
        TTabSheet *MulPlatFormSheet;
        TTabControl *TabControl5;
        TEdit *pid;
        TCSpinEdit *pSipPeriod;
        TCSpinEdit *pHttpPeriod;
        TEdit *pHttpPwd;
        TEdit *pRegIp;
        TCSpinEdit *pRegPort;
        TCSpinEdit *pSipPort;
        TEdit *pSipIp;
        TEdit *pAlarmIp;
        TCSpinEdit *pAlarmPort;
        TLabel *Label68;
        TLabel *Label69;
        TLabel *Label70;
        TLabel *Label99;
        TCheckBox *pHttpHeartEn;
        TLabel *Label107;
        TComboBox *pptc;
        TLabel *Label116;
        TCheckBox *pSipHeartEn;
        TButton *Button24;
        TCheckBox *StorageEn;
        TTabSheet *AdvancedSheet;
        TPageControl *PageControl3;
        TTabSheet *NetSheet;
        TButton *Button26;
        TButton *Button25;
        TTabSheet *NewStorageSheet;
        TTabSheet *StorageSheet;
        TTabControl *TabControl6;
        TLabel *Label100;
        TLabel *Label101;
        TLabel *Label102;
        TLabel *Label103;
        TCheckBox *recordEnable;
        TCheckBox *recycleRecEnable;
        TEdit *filenameSuffix;
        TCSpinEdit *splitTime;
        TTabControl *TabControl7;
        TLabel *Label104;
        TCheckBox *recordEnable1;
        TCheckBox *recycleRecEnable1;
        TLabel *Label105;
        TLabel *Label106;
        TCSpinEdit *splitTime1;
        TEdit *filenameSuffix1;
        TLabel *Label108;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TGroupBox *GroupBox12;
        TLabel *Label110;
        TLabel *Label113;
        TLabel *Label115;
        TLabel *Label118;
        TCSpinEdit *startHour1;
        TCSpinEdit *startMin1;
        TLabel *Label111;
        TLabel *Label112;
        TCSpinEdit *stopMin1;
        TLabel *Label114;
        TLabel *Label117;
        TCSpinEdit *stopHour1;
        TLabel *Label119;
        TCSpinEdit *startHour2;
        TLabel *Label120;
        TCSpinEdit *startMin2;
        TLabel *Label121;
        TLabel *Label122;
        TCSpinEdit *stopHour2;
        TLabel *Label123;
        TCSpinEdit *stopMin2;
        TLabel *Label124;
        TCSpinEdit *stopMin4;
        TLabel *Label125;
        TLabel *Label126;
        TCSpinEdit *stopHour4;
        TLabel *Label127;
        TLabel *Label128;
        TCSpinEdit *startMin4;
        TLabel *Label129;
        TCSpinEdit *startHour4;
        TCSpinEdit *startHour3;
        TLabel *Label130;
        TCSpinEdit *startMin3;
        TLabel *Label131;
        TLabel *Label132;
        TCSpinEdit *stopHour3;
        TLabel *Label133;
        TCSpinEdit *stopMin3;
        TLabel *Label134;
        TLabel *Label109;
        TLabel *Label135;
        TLabel *Label136;
        TLabel *Label137;
        TCSpinEdit *startHour8;
        TCSpinEdit *startHour5;
        TLabel *Label138;
        TCSpinEdit *startMin5;
        TLabel *Label139;
        TLabel *Label140;
        TCSpinEdit *startMin6;
        TLabel *Label141;
        TCSpinEdit *startHour6;
        TCSpinEdit *startHour7;
        TLabel *Label142;
        TLabel *Label143;
        TCSpinEdit *startMin7;
        TLabel *Label144;
        TLabel *Label145;
        TCSpinEdit *startMin8;
        TLabel *Label146;
        TLabel *Label147;
        TLabel *Label148;
        TLabel *Label149;
        TCSpinEdit *stopHour5;
        TLabel *Label150;
        TCSpinEdit *stopMin5;
        TLabel *Label151;
        TLabel *Label152;
        TCSpinEdit *stopMin6;
        TLabel *Label153;
        TCSpinEdit *stopHour6;
        TCSpinEdit *stopHour7;
        TLabel *Label154;
        TCSpinEdit *stopMin7;
        TLabel *Label155;
        TLabel *Label156;
        TCSpinEdit *stopMin8;
        TLabel *Label157;
        TCSpinEdit *stopHour8;
        TComboBox *weekDay1;
        TComboBox *weekDay2;
        TComboBox *weekDay3;
        TComboBox *weekDay4;
        TComboBox *weekDay8;
        TComboBox *weekDay7;
        TComboBox *weekDay6;
        TComboBox *weekDay5;
        TComboBox *recordType1;
        TComboBox *recordType2;
        TComboBox *recordType3;
        TComboBox *recordType4;
        TComboBox *recordType5;
        TComboBox *recordType6;
        TComboBox *recordType7;
        TComboBox *recordType8;
        TTabSheet *DVRSetSheet;
        TGroupBox *GroupBox13;
        TLabel *Label158;
        TComboBox *voDev;
        TLabel *Label159;
        TComboBox *splitMod;
        TLabel *Label160;
        TComboBox *intfSync;
        TButton *Button27;
        TComboBox *intfSync1;
        TCSpinEdit *pSipPort2;
        TEdit *pSipIp2;
        TLabel *Label161;
        TLabel *Label162;
        TLabel *Label169;
        TComboBox *CtrlMode;
        TPanel *OSDColor;
        TLabel *Label170;
        TColorDialog *ColorDialog1;
        TCheckBox *DisplayLocal;
        TCheckBox *DisplayIcon;
        TCheckBox *LoopMute;
        TCheckBox *ScreenSame;
        TLabel *Label171;
        TCheckBox *IrDisable;
        TLabel *Label172;
        TPanel *TipColor;
        TLabel *Label173;
        TPanel *ScreenBkColor;
        TTabControl *TabControl1;
        TPanel *Panel2;
        TLabel *Label35;
        TLabel *Label60;
        TLabel *Label55;
        TComboBox *AudioCodec;
        TComboBox *Norm;
        TGroupBox *GroupBox9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label14;
        TLabel *Label18;
        TLabel *Label23;
        TLabel *Label61;
        TLabel *Label62;
        TLabel *Label63;
        TLabel *Label64;
        TEdit *CamAddr;
        TEdit *SioNum;
        TComboBox *CamType;
        TComboBox *PtzType;
        TComboBox *BaudRate;
        TComboBox *Parity;
        TComboBox *Databit;
        TComboBox *Stopbit;
        TComboBox *Flow;
        TPageControl *PageControl2;
        TTabSheet *TabSheet2;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label15;
        TLabel *Label17;
        TLabel *Label22;
        TLabel *Label28;
        TLabel *Label42;
        TEdit *IFrame;
        TEdit *FrameRate;
        TEdit *BitRate;
        TComboBox *Codec;
        TComboBox *VideoSize;
        TComboBox *BitType;
        TTabSheet *TabSheet1;
        TLabel *Label92;
        TLabel *Label93;
        TLabel *Label94;
        TLabel *Label95;
        TLabel *Label96;
        TLabel *Label97;
        TLabel *Label98;
        TEdit *SubIFrame;
        TEdit *SubFrameRate;
        TEdit *SubBitRate;
        TComboBox *SubCodec;
        TComboBox *SubVideoSize;
        TComboBox *SubBitType;
        TComboBox *AudioChn;
        TCheckBox *ShowMtName;
        TLabel *Label174;
        TCSpinEdit *Alpha;
        TComboBox *PktDropMode;
        TLabel *Label175;
        TTimer *Timer1;
        TTabSheet *DiskSheet;
        TListView *lDiskInfo;
        TProgressBar *formatPb;
        TButton *Button41;
        TEdit *formatInfo;
        TLabel *Label209;
        TLabel *Label203;
        TComboBox *storageStreamType;
        TComboBox *cbRecycleMode;
        TGroupBox *GroupBox11;
        TPageControl *StorageSchPageControl;
        TTabSheet *NormalSchTabSheet;
        TPaintBox *PaintBox1;
        TTabSheet *MdSchTabSheet;
        TPaintBox *PaintBoxMd;
        TTabSheet *GpinSchTabSheet;
        TPaintBox *PaintBoxGpin;
        TLabel *Label210;
        TComboBox *storageStreamType1;
        TTabSheet *ProductSheet;
        TButton *Button31;
        TButton *Button34;
        TGroupBox *GroupBox15;
        TLabel *Label183;
        TLabel *Label184;
        TLabel *Label186;
        TLabel *Label187;
        TLabel *Label188;
        TLabel *Label194;
        TLabel *Label195;
        TLabel *Label196;
        TEdit *ProductType2;
        TEdit *ProductMac2;
        TEdit *ProductNo2;
        TEdit *ProductId2;
        TEdit *ProductSrv2;
        TCSpinEdit *ProductSrvPort2;
        TEdit *ProductTime2;
        TEdit *ProductAcc2;
        TGroupBox *GroupBox16;
        TLabel *Label181;
        TLabel *Label185;
        TLabel *Label189;
        TLabel *Label190;
        TLabel *Label192;
        TLabel *Label193;
        TEdit *ProductNo;
        TEdit *ProductId;
        TCSpinEdit *ProductSrvPort;
        TEdit *ProductAcc;
        TLabel *Label182;
        TEdit *ProductMac;
        TLabel *Label191;
        TEdit *ProductTime;
        TComboBox *ProductType;
        TComboBox *ProductSrv;
        TGroupBox *GroupBox17;
        TLabel *Label198;
        TLabel *Label199;
        TCSpinEdit *ExTextX;
        TCSpinEdit *ExTextY;
        TCheckBox *ShowTextEx;
        TEdit *ExText;
        TComboBox *cbQP;
        TComboBox *subsQP;
        TCheckBox *HexMode;
		TComboBox *Language;
		TComboBox *Skin;
        TGroupBox *GroupBox6;
        TLabel *Label79;
        TLabel *Label81;
        TLabel *Label83;
        TLabel *Label84;
        TLabel *Label86;
        TLabel *Label87;
        TLabel *Label200;
        TComboBox *WifiAuth;
        TComboBox *WifiEncry;
        TEdit *WifiPwd;
        TEdit *WifiIp;
        TEdit *WifiMask;
        TButton *Button30;
        TComboBox *WifiSsid;
        TEdit *WifiRssi;
        TCheckBox *WifiEn;
        TLabel *Label201;
        TLabel *Label202;
        TEdit *eVideoChnName;
        TEdit *eVideoChnId;
        TLabel *Label204;
        TLabel *Label205;
        TButton *Button28;
        TButton *Button29;
        TButton *Button32;
        TLabel *LabelMac;
        TLabel *LabelEth;
        TLabel *Label82;
        TEdit *dId;
        TEdit *Edit2;
        TButton *Button35;
        TPageControl *PageControl4;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TGroupBox *GroupBox3;
        TButton *Button13;
        TTabControl *TabControl3;
        TLabel *Label43;
        TLabel *Label48;
        TLabel *Label49;
        TLabel *Label58;
        TLabel *Label65;
        TLabel *Label66;
        TCheckBox *VLoseEn;
        TCheckBox *VMDEn;
        TEdit *MDTime;
        TComboBox *VLoseOut;
        TComboBox *VMDOut;
        TCSpinEdit *VLoseOff;
        TCSpinEdit *VMDOff;
        TTabSheet *TabSheet5;
        TLabel *Label88;
        TLabel *Label89;
        TLabel *Label90;
        TLabel *Label91;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TCSpinEdit *ShootTime;
        TCSpinEdit *PtzResetTime;
        TCSpinEdit *RecordTime;
        TCSpinEdit *Shoots;
        TButton *Button23;
        TCSpinEdit *Preset1;
        TCSpinEdit *Preset2;
        TCSpinEdit *RebootDays;
        TCSpinEdit *RebootHour;
        TCSpinEdit *RebootMin;
        TTabSheet *TabSheet6;
        TGroupBox *GroupBox4;
        TButton *Button14;
        TTabControl *TabControl4;
        TLabel *Label50;
        TLabel *Label54;
        TLabel *Label59;
        TLabel *Label67;
        TLabel *Label197;
        TComboBox *GpinOut1;
        TCSpinEdit *GpinV1;
        TCSpinEdit *GpinVP1;
        TCSpinEdit *GpinOff1;
        TComboBox *Gpin1;
        TLabel *Label163;
        TCSpinEdit *PreRecTime;
        TGroupBox *GroupBox8;
        TCheckBox *FtpEn;
        TLabel *Label164;
        TEdit *FtpSrv;
        TCSpinEdit *FtpPort;
        TLabel *Label165;
        TEdit *FtpAcc;
        TLabel *Label166;
        TEdit *FtpPwd;
        TLabel *Label167;
        TGroupBox *GroupBox14;
        TLabel *Label168;
        TLabel *Label178;
        TEdit *AlarmSrv;
        TCSpinEdit *AlarmPort;
        TCheckBox *AlermEn;
        TGroupBox *GroupBox18;
        TLabel *Label176;
        TLabel *Label177;
        TLabel *Label179;
        TLabel *Label180;
        TEdit *SmtpSrv;
        TCSpinEdit *SmtpPort;
        TEdit *SmtpAcc;
        TEdit *SmtpPwd;
        TCheckBox *SmtpEn;
        TLabel *Label206;
        TEdit *SmtpSender;
        TLabel *Label207;
        TLabel *Label208;
        TLabel *Label211;
        TEdit *Email1;
        TEdit *Email2;
        TEdit *Email3;
        TLabel *Label212;
        TEdit *Email4;
        TLabel *Label213;
        TComboBox *SmtpEnc;
        TButton *Button33;
        TButton *Button36;
        TTabSheet *AccSheet;
        TListView *AccList;
        TButton *Button37;
        TButton *Button38;
        TButton *Button39;
        TCheckBox *EncEn;
        TLabel *Label214;
        TComboBox *AlarmVoice;
        TLabel *Label215;
        TComboBox *GpinMode;
        TCheckBox *RouterEn;
        TCheckBox *DhcpdEn;
        TCheckBox *MobileEn;
        TLabel *Label216;
        TEdit *apn;
        TLabel *Label217;
        TCSpinEdit *RouterFlow;
        TTimer *Timer2;
        TButton *Button40;
        TLabel *Label218;
        TCSpinEdit *AAC;
        TLabel *Label219;
        TComboBox *WBMode;
        TLabel *Label220;
        TCSpinEdit *TankH;
        TTabSheet *NVRSheet;
        TTabControl *TabControl8;
        TPanel *Panel1;
        TLabel *Label222;
        TLabel *Label223;
        TLabel *Label224;
        TLabel *Label225;
        TEdit *NvrIp;
        TEdit *NvrAcc;
        TEdit *NvrPwd;
        TLabel *Label221;
        TLabel *Label226;
        TComboBox *NvrStorage;
        TComboBox *NvrType;
        TCSpinEdit *NvrPort;
        TButton *Button42;
        TLabel *Label227;
        TCSpinEdit *DisplayChn;
        TLabel *Label228;
        TComboBox *VideoFlip;
        TButton *Button43;
        TLabel *Label229;
        TCSpinEdit *IpFlow;
        TTabSheet *TabSheet7;
        TButton *Button44;
        TCheckListBox *CheckListBox1;
        TLabel *Label230;
        TComboBox *LedType;
        TCheckBox *RouterRange;
        TLabel *Label231;
        TCSpinEdit *TimOffset;
        TLabel *Label56;
        TComboBox *TcpBufSize;
        TComboBox *CarType;
        TEdit *CarPlate;
        TEdit *Sim;
        TLabel *Label232;
        TLabel *Label57;
        TLabel *Label233;
        TComboBox *CarColor;
        TLabel *Label235;
        TLabel *Label234;
        TCSpinEdit *Zone;
        TLabel *Label236;
        TEdit *ServerId;
        TTabSheet *DdnsSheet;
        TGroupBox *GroupBox5;
        TLabel *Label75;
        TLabel *Label76;
        TLabel *Label77;
        TLabel *Label78;
        TComboBox *DdnsProtocol;
        TEdit *DdnsDomain;
        TEdit *DdnsUser;
        TEdit *DdnsPwd;
        TButton *Button19;
        TPageControl *PageControl5;
        TTabSheet *IRSheet;
        TCheckBox *IRStatus;
        TGroupBox *GroupBox10;
        TSpeedButton *IR2;
        TSpeedButton *IR3;
        TSpeedButton *IR4;
        TSpeedButton *IR1;
        TTabSheet *SwitchSheet;
        TButton *Button46;
        TLabel *Label237;
        TGroupBox *GroupBox19;
        TListView *SwitchDevList;
        TGroupBox *GroupBox20;
        TListView *SwitchChnList;
        TButton *Button47;
        TButton *Button45;
        TButton *Button48;
        TButton *Button51;
        TLabel *Label238;
        TCSpinEdit *Zones;
        TLabel *Label239;
        TComboBox *GnssType;
        TLabel *Label241;
        TCSpinEdit *GnssTime;
        TLabel *Label240;
        TComboBox *GnssMode;
        TLabel *Label242;
        TCSpinEdit *GnssDis;
        TCSpinEdit *GpsuploadIntval;
        TCSpinEdit *GpsAlarmSpeed;
        TCSpinEdit *DriveTireTime;
        TCSpinEdit *DriveRestTime;
        TEdit *Imei;
        TLabel *Label243;
        TLabel *Label244;
        TCSpinEdit *TankH_water;
        TLabel *Label245;
        TComboBox *DisplayMode;        
		TComboBox *IPVer;
        TLabel *Label246;
        TLabel *Label247;
        TComboBox *MobileAuth;
        TComboBox *cbox_rollovertestsen;
        TComboBox *cbox_collsitiontestsen;
        TTabSheet *tbsheetflogctrl;
        TCheckBox *cbox_flogcltlauto;
        TButton *btn_flogctrl;
        TGroupBox *GroupBox21;
        TLabel *Label254;
        TLabel *Label255;
        TLabel *Label256;
        TLabel *Label257;
        TLabel *Label258;
        TLabel *Label259;
        TEdit *et_pm10maxval;
        TEdit *et_timeout1;
        TEdit *et_pm10minval;
        TEdit *et_timeout2;
        TGroupBox *GroupBox22;
        TGroupBox *GroupBox23;
        TCheckBox *cbox_flog1;
        TRadioButton *rbtn_flogStart1;
        TRadioButton *rbtn_flogStop1;
        TGroupBox *GroupBox24;
        TCheckBox *cbox_flog2;
        TRadioButton *rbtn_flogStart2;
        TRadioButton *rbtn_flogStop2;
        TGroupBox *GroupBox25;
        TCheckBox *cbox_flog3;
        TRadioButton *rbtn_flogStart3;
        TRadioButton *rbtn_flogStop3;
        TGroupBox *GroupBox26;
        TCheckBox *cbox_flog4;
        TRadioButton *rbtn_flogStart4;
        TRadioButton *rbtn_flogStop4;
        TGroupBox *GroupBox27;
        TCheckBox *cbox_flog5;
        TRadioButton *rbtn_flogStart5;
        TRadioButton *rbtn_flogStop5;
        TGroupBox *GroupBox28;
        TCheckBox *cbox_flog6;
        TRadioButton *rbtn_flogStart6;
        TRadioButton *rbtn_flogStop6;
        TGroupBox *GroupBox29;
        TCheckBox *cbox_flog7;
        TRadioButton *rbtn_flogStart7;
        TRadioButton *rbtn_flogStop7;
        TGroupBox *GroupBox30;
        TCheckBox *CheckBox8;
        TRadioButton *RadioButton15;
        TRadioButton *RadioButton16;
        TGroupBox *GroupBox31;
        TCheckBox *cbox_flog8;
        TRadioButton *rbtn_flogStart8;
        TRadioButton *rbtn_flogStop8;
        TGroupBox *GroupBox32;
        TCheckBox *cbox_flog9;
        TRadioButton *rbtn_flogStart9;
        TRadioButton *rbtn_flogStop9;
        TGroupBox *GroupBox33;
        TCheckBox *cbox_flog10;
        TRadioButton *rbtn_flogStart10;
        TRadioButton *rbtn_flogStop10;
        TGroupBox *GroupBox34;
        TCheckBox *cbox_flog11;
        TRadioButton *rbtn_flogStart11;
        TRadioButton *rbtn_flogStop11;
        TGroupBox *GroupBox35;
        TCheckBox *cbox_flog12;
        TRadioButton *rbtn_flogStart12;
        TRadioButton *rbtn_flogStop12;
        TGroupBox *GroupBox36;
        TCheckBox *cbox_flog13;
        TRadioButton *rbtn_flogStart13;
        TRadioButton *rbtn_flogStop13;
        TGroupBox *GroupBox37;
        TCheckBox *cbox_flog14;
        TRadioButton *rbtn_flogStart14;
        TRadioButton *rbtn_flogStop14;
        TGroupBox *GroupBox38;
        TCheckBox *cbox_flog15;
        TRadioButton *rbtn_flogStart15;
        TRadioButton *rbtn_flogStop15;
        TGroupBox *GroupBox39;
        TCheckBox *cbox_flog16;
        TRadioButton *rbtn_flogStart16;
        TRadioButton *rbtn_flogStop16;
        TGroupBox *GroupBox40;
        TCheckBox *cbox_flog17;
        TRadioButton *rbtn_flogStart17;
        TRadioButton *rbtn_flogStop17;
        TGroupBox *GroupBox41;
        TCheckBox *cbox_flog18;
        TRadioButton *rbtn_flogStart18;
        TRadioButton *rbtn_flogStop18;
        TGroupBox *GroupBox42;
        TCheckBox *cbox_flog19;
        TRadioButton *rbtn_flogStart19;
        TRadioButton *rbtn_flogStop19;
        TGroupBox *GroupBox43;
        TCheckBox *cbox_flog20;
        TRadioButton *rbtn_flogStart20;
        TRadioButton *rbtn_flogStop20;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall PageControl1Change(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall ButtShowVidClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Button12Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button14Click(TObject *Sender);
        void __fastcall Button15Click(TObject *Sender);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall Button17Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button18Click(TObject *Sender);
        void __fastcall TabControl1Change(TObject *Sender);
        void __fastcall TabControl2Change(TObject *Sender);
    void __fastcall Button7Click(TObject *Sender);
    void __fastcall LogIpSelect(TObject *Sender);
    void __fastcall Button19Click(TObject *Sender);
    void __fastcall Button20Click(TObject *Sender);
    void __fastcall TabControl3Change(TObject *Sender);
        void __fastcall Button21Click(TObject *Sender);
        void __fastcall Button23Click(TObject *Sender);
        void __fastcall TabControl4Change(TObject *Sender);
        void __fastcall TabControl4Changing(TObject *Sender,
          bool &AllowChange);
        void __fastcall TabControl5Change(TObject *Sender);
        void __fastcall Button24Click(TObject *Sender);
        void __fastcall Button25Click(TObject *Sender);
        void __fastcall PageControl3Change(TObject *Sender);
        void __fastcall TabControl6Change(TObject *Sender);
        void __fastcall TabControl7Change(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall Button27Click(TObject *Sender);
        void __fastcall PrintDevInfo(TObject *Sender);
        void __fastcall CreateNick(TObject *Sender);
        void __fastcall OSDColorClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button30Click(TObject *Sender);
        void __fastcall WifiSsidChange(TObject *Sender);
        void __fastcall Button33Click(TObject *Sender);
        void __fastcall SetPrinter(TObject *Sender);
        void __fastcall StorageSchPageControlChange(TObject *Sender);
        void __fastcall Button41Click(TObject *Sender);
        void __fastcall Button31Click(TObject *Sender);
        void __fastcall ProductSrvSelect(TObject *Sender);
        void __fastcall Button28Click(TObject *Sender);
        void __fastcall Button29Click(TObject *Sender);
        void __fastcall Button32Click(TObject *Sender);
        void __fastcall LogIpClick(TObject *Sender);
        void __fastcall Button35Click(TObject *Sender);
        void __fastcall Button36Click(TObject *Sender);
        void __fastcall Button37Click(TObject *Sender);
        void __fastcall Button39Click(TObject *Sender);
        void __fastcall Button38Click(TObject *Sender);
        void __fastcall ProtocolChange(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall Button40Click(TObject *Sender);
        void __fastcall Button42Click(TObject *Sender);
        void __fastcall TabControl8Change(TObject *Sender);
        void __fastcall Button43Click(TObject *Sender);
        void __fastcall PageControl4Change(TObject *Sender);
        void __fastcall Button44Click(TObject *Sender);
        void __fastcall IR1Click(TObject *Sender);
        void __fastcall Button45Click(TObject *Sender);
        void __fastcall Button47Click(TObject *Sender);
        void __fastcall Button48Click(TObject *Sender);
        void __fastcall Button51Click(TObject *Sender);
        void __fastcall PageControl5Change(TObject *Sender);
        void __fastcall Button46Click(TObject *Sender);
        void __fastcall btn_flogctrlClick(TObject *Sender);
        void __fastcall cbox_flogcltlautoClick(TObject *Sender);
private:	// User declarations
        UINT32 SrvIP;
        int SrvPort;
        TDisplayForm *Display;  
        TPrintForm *PrinterSet;
        CDataClient *DataClient;
        int FileIndex;
        VIDEO_CHANNEL_PARA *VideoPara;
        OSD_PARA *OsdPara;
        EX_OSD_PARA *ExOsdPara;
        VIDEO_ALARM *AlarmPara;
        GPIN_ALARM_PARA *GpinPara;
        MNG_NVR *NvrPara;
        //SWITCH_OUT_PARA *SwitchPara;
        EX_GPIN_ALARM_PARA *ExGpinPara;
        EX_GPOUT_ALARM_PARA *ExSwitchPara;
        STORAGE_TEMP_PARA *StoragePara;
        TDatePlan *NormalStoragePlan;
        TDatePlan *MdStoragePlan;
        TDatePlan *GpinStoragePlan;
        TDatePlan *MotionAlarmPlan;
        TDatePlan *GpinAlarmPlan;
        TDatePlan *HideAlarmPlan;
        PRODUCT_INFO_ALL m_ProductInfo;
        CUdpSocket *udp;

        TCursor Save_Cursor;

        TList *Devices;
        void createIdFile(char *id, char *nickName, char *pwd);
        void __fastcall WndProc(Messages::TMessage& Message);
        void DownloadFile();
        void SetVideoPara(int index);
        void SetVideoSubPara(int index);
        void SetExVideoPara(int index);
        void SetOsdPara(int index);
        void SetExOsdPara(int index);
        void SetAlarmPara(int index);    
        void SetNvrPara(int index);
        void SetTimePara(void *Para);
        //void SetSysInfoPara(void *Para);
        void SetDiskInfoPara(void *Para);
        void SetGpinPara(int index);
        void SetPlatformPara(void *Para);
        void SetAdvancedPara(void *Para);
        void SetDVRPara(void *Para);
        void SetDevIdPara(void *Para);
        void SetStoragePara(int Index, int Type);
        int QueryDevice();
        int SetDeviceIp(DEVICE_ADDRESS *da);
        void ShowPage(int mode);
        int nickNameCreate(unsigned char lastmac[3], char *nickName);
        void SaveProductInfo();
        int ReadProductInfo();
        int ReadServerInfo();
        void SwitchChnBuild(SWITCH_CHN *p, TListItem *item);
        void SwitchDevBuild(SWITCH_DEV *p, TListItem *item);
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
        void SetPara(int type, void *Para);
        void StartVideo(long addr, int port, char *callId);
        int ControlTalk();
        int StartTalk(long addr, int port, char *CallId); 
	void StopTalk(int Mode = 1);
        void AddInfo(int n, AnsiString Info);
        void OnAccList(MNG_ACC *acc, int num);
        int flogSel();
        int GetFlogSt();
        void setParmToFlogCtrl(DEV_ENV_SET env);
        void setFlogBtnEnable(bool bst);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
