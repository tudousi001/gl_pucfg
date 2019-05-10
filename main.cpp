//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "srv.h"
#include "TalkClt.h"
#include "fileopt.h"    
#include "md5.h"
#include "printer.h"
#include "IpSet.h"
#include "acc.h"      
#include "switch.h"  
#include "SwitchDev.h"
//#include "TSCLib.h"
#include "TSCprinter.h"
#include <time.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "pies"
#pragma link "CGAUGES"
#pragma link "CSPIN"
#pragma link "WinSkinData"
#pragma link "WinSkinData"
#pragma link "WinSkinData"
#pragma resource "*.dfm"
TMainForm *MainForm;

#define QUERY_MNG_PORT    7778
#define QUERY_MNG_ADDR    "238.238.238.238"
#define QUERY_REPLAY_ADDR  "238.238.238.239"


CMClient *Client = 0;
CTalkClt *g_Talker = 0;

MNG_WIFI_NETs g_WifiNet;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
    Client = 0;
    Display = NULL;
    DataClient = NULL;
    Devices = NULL;
    VideoPara = 0;
    OsdPara = 0;
    ExOsdPara = 0;
    AlarmPara = 0;
    GpinPara = 0;
    NvrPara = 0;
    //SwitchPara = 0;
    ExGpinPara = 0;
    ExSwitchPara = 0;
    StoragePara = 0;
    NormalStoragePlan = 0;
    MdStoragePlan = 0;
    GpinStoragePlan = 0;
    MotionAlarmPlan = 0;
    GpinAlarmPlan = 0;
    HideAlarmPlan = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    if(udp)
    {
        delete udp;
        udp = 0;
    }
    if(Client)
    {
        delete Client;
        Client = 0;
    }
    if(DataClient)
    {
        delete DataClient;
        DataClient = NULL;
    }
    if(Display)
    {
        Display->Close();
        delete Display;
        Display = 0;
    }
    for(int i = 0; i < Devices->Count; i++)
    {
        delete (DEVICE_ADDRESS *)Devices->Items[i];
    }
    if(NormalStoragePlan)
        delete NormalStoragePlan;
    if(MdStoragePlan)
        delete MdStoragePlan;
    if(GpinStoragePlan)
        delete GpinStoragePlan;
    if(MotionAlarmPlan)
        delete MotionAlarmPlan;
    if(GpinAlarmPlan)
        delete GpinAlarmPlan;
    if(HideAlarmPlan)
        delete HideAlarmPlan;
    delete Devices;
}
//---------------------------------------------------------------------------

void ip2ip(UINT32 ip, char *ipstring)
{
    union
    {
        UINT32 ip;
        UINT8 i[4];
    }t;
    t.ip = ip;
    sprintf(ipstring, "%d.%d.%d.%d", t.i[0], t.i[1], t.i[2], t.i[3]);
}

int time2str(UINT16 startTime, UINT16 stopTime, char *out)
{
    if(out)
    {
        sprintf(out, "%02d%02d-%02d%02d", startTime/60, startTime%60, stopTime/60, stopTime%60);
        return 0;
    }
    return -1;
}

char char2num(char c)
{
    if((c < '0')||(c > '9'))
        return -1;

    return c-'0';
}

int str2time(char *str)
{
    int x, x1, x2;
    UINT8 *p = str;
    
    if(!p)
        return -1;

    x1 = char2num(*p);
    if(x1 < 0)
        return -1;
    p++;
    x = char2num(*p);
    if(x < 0)
        return -1;

    x1 = x1*10 + x;
    if(x1 > 24)
        return -1;

    p++;
    x2 = char2num(*p);
    if(x2 < 0)
        return -1;
    p++;
    x = char2num(*p);
    if(x < 0)
        return -1;

    x2 = x2*10 + x;   
    if(x > 60)
        return -1;

    return x1*60+x2;
}
int scheduleExSetHour(_UINT8 weekDay, _UINT8 startHour, _UINT8 stopHour, STORAGE_SCHEDULE_EX *scheduleEx)
{
    int i = 0;
    
    if(weekDay)
    {
        for(i=startHour; i<stopHour; i++)
        {
            BIT_SET(scheduleEx->daySch[weekDay-1], 1<<i);
        }
    }
    return OK;
}

int scheduleExPutChar(STORAGE_SCHEDULE_EX *scheduleEx, char *scheduleChar)
{
    int i, j;
    if(!scheduleChar || !scheduleEx)
        return ERROR;
    
    for(i=0; i<7; i++)
    {
        for(j=0; j<24; j++)
        {
            if(HAS_BIT(scheduleEx->daySch[i], 1<<j))
                scheduleChar[i*24+j] = '1';
            else
                scheduleChar[i*24+j] = '0';
        }
    }
    return OK;
}

/*scheduleChar[168] = ""*/
int schExGetChar(char *scheduleChar, _UINT32 daySch[])
{
    int i, j=0;
    if(!scheduleChar)
        return ERROR;
    
    for(i=0; i<7; i++)
    {
        //DBG_INFO(("scheduleChar=%s\n", &scheduleChar[i*24+j]));
        for(j=0; j<24; j++)
        {
            if(scheduleChar[i*24+j] == '1')
                BIT_SET(daySch[i], 1<<j);
            else
                BIT_CLR(daySch[i], 1<<j);
        }
        //DBG_INFO(("scheduleEx->daysch[%d]=%x\n", i, daySch[i]));
    }
    return OK;
}


int schExPutChar(_UINT32 daySch[], char *scheduleChar)
{
    int i, j;
    if(!scheduleChar)
        return ERROR;
    
    for(i=0; i<7; i++)
    {
        for(j=0; j<24; j++)
        {
            if(HAS_BIT(daySch[i], 1<<j))
                scheduleChar[i*24+j] = '1';
            else
                scheduleChar[i*24+j] = '0';
        }
    }
    return OK;
}
/*scheduleChar[168] = ""*/
int scheduleExGetChar(char *scheduleChar, int recordType, STORAGE_SCHEDULE_EX *scheduleEx)
{
    int i, j=0;
    if(!scheduleChar || !scheduleEx || recordType >= 3)
        return ERROR;
    
    scheduleEx->recordType = recordType;
    for(i=0; i<7; i++)
    {
        //DBG_INFO(("scheduleChar=%s\n", &scheduleChar[i*24+j]));
        for(j=0; j<24; j++)
        {
            if(scheduleChar[i*24+j] == '1')
                BIT_SET(scheduleEx->daySch[i], 1<<j);
            else
                BIT_CLR(scheduleEx->daySch[i], 1<<j);
        }
        //DBG_INFO(("scheduleEx->daysch[%d]=%x\n", i, scheduleEx->daySch[i]));
    }
    return OK;
}

void __fastcall TMainForm::WndProc(Messages::TMessage& Message)
{
    int i, n;
    char buf[32];

    try
    {
    if(Message.Msg == WM_CLIENT_DOWN)
    {
        if(DataClient)
        {
            delete DataClient;
            DataClient = NULL;
        }
        if(Client)
        {
            delete Client;
            Client = 0;
            //ShowMessage("连接断开");
        }
        if(Display)
        {
        //Display->Close();
        delete Display;
        Display = 0;
        }
        ShowPage(0);
        Label29->Caption = "";
        CGauge1->Progress = 0;
        return;
    }
    else         
    if(Message.Msg == WM_MNG_VERSION)
    {
        char *p = (char *)Message.WParam;
        Ver->Text = p;
    }
    else
    if(Message.Msg == WM_MNG_BASE_INFO)
    {
        MNG_BASIC_INFO *p = (MNG_BASIC_INFO *)Message.WParam;
        Name->Text = p->name;
        Id->Text = p->id;
        DomainId->Text = p->domainId;
        PWD->Text = p->pwd;
        ip2ip(p->ip, buf);
        IP->Text = buf;
        ip2ip(p->mask, buf);
        IPMask->Text = buf;
        ip2ip(p->gataway, buf);
        Gataway->Text = buf;
        ip2ip(p->dns[0], buf);
        DNS1->Text = buf;
        ip2ip(p->dns[1], buf);
        DNS2->Text = buf;
        MngPort->Value = (p->mngPort);
        SipPort->Value = (p->sipPort);
        if(p->regSrv[0])
        {
            RegIp->Text = p->regSrv;
        }
        else
        {
            ip2ip(p->srvIp, buf);
            RegIp->Text = buf;
        }
        RegPort->Value = (p->srvPort);
        PPPoEEn->Checked = p->pppoeEn;
        PPPoEName->Text = p->pppoeName;
        PPPoEPWD->Text = p->pppoePwd;
        ip2ip(p->pppIp, buf);
        PPPIp->Text = buf;
        ip2ip(p->sipSrvIp, buf);
        SipSrvIp->Text = buf;
        SipSrvPort->Value = (p->sipSrvPort);
        sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
                     p->mac[0], p->mac[1], p->mac[2],
                     p->mac[3], p->mac[4], p->mac[5]);
        MAC->Text = buf;
        HWSt->Text = IntToHex((int)p->hwst, 8);
        ip2ip(p->multicastAddr, buf);
        MulticastAddr->Text = buf;
        MulticastPort->Value = (p->multicastPort);
        HttpPeriod->Value = p->httpHeartPeriod; 
        HttpHeartEn->Checked = p->httpHeartEn;
        SipPeriod->Value = p->sipHeartPeriod;
        SipHeartEn->Checked = p->sipHeartEn;
        ip2ip(p->alarmSrvAddr, buf);
        AlarmSrvAddr->Text = buf;
        AlarmSrvPort->Value = p->alarmSrvPort;
        LinkMode->ItemIndex = p->linkMode;
        if(p->protocol == DVS_PROTOCOL_NONE)
            Protocol->Text = "None";
        else
            Protocol->ItemIndex = p->protocol;
        PortBase->Value = p->portBase;
        PortRange->Value = p->portRange;
        ButtShowVid->Enabled = (p->hwst&0x2)?0:1;
        ShowPage(1);
        return;
    }
    else
    if(Message.Msg == WM_MNG_ADVANCED_INFO)
    {
        MNG_SYS_ADVANCED_INFO *p = (MNG_SYS_ADVANCED_INFO *)Message.WParam;
        Button25->Enabled = true;
        SetAdvancedPara(p);
        return;
    }
    else
    if(Message.Msg == WM_STORAGE_NEW_INFO)
    {
        STORAGE_TEMP_PARA *p = (STORAGE_TEMP_PARA *)Message.WParam;
        StoragePara = p;
        n = Message.LParam;
        TabControl6->Tabs->Clear();
        for(i = 1; i <= n; i++)
        {
            sprintf(buf, "通道%d", i);
            TabControl6->Tabs->Add(buf);
        }
        //PageControl3->ActivePage = NewStorageSheet;
        BitBtn1->Enabled = true;
        SetStoragePara(0, 0);
        return;
    }
    else
    if(Message.Msg == WM_MNG_FORMAT_PROGRESS)
    {
        char temp[128];
        if(Message.LParam > 0)
        {
        int has, all;
        char *progrees = (char *)Message.WParam, *pos;
        has = atoi(progrees);
        pos = strstr(progrees, "/");
        all = atoi(pos+1);
        //formatPb
        //formatPb->Position
        if(all - has <= 2)
            formatInfo->Text = "正在产生日志文件...";
        else
        {
            sprintf(temp, "正在格式化(%s)...", progrees);
            formatInfo->Text = temp;
        }
        formatPb->Max = all;
        formatPb->Position = has;
        }
        else
        if(Message.LParam < 0)       
        {
            Timer2->Enabled = 0;
            Button41->Enabled = TRUE;
        }
        else
        {
            formatPb->Position = Message.WParam ;
            if(Message.WParam == 100)
            {
                 Timer2->Enabled = 0;
                 Button41->Enabled = TRUE;
                 sprintf(temp, "格式化完成");
                formatInfo->Text = temp;
            }
            else         
            if((int)Message.WParam < 0)
            {
                Timer2->Enabled = 0;
                Button41->Enabled = TRUE;
                formatInfo->Text = "格式化失败";
            }
            else
            {
            sprintf(temp, "正在格式化(%d/100)...", Message.WParam);
            formatInfo->Text = temp;
            }
        }
        return;
    }
    else
    if(Message.Msg == WM_STORAGE_INFO)
    {
        STORAGE_TEMP_PARA *p = (STORAGE_TEMP_PARA *)Message.WParam;
        StoragePara = p;
        n = Message.LParam;
        TabControl7->Tabs->Clear();
        for(i = 1; i <= n; i++)
        {
            sprintf(buf, "通道%d", i);
            TabControl7->Tabs->Add(buf);
        }
        PageControl3->ActivePage = StorageSheet;
        BitBtn2->Enabled = true;
        SetStoragePara(0, 1);
        return;
    }
    else
    if(Message.Msg == WM_MNG_PLATFORM_INFO)
    {
        PLATFORM_INFO_S *p = (PLATFORM_INFO_S *)Message.WParam;
        SetPlatformPara(p);
        return;
    }
    else 
    if(Message.Msg == WM_MNG_DVR_INFO)
    {
        MNG_DVR_S *p = (MNG_DVR_S *)Message.WParam;
        SetDVRPara(p);
        Button27->Enabled = true;
        return;
    }
    else

    if(Message.Msg == WM_MNG_GET_DISK_INFO)
    {  
        MNG_SYS_INFO_S *p = (MNG_SYS_INFO_S *)Message.WParam;
        SetDiskInfoPara((void *)p);        
    }
    else
    if(Message.Msg == WM_MNG_DEV_ID_INFO)
    {
        MNG_DEV_ID *p = (MNG_DEV_ID *)Message.WParam;
        SetDevIdPara(p);
        //Button28->Enabled = true;
        //Button29->Enabled = true;
    }
    else
    if(Message.Msg == WM_MNG_VIDEO_INFO)
    {
        VIDEO_CHANNEL_PARA *p = (VIDEO_CHANNEL_PARA *)Message.WParam;
        VideoPara = p;
        n = Message.LParam;
        TabControl1->Tabs->Clear();
        for(i = 1; i <= n; i++)
        {
            sprintf(buf, "通道%d", i);
            TabControl1->Tabs->Add(buf);
        }
        PageControl2->ActivePage = TabSheet2;
        if(n)
            SetVideoPara(0);
        return;
    }
    else
    if(Message.Msg == WM_MNG_VIDEO_SUB_INFO)
    {
        SetVideoSubPara(TabControl1->TabIndex);
    }
    else
    if(Message.Msg == WM_MNG_GET_EX_VIDEO_PARA)
    {
        SetExVideoPara(TabControl1->TabIndex);
    }
    else
    if(Message.Msg == WM_MNG_OSD_INFO)
    {
        OSD_PARA *p = (OSD_PARA *)Message.WParam;
        OsdPara = p;
        n = Message.LParam;
        TabControl2->Tabs->Clear();
        for(i = 1; i <= n; i++)
        {
            sprintf(buf, "通道%d", i);
            TabControl2->Tabs->Add(buf);
        }
        if(n)
            SetOsdPara(0);
        return;
    }
    else
    if(Message.Msg == WM_MNG_OSD_EX_INFO)
    {
        EX_OSD_PARA *p = (EX_OSD_PARA *)Message.WParam;
        ExOsdPara = p;
        n = Message.LParam;
        TabControl2->Tabs->Clear();
        if(n == 0)
        {
          ShowMessage("获取视频通道错误!");
          //PageControl2->ActivePage = TabSheet2;
          //TabSheet2->Enabled = false;
          return;  
        }
        for(i = 1; i <= n; i++)
        {
            sprintf(buf, "通道%d", i);
            TabControl2->Tabs->Add(buf);
        }
        if(n)
            SetExOsdPara(0);
        return;
    }
    else
    if(Message.Msg == WM_MNG_GET_NVR_PARA)
    {
        NvrPara = (MNG_NVR *)Message.WParam;
        n = Message.LParam;
        TabControl8->Tabs->Clear();
        for(i = 1; i <= n; i++)
        {
            sprintf(buf, "通道%d", i);
            TabControl8->Tabs->Add(buf);   
            SetNvrPara(0);
        }
        return;
    }
    else
    if(Message.Msg == WM_MNG_VIDEO_ALARM)
    {
        VIDEO_ALARM *p = (VIDEO_ALARM *)Message.WParam;
        AlarmPara = p;
        n = Message.LParam;
        TabControl3->Tabs->Clear();
        for(i = 1; i <= n; i++)
        {
            sprintf(buf, "通道%d", i);
            TabControl3->Tabs->Add(buf);
        }
        
        if(n)
            SetAlarmPara(0);
        /*VLoseEn->Checked = p->vLose.alarmEn;
        VLoseOut->ItemIndex = p->vLose.alarmOutChn;
        VLoseOff->Value = p->vLose.alarmOutSt;
        VMDEn->Checked = p->vMd.state;
        VMDOut->ItemIndex = p->vMd.alarmChn;
        VMDOff->Value = p->vMd.alarmState;
        time2str(p->vMd.startTime, p->vMd.stopTime, buf);
        MDTime->Text = buf;          */
        return;
    }
    else
    if(Message.Msg == WM_MNG_GPIN_ALARM)
    {
        GPIN_ALARM_PARA *p = (GPIN_ALARM_PARA *)Message.WParam;
        GpinPara = p;
        n = Message.LParam;
        TabControl4->Tabs->Clear();
        for(i = 1; i <= n; i++)
        {    
            sprintf(buf, "通道%d", i);
            TabControl4->Tabs->Add(buf);
        }
        
        if(n)
            SetGpinPara(0);
        /*
        Gpin1->Checked = p[0].alarmEn;
        Gpin2->Checked = p[1].alarmEn;
        Gpin3->Checked = p[2].alarmEn;
        Gpin4->Checked = p[3].alarmEn;
        GpinOut1->ItemIndex = p[0].alarmOutChn;
        GpinOut2->ItemIndex = p[1].alarmOutChn;
        GpinOut3->ItemIndex = p[2].alarmOutChn;
        GpinOut4->ItemIndex = p[3].alarmOutChn;
        GpinV1->Value = p[0].record[0].chn;
        GpinV2->Value = p[1].record[0].chn;
        GpinV3->Value = p[2].record[0].chn;
        GpinV4->Value = p[3].record[0].chn;
        GpinVP1->Value = p[0].record[0].preset;
        GpinVP2->Value = p[1].record[0].preset;
        GpinVP3->Value = p[2].record[0].preset;
        GpinVP4->Value = p[3].record[0].preset;
        GpinOff1->Value = p[0].alarmOutSt;
        GpinOff2->Value = p[1].alarmOutSt;
        GpinOff3->Value = p[2].alarmOutSt;
        GpinOff4->Value = p[3].alarmOutSt;   */
        return;
    }
    else   
    if(Message.Msg == WM_MNG_ALARM_BASIC)
    {
        ALARM_PARA *p = (ALARM_PARA *)Message.WParam;
        ShootTime->Value = p->shootTiming;
        Shoots->Value = p->shootTimes;
        PtzResetTime->Value = p->ptzResetTime/60;
        RecordTime->Value = p->alarmRecordTime;
        PreRecTime->Value = p->alarmPreRecordTime;
        Preset1->Value = p->ptzPreset[0];
        Preset2->Value = p->ptzPreset[1];
        RebootDays->Value = p->rebootDays;
        RebootHour->Value = p->rebootTime/60;
        RebootMin->Value = p->rebootTime%60;
        AAC->Value = p->aac;
        WBMode->ItemIndex = p->wbMode;
        AlarmVoice->ItemIndex = p->alarmVoiceMode;
        TankH->Value = p->TankH;
		TankH_water->Value = p->TankH_water;
        if(p->encEn & 0x80)
            EncEn->Checked = (p->encEn&0x7f);
        else
            EncEn->Enabled = 0;
        return;
    }
    else
    if((WM_MNG_SET_VIDEO_INFO == Message.Msg)
       ||(WM_MNG_SET_BASE_OK == Message.Msg))
    {
        ShowMessage("设置成功");
    }
    else
    if(WM_MNG_FORMAT_DISK == Message.Msg)
    {                    
        Button41->Enabled = TRUE;
        if(Message.WParam == 0)
        {
            formatInfo->Text = "格式化完毕";
            if(MessageBox(Handle, "格式化成功,是否重启设备(重启生效)", "请确认", MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDOK)
            {
                Client->SendReboot();
            }
        }
        else
            ShowMessage("格式化失败");
        return;
    }  
    else
    if(WM_MNG_REBOOTING == Message.Msg)
    {
        ShowPage(0);
        if(Display)
        {
        Display->Close();
        delete Display;
        Display = 0;
        }
        ShowMessage("正在重启");
        return;
    }
    else
    if(WM_MNG_UPLOAD_ERROR == Message.Msg)
    {
        switch(Message.WParam)
        {
            case 0:
                ShowMessage("文件不存在");
                break;
            default:
                ShowMessage("下载错误");
                break;
        }
        return;
    }
    else
    if(WM_MNG_DOWNLOAD_POS == Message.Msg)
    {
        if(Message.LParam)
        {
            CGauge1->MaxValue = Message.LParam;
            CGauge1->Progress = 0;    
            Label29->Caption = "传输数据...";
        }
        else
        {
            CGauge1->Progress = Message.WParam;
            if(CGauge1->Progress >= CGauge1->MaxValue)
            {
                Label29->Caption = "保存文件...";
                CGauge1->Progress = 0;
                Timer1->Interval = CGauge1->MaxValue/1000;
                Timer1->Enabled = 1;
            }
        }
        return;
    }
    else
    if(WM_MNG_DOWNLOAD_OVER == Message.Msg)
    {
        Timer1->Enabled = 0;
        CGauge1->Progress = CGauge1->MaxValue;
        if(Message.WParam)
        {
            Label29->Caption = "保存失败";    
            Result->Lines->Add(Label29->Caption);
        }
        else
        {
            Label29->Caption = "升级成功";
            Result->Lines->Add(Label29->Caption);
            DownloadFile();
        }
        return;
    }
    else
    if(WM_MNG_DOWNLOAD_ERROR == Message.Msg)
    {
        if(Message.WParam)
        {
            ShowMessage("升级失败");
        }
        return;
    }
    else
    if(Message.Msg == WM_MNG_DISPLAY_END)
    {
        StopTalk();
        if(Client)
            Client->SendVideoStop(Message.WParam);
        if(DataClient)
        {
            delete DataClient;
            DataClient = NULL;
        }
        if(Display)
        {
            delete Display;
            Display = 0;
        }
        return;
    }
    else
    if(Message.Msg == WM_MNG_DISPLAY_START)
    {
        return;
    }
    else
    if(Message.Msg == WM_MNG_UNSUPPORT)
    {
        if((Message.WParam & 0xf000)!= MNG_CMD_GET_BASE_INFO)
        {
            if(Message.WParam == MNG_CMD_REGISTER)  
                ShowMessage("用户名或密码错误");
            else
                ShowMessage("不支持该命令");
            if(Message.WParam == MNG_CMD_VIDEO_REQUEST)
            {
                if(Display)
                {
                    Display->Close();
                    delete Display;
                    Display = 0;
                }
            }
        }
        return;
    }
    else
    if(Message.Msg == WM_MNG_CMD_GET_ENV)
    {
        DEV_ENV_SET *p = (DEV_ENV_SET *)Message.WParam;
        n = Message.LParam;
        if(n > CheckListBox1->Items->Count)
            n = CheckListBox1->Items->Count;
        Zones->Value = p[0].chnNum;
		for(i = 0; i < n; i++)
			CheckListBox1->Checked[i] = 0;
        for(i = 0; i < n; i++)
        {
            if(p[i].type)
            {
                 CheckListBox1->Checked[p[i].type-1] = 1;
                 if(p[i].type == 7)
                 {
                     setParmToFlogCtrl(p[i]);
                 }
            }
            //else    
             //    CheckListBox1->Checked[i] = 0;
        }
    }
    else         
    if(Message.Msg == WM_MNG_CMD_GET_SWITCH)
    {
        TListItem *item;
        char *sdt[] = SWITCH_DEV_TYPE_STRs;
        char *st[] = SWITCH_TYPE_STRs;
        char *mt[] = SWITCH_MODE_STRs;
        SWITCH_INFO *p = (SWITCH_INFO *)Message.WParam;
        SwitchChnList->Clear();
        SwitchDevList->Clear();     
        if(!p)
            return;
        for(i = 0; i < p->devNum; i++)
        {
             item = SwitchDevList->Items->Add();
             sprintf(buf, "%d", i+1);
             item->Caption = buf;
             item->SubItems->Add(sdt[p->devs[i].type]);
             sprintf(buf, "%d", p->devs[i].id);
             item->SubItems->Add(buf);
             sprintf(buf, "%d", p->devs[i].chnNum);
             item->SubItems->Add(buf);
             item->Data = (void *)(int)p->devs[i].type;
        }
        for(i = 0; i < p->chnNum; i++)
        {
            item = SwitchChnList->Items->Add();
            if(p->chns[i].type < e_SWITCH_TYPE_MAX)
                item->Caption = st[p->chns[i].type];
            else
                item->Caption = "未知";
             sprintf(buf, "%d", p->chns[i].chn+1);
             item->SubItems->Add(buf);
             item->SubItems->Add(mt[p->chns[i].mode]);
             sprintf(buf, "%d", p->chns[i].tim);
             item->SubItems->Add(buf);
             sprintf(buf, "%d", p->chns[i].sid+1);
             item->SubItems->Add(buf);
             sprintf(buf, "%d", p->chns[i].schn+1);
             item->SubItems->Add(buf);
             item->SubItems->Add(p->chns[i].limit);
             item->SubItems->Add(p->chns[i].delay);
             item->Data = (void *)(int)p->chns[i].type;
        }
    }
    else
    if(Message.Msg == WM_MNG_CMD_RET)
    {
        switch(Message.WParam)
        {
            case MNG_CMD_TALK_REQUEST:
            {
                StopTalk(0);
                ShowMessage("设备语音未开启或语音通道忙");
                break;
            }
            case MNG_CMD_REGISTER:
            {
                ShowMessage("用户名或密码错误");
                break;
            }
            default:
            {
                ShowMessage("设置失败");
                break;
            }
        }
        return;
    }
    }catch(...){}

    TForm::WndProc(Message);
}

int qp2qpLevel(int value)
{
    int i, v[5] = {25, 28, 31, 34, 39};
    
    i = 4;
    while(i > 0)
    {
        if(value > v[i-1])
            break;
        i--;
    }
    return i;
}

void TMainForm::SetNvrPara(int index)
{
    if(NvrPara)
    {
        MNG_NVR *p = NvrPara + index;
        NvrIp->Text = p->ip;
        NvrPort->Value = p->port;
        NvrAcc->Text = p->acc;
        NvrPwd->Text = p->pwd;
        NvrType->ItemIndex = p->type;
        NvrStorage->ItemIndex = p->stroage;
        Zone->Value = p->timeZone;
    }
}

void TMainForm::SetVideoPara(int index)
{
    if((index >= 0) && VideoPara)
    {
        VIDEO_CHANNEL_PARA *p = VideoPara+index;
        Norm->ItemIndex = p->norm;
        Codec->ItemIndex = p->codec;
        BitRate->Text = IntToStr(p->bitRate/1000);
        FrameRate->Text = IntToStr(p->frameRate);
        IFrame->Text = IntToStr(p->iFram);
        VideoSize->ItemIndex = p->size;
        cbQP->ItemIndex = qp2qpLevel(p->qp);
        BaudRate->Text = IntToStr(p->sio.baudRate);
        AudioCodec->ItemIndex = p->audCodec;
        AudioChn->ItemIndex = p->audChn;
        BitType->ItemIndex = p->vbr?1:0;
        CamType->Text = p->sio.protocol;
        PtzType->ItemIndex = p->sio.mode;
        CamAddr->Text = IntToStr(p->sio.addr);
        SioNum->Text = IntToStr(p->sio.port);
        Parity->ItemIndex = p->sio.parity;
        Databit->ItemIndex = p->sio.dataBit-5;
        Stopbit->ItemIndex = p->sio.stopBit-1;
        Flow->ItemIndex = p->sio.flowControl;
        VideoFlip->ItemIndex = p->flipMode;
        DisplayMode->ItemIndex = 0;
    }
    if(!Client->m_vSub)
    {
        SubCodec->ItemIndex = -1;
        SubBitRate->Text = "";
        SubFrameRate->Text = "";
        SubIFrame->Text = "";
        SubVideoSize->ItemIndex = -1;
        subsQP->ItemIndex = -1;
        SubBitType->ItemIndex = -1;
        TabSheet1->TabVisible = 0;
        return;
    }    
}

void TMainForm::SetVideoSubPara(int index)
{
    if(!Client)
        return;

    if(!Client->m_vSub)
    {
        SubCodec->ItemIndex = -1;
        SubBitRate->Text = "";
        SubFrameRate->Text = "";
        SubIFrame->Text = "";
        SubVideoSize->ItemIndex = -1;
        subsQP->ItemIndex = -1;
        SubBitType->ItemIndex = -1;
        TabSheet1->TabVisible = 0;
        return;
    }
    VIDEO_SUB_PARA *p = Client->m_vSub+index;
    SubCodec->ItemIndex = p->codec;
    SubBitRate->Text = IntToStr(p->bitRate/1000);
    SubFrameRate->Text = IntToStr(p->frameRate);
    SubIFrame->Text = IntToStr(p->iFram);
    SubVideoSize->ItemIndex = p->size;
    subsQP->ItemIndex = qp2qpLevel(p->qp);
    SubBitType->ItemIndex = p->vbr?1:0;
    TabSheet1->TabVisible = 1;
}

void TMainForm::SetExVideoPara(int index)
{
    if(!Client)
        return;
    if(!Client->m_vExVideo)
        return;
    EX_VIDEO_CHANNEL_PARA *p = Client->m_vExVideo+index;
    eVideoChnId->Text = p->chnId;
    eVideoChnName->Text = p->chnName;
}


void TMainForm::SetGpinPara(int index)
{
    if((index >= 0) && GpinPara)
    {
        GPIN_ALARM_PARA *p = GpinPara+index;
        Gpin1->ItemIndex = p->alarmEn;
        GpinOut1->ItemIndex = p->alarmOutChn;
        GpinV1->Value = p->record[0].chn;
        GpinVP1->Value = p->record[0].preset;
        GpinOff1->Value = p->alarmOutSt;
        GpinMode->ItemIndex = p->alarmInMode;
    }
}

void TMainForm::SetOsdPara(int index)
{
    if((index >= 0) && OsdPara)
    {
        OSD_PARA *p = OsdPara+index;
        TimeX->Value = p->timeX;
        TimeY->Value = p->timeY;
        ShowTime->Checked = p->timeEnable;
        ShowDate->Checked = p->weekEnable;
        Text->Text = p->text;
        ShowText->Checked = p->textEnable;
        TextX->Value = p->textX;
        TextY->Value = p->textY;
        OSDColor->Color = p->textClr;
    }
}
void TMainForm::SetExOsdPara(int index)
{
    if((index >= 0) && ExOsdPara)
    {
        EX_OSD_PARA *p = ExOsdPara+index;
        ExTextX->Value = p->textX;
        ExTextY->Value = p->textY;
        ShowTextEx->Checked = p->exOsdEn;
        ExText->Text = p->text;
    }
}

void TMainForm::SetAlarmPara(int index)
{
    char buf[20];
    if((index >= 0) && AlarmPara)
    {
        VIDEO_ALARM *p = AlarmPara+index;
        VLoseEn->Checked = p->vLose.alarmEn;
        VLoseOut->ItemIndex = p->vLose.alarmOutChn;
        VLoseOff->Value = p->vLose.alarmOutSt;
        VMDEn->Checked = p->vMd.state;
        VMDOut->ItemIndex = p->vMd.alarmChn;
        VMDOff->Value = p->vMd.alarmState;
        time2str(p->vMd.startTime, p->vMd.stopTime, buf);
        MDTime->Text = buf;
    }
}

void TMainForm::SetPlatformPara(void *Para)
{
    char buf[32];
    PLATFORM_INFO_S *p = (PLATFORM_INFO_S *)Para;
    if(p)
    {
        if(p->platformId >= 0 && p->platformId <= MAX_PLATFORM_NUM)
        {
            pptc->ItemIndex = -1;
            if(p->protocol == DVS_PROTOCOL_NONE)
                pptc->Text = "None";
            else
                pptc->ItemIndex = p->protocol;
            pid->Text = p->devId;
            pHttpPwd->Text = p->pwd;
            if(p->regSrv[0])
            {
                pRegIp->Text = p->regSrv;
            }
            else
            {
                ip2ip(p->srvIp, buf);
                pRegIp->Text = buf;
            }
            pRegPort->Value = p->srvPort;
            ip2ip(p->alarmSrvIp, buf);
            pAlarmIp->Text = buf;
            pAlarmPort->Value = p->alarmSrvPort;
            ip2ip(p->sipSrvIp, buf);
            pSipIp->Text = buf;
            pSipPort->Value = p->sipSrvPort;         
            ip2ip(p->sipSrvIp2, buf);
            pSipIp2->Text = buf;   
            pSipPort2->Value = p->sipSrvPort2;                
            pHttpPeriod->Value = p->httpHeartPeriod; 
            pHttpHeartEn->Checked = p->httpHeartEnabled;
            pSipPeriod->Value = p->sipHeartPeriod;
            pSipHeartEn->Checked = p->sipHeartEnabled;
            StorageEn->Checked = p->storageProtocol;
            dId->Text = p->domainId;
            ServerId->Text = p->serverId;
        }
    }
}

void TMainForm::SetAdvancedPara(void *Para)
{
    MNG_SYS_ADVANCED_INFO *p = (MNG_SYS_ADVANCED_INFO *)Para;
    if(p)
    {
        TcpBufSize->ItemIndex = p->tcpSbufLevel;
        TimOffset->Value = p->timOffset;
        Sim->Text = p->sim;
        CarPlate->Text = p->plate;
        CarColor->ItemIndex = p->carColor;
        CarType->ItemIndex = p->carType;
        GnssType->ItemIndex = p->gnssType;
        GnssMode->ItemIndex = p->gnssMode;
        GnssTime->Value = p->gnssTim;
        GnssDis->Value = p->gnssDis;
        GpsuploadIntval->Value = p->gpsUploadIntval;
        GpsAlarmSpeed->Value = p->alarmSpeed;
        DriveTireTime->Value = p->tireTime;
        DriveRestTime->Value = p->restTime;
        cbox_rollovertestsen->ItemIndex = p->rollOverSen - 1;
        cbox_collsitiontestsen->ItemIndex = p->collisionSen - 1;

    }
}

static char *DISK_NAME_Msg[3]=
{
    "硬盘",
    "USB硬盘",
    "SD卡",
};

/*
static char *DISK_STAT_Msg[5]=
{
    "未格式化",
    "未挂载",
    "无磁盘",
    "正常",
    "磁盘满"
};   */
static char *DISK_STAT_Msg[6]=
{
    "无磁盘",
    "正常",
    "磁盘满",
    "未格式化",
    "未挂载",
    "正在格式化"
};

void TMainForm::SetDiskInfoPara(void *Para)
{
   int i;
   TListItem *Item;
   char temp[128] = {0};
   MNG_SYS_INFO_S *sysInfo = (MNG_SYS_INFO_S *)Para;

   lDiskInfo->Clear();
   for(i=0; i<11; i++)
   {
      if(i < 8)
      {
          if(sysInfo->disk[i].diskState)
          {
              Item = lDiskInfo->Items->Add();
              sprintf(temp, "%s%d", DISK_NAME_Msg[0],i+1);
              Item->Caption = temp; //IntToStr(p->presets[i].index);
              Item->SubItems->Add(DISK_STAT_Msg[sysInfo->disk[i].diskState/*+2*/]); 
              sprintf(temp, "%0.2fG", (sysInfo->disk[i].total*1.0/(1024)));
              Item->SubItems->Add(temp);                
              sprintf(temp, "%0.2fG", (sysInfo->disk[i].free*1.0/(1024)));
              Item->SubItems->Add(temp);
          }
      }
      if(i >=8 && i<10)
      {
          if(sysInfo->disk[i].diskState)
          {             
              Item = lDiskInfo->Items->Add();
              sprintf(temp, "%s%d", DISK_NAME_Msg[1],i-8+1);
              Item->Caption = temp; //IntToStr(p->presets[i].index);
              Item->SubItems->Add(DISK_STAT_Msg[sysInfo->disk[i].diskState]); 
              sprintf(temp, "%0.2fG", (sysInfo->disk[i].total*1.0/(1024)));
              Item->SubItems->Add(temp);                
              sprintf(temp, "%0.2fG", (sysInfo->disk[i].free*1.0/(1024)));
              Item->SubItems->Add(temp);
          }           
      }
      if(i == 10)
      {
          if(sysInfo->disk[i].diskState)
          {                   
              Item = lDiskInfo->Items->Add();
              sprintf(temp, "%s%d", DISK_NAME_Msg[2],i-10+1);
              Item->Caption = temp; //IntToStr(p->presets[i].index);
              Item->SubItems->Add(DISK_STAT_Msg[sysInfo->disk[i].diskState+2]); 
              sprintf(temp, "%0.2fG", (sysInfo->disk[i].total*1.0/(1024)));
              Item->SubItems->Add(temp);                
              sprintf(temp, "%0.2fG", (sysInfo->disk[i].free*1.0/(1024)));
              Item->SubItems->Add(temp);
          }   
      }
   }                
}
void TMainForm::SetDVRPara(void *Para)
{
    MNG_DVR_S *p = (MNG_DVR_S *)Para;
    if(p)
    {
        CtrlMode->ItemIndex = p->ctrlMode;
        DisplayLocal->Checked = p->displayLocal;
        DisplayIcon->Checked = p->displayIcon;
        ScreenSame->Checked = p->screenSame;
        LoopMute->Checked = p->loopMute;
        IrDisable->Checked = p->irDisable;
        TipColor->Color = p->tipClr;
        ScreenBkColor->Color = p->screenBkClr;
        PktDropMode->ItemIndex = p->pktDropMode;
        ShowMtName->Checked = p->showMtName;
        Alpha->Value = p->alpha;
        voDev->ItemIndex = p->voDev;
        Language->ItemIndex = p->language;
        Skin->ItemIndex = p->skin;
        DisplayChn->Value = p->displayChn;
        LedType->ItemIndex = p->ledType;

        if(p->splitMod == -1)
        {
            splitMod->Enabled = false;
        }
        else
        {
            splitMod->Enabled = true;
            switch(p->splitMod)
            {
                case DISPLAY_MODE_1:
                    splitMod->ItemIndex = 0;
                    break;
                case DISPLAY_MODE_4:
                    splitMod->ItemIndex = 1;
                    break;
                case DISPLAY_MODE_6:
                    splitMod->ItemIndex = 2;
                    break;
                case DISPLAY_MODE_8:
                    splitMod->ItemIndex = 3;
                    break;
                case DISPLAY_MODE_9:
                    splitMod->ItemIndex = 4;
                    break;
                case DISPLAY_MODE_16:
                    splitMod->ItemIndex = 5;
                    break;
                default:
                    splitMod->ItemIndex = 6;
                    break;
            }
        }

        intfSync->ItemIndex = p->intfSync[2];
        intfSync1->ItemIndex = p->intfSync[0];
    }
}
 
void TMainForm::SetDevIdPara(void *Para)
{
#if 0
    char buf[32];
    MNG_DEV_ID *p = (MNG_DEV_ID *)Para;
    if(p)
    {
        DevId->Text = p->devId;
        sipAddr->Text = p->sipSrvAddr;
        CSpinEPort->Value = p->sipRegPort;
        sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
                     p->devMac[0], p->devMac[1], p->devMac[2],
                     p->devMac[3], p->devMac[4], p->devMac[5]);
       mac2->Text = buf;
       sprintf(buf, "0x%08x", p->devCap);
       devCap->Text = buf;
       //devPwd->Text = p->devPwd;
       Button29->Enabled = true;
       Button28->Enabled = true;
    }
#endif
}
void TMainForm::SetStoragePara(int Index, int Type)
{
    char detailSchedule[256] = {0};
    if(Index >= 0 && StoragePara)
    {
        if(Type == 0)
        {
            STORAGE_TEMP_PARA *p = (STORAGE_TEMP_PARA *)StoragePara+Index;
            recordEnable->Checked = p->basic.recordEnable;
            recycleRecEnable->Checked = p->basic.recycleRecEnable;
            filenameSuffix->Text = p->basic.filenameSuffix;
            splitTime->Value = p->basic.splitTime/60;
            storageStreamType->ItemIndex = p->streamType;
            cbRecycleMode->ItemIndex = p->recycleMode;
            
            if(StorageSchPageControl->ActivePage == NormalSchTabSheet)
            {
                scheduleExPutChar(&p->scheduleEx[0], detailSchedule);
                NormalStoragePlan->SetPlan(detailSchedule);  
                //NormalStoragePlan->OnPaint(this);
            }
            else
            if(StorageSchPageControl->ActivePage == MdSchTabSheet)
            {
                scheduleExPutChar(&p->scheduleEx[1], detailSchedule);
                MdStoragePlan->SetPlan(detailSchedule);  
                //MdStoragePlan->OnPaint(this);
            }
            else
            if(StorageSchPageControl->ActivePage == GpinSchTabSheet)
            {
                scheduleExPutChar(&p->scheduleEx[2], detailSchedule);
                GpinStoragePlan->SetPlan(detailSchedule);  
                //GpinStoragePlan->OnPaint(this);
            }
        }
        else if(Type == 1)
        {
            STORAGE_TEMP_PARA *p = (STORAGE_TEMP_PARA *)StoragePara+Index;
            recordEnable1->Checked = p->basic.recordEnable;
            recycleRecEnable1->Checked = p->basic.recycleRecEnable;
            filenameSuffix1->Text = p->basic.filenameSuffix;
            splitTime1->Value = p->basic.splitTime/60; 
            storageStreamType1->ItemIndex = p->streamType;

            weekDay1->ItemIndex = p->schedule[0].weekday;
            startHour1->Value = p->schedule[0].startHour;
            startMin1->Value = p->schedule[0].startMin;
            stopHour1->Value = p->schedule[0].stopHour;
            stopMin1->Value = p->schedule[0].stopMin;
            recordType1->ItemIndex = p->schedule[0].recordType;
			
            weekDay2->ItemIndex = p->schedule[1].weekday;
            startHour2->Value = p->schedule[1].startHour;
            startMin2->Value = p->schedule[1].startMin;
            stopHour2->Value = p->schedule[1].stopHour;
            stopMin2->Value = p->schedule[1].stopMin;
            recordType2->ItemIndex = p->schedule[1].recordType;
			
            weekDay3->ItemIndex = p->schedule[2].weekday;
            startHour3->Value = p->schedule[2].startHour;
            startMin3->Value = p->schedule[2].startMin;
            stopHour3->Value = p->schedule[2].stopHour;
            stopMin3->Value = p->schedule[2].stopMin;
            recordType3->ItemIndex = p->schedule[2].recordType;

            weekDay4->ItemIndex = p->schedule[3].weekday;
            startHour4->Value = p->schedule[3].startHour;
            startMin4->Value = p->schedule[3].startMin;
            stopHour4->Value = p->schedule[3].stopHour;
            stopMin4->Value = p->schedule[3].stopMin;
            recordType4->ItemIndex = p->schedule[3].recordType;

            weekDay5->ItemIndex = p->schedule[4].weekday;
            startHour5->Value = p->schedule[4].startHour;
            startMin5->Value = p->schedule[4].startMin;
            stopHour5->Value = p->schedule[4].stopHour;
            stopMin5->Value = p->schedule[4].stopMin;
            recordType5->ItemIndex = p->schedule[4].recordType;

            weekDay6->ItemIndex = p->schedule[5].weekday;
            startHour6->Value = p->schedule[5].startHour;
            startMin6->Value = p->schedule[5].startMin;
            stopHour6->Value = p->schedule[5].stopHour;
            stopMin6->Value = p->schedule[5].stopMin;
            recordType6->ItemIndex = p->schedule[5].recordType;

            weekDay7->ItemIndex = p->schedule[6].weekday;
            startHour7->Value = p->schedule[6].startHour;
            startMin7->Value = p->schedule[6].startMin;
            stopHour7->Value = p->schedule[6].stopHour;
            stopMin7->Value = p->schedule[6].stopMin;
            recordType7->ItemIndex = p->schedule[6].recordType;

            weekDay8->ItemIndex = p->schedule[7].weekday;
            startHour8->Value = p->schedule[7].startHour;
            startMin8->Value = p->schedule[7].startMin;
            stopHour8->Value = p->schedule[7].stopHour;
            stopMin8->Value = p->schedule[7].stopMin;
            recordType8->ItemIndex = p->schedule[7].recordType;
        }
    }
}

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
    MNG_BASIC_INFO p;
    int t, n;

    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    memset(&p, 0, sizeof(p));
    strcpy(p.name, Name->Text.c_str());
    strcpy(p.id, Id->Text.c_str());
    n = strlen(p.id)-1;
    while(n > 0)
    {
        if((p.id[n] == ' ')||(p.id[n] == '\r')||(p.id[n] == '\n'))
            p.id[n] = 0;
        n--;
    }
    strcpy(p.domainId, DomainId->Text.c_str());
    strcpy(p.pwd, PWD->Text.c_str());
    p.ip = inet_addr(IP->Text.c_str());
    p.mask = inet_addr(IPMask->Text.c_str());
    p.mngPort = MngPort->Value;
    p.sipPort = SipPort->Value;
    strcpy(p.regSrv, RegIp->Text.c_str());
    t = inet_addr(RegIp->Text.c_str());
    if(t == -1)
    {
        t = 0;
    }
    p.srvIp = t;
    p.srvPort = RegPort->Value;
    if(Gataway->Text.IsEmpty())
        p.gataway = 0;
    else
        p.gataway = inet_addr(Gataway->Text.c_str());
    if(p.gataway)
    {
        if((p.gataway&p.mask) != (p.ip&p.mask))
        {
            ShowMessage("网关非法");
            return;
        }
    }
    if(DNS1->Text.IsEmpty())
        p.dns[0] = 0;
    else
        p.dns[0] = inet_addr(DNS1->Text.c_str());
    if(DNS2->Text.IsEmpty())
        p.dns[1] = 0;
    else
        p.dns[1] = inet_addr(DNS2->Text.c_str());
    if(SipSrvIp->Text.IsEmpty())
        p.sipSrvIp = 0;
    else
        p.sipSrvIp = inet_addr(SipSrvIp->Text.c_str());
    p.sipSrvPort = SipSrvPort->Value;
    p.multicastAddr = inet_addr(MulticastAddr->Text.c_str());
    p.multicastPort = MulticastPort->Value;
    p.alarmSrvAddr = inet_addr(AlarmSrvAddr->Text.c_str());
    p.alarmSrvPort = AlarmSrvPort->Value;
    p.httpHeartPeriod = HttpPeriod->Value;
    p.sipHeartPeriod = SipPeriod->Value;
    p.linkMode = LinkMode->ItemIndex;
    if(Protocol->Text == "None")
        p.protocol = DVS_PROTOCOL_NONE;
    else
        p.protocol = Protocol->ItemIndex;
    p.portBase = PortBase->Value;
    p.portRange = PortRange->Value;
    p.httpHeartEn = HttpHeartEn->Checked;
    p.sipHeartEn = SipHeartEn->Checked;
    p.pppoeEn = PPPoEEn->Checked;
    strcpy(p.pppoeName, PPPoEName->Text.c_str());
    strcpy(p.pppoePwd, PPPoEPWD->Text.c_str());
    sscanf(MAC->Text.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X",
                     &p.mac[0], &p.mac[1], &p.mac[2],
                     &p.mac[3], &p.mac[4], &p.mac[5]);
    Client->SetBasic(&p);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button2Click(TObject *Sender)
{
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    Client->SendReboot();
}
//---------------------------------------------------------------------------

/*qp  : 0~25,26~28, 29~31, 32~34, 35~39;值越小越好*/

int qp2qpValue(int level)
{
    int v[5] = {25, 28, 31, 34, 39};
    
    if(level >= 4)
        return v[4];

    if(level < 0)
        return v[2];
    
    return v[level];
}
void __fastcall TMainForm::Button3Click(TObject *Sender)
{
    VIDEO_CHANNEL_PARA p = {0};
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }               
    EX_VIDEO_CHANNEL_PARA exVideo={""};
    strcpy(exVideo.chnId, eVideoChnId->Text.c_str());
    strcpy(exVideo.chnName, eVideoChnName->Text.c_str());
    Client->SetExVideoPara(TabControl1->TabIndex, &exVideo);
    
    p.norm = Norm->ItemIndex;
    //p.camType = CamType->ItemIndex;
    p.codec =    Codec->ItemIndex;
    p.bitRate =   StrToInt(BitRate->Text)*1000;
    p.frameRate =   StrToInt(FrameRate->Text);
    p.iFram =   StrToInt(IFrame->Text);
    p.size = VideoSize->ItemIndex;
        p.qp = qp2qpValue(cbQP->ItemIndex);
    p.vbr = BitType->ItemIndex;
    p.flipMode = VideoFlip->ItemIndex;

    //if(BaudRate->ItemIndex >= 0)
    p.sio.baudRate = StrToInt(BaudRate->Text);
    p.audCodec  = AudioCodec->ItemIndex;
    p.audChn = AudioChn->ItemIndex;
    p.sio.addr = StrToInt(CamAddr->Text);
    p.sio.port = StrToInt(SioNum->Text);
    strcpy(p.sio.protocol, CamType->Text.c_str());
    p.sio.mode = PtzType->ItemIndex;
    p.sio.parity = Parity->ItemIndex;
    p.sio.dataBit = Databit->ItemIndex+5;
    p.sio.stopBit = Stopbit->ItemIndex+1;
    p.sio.flowControl = Flow->ItemIndex;
    p.sio.displayMode  =  DisplayMode->ItemIndex;

    Client->SetVideo(TabControl1->TabIndex, &p);

    if(Client->m_vSub)
    {
        VIDEO_SUB_PARA pS;
        pS.bitRate = StrToInt(SubBitRate->Text)*1000;
        pS.iFram = StrToInt(SubIFrame->Text);
        pS.codec = SubCodec->ItemIndex;
        pS.size = SubVideoSize->ItemIndex;
        pS.frameRate = StrToInt(SubFrameRate->Text);
        pS.vbr = SubBitType->ItemIndex;
            pS.qp = qp2qpValue(subsQP->ItemIndex);
        Client->SetSubVideo(TabControl1->TabIndex, &pS);
    }
}
//---------------------------------------------------------------------------
void TMainForm::ShowPage(int mode)
{
    if(!mode)
    {
        BasicSheet->TabVisible = 0;
        AdvancedSheet->TabVisible = 0;
        VideoSheet->TabVisible = 0;
        OsdSheet->TabVisible = 0;
        AlarmSheet->TabVisible = 0;
        LogInfoSheet->TabVisible = 0;
        UpgradeSheet->TabVisible = 0;
        IOSheet->TabVisible = 0;
        DynamicSheet->TabVisible = 0;
        MulPlatFormSheet->TabVisible = 0;
        NVRSheet->TabVisible = 0;
    }
    else
    {
        BasicSheet->TabVisible = 1;
        AdvancedSheet->TabVisible = 1;
        VideoSheet->TabVisible = 1;
        OsdSheet->TabVisible = 1;
        AlarmSheet->TabVisible = 1;
        LogInfoSheet->TabVisible = 1;
        UpgradeSheet->TabVisible = 1;
        PageControl1->Visible = 1;
        IOSheet->TabVisible = 1;
        DynamicSheet->TabVisible = 1;
        MulPlatFormSheet->TabVisible = 1;
        NVRSheet->TabVisible = 1;
        PageControl1->ActivePage = BasicSheet;
    }                    
    PageControl5->ActivePage = IRSheet;
}
void __fastcall TMainForm::Button5Click(TObject *Sender)
{
    int ret;

    ShowPage(0);
    SrvIP = inet_addr(LogIp->Text.c_str());
    SrvPort = atoi(LogPort->Text.c_str());
    if((SrvIP != INADDR_NONE)&&(SrvPort))
    {
        if(Client)
            delete Client;
        Client = new CMClient();
        TCursor Save_Cursor = Screen->Cursor;
        Screen->Cursor = crHourGlass;
        ret = Client->Connect(SrvIP, SrvPort);
        Screen->Cursor = Save_Cursor;
        if(ret != CLIENT_OK)
        {
            ShowMessage("连接失败");
            delete Client;
            Client = NULL;
            return;
        }
        Client->SendRegister(LogName->Text.c_str(), LogPwd->Text.c_str());
        ReadProductInfo();
    }
    else
    {
         ShowMessage("Ip地址或端口号错误");
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PageControl1Change(TObject *Sender)
{
    if(!Client)
        return;

    if(PageControl1->ActivePage == BasicSheet)
    {
        Client->SendGetBaseInfo();
    }
    else if(PageControl1->ActivePage == AdvancedSheet)
    {
        if(!NormalStoragePlan)
            NormalStoragePlan = new TDatePlan(PaintBox1, 17, 16);
        NormalStoragePlan->Clear();
        if(!MdStoragePlan)
            MdStoragePlan = new TDatePlan(PaintBoxMd, 17, 16);
        NormalStoragePlan->Clear();              
        if(!GpinStoragePlan)
            GpinStoragePlan = new TDatePlan(PaintBoxGpin, 17, 16);
        NormalStoragePlan->Clear();
        
        BitBtn1->Enabled = false;

        PageControl3->ActivePage = StorageSheet;
        Client->SendGetStorageInfo(0);
    }
    else
    if(PageControl1->ActivePage == VideoSheet)
    {
        Client->SendGetVideoInfo();
    }
    else
    if(PageControl1->ActivePage == OsdSheet)
    {
        Client->SendGetOsdInfo();     
        Client->SendGetExOsdInfo();
    }
    else
    if(PageControl1->ActivePage == AlarmSheet)
    {
        Client->SendGetAlarmInfo();
        PageControl4->ActivePageIndex = 0;
    }
    /*else
    if(PageControl1->ActivePage == DdnsSheet)
    {
        Client->SendGetDdnsInfo();
    } */
    else
    if(PageControl1->ActivePage == DynamicSheet)
    {
        Client->SendGetDynamicNetInfo();
    }
    else
    if(PageControl1->ActivePage == MulPlatFormSheet)
    {
        char buf[32];
        TabControl5->Tabs->Clear();
  
        for(int i = 1; i <= MAX_PLATFORM_NUM; i++)
        {
            sprintf(buf, "平台%d", i);
            TabControl5->Tabs->Add(buf);
        }
        int Id = 1;
        Client->SendGetPlatFormInfo(Id);
    }   
    else
    if(PageControl1->ActivePage == NVRSheet)
    {        
        //TabControl8->Tabs->Clear();
        Client->SendGetNVRInfo();
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Button6Click(TObject *Sender)
{
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }

    Client->SendGetFile(Edit1->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtShowVidClick(TObject *Sender)
{
    int chn = TabControl1->TabIndex;

    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }

    if(Display)
    {
        return;
    }

    Display = new TDisplayForm(NULL);
    if(Display == NULL)
    {
        ShowMessage("创建播放窗口失败");
        return;
    }
    if(Display->Init(chn) < 0)
    {
        ShowMessage("初始化播放窗口失败");
        delete Display;
        Display = NULL;
        return;
    }                     
    Display->Show();
	
    if(PageControl2->ActivePage == TabSheet1)
        Client->SendVideoRequest(chn, STREAM_MINOR);
    else
        Client->SendVideoRequest(chn, STREAM_MAIN);
	
    Client->SendParaGetCmd(chn, MNG_CMD_GET_DISPLAY);   
    Client->SendParaGetCmd(chn, MNG_CMD_VOLUME_GET);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Button8Click(TObject *Sender)
{
    if(Client)
    {
        Client->SendTime();
    }
}

void __fastcall TMainForm::Button20Click(TObject *Sender)
{
    if(Client)
    {
        if(MessageBox(Handle, "确定擦除现有的配置?", "请确认", MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDOK)
            Client->SendErase();
    }
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::Button9Click(TObject *Sender)
{
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    if(OpenDialog1->Execute())
    {
        //Edit2->Text = OpenDialog1->FileName;
        FileIndex = 0;                  
        Files->Lines->Clear();
        Result->Lines->Clear();
        Files->Lines->AddStrings(OpenDialog1->Files);
        DownloadFile();
        //Client->StartDownload(OpenDialog1->FileName.c_str());
    }
}
//---------------------------------------------------------------------------
void TMainForm::DownloadFile()
{
    if((!Client) || (FileIndex >= Files->Lines->Count))
    {
        return;
    }

    Result->Lines->Add("文件:" + Files->Lines->Strings[FileIndex] + "...");
    Client->StartDownload(Files->Lines->Strings[FileIndex].c_str());
    FileIndex++;
}


void __fastcall TMainForm::Button10Click(TObject *Sender)
{
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    if(!Files->Lines->Count)
    {
        if(!OpenDialog1->Execute())
        {
            return;
        }
        Files->Lines->Clear();   
        Files->Lines->AddStrings(OpenDialog1->Files);
        //Edit2->Text = OpenDialog1->FileName;
    }
    FileIndex = 0;
    Result->Lines->Clear();
    DownloadFile();
    //Client->StartDownload(Edit2->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button12Click(TObject *Sender)
{
    OSD_PARA p;   
    EX_OSD_PARA pEx;
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    p.timeX = TimeX->Value;
    p.timeY = TimeY->Value;
    p.timeEnable = ShowTime->Checked;
    p.weekEnable = ShowDate->Checked;
    p.textX = TextX->Value;
    p.textY = TextY->Value;
    p.textEnable = ShowText->Checked;
    p.textClr = OSDColor->Color;
    strcpy(p.text, Text->Text.c_str());

    if(Client->SetOsd(TabControl2->TabIndex, &p) < 0)
        ShowMessage("不支持该命令");

    pEx.textX = ExTextX->Value;
    pEx.textY = ExTextY->Value;
    pEx.exOsdEn = ShowTextEx->Checked;
    strcpy(pEx.text, ExText->Text.c_str());
     
    if(Client->SetOsdEx(TabControl2->TabIndex, &pEx) < 0)
        ;//ShowMessage("不支持该命令");

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button13Click(TObject *Sender)
{
    char t[12];
    int n;
    VIDEO_ALARM p;

    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }

    p.vLose.alarmEn = VLoseEn->Checked;
    p.vLose.alarmOutChn = VLoseOut->ItemIndex;
    p.vLose.alarmOutSt = VLoseOff->Value;

    p.vMd.state = VMDEn->Checked;
    strcpy(t, MDTime->Text.c_str());
    n = str2time(t);
    if((n < 0)||(n > 1440))
    {
        ShowMessage("开始时间错误");
        return;
    }

    p.vMd.startTime = n;
    n = str2time(t+5);
    if((n < 0)||(n > 1440)/*||(n < p.vMd.startTime)*/)
    {
        ShowMessage("结束时间错误");
        return;
    }
    p.vMd.stopTime = n;
    p.vMd.alarmChn = VMDOut->ItemIndex;
    p.vMd.alarmState = VMDOff->Value;

    if(Client->SetVideoAlarm(TabControl3->TabIndex, &p) < 0)
        ShowMessage("错误");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button14Click(TObject *Sender)
{
    GPIN_ALARM_PARA st;

    if(!Client)
    {
        return;
    }
    if(Gpin1->ItemIndex < 0)
        Gpin1->ItemIndex = 0;
    st.alarmEn = Gpin1->ItemIndex;
    st.alarmOutChn = GpinOut1->ItemIndex;
    st.record[0].chn = GpinV1->Value;
    st.record[0].preset = GpinVP1->Value;
    st.alarmInMode = GpinMode->ItemIndex;
    st.alarmOutSt = GpinOff1->Value;
    if(Client->SetGpinAlarm(TabControl4->TabIndex, &st) < 0)
        ShowMessage("错误");
    /*GPIN_ALARM_PARA st[4];

    if(!Client)
    {
        return;
    }

    st[0].alarmEn = Gpin1->Checked;
    st[1].alarmEn = Gpin2->Checked;
    st[2].alarmEn = Gpin3->Checked;
    st[3].alarmEn = Gpin4->Checked;
    st[0].alarmOutChn = GpinOut1->ItemIndex;
    st[1].alarmOutChn = GpinOut2->ItemIndex;
    st[2].alarmOutChn = GpinOut3->ItemIndex;
    st[3].alarmOutChn = GpinOut4->ItemIndex;

    st[0].record[0].chn = GpinV1->Value;
    st[1].record[0].chn = GpinV2->Value;
    st[2].record[0].chn = GpinV3->Value;
    st[3].record[0].chn = GpinV4->Value;

    st[0].record[0].preset = GpinVP1->Value;
    st[1].record[0].preset = GpinVP2->Value;
    st[2].record[0].preset = GpinVP3->Value;
    st[3].record[0].preset = GpinVP4->Value;

    st[0].alarmOutSt = GpinOff1->Value;
    st[1].alarmOutSt = GpinOff2->Value;
    st[2].alarmOutSt = GpinOff3->Value;
    st[3].alarmOutSt = GpinOff4->Value;

    if(Client->SetGpinAlarm(4, st) < 0)
        ShowMessage("错误");            */
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button23Click(TObject *Sender)
{
    if(!Client->m_AlarmPara)
    {
        ShowMessage("设备不支持此项设置");
        return;
    }
    ALARM_PARA *p = Client->m_AlarmPara;

    p->shootTiming = ShootTime->Value;
    p->shootTimes = Shoots->Value;
    p->ptzResetTime = PtzResetTime->Value*60;
    p->alarmRecordTime = RecordTime->Value;
    p->ptzPreset[0] = Preset1->Value;
    p->ptzPreset[1] = Preset2->Value;
    p->rebootDays = RebootDays->Value;
    p->rebootTime = RebootHour->Value*60+RebootMin->Value;
    p->alarmPreRecordTime = PreRecTime->Value;
    p->encEn = EncEn->Checked;
    p->aac = AAC->Value;
    p->alarmVoiceMode = AlarmVoice->ItemIndex;
    p->wbMode = WBMode->ItemIndex;
    p->TankH = TankH->Value;
    p->TankH_water = TankH_water->Value;
    if(Client->SetAlarmBasic(Client->m_AlarmPara) < 0)
        ShowMessage("错误");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button19Click(TObject *Sender)
{
    DDNS_INFO ddns;
    if(!Client)
    {
        return;
    }
    ddns.protocol = DdnsProtocol->ItemIndex;
    strcpy(ddns.hostName, DdnsDomain->Text.c_str());
    strcpy(ddns.acc, DdnsUser->Text.c_str());
    strcpy(ddns.pwd, DdnsPwd->Text.c_str());

    if(Client->SetDdns(&ddns) < 0)
        ShowMessage("错误");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button15Click(TObject *Sender)
{
    if(!Client)
    {
        return;
    }

    Client->SendGetLog(0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button16Click(TObject *Sender)
{
    if(!Client)
    {
        return;
    }

    Client->SendGetLog(1);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button17Click(TObject *Sender)
{
    LogShow->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    udp = 0;
    PageControl1->ActivePage = LogSheet;
    PageControl5->ActivePage = IRSheet;
    BasicSheet->TabVisible = 0;
    AdvancedSheet->TabVisible = 0;
    VideoSheet->TabVisible = 0;
    OsdSheet->TabVisible = 0;
    AlarmSheet->TabVisible = 0;
    LogInfoSheet->TabVisible = 0;
    UpgradeSheet->TabVisible = 0;
    //DdnsSheet->TabVisible = 0;
    IOSheet->TabVisible = 0;
    DynamicSheet->TabVisible = 0;
    MulPlatFormSheet->TabVisible = 0;
    NVRSheet->TabVisible = 0;
    Devices = new TList;
    ReadProductInfo();
    ReadServerInfo();
/*#ifdef SUPPORT_PRINT
    ProductSheet->TabVisible = 1;
#else
    ProductSheet->TabVisible = 0;
#endif*/
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button18Click(TObject *Sender)
{
    if(SaveDialog1->Execute())
    {
        LogShow->Lines->SaveToFile(SaveDialog1->FileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TabControl1Change(TObject *Sender)
{
    SetVideoPara(TabControl1->TabIndex);  
    SetVideoSubPara(TabControl1->TabIndex);     
    SetExVideoPara(TabControl1->TabIndex);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TabControl2Change(TObject *Sender)
{
    SetOsdPara(TabControl2->TabIndex);
    SetExOsdPara(TabControl2->TabIndex);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TabControl3Change(TObject *Sender)
{
    SetAlarmPara(TabControl3->TabIndex);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TabControl4Change(TObject *Sender)
{
    SetGpinPara(TabControl4->TabIndex);
}

void __fastcall TMainForm::TabControl4Changing(TObject *Sender,
      bool &AllowChange)
{
    int i = TabControl4->TabIndex;
    if(Gpin1->ItemIndex < 0)
        Gpin1->ItemIndex = 0;
    GpinPara[i].alarmEn = Gpin1->ItemIndex;
    GpinPara[i].alarmOutChn = GpinOut1->ItemIndex;
    GpinPara[i].record[0].chn = GpinV1->Value;
    GpinPara[i].record[0].preset = GpinVP1->Value;
    GpinPara[i].alarmOutSt = GpinOff1->Value;
}
//---------------------------------------------------------------------------

int TMainForm::QueryDevice()
{
    struct sockaddr_in addr;
    int i, ret;
    char buf[128];

    if(!udp)
    {
        udp = new CUdpSocket();
        if(!udp)
            return -1;
        if(udp->Create() < 0)
        {
            delete udp;
            return 0;
        }
        udp->SetBroadcast(1);
        udp->Join(QUERY_REPLAY_ADDR);
        udp->SetLoopbackMode(0);
    }
    for(i = 0; i < Devices->Count; i++)
    {
        delete (DEVICE_ADDRESS *)Devices->Items[i];
    }
    Devices->Clear();
    LogIp->Items->Clear();

    i = inet_addr(QUERY_REPLAY_ADDR);
    memcpy(buf, &i, 4);
    strcpy(buf+4, "QAddr");
    ret = udp->Sendto(QUERY_MNG_ADDR, QUERY_MNG_PORT, buf, 16);
    if(ret < 0)
        return 0;
    ret = udp->Sendto(htonl(INADDR_BROADCAST), QUERY_MNG_PORT, buf, 16);

        while(ret > 0)
        {
            ret = udp->WaitData(200);
            if(ret <= 0)
                break;

            ret = udp->RecvFrom(buf, 128, &addr);
            if(ret > 0)
            {
                DEVICE_ADDRESS *da;
                memcpy(&i, buf+8, 4);
                if((i < 0) || (i > 65535))
                    continue;
                for(i = 0; i < Devices->Count; i++)
                {
                    da = (DEVICE_ADDRESS *)Devices->Items[i];
                    if(da->ip == addr.sin_addr.s_addr)
                    {
                        if(da->macSt && (!memcmp(da->mac, buf+12, 6)))
                            break;
                    }
                }
                if(i < Devices->Count)
                    continue;
                da = new DEVICE_ADDRESS;
                if(!da)
                    break;
                da->ip = addr.sin_addr.s_addr;
                memcpy(&da->port, buf+8, 4);
                if(ret > 12)
                {                  
                    da->ver = 1;
                    da->macSt = 1;
                    da->ethSt = -1;
                    memcpy(da->mac, buf+12, 6);
                    if(ret > 18)
                    {
                       da->ver = buf[18];
                       da->ethSt = buf[19]?1:0;
                    }
                }
                else
                {
                    da->macSt = 0;
                    da->ver = 0;
                    da->ethSt = -1;
                    memset(da->mac, 0, 6);
                }
                Devices->Add(da);
            }
        }
    return Devices->Count;
}

void __fastcall TMainForm::Button7Click(TObject *Sender)
{
    int i;
    char buf[64];

    HINSTANCE h = ShellExecute(0, "open", "arp.exe", "-d", "", SW_HIDE);

    TCursor Save_Cursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    i = QueryDevice();
    Screen->Cursor = Save_Cursor;
    if(i)
    {
        DEVICE_ADDRESS *da;
        for(i = 0; i < Devices->Count; i++)
        {
            da = (DEVICE_ADDRESS *)Devices->Items[i];
            ip2ip(da->ip, buf);
            LogIp->Items->Add(buf);
        }
        Num->Caption = IntToStr(i);
        //LogIp->Text = buf;
        LogPort->Text = IntToStr(((DEVICE_ADDRESS *)Devices->Items[0])->port);
        LogIp->ItemIndex = 0;
        LogIpClick(LogIp);
    }
    else
    {
        //ShowMessage("没有搜索到设备");
        Num->Caption = "0";
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LogIpSelect(TObject *Sender)
{
    int i = LogIp->ItemIndex;
    if(i < 0)
        return;
    DEVICE_ADDRESS *da = (DEVICE_ADDRESS *)Devices->Items[i];
    LogPort->Text = IntToStr(da->port);
}
//---------------------------------------------------------------------------
void TMainForm::OnAccList(MNG_ACC *acc, int num)
{
    AccList->Items->Clear();
    for(int i = 0; i < num; i++)
    {
        TListItem *itm = AccList->Items->Add();
        itm->Caption = acc->name;
        itm->SubItems->Add(acc->level);
        itm->SubItems->Add(acc->description);
        acc++;
    }
}

void TMainForm::SetPara(int type, void *Para)
{
    char buf[32];
    switch(type)
    {
        case MNG_CMD_GET_ALARM_SERVICE:
        {
            SERVICE_INFO *p = (SERVICE_INFO *)Para;
            FtpEn->Checked = p->ftpEn;
            AlermEn->Checked = p->alarmEn;
            SmtpEn->Checked = p->smtpEn;
            SmtpEnc->ItemIndex = p->smtpEnc;
            SmtpPort->Value = p->smtpPort;
            AlarmPort->Value = p->alarmPort;
            FtpSrv->Text = p->ftpServer;
            FtpAcc->Text = p->ftpAcc;
            FtpPwd->Text = p->ftpPwd;
            FtpPort->Value = p->ftpPort;
            AlarmSrv->Text = p->alarmServer;
            SmtpSrv->Text = p->smtpServer;
            SmtpAcc->Text = p->smtpAcc;
            SmtpPwd->Text = p->smtpPwd;
            SmtpSender->Text = p->smtpEmail;
            Email1->Text = p->email[0];
            Email2->Text = p->email[1];
            Email3->Text = p->email[2];
            Email4->Text = p->email[3];
            break;
        }
        case MNG_CMD_GET_DDNS:
        {
            DDNS_INFO *p = (DDNS_INFO *)Para;
            DdnsProtocol->ItemIndex = p->protocol;
            DdnsDomain->Text = p->hostName;
            DdnsUser->Text = p->acc;
            DdnsPwd->Text = p->pwd;
            break;
        }
        case MNG_CMD_GET_DISPLAY:
        {
            MNG_DISPALY_CTRL *p = (MNG_DISPALY_CTRL *)Para;
            if(Display)
                Display->SetDisplay(p->bright, p->contrast, p->saturation, p->hue);
            break;
        }
        case MNG_CMD_VOLUME_GET:
        {
            VOLUME *p = (VOLUME *)Para;
            if(Display)
                Display->SetVolum(p);
            break;
        }
        case MNG_CMD_GET_DYNAMIC_NET_INFO:
        {
            MNG_DYNAMIC_NET_INFO *p = (MNG_DYNAMIC_NET_INFO *)Para;
            if(p->wifiIp)
            {
                ip2ip(p->wifiIp, buf);
                WifiIp->Text = buf;
                ip2ip(p->wifiMask, buf);
                WifiMask->Text = buf;
            }
            else
            {
                WifiIp->Text = "";
                WifiMask->Text = "";
            }
            DhcpEn->Checked = p->dhcpEn;
            UpnpEn->Checked = p->upnpEn;
            WifiEn->Checked = p->wifiEn;
            IPVer->ItemIndex = p->ipVer;
            MobileAuth->ItemIndex = p->mobileAuth;
            RouterEn->Checked = p->routerEn;
            RouterRange->Checked = p->routerRange;
            RouterFlow->Value = p->routerFlow;
            IpFlow->Value = p->ipFlow;
            DhcpdEn->Checked = p->dhcpdEn;
            PortMapEn->Checked = p->portMapEn;
            MobileEn->Checked = p->mobileEn;
            MobileType->ItemIndex = p->mobileType;
            MobileCard->ItemIndex = p->mobileCard;
            WifiSsid->Text = p->wifi.wifiSsid;
            WifiPwd->Text = p->wifi.wifiPwd;
            WifiAuth->ItemIndex = p->wifi.wifiAuth;
            WifiEncry->ItemIndex = p->wifi.wifiEncryp;
            //WifiIndex->ItemIndex = p->wifi.wifiIndex;
            WifiRssi->Text = "";       
            apn->Text = p->APN;
            break;
        }
        case MNG_CMD_GET_WIFI_SEARCH:
        {
            MNG_WIFI_NETs *p = (MNG_WIFI_NETs *)Para;
            WifiSsid->Items->Clear();
            g_WifiNet.count = p->count;
            for(int i = 0; i < p->count; i++)
            {
                WifiSsid->Items->Add(p->net[i].ssid);
                memcpy(&g_WifiNet.net[i], &p->net[i], sizeof(MNG_WIFI_NET));
            }
            //Screen->Cursor = Save_Cursor;
            break;
        }
        case MNG_CMD_GET_PRODUCT_INFO:
        {
            PRODUCT_INFO *p = (PRODUCT_INFO *)Para;
            ProductType2->Text = p->productType;
            memcpy(buf, p->productNo, 16);
            buf[16]=0;
            ProductNo2->Text = buf;
            if(p->productType[0])
            {
                ProductNo->Text = ProductNo2->Text;
                ProductType->Text = ProductType2->Text;
            }
            ProductAcc2->Text = p->user;
            //ProductPwd2->Text = p->pwd;
            ProductSrv2->Text = p->regsrv;
            ProductSrvPort2->Value = p->regPort;
            memcpy(buf, p->id, 20);
            buf[20]=0;
            ProductId2->Text = buf;
            sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
                         p->mac[0], p->mac[1], p->mac[2],
                         p->mac[3], p->mac[4], p->mac[5]);
            ProductMac2->Text = buf;
            if(p->productType[0])
                ProductMac->Text = buf;
            if(p->imei[0])
            {
                memcpy(buf,p->imei,15);
                buf[15] = 0;
                Imei->Text = buf;
            }
            //struct tm *t = localtime((const long *)&p->date);
            //sprintf(buf, "%04d-%02d-%02d", t->tm_year, t->tm_mon, t->tm_mday);
            ProductTime2->Text = ctime((const long *)&p->date);
            ProductType->Enabled = !p->productType[0];
            ProductNo->Enabled = !p->productType[0];
            Button34->Enabled = p->productType[0];
           // Button31->Enabled = !p->productType[0];
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TMainForm::StartVideo(long addr, int port, char *callId)
{
    if(Display)
    {
        char tmp[16];
        AnsiString S="";
        addr = inet_addr(LogIp->Text.c_str());
        ip2ip(addr, tmp);
        S += LogIp->Text;//(AnsiString)tmp;
        S += ":";
        S += IntToStr(port);
        AddInfo(-1, S);
        AddInfo(0, "连接...");
        DataClient = new CDataClient(Display, callId);
        if(DataClient->Connect(addr, port) != CLIENT_OK)
            AddInfo(0, "连接失败");
        else
            AddInfo(0, "接收数据...");
    }
}

void TMainForm::AddInfo(int n, AnsiString Info)
{
    if(Display)
    {
        Display->AddInfo(n, Info);
    }
}

long CALLBACK OnVoice(long lType, long data, long length)
{
    int ret = 0;
    if(g_Talker)
    {
        ret = g_Talker->Send((char *)data, length);
    }

    return ret;
}

int TMainForm::ControlTalk()
{
	if(g_Talker)
	{
	    StopTalk();
	    g_Talker = 0;
	}

	if(!DataClient || !Display)
		return -1;

	if(Display->StartTalk(OnVoice))
	{
		ShowMessage("启动语音失败");
		return -1;
	}

	if(Client->SendTalkRequest())
	{
		Display->StopTalk();
		ShowMessage("发送消息失败");
		return -1;
	}

	return 0;
}

void TMainForm::StopTalk(int Mode)
{  
    if(g_Talker)
    {
        if(Display)
            Display->StopTalk();
        delete g_Talker;
        g_Talker = 0;
        if(Mode)
            Client->SendMsg(MNG_CMD_TALK_STOP, 0, 0);
    }
}


int TMainForm::StartTalk(long addr, int port, char *CallId)
{
    if(!Display)
    {
        return -1;
    }

    addr = Client->GetPeerIp();

    /*CTalkClt *t = new CTalkClt();
    if(!t || t->Connect(addr, port, CallId))
    {
        Display->StopTalk();
        return -1;
    } */
    CTalkClt *t = new CTalkClt();
    if(!t || t->Start())
    {
        Display->StopTalk();
            return -1;
    }

    t->SetPeer(addr, port);

    g_Talker = t;

    return 0;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::Button21Click(TObject *Sender)
{
    MNG_DYNAMIC_NET_INFO p;
    int n;

    memset((char *)&p, 0, sizeof(p));

    p.dhcpEn = DhcpEn->Checked;
    p.upnpEn = UpnpEn->Checked;
    p.portMapEn = PortMapEn->Checked;
    p.mobileEn = MobileEn->Checked;
    p.mobileType = MobileType->ItemIndex;
    p.mobileCard = MobileCard->ItemIndex;
    p.wifiEn = WifiEn->Checked;
    p.wifiIp = inet_addr(WifiIp->Text.c_str());    
    p.wifiMask = inet_addr(WifiMask->Text.c_str());
    p.ipVer = IPVer->ItemIndex;
    p.mobileAuth = MobileAuth->ItemIndex;
    strcpy(p.APN, apn->Text.c_str());
    n = strlen(p.APN)-1;
    while(n > 0)
    {
        if((p.APN[n] == ' ')||(p.APN[n] == '\r')||(p.APN[n] == '\n'))
            p.APN[n] = 0;
        n--;
    }
    p.dhcpdEn = DhcpdEn->Checked;
    p.routerEn = RouterEn->Checked;
    p.routerFlow = RouterFlow->Value;
    p.ipFlow = IpFlow->Value;
    p.routerRange = RouterRange->Checked;
    //if(p.wifiEn)
    {
        strcpy(p.wifi.wifiSsid, WifiSsid->Text.c_str());
        strcpy(p.wifi.wifiPwd, WifiPwd->Text.c_str());
        p.wifi.wifiAuth = WifiAuth->ItemIndex;
        p.wifi.wifiEncryp = WifiEncry->ItemIndex;
        p.wifi.wifiIndex = 0;//WifiIndex->ItemIndex;
    }
    Client->SetDynamicNetInfo(&p);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TabControl5Change(TObject *Sender)
{
    int Id = TabControl5->TabIndex+1;
    Client->SendGetPlatFormInfo(Id);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button24Click(TObject *Sender)
{
    int t;
    PLATFORM_INFO_S p;
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    
    p.platformId = TabControl5->TabIndex+1;
    if(pptc->Text == "None")
        p.protocol = DVS_PROTOCOL_NONE;
    else
        p.protocol = pptc->ItemIndex;
    strcpy(p.devId, pid->Text.c_str());     
    //strcpy(p., did->Text.c_str());
    strcpy(p.pwd, pHttpPwd->Text.c_str());
    strcpy(p.regSrv, pRegIp->Text.c_str());
    t = inet_addr(pRegIp->Text.c_str());
    if(t == -1)
    {
        t = 0;
    }
    p.srvIp = t;
    p.srvPort = pRegPort->Value;
    p.alarmSrvIp = inet_addr(pAlarmIp->Text.c_str());
    p.alarmSrvPort = pAlarmPort->Value;
    p.sipSrvPort = pSipPort->Value;
    p.sipSrvPort2 = pSipPort2->Value;
    p.sipSrvAddr[0] = 0;
    strcpy(p.domainId, dId->Text.c_str());
    p.sipSrvIp = inet_addr(pSipIp->Text.c_str());
    p.sipSrvIp2 = inet_addr(pSipIp2->Text.c_str());
    strcpy(p.serverId, ServerId->Text.c_str());

    p.sipHeartEnabled = pSipHeartEn->Checked;
    p.httpHeartEnabled = pHttpHeartEn->Checked;
    p.sipHeartPeriod = pSipPeriod->Value;
    p.httpHeartPeriod = pHttpPeriod->Value;
    p.storageProtocol = StorageEn->Checked;

    Client->SetPlatFormInfo(&p);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button25Click(TObject *Sender)
{
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }

    MNG_SYS_ADVANCED_INFO p;

    p.tcpSbufLevel = TcpBufSize->ItemIndex;
    p.timOffset = TimOffset->Value;
    p.carType = CarType->ItemIndex;
    p.carColor = CarColor->ItemIndex;
    memset(p.plate,0,10);
    strcpy(p.plate, CarPlate->Text.c_str());
    strcpy(p.sim, Sim->Text.c_str());
    p.gnssMode = GnssMode->ItemIndex;
    p.gnssType = GnssType->ItemIndex;
    p.gnssTim = GnssTime->Value;
    p.gnssDis = GnssDis->Value;
    p.gpsUploadIntval =  GpsuploadIntval->Value;
    p.alarmSpeed = GpsAlarmSpeed->Value;
    p.tireTime = DriveTireTime->Value;
    p.restTime = DriveRestTime->Value;
    p.rollOverSen =  cbox_rollovertestsen->ItemIndex + 1;
    p.collisionSen = cbox_collsitiontestsen->ItemIndex + 1;
    Client->SetAdvancedInfo(&p);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PageControl3Change(TObject *Sender)
{
    if(!Client)
        return;

    if(PageControl3->ActivePage == NewStorageSheet)
    {
        NormalStoragePlan->Clear();
        BitBtn1->Enabled = false;
        Client->SendGetStorageNewInfo(0);
    }
    else if(PageControl3->ActivePage == StorageSheet)
    {
        BitBtn2->Enabled = false;
        Client->SendGetStorageInfo(0);
    }
    else if(PageControl3->ActivePage == NetSheet)
    {
        Button25->Enabled = false;
        Client->SendGetAdvancedInfo();
    }
    else if(PageControl3->ActivePage == DVRSetSheet)
    {
        Button27->Enabled = false;
        Client->SendGetDVRSet();
    }
    else if(PageControl3->ActivePage == DiskSheet)
    {
        formatInfo->Text = "";
        Client->SendGetDiskInfo();
    }
#ifdef SUPPORT_DID
    else if(PageControl3->ActivePage == DevIdSheet)
    {
        MNG_DEV_ID tmpDevId;
        char name[64];
        Button28->Enabled = false;
        Button29->Enabled = false;
        //DevId->Enabled = false;
        //nickName->Enabled = false;
        mac2->Enabled = false;
        devCap->Enabled = false;
        strcpy(tmpDevId.devId, DevId->Text.c_str());
        //strcpy(tmpDevId.devName, LogName->Text.c_str());
        Client->SendGetDevId(&tmpDevId);
        int fd;
        fd = fileOpen("c:\\printer.cfg", FILE_OPEN_MODE_READ);
        if(fd >= 0)
        {
            char tmp[512] = {0};
            PRINTER_SAVE_S *printInfo = (PRINTER_SAVE_S *)tmp;
            fileSeek(fd, 0);
            fileRead(fd, tmp, sizeof(tmp));
            if(!strcmp(printInfo->sign, "hc-iVss"))
            {
                devType->Text = printInfo->devType;
                devVer->Text = printInfo->devVer;
                devBatch->Text = printInfo->devBatch;
                userAddr->Text = printInfo->userAddr;
                DevName->Text = printInfo->devName;
            }
            fileClose(fd);
        }
    }
#endif
    else if(PageControl3->ActivePage == ProductSheet)
    {
        Client->SendMsg(MNG_CMD_GET_PRODUCT_INFO, 0, 0);
    }        
    else if(PageControl3->ActivePage == AccSheet)
    {
        Client->SendMsg(MNG_CMD_GET_ACC, 0, 0);
    }          
    else
    if(PageControl1->ActivePage == DdnsSheet)
    {
        Client->SendGetDdnsInfo();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TabControl6Change(TObject *Sender)
{
    SetStoragePara(TabControl6->TabIndex, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TabControl7Change(TObject *Sender)
{
    SetStoragePara(TabControl7->TabIndex, 1);    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BitBtn1Click(TObject *Sender)
{
    if(!Client)
        return;
    char detailSchedule[256] = {0};
    int n = TabControl6->TabIndex;

    if(!StoragePara)
    {
        ShowMessage("设置出错");
        return;
    }
	
    STORAGE_TEMP_PARA *para = StoragePara+n;
    para->basic.recordEnable = recordEnable->Checked;
    para->basic.recycleRecEnable = recycleRecEnable->Checked;
    para->streamType = storageStreamType->ItemIndex;
    para->recycleMode = cbRecycleMode->ItemIndex;
    strncpy(para->basic.filenameSuffix, filenameSuffix->Text.c_str(), 32);
    para->basic.splitTime = splitTime->Value*60;
    NormalStoragePlan->GetPlan(detailSchedule);
    scheduleExGetChar(detailSchedule, 0, &para->scheduleEx[0]);
    MdStoragePlan->GetPlan(detailSchedule);
    scheduleExGetChar(detailSchedule, 1, &para->scheduleEx[1]);
    GpinStoragePlan->GetPlan(detailSchedule);   
    scheduleExGetChar(detailSchedule, 2, &para->scheduleEx[2]);
    //para->detailSchedule[7*24] = 0;
	
    Client->SetStorageNewInfo(para, n+1);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BitBtn2Click(TObject *Sender)
{
    if(!Client)
        return;

    int n = TabControl7->TabIndex;

    if(!StoragePara)
    {
        ShowMessage("设置出错");
        return;
    }
	
    STORAGE_TEMP_PARA *para = StoragePara+n;
    para->basic.recordEnable = recordEnable1->Checked;
    para->basic.recycleRecEnable = recycleRecEnable1->Checked;
    strncpy(para->basic.filenameSuffix, filenameSuffix1->Text.c_str(), 32);
    para->basic.splitTime = splitTime1->Value*60;  
    para->streamType = storageStreamType1->ItemIndex;

    para->schedule[0].weekday = weekDay1->ItemIndex;
    para->schedule[0].startHour = startHour1->Value;
    para->schedule[0].startMin = startMin1->Value;
    para->schedule[0].stopHour = stopHour1->Value;
    para->schedule[0].stopMin = stopMin1->Value;
    para->schedule[0].recordType = recordType1->ItemIndex;

    para->schedule[1].weekday = weekDay2->ItemIndex;
    para->schedule[1].startHour = startHour2->Value;
    para->schedule[1].startMin = startMin2->Value;
    para->schedule[1].stopHour = stopHour2->Value;
    para->schedule[1].stopMin = stopMin2->Value;
    para->schedule[1].recordType = recordType2->ItemIndex;

    para->schedule[2].weekday = weekDay3->ItemIndex;
    para->schedule[2].startHour = startHour3->Value;
    para->schedule[2].startMin = startMin3->Value;
    para->schedule[2].stopHour = stopHour3->Value;
    para->schedule[2].stopMin = stopMin3->Value;
    para->schedule[2].recordType = recordType3->ItemIndex;

    para->schedule[3].weekday = weekDay4->ItemIndex;
    para->schedule[3].startHour = startHour4->Value;
    para->schedule[3].startMin = startMin4->Value;
    para->schedule[3].stopHour = stopHour4->Value;
    para->schedule[3].stopMin = stopMin4->Value;
    para->schedule[3].recordType = recordType4->ItemIndex;

    para->schedule[4].weekday = weekDay5->ItemIndex;
    para->schedule[4].startHour = startHour5->Value;
    para->schedule[4].startMin = startMin5->Value;
    para->schedule[4].stopHour = stopHour5->Value;
    para->schedule[4].stopMin = stopMin5->Value;
    para->schedule[4].recordType = recordType5->ItemIndex;

    para->schedule[5].weekday = weekDay6->ItemIndex;
    para->schedule[5].startHour = startHour6->Value;
    para->schedule[5].startMin = startMin6->Value;
    para->schedule[5].stopHour = stopHour6->Value;
    para->schedule[5].stopMin = stopMin6->Value;
    para->schedule[5].recordType = recordType6->ItemIndex;

    para->schedule[6].weekday = weekDay7->ItemIndex;
    para->schedule[6].startHour = startHour7->Value;
    para->schedule[6].startMin = startMin7->Value;
    para->schedule[6].stopHour = stopHour7->Value;
    para->schedule[6].stopMin = stopMin7->Value;
    para->schedule[6].recordType = recordType7->ItemIndex;

    para->schedule[7].weekday = weekDay8->ItemIndex;
    para->schedule[7].startHour = startHour8->Value;
    para->schedule[7].startMin = startMin8->Value;
    para->schedule[7].stopHour = stopHour8->Value;
    para->schedule[7].stopMin = stopMin8->Value;
    para->schedule[7].recordType = recordType8->ItemIndex;

    Client->SetStorageInfo(para, n+1);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button27Click(TObject *Sender)
{
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }

    MNG_DVR_S p;

    switch(splitMod->ItemIndex)
    {
        case 0:
            p.splitMod= DISPLAY_MODE_1;
            break;
        case 1:
            p.splitMod= DISPLAY_MODE_4;
            break;
        case 2:
            p.splitMod= DISPLAY_MODE_6;
            break;
        case 3:
            p.splitMod= DISPLAY_MODE_8;
            break;
        case 4:
            p.splitMod= DISPLAY_MODE_9;
            break;
        case 5:
            p.splitMod= DISPLAY_MODE_16;
            break;
        default:
            //p.intfSync = -1;
            break;
    }
    p.intfSync[0] = intfSync1->ItemIndex;
    p.intfSync[2] = intfSync->ItemIndex;
    p.language = Language->ItemIndex;
    p.skin = Skin->ItemIndex;
    p.voDev = voDev->ItemIndex;
    p.ctrlMode = CtrlMode->ItemIndex;
    p.displayLocal = DisplayLocal->Checked;
    p.displayIcon = DisplayIcon->Checked;
    p.screenSame = ScreenSame->Checked;
    p.loopMute = LoopMute->Checked;
    p.irDisable = IrDisable->Checked;
    p.tipClr = TipColor->Color;
    p.screenBkClr = ScreenBkColor->Color;
    p.pktDropMode = PktDropMode->ItemIndex;
    p.showMtName = ShowMtName->Checked;
    p.alpha = Alpha->Value;
    p.displayChn = DisplayChn->Value;
    p.ledType = LedType->ItemIndex;

    Client->SetDVRSet(&p);
}
//---------------------------------------------------------------------------


#define XML_FILE_NAME "c:\\database.xml"
#define XML_PARA_VERSION "0"
void TMainForm::createIdFile(char *id, char *nickName, char *pwd)
{
#if 0
    int fd;
    char buf[128];
    char tmp[128];       
    AnsiString Value;
    
    fd = fileOpen(XML_FILE_NAME, FILE_OPEN_MODE_READ);
    if(fd<0)
    {
        fd = fileOpen(XML_FILE_NAME, FILE_OPEN_MODE_CREAT);
        if(fd < 0)
        {
            ShowMessage("创建xml文件失败");
            return;
        }
        fileClose(fd);
        XMLCfg->Active = true;
        XMLCfg->Version = "1.0";
        XMLCfg->Encoding = "gb2312";
        _di_IXMLNode sub, sub1;
        
        sub = XMLCfg->AddChild(WideString("data"));
        sub->SetAttribute(WideString("name"), WideString("database"));
        sub->SetAttribute(WideString("version"), WideString(XML_PARA_VERSION));

        XMLCfg->SaveToFile(WideString(XML_FILE_NAME));
        XMLCfg->Active = false;
    }
    else
        fileClose(fd);         

        
    XMLCfg->LoadFromFile(WideString(XML_FILE_NAME));
    XMLCfg->Active = true;
    
    _di_IXMLNode xmlNode, sNode, ssNode;
    xmlNode = XMLCfg->DocumentElement;
    _di_IXMLNodeList xmlNodeList = xmlNode->ChildNodes;

    
    Value = AnsiString(xmlNode->GetAttribute(WideString("name")));
    if(Value != WideString("database"))
    {    
        MessageBox(Handle, "配置文件格式错误", "错误", MB_OK|MB_ICONWARNING);
        XMLCfg->Active = false;
        return;
    }

    Value = AnsiString(xmlNode->GetAttribute(WideString("version")));
    if(Value != WideString(XML_PARA_VERSION))
    {
        MessageBox(Handle, "配置文件格式错误", "错误", MB_OK|MB_ICONWARNING);
        XMLCfg->Active = false;
        return;
    }         
    
    xmlNode = XMLCfg->DocumentElement;
    xmlNodeList = xmlNode->ChildNodes;      
    for(int i = 0; i < xmlNodeList->Count; i++)
    {
        _di_IXMLNode Node;
        xmlNode = xmlNodeList->Nodes[i];
        _di_IXMLNodeList NodeList = xmlNode->ChildNodes;
        Node = NodeList->Nodes[0];
        if(Node->NodeName == WideString("devId"))
        {
            Value = AnsiString(Node->Text);
            if(Value == id)
            {
                MessageBox(Handle, "文件中已存在该ID", "错误", MB_OK|MB_ICONWARNING);
                XMLCfg->Active = false;
                return;
            }
        }
        
    }

    xmlNode = XMLCfg->DocumentElement;
    xmlNodeList = xmlNode->ChildNodes;

    sNode = xmlNode->AddChild(WideString("device"));
    ssNode = sNode->AddChild(WideString("devId"));
    ssNode->SetText(WideString(id));          
    ssNode = sNode->AddChild(WideString("devIdCheck"));  
    md5Encrypt(id, tmp);
    md5Encrypt(tmp, buf);
    ssNode->SetText(WideString(buf));
    ssNode = sNode->AddChild(WideString("nickName"));
    ssNode->SetText(WideString(nickName));
    ssNode = sNode->AddChild(WideString("pwd"));
    md5Encrypt(pwd, tmp);
    md5Encrypt(tmp, buf);
    ssNode->SetText(WideString(buf));

    XMLCfg->SaveToFile(WideString(XML_FILE_NAME));
    XMLCfg->Active = false;
#endif
}


void __fastcall TMainForm::PrintDevInfo(TObject *Sender)
{
#ifdef SUPPORT_DID
    char pwd[128] = {0};
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    MNG_DEV_ID *p = Client->m_DevIdInfo;
    strcpy(p->devId, DevId->Text.c_str());
    strcpy(pwd, devPwd->Text.c_str());
    strcpy(p->sipSrvAddr, sipAddr->Text.c_str()); 
    //p->sipRegIp = inet_addr(sipAddr->Text.c_str());
    p->sipRegPort = CSpinEPort->Value;
    if(strlen(pwd) > 30)
    {
        ShowMessage("密码长度不能大于30");
        return;
    }
    if(!pwd[0])
    {
        ShowMessage("密码为空请设置密码");
        return;
    }
    
    
    char tmp[1024] = {0};
    PRINTER_SAVE_S *printInfo = (PRINTER_SAVE_S *)tmp;
    int fd;
    fd = fileOpen("c:\\printer.cfg", FILE_OPEN_MODE_CREAT);
    if(fd<0)
    {
       ShowMessage("创建打印文件失败");
       return;
    }

    fileSeek(fd, 0);
    strcpy(printInfo->sign, "iVss");
    strcpy(printInfo->devType, devType->Text.c_str());
    strcpy(printInfo->devVer, devVer->Text.c_str());
    strcpy(printInfo->devBatch, devBatch->Text.c_str());
    strcpy(printInfo->devName, DevName->Text.c_str());
    strcpy(printInfo->userAddr, userAddr->Text.c_str());
    fileWrite(fd, tmp, sizeof(tmp));
    fileClose(fd);

    createIdFile(p->devId, nickName->Text.c_str(),devPwd->Text.c_str());
    Client->SetDevId(p);
#endif
}
//---------------------------------------------------------------------------
/*32进制*/
int TMainForm::nickNameCreate(unsigned char lastmac[3], char *nickName)
{
    int i = 0;
    int l, h;
    char *enc16 = "abdefgjkpmnrsxy";//"THABDEFJGKLMNRSX";
    for(i=0; i<3; i++)
    {
        l =  lastmac[i]&0xf;
        h = (lastmac[i]>>4)&0xf;
        nickName[i*2] = enc16[h];
        nickName[i*2+1] = enc16[l];
    }
    nickName[6] = 0;
    return 0;   
}


void __fastcall TMainForm::CreateNick(TObject *Sender)
{
#ifdef SUPPORT_DID
    int i = 0;
    int k = 0, pwdlen;
    char pwdstr[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    char buf[32];
    char pwd[32] = {0};
    _UINT32 devCap = 0;
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%02x%02x%02x%02x%02x%02x%08x%02x",
                Client->m_DevIdInfo->devMac[0],
                Client->m_DevIdInfo->devMac[1],
                Client->m_DevIdInfo->devMac[2],
                Client->m_DevIdInfo->devMac[3],
                Client->m_DevIdInfo->devMac[4],
                Client->m_DevIdInfo->devMac[5],
                Client->m_DevIdInfo->devCap,
                Client->m_DevIdInfo->chnNum);
    DevId->Text = buf;
    pwdlen = atoi(pwdLen->Text.c_str());
    for(i=0; i<pwdlen; i++)
    {          
        k = rand();
        k = k % 62;
        pwd[i] = pwdstr[k];
    }
    pwd[i] = 0;    
    if(randPwd->Checked)
        devPwd->Text = pwd;
    else
        devPwd->Text = Client->m_DevIdInfo->devPwd;
    char nick[16];
    memset(nick, 0, sizeof(nick));
    nickNameCreate(&Client->m_DevIdInfo->devMac[3], nick);
    nickName->Text = nick;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetPrinter(TObject *Sender)
{
//#if 0
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
//    #endif
    PrinterSet = new TPrintForm(NULL);
    if(!PrinterSet)
    {
        ShowMessage("创建打印窗口失败");
        return ;
    }
    PrinterSet->Show();
}
//---------------------------------------------------------------------------

#define MAX_BOX_WIDTH 550
#define MAX_BOX_HEIGT 310
#define MAX_LINE_WIDTH 5

void __fastcall TMainForm::Button33Click(TObject *Sender)
{
#if 1//def SUPPORT_PRINT
#if 1
    static int loadFlag = 0;
    if(0 == loadFlag)
    {
        LoadTSCDll();
        loadFlag = 1;
    }

    int ret = 0;
    int idlenoffset = 0;
    char str_devType[32] = {0};
    char str_devsn[32] = {0};
    char str_devId[32] = {0};
    char str_Imei[32] = {0};
    char tmpCodeInfo[128] = {0};
    char tmpCodeVal[256] = { 0 };

    if(strlen(Imei->Text.c_str()) != 15)
    {
        ShowMessage("暂未获取到IMEI号，请到日志查询页面查询后，再进行操作");
        return;
    }

    ret = TSC_openport("TSC TTP-244 Pro");//找打打印机端口
    if(1 != ret)
    {
        ShowMessage("连接打印机失败，请检查打印机连接是否正常");
        return;
    }
    ret = TSC_setup("40", "22", "4", "10", "0", "1", "0"); //配置打印纸尺寸信息
    if(1 != ret)
    {
        ShowMessage("打印机参数设置失败");
        return;
    }
    TSC_clearbuffer();     //  清屏
    TSC_sendcommand("DIRECTION 1"); //
    TSC_sendcommand("CLS");//需要清除上一次的打印记忆  //
 /*
    char tmpCodeVal[256] = { 0 };
    sprintf(tmpCodeVal,"QRCODE 240,56,H,4,A,0,M2,S7,\"%s\"", "IMEI:8658200303201987");
    sendcommand(tmpCodeVal);
    //sendcommand("QRCODE 260,20,H,7,A,0,M2,S7,\"" + codeValue + "\"");
    windowsfont(140, 16, 30, 0, 0, 0, "黑体", "光路科技");
    sprintf(str_devType,"型号:%s",ProductType2->Text.c_str());
    windowsfont(40, 56, 24, 0, 0, 0, "黑体", str_devType);
    //windowsfont(16, 110, 20, 0, 0, 0, "黑体", "ip:192.168.0.200");
    //windowsfont(16, 120, 20, 0, 0, 0, "黑体", "IMEI:865820030321987");
    sprintf(str_devsn,"SN:%s",ProductNo2->Text.c_str());
    windowsfont(40, 88, 24, 0, 0, 0, "黑体", str_devsn);
    windowsfont(40, 120, 24, 0, 0, 0, "黑体", "日期:20180927");
 */
    idlenoffset = strlen(ProductId2->Text.c_str())-12;
    sprintf(tmpCodeInfo,"GL-%s:id=%s,imei=%s",ProductType2->Text.c_str(),ProductId2->Text.c_str()+idlenoffset,Imei->Text.c_str());
    sprintf(tmpCodeVal,"QRCODE 216,24,H,3,A,0,M2,S7,\"%s\"", tmpCodeInfo);
    TSC_sendcommand(tmpCodeVal);
    //sendcommand("QRCODE 260,20,H,7,A,0,M2,S7,\"" + codeValue + "\"");
    //windowsfont(140, 16, 30, 0, 0, 0, "黑体", "光路科技");
    sprintf(str_devType,"型号:%s",ProductType2->Text.c_str());
    TSC_windowsfont(12, 24, 20, 0, 0, 0, "黑体", str_devType);
    sprintf(str_devsn,"SN:%s",ProductNo2->Text.c_str());
    TSC_windowsfont(12, 48, 20, 0, 0, 0, "黑体", str_devsn);
    sprintf(str_devId,"ID:%s",ProductId2->Text.c_str() + idlenoffset);
    TSC_windowsfont(12, 72, 20, 0, 0, 0, "黑体", str_devId);
    sprintf(str_Imei,"IMEI:%s",Imei->Text.c_str());
    TSC_windowsfont(12, 96, 20, 0, 0, 0, "黑体", str_Imei);
    TSC_windowsfont(40, 128, 24, 0, 0, 0, "黑体", "济南光路科技有限公司");//
    TSC_printlabel("1", "1"); //
    TSC_closeport();    //

    if(1 == loadFlag)
    {
        UnLoadDll();
        loadFlag = 1;
    }

#else
    int state;
    char tmp[256] = {0};

    state = BPLA_SetAllRotate(1);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_SetAllRotate failed");
        return;
    }
    BPLA_SetEnd(0);
    state = BPLA_StartArea(0, 580,0,0,0,0,0,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_StartArea failed");
        return;
    }
   
    state = BPLA_PrintBox(20, 20, MAX_BOX_WIDTH, MAX_BOX_HEIGT, MAX_LINE_WIDTH, MAX_LINE_WIDTH, 0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintBox failed!");
        return;
    }
    int i;
    for(i=0; i<5; i++)
    {
        state = BPLA_PrintLine(20, 20+(i+1)*(MAX_BOX_HEIGT/6), MAX_BOX_WIDTH, 20+(i+1)*(MAX_BOX_HEIGT/6), MAX_LINE_WIDTH-1);
        if(state!=BPLA_OK)
        {
            ShowMessage("BPLA_PrintLine(20, 70, MAX_BOX_WIDTH, 70, 2)failed!");
            return;
        }           
    }

    state = BPLA_PrintLine(20+MAX_BOX_WIDTH/2, 20, 20+MAX_BOX_WIDTH/2, 20+3*(MAX_BOX_HEIGT/6), MAX_LINE_WIDTH-1);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintLine(290, 170, 290, MAX_BOX_HEIGT, 2)failed!");
        return;
    }
    state = BPLA_PrintTruetype("光路科技有限公司",50, MAX_BOX_HEIGT-(MAX_BOX_HEIGT/6)+25,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(光路科技有限公司)failed!");
        return;
    }

#if 0
    state = BPLA_PrintTruetype("设备串号：",35, 80,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(设备串号)failed!");
        return;
    }

    strcpy(tmp, DevId->Text.c_str());
    state = BPLA_PrintTruetype(tmp, 140, 80, "黑体", 40, 0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(设备串号S)failed!");
        return;
    }
    
    state = BPLA_PrintTruetype("登录地址：",35, 130,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(登录地址)failed!");
        return;
    }    
    strcpy(tmp, userAddr->Text.c_str());
    state = BPLA_PrintTruetype(tmp, 140, 130, "黑体", 40, 0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(登录地址S)failed!");
        return;
    }    
                              
    state = BPLA_PrintTruetype("帐号：",35, 180,"黑体", 40, 0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(帐号)failed!");
        return;
    }    
    strcpy(tmp, nickName->Text.c_str()); 
    state = BPLA_PrintTruetype(tmp,100, 180,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(帐号S)failed!");
        return;
    }    
    
    state = BPLA_PrintTruetype("密码：",310, 180,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(密码)failed!");
        return;
    }    
    strcpy(tmp, devPwd->Text.c_str()); 
    state = BPLA_PrintTruetype(tmp,370, 180,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(密码S)failed!");
        return;
    }    
    
    state = BPLA_PrintTruetype("名称：",35, 230,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(名称)failed!");
        return;
    }    
    strcpy(tmp, DevName->Text.c_str()); 
    state = BPLA_PrintTruetype(tmp,100, 230,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(名称S)failed!");
        return;
    }    
          
    state = BPLA_PrintTruetype("型号：",310, 230,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(型号)failed!");
        return;
    }    
    strcpy(tmp, devType->Text.c_str()); 
    state = BPLA_PrintTruetype(tmp,370, 230,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(型号S)failed!");
        return;
    }    

    
    state = BPLA_PrintTruetype("批号：",35, 280,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(批号)failed!");
        return;
    }                      
    strcpy(tmp, devBatch->Text.c_str()); 
    state = BPLA_PrintTruetype(tmp,100, 280,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(批号S)failed!");
        return;
    }              
            
    state = BPLA_PrintTruetype("版本：",310, 280,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(版本)failed!");
        return;
    }              
    strcpy(tmp, devVer->Text.c_str()); 
    state = BPLA_PrintTruetype(tmp,370, 280,"黑体",40,0);
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_PrintTruetype(版本S)failed!");
        return;
    }              
#endif
    state = BPLA_Print(1,1,1);   
    if(state!=BPLA_OK)
    {
        ShowMessage("BPLA_Print failed!");
        return;
    }        
    Sleep(1800);//由于状态查询属于实时指令，而打印机从接收数据到
    ShowMessage("打印成功");
#endif
#endif
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::OSDColorClick(TObject *Sender)
{
    TPanel *p = (TPanel *)Sender;
    ColorDialog1->Color = p->Color;
    if(ColorDialog1->Execute())
    {
        p->Color = ColorDialog1->Color;
    }
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{
    int n = CGauge1->MaxValue/20;
    if(CGauge1->Progress < CGauge1->MaxValue-2*n)
        CGauge1->Progress += n;
    else
        Timer1->Enabled = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button30Click(TObject *Sender)
{
    //Save_Cursor = Screen->Cursor;
    //Screen->Cursor = crHourGlass;
    Client->SendMsg(MNG_CMD_GET_WIFI_SEARCH, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WifiSsidChange(TObject *Sender)
{
    int i  = WifiSsid->ItemIndex;
    if(i >= 0)
    {
        //WifiAuth->Enabled = 0;
        //WifiEncry->Enabled = 0;
        WifiAuth->ItemIndex = g_WifiNet.net[i].auth;
        WifiEncry->ItemIndex = g_WifiNet.net[i].encryp;
    }
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::StorageSchPageControlChange(TObject *Sender)
{
    char detailSchedule[256] = {0};
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    int index = TabControl6->TabIndex;
    STORAGE_TEMP_PARA *p = (STORAGE_TEMP_PARA *)StoragePara+index;//+TabControl6-;
   #if 0 
    scheduleExPutChar(&p->scheduleEx[0], detailSchedule);
    NormalStoragePlan->SetPlan(detailSchedule);  
    NormalStoragePlan->OnPaint(this);
        #endif    
    if(StorageSchPageControl->ActivePage == NormalSchTabSheet)
    {
        scheduleExPutChar(&p->scheduleEx[0], detailSchedule);
        NormalStoragePlan->SetPlan(detailSchedule);
        //NormalStoragePlan->OnPaint(this);
    }
    else
    if(StorageSchPageControl->ActivePage == MdSchTabSheet)
    {
        scheduleExPutChar(&p->scheduleEx[1], detailSchedule);
        MdStoragePlan->SetPlan(detailSchedule);
        //MdStoragePlan->OnPaint(this);
    }
    else
    if(StorageSchPageControl->ActivePage == GpinSchTabSheet)
    {
        scheduleExPutChar(&p->scheduleEx[2], detailSchedule);
        GpinStoragePlan->SetPlan(detailSchedule);
        //GpinStoragePlan->OnPaint(this);
    }        
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button41Click(TObject *Sender)
{
    int diskNum = 0, i=0;
    char temp[128] = {0};
    if(!Client)
    {
        return;
    }
    TListItem *Item = lDiskInfo->Selected;
    if(Item)
    {
        strcpy(temp, Item->Caption.c_str());
        for(i=0; i<3; i++)
        {
            if(strstr(temp, DISK_NAME_Msg[i]))
                break;
        }
        switch(i)
        {
            case 0:
                diskNum = StrToInt(temp[strlen(DISK_NAME_Msg[i])]);
                break;
            case 1:
                diskNum = StrToInt(temp[strlen(DISK_NAME_Msg[i])])+8;
                break;
            case 2:
                diskNum = StrToInt(temp[strlen(DISK_NAME_Msg[i])])+10;
                break;
            default:
                return;
        }
    }
    else
    {
        ShowMessage("未选中磁盘");

        return;
    }
    
    if(MessageBox(Handle, "确定格式化磁盘?格式化磁盘将删除所有数据", "请确认", MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDOK)
    {
        formatPb->Position = 0;
        diskNum -= 1;
        Client->SendFormatDisk(diskNum);
        //Button41->Enabled = FALSE;
        formatInfo->Text = "请等待...";
        Timer2->Enabled = 1;
    }
}
//---------------------------------------------------------------------------
char *productIni = "c:\\pu_product_info.ini";

void TMainForm::SaveProductInfo()
{
        TIniFile *ini;
        int i, n, sn;
        char buf[32];
        AnsiString S;

      //  ini = new TIniFile(
      //          ChangeFileExt( Application->ExeName, ".INI" ) );
        ini = new TIniFile(productIni);
        if(!ini)
        {
            //ShowMessage("对不起,文件打不开!");
            return;
        }

        ini->WriteInteger("common", "MacH", m_ProductInfo.macH);
        ini->WriteInteger("common", "MacL", m_ProductInfo.macL);
        ini->WriteInteger("common", "sNoH", m_ProductInfo.sNoH);
        ini->WriteInteger("common", "sNoL", m_ProductInfo.sNoL);
        ini->WriteInteger("common", "LastTime", m_ProductInfo.lastTime);
        n = ProductType->Items->Count;
        if(ProductType->ItemIndex < 0)
             n++;
        ini->WriteInteger("common", "types", n);

        for(i = 0; i < ProductType->Items->Count; i++)
        {
            sprintf(buf, "p%d", i+1);
            ini->WriteString(buf, "name", ProductType->Items->Strings[i]);
        }
        if(n > ProductSrv->Items->Count)
        {
            sprintf(buf, "p%d", n);
            ini->WriteString(buf, "name", ProductType->Text);
        }

        sn = ProductSrv->Items->Count;
        if(ProductSrv->ItemIndex < 0)
             sn++;
        ini->WriteInteger("common", "servers", sn);

        for(i = 0; i < ProductSrv->Items->Count; i++)
        {
            sprintf(buf, "s%d", i+1);
            ini->WriteString(buf, "server", ProductSrv->Items->Strings[i]);
            if(i == ProductSrv->ItemIndex)
            {
            ini->WriteString(buf, "server", ProductSrv->Text);
            ini->WriteInteger(buf, "port", m_ProductInfo.regPort);
            ini->WriteInteger(buf, "idH", m_ProductInfo.idH);
            ini->WriteInteger(buf, "idL", m_ProductInfo.idL);
            ini->WriteInteger(buf, "HexMode", m_ProductInfo.hexMode);
            }
        }
        if(sn > ProductSrv->Items->Count)
        {
            sprintf(buf, "s%d", sn);
            ini->WriteString(buf, "server", ProductSrv->Text);
            ini->WriteInteger(buf, "port", m_ProductInfo.regPort);
            ini->WriteInteger(buf, "idH", m_ProductInfo.idH);
            ini->WriteInteger(buf, "idL", m_ProductInfo.idL);
            ini->WriteInteger(buf, "HexMode", m_ProductInfo.hexMode);
        }

        delete ini;
}

int TMainForm::ReadProductInfo()
{
        int i, n;
        TIniFile *ini;   
        char buf[32];
        AnsiString S;

        //ini = new TIniFile(
        //        ChangeFileExt( Application->ExeName, ".INI" ) );
        ini = new TIniFile(productIni);
        if(!ini)
        {
            //ShowMessage("对不起,文件打不开!");
            return -1;
        }

        m_ProductInfo.macH = ini->ReadInteger("common", "MacH", 0x000c);
        m_ProductInfo.macL = ini->ReadInteger("common", "MacL", 0);
        m_ProductInfo.sNoH = ini->ReadInteger("common", "sNoH", 0);
        m_ProductInfo.sNoL = ini->ReadInteger("common", "sNoL", 0);
        m_ProductInfo.sNoL = ini->ReadInteger("common", "sNoL", 0);
        m_ProductInfo.lastTime = ini->ReadInteger("common", "LastTime", 0);
        if(m_ProductInfo.lastTime)
            ProductTime->Text = ctime((const long *)&m_ProductInfo.lastTime);

        n = ini->ReadInteger("common", "types", 0);
        for(i = 0; i < n; i++)
        {
            sprintf(buf, "p%d", i+1);
            S = ini->ReadString(buf, "name", "");
            ProductType->Items->Add(S);
        }

        n = ini->ReadInteger("common", "servers", 0);
        for(i = 0; i < n; i++)
        {
            sprintf(buf, "s%d", i+1);
            S = ini->ReadString(buf, "server", "");
            ProductSrv->Items->Add(S);
        }

        delete ini;

        ProductType->ItemIndex = 0;
        ProductSrv->ItemIndex = 0;
        sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
                         (m_ProductInfo.macH>>8)&0xff, (m_ProductInfo.macH)&0xff,
                         (m_ProductInfo.macL>>24)&0xff, (m_ProductInfo.macL>>16)&0xff,
                         (m_ProductInfo.macL>>8)&0xff, (m_ProductInfo.macL)&0xff);
        ProductMac->Text = buf;
        sprintf(buf, "%08x%08x", m_ProductInfo.sNoH, m_ProductInfo.sNoL);
        ProductNo->Text = buf;


        return 0;
}

int TMainForm::ReadServerInfo()
{
        TIniFile *ini;     
        char buf[32];
        AnsiString S = "s";

        if(ProductSrv->ItemIndex < 0)
            return -1;

        S += (ProductSrv->ItemIndex+1);

        ini = new TIniFile(productIni);
        if(!ini)
        {
            //ShowMessage("对不起,文件打不开!");
            return -1;
        }

        m_ProductInfo.regPort = ini->ReadInteger(S, "port", 8000);
        m_ProductInfo.idH = ini->ReadInteger(S, "idH", 0);
        m_ProductInfo.idL = ini->ReadInteger(S, "idL", 0);       
        m_ProductInfo.hexMode = ini->ReadInteger(S, "HexMode", 0);

        delete ini;

        if(m_ProductInfo.idH || m_ProductInfo.idL)
        {
            if(m_ProductInfo.hexMode)
            sprintf(buf, "00%08x%08x", m_ProductInfo.idH, m_ProductInfo.idL);
            else
            sprintf(buf, "00%08u%08u", m_ProductInfo.idH, m_ProductInfo.idL);
            ProductId->Text = buf;
        }
        ProductSrvPort->Value = m_ProductInfo.regPort;
        HexMode->Checked = m_ProductInfo.hexMode;

        return 0;
}

void __fastcall TMainForm::Button31Click(TObject *Sender)
{
    char buf[32];
    PRODUCT_INFO p;
    memset(&p, 0, sizeof(p));

    p.date = time(0);
    ctime((const long *)&p.date);
    sscanf(ProductMac2->Text.c_str(), "%02X:%02X:%02X:%02X:%02X:%02X",
              &p.mac[0], &p.mac[1], &p.mac[2], &p.mac[3], &p.mac[4], &p.mac[5]);

    strncpy(p.productNo, ProductNo2->Text.c_str(), 16);
    strcpy(p.productType, ProductType2->Text.c_str());
    strcpy(p.user, ProductAcc2->Text.c_str());
    strcpy(p.pwd, "123456");
    strcpy(p.id, ProductId2->Text.c_str());
    p.regPort = ProductSrvPort2->Value;
    strcpy(p.regsrv, ProductSrv2->Text.c_str());
    Client->SendMsg(MNG_CMD_SET_PRODUCT_INFO, &p, sizeof(p));

    /*if(ProductType->ItemIndex < 0)
    {
        ProductType->Items->Add(ProductType->Text);
        ProductType->ItemIndex = ProductType->Items->Count-1;
    }
    if(ProductSrv->ItemIndex < 0)
    {
        ProductSrv->Items->Add(ProductSrv->Text);
        ProductSrv->ItemIndex = ProductSrv->Items->Count-1;
    }

    m_ProductInfo.hexMode = HexMode->Checked;
    m_ProductInfo.macH = (p.mac[0]<<8) | p.mac[1];
    m_ProductInfo.macL = (p.mac[2]<<24) | (p.mac[3]<<16) | (p.mac[4]<<8) | p.mac[5];
    if(ProductNo->Enabled)
    {
        m_ProductInfo.sNoL++;
        m_ProductInfo.macL++;
    }
    if(!ProductId->Text.IsEmpty())
    {
        if(HexMode->Checked)
        {
        sscanf(ProductId->Text.c_str(), "00%08X%08X",
              &m_ProductInfo.idH, &m_ProductInfo.idL);

        m_ProductInfo.idL++;
        sprintf(buf, "00%08x%08x", m_ProductInfo.idH, m_ProductInfo.idL);
        }
        else
        {
        sscanf(ProductId->Text.c_str(), "00%08u%08u",
              &m_ProductInfo.idH, &m_ProductInfo.idL);

        m_ProductInfo.idL++;
        sprintf(buf, "00%08u%08u", m_ProductInfo.idH, m_ProductInfo.idL);
        }
        ProductId->Text = buf;
    }

    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
                         (m_ProductInfo.macH>>8)&0xff, (m_ProductInfo.macH)&0xff,
                         (m_ProductInfo.macL>>24)&0xff, (m_ProductInfo.macL>>16)&0xff,
                         (m_ProductInfo.macL>>8)&0xff, (m_ProductInfo.macL)&0xff);
    ProductMac->Text = buf;
    m_ProductInfo.lastTime = p.date;
    strcpy(m_ProductInfo.regsrv, ProductType->Text.c_str());
    m_ProductInfo.regPort = ProductSrvPort->Value;
    strcpy(m_ProductInfo.productType, ProductType->Text.c_str());
    ProductTime->Text = ctime((const long *)&m_ProductInfo.lastTime);
    if(HexMode->Checked)
    {
        sprintf(buf, "%08x%08x", m_ProductInfo.sNoH, m_ProductInfo.sNoL);
    }
    else
    {
        sprintf(buf, "%08u%08u", m_ProductInfo.sNoH, m_ProductInfo.sNoL);
    }
    ProductNo->Text = buf;

    if(Button31 == Sender)
        SaveProductInfo();        */
    Client->SendMsg(MNG_CMD_GET_PRODUCT_INFO, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProductSrvSelect(TObject *Sender)
{
    ReadServerInfo();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Button28Click(TObject *Sender)
{
    if(Client)
    {
        if(MessageBox(Handle, "确定保存为缺省配置?", "请确认", MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDOK)
            Client->SendMsg(MNG_CMD_SET_DEFAULT_PARA, 0, 0);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button29Click(TObject *Sender)
{
    if(Client)
    {
        if(MessageBox(Handle, "确定清除缺省配置?", "请确认", MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2) == IDOK)
            Client->SendMsg(MNG_CMD_ERASE_DEFAULT_PARA, 0, 0);
    }

}
//---------------------------------------------------------------------------
typedef struct
{
    long qaddr;
    char cmd[6];
    char mac[6];
    long ip;
    long mask;
    long gw;
    char acc[16];
    char pwd[16];
}QUERY_SET_CMD;

int TMainForm::SetDeviceIp(DEVICE_ADDRESS *da)
{
    int ret, len;
    char buf[128];             
    char *err[] = {"修改成功", "认证失败", "地址错误"};
    struct sockaddr_in addr;
    QUERY_SET_CMD *cmd = (QUERY_SET_CMD *)buf;

    TIPSetForm *fm = new TIPSetForm(this);
    if(fm->ShowModal() == mrOk)
    {
    cmd->qaddr = inet_addr(QUERY_REPLAY_ADDR);
    strcpy(cmd->cmd, "SAddr");
    memcpy(cmd->mac, da->mac, 6);
    cmd->ip = inet_addr(fm->IP->Text.c_str());
    cmd->mask = inet_addr(fm->IPMask->Text.c_str());
    cmd->gw = inet_addr(fm->Gataway->Text.c_str());
    strcpy(cmd->acc, LogName->Text.c_str());
    strcpy(cmd->pwd, LogPwd->Text.c_str());

    len = sizeof(QUERY_SET_CMD);
    ret = udp->Sendto(QUERY_MNG_ADDR, QUERY_MNG_PORT, buf, len);
    ret = udp->Sendto(htonl(INADDR_BROADCAST), QUERY_MNG_PORT, buf, len);
    ret = udp->WaitData(3000);
    if(ret > 0)
    {
       ret = udp->RecvFrom(buf, 128, &addr);
       if(ret > 0)
       {
           ShowMessage(err[cmd->ip]);
       }
    }
    while(ret > 0)
    {
        ret = udp->WaitData(300);
        if(ret > 0)
            ret = udp->RecvFrom(buf, 128, &addr);
    }
    }
    delete fm;
}

void __fastcall TMainForm::Button32Click(TObject *Sender)
{
    if(LogIp->ItemIndex >= 0)
    {
        DEVICE_ADDRESS *da = (DEVICE_ADDRESS *)Devices->Items[LogIp->ItemIndex];
        if(da->ver > 1)
        {
            SetDeviceIp(da);
            return;
        }
    }
    ShowMessage("该设备不支持一键修改IP");
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::LogIpClick(TObject *Sender)
{
    char buf[32];
    AnsiString S="网络状态:";
    char *eth[] = {"未知", "无线", "有线"};

    if(LogIp->ItemIndex >= 0)
    {
        DEVICE_ADDRESS *da = (DEVICE_ADDRESS *)Devices->Items[LogIp->ItemIndex];
        if(da->ver >= 1)
        {
            sprintf(buf, "Mac[%02x:%02x:%02x:%02x:%02x:%02x]",
                da->mac[0], da->mac[1],da->mac[2],
                da->mac[3],da->mac[4],da->mac[5]);
            LabelMac->Caption = buf;
            LabelEth->Caption = S+eth[da->ethSt+1];
            return;
        }
    }
    LabelMac->Caption = "Mac[unknow]";     
    LabelEth->Caption = S+eth[0];
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button35Click(TObject *Sender)
{
    char buf[128];

    if(Edit2->Text.Length() < 2)
        return;
    strcpy(buf, Edit2->Text.c_str());
    Client->SendMsg(MNG_CMD_RUN_CMD, buf, strlen(buf)+1);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Button36Click(TObject *Sender)
{
    SERVICE_INFO p;

    p.ftpEn = FtpEn->Checked;
    p.alarmEn = AlermEn->Checked;
    p.smtpEn = SmtpEn->Checked;
    p.smtpEnc = SmtpEnc->ItemIndex;
    p.smtpPort = SmtpPort->Value;
    p.ftpPort = FtpPort->Value;
    p.alarmPort = AlarmPort->Value;
    strcpy(p.ftpServer, FtpSrv->Text.c_str());       
    strcpy(p.ftpAcc, FtpAcc->Text.c_str());
    strcpy(p.ftpPwd, FtpPwd->Text.c_str());
    strcpy(p.alarmServer, AlarmSrv->Text.c_str());
    strcpy(p.smtpServer, SmtpSrv->Text.c_str());
    strcpy(p.smtpAcc, SmtpAcc->Text.c_str());        
    strcpy(p.smtpPwd, SmtpPwd->Text.c_str());
    strcpy(p.smtpEmail, SmtpSender->Text.c_str());
    strcpy(p.email[0], Email1->Text.c_str());
    strcpy(p.email[1], Email2->Text.c_str());
    strcpy(p.email[2], Email3->Text.c_str());
    strcpy(p.email[3], Email4->Text.c_str());
    Client->SendMsg(MNG_CMD_SET_ALARM_SERVICE, &p, sizeof(p));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button37Click(TObject *Sender)
{
    MNG_ACC acc;
    int set = 1;
    TAccForm *fm = new TAccForm(this);
    if(fm->ShowModal() == mrOk)
    {
        fm->GetAcc(&acc);
        if(acc.name[0])
        {
        for(int i = 0; i < AccList->Items->Count; i++)
        {
            if(AccList->Items->Item[i]->Caption == acc.name)
            {
                set = 0;
            }
        }
        if(set)
        {
            Client->SendMsg(MNG_CMD_ADD_ACC, &acc, sizeof(acc));
            Client->SendMsg(MNG_CMD_GET_ACC, 0, 0);
        }
        else
            ShowMessage("账号已存在");
        }
    }
    delete fm;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button39Click(TObject *Sender)
{
    MNG_ACC acc;

    if(!AccList->Selected)
    {
        ShowMessage("请选择账号");
        return;
    }
    acc.level = StrToInt(AccList->Selected->SubItems->Strings[0]);
    strcpy(acc.name, AccList->Selected->Caption.c_str());
    strcpy(acc.description, AccList->Selected->SubItems->Strings[1].c_str());
    TAccForm *fm = new TAccForm(this);
    fm->SetAcc(&acc);
    if(fm->ShowModal() == mrOk)
    {
        fm->GetAcc(&acc);
        Client->SendMsg(MNG_CMD_SET_ACC, &acc, sizeof(acc));
        Client->SendMsg(MNG_CMD_GET_ACC, 0, 0);
    }
    delete fm;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button38Click(TObject *Sender)
{          
    MNG_ACC acc;
    if(!AccList->Selected)
    {
        ShowMessage("请选择账号");
        return;
    }
    strcpy(acc.name, AccList->Selected->Caption.c_str());
    Client->SendMsg(MNG_CMD_DEL_ACC, &acc, sizeof(acc));    
    Client->SendMsg(MNG_CMD_GET_ACC, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ProtocolChange(TObject *Sender)
{
    switch(Protocol->ItemIndex)
    {
        case 6:
            RegPort->Value = 5060;
            break;
        case 0:
            RegPort->Value = 8000;
            break;      
        case 2:
            RegPort->Value = 6005;
            break;
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Timer2Timer(TObject *Sender)
{
    if(Client)
    Client->SendMsg(MNG_CMD_FORMAT_PROGREES, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button40Click(TObject *Sender)
{
    MNG_PTCMD p = {1, 4, ""};
    SYSTEMTIME st;

    GetLocalTime(&st);
    sprintf(p.date, "%04d%02d%02d",
                 st.wYear, st.wMonth, st.wDay);
    sprintf(p.tim,"%02d%02d%02d",
                 st.wHour, st.wMinute, st.wSecond);
    if(Client)
        Client->SendMsg(MNG_CMD_CTRL_SNAP, &p, sizeof(p));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button42Click(TObject *Sender)
{
    MNG_NVR p;
    if(!Client)
    {
        ShowMessage("还未登陆");
        return;
    }
    p.pos = TabControl8->TabIndex;
    p.type = NvrType->ItemIndex;
    p.stroage = NvrStorage->ItemIndex;
    p.port = NvrPort->Value;
    p.timeZone = Zone->Value;
    strcpy(p.ip, NvrIp->Text.c_str());
    strcpy(p.acc, NvrAcc->Text.c_str());
    strcpy(p.pwd, NvrPwd->Text.c_str());
    Client->SetNvr(TabControl8->TabIndex, &p);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TabControl8Change(TObject *Sender)
{
     SetNvrPara(TabControl8->TabIndex);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button43Click(TObject *Sender)
{
    if(OpenDialog1->Execute())
    {
        FILE *f = fopen(OpenDialog1->FileName.c_str(), "r+");
        if(f)
        {
            char buf[512];
            while(fgets(buf, 511, f) > 0)
            {
                int n = strlen(buf)-1;
                while(n >= 0)
                {
                    if((buf[n] == ' ')||(buf[n] == '\r')||(buf[n] == '\n'))
                      buf[n] = 0;
                    n--;
                }
                if(!strncmp(buf, "register=", 9))
                {
                    //register=id,srv,port,protocol
                    char ip[16], mask[16], gw[16], dns[16];
                    char id[32], srv[32];
                    int port, protocol, n, nid;
                    if(sscanf(buf+9, "%[^,],%[^,],%[^,],%[^,],%[^,],%u,%u,%[^,],",
                     ip, mask, gw, dns, srv, &port, &protocol, id) < 8)
                    {
                         ShowMessage("注册参数错误");
                         break;
                    }
                    MNG_BASIC_INFO *p = Client->m_Basic;
                    strcpy(buf, OpenDialog1->FileName.c_str());
                    char *t = strrchr(buf, '.');
                    sprintf(t, ".id");
                    FILE *f2 = fopen(buf, "r+");
                    if(f2)
                    {
                        fread(id, 1, 31, f2);
                        n = strlen(id);
                        while((id[n-1]>0x39)||(id[n-1]<0x30))
                        {
                            id[n-1] = 0;
                            n--;
                        }
                        n-=5;
                        nid = atoi(id+n);
                        sprintf(id+n, "%05u", nid+1);
                        fseek(f2, 0, 0);
                        fwrite(id, strlen(id), 1, f2);
                        fclose(f2);
                    }
                    strcpy(p->id, id);
                    p->ip = inet_addr(ip);
                    p->mask = inet_addr(mask);
                    p->gataway = inet_addr(gw);
                    p->dns[0] = inet_addr(dns);
                    strcpy(p->regSrv, srv);
                    p->srvPort = port;
                    p->protocol = protocol;
                    if(Client)
                    {
                        Client->SetBasic(p);
                    }
                }
                else
                if(!strncmp(buf, "nvr=", 4))
                {
                    MNG_NVR p;
                    int chn, port, type, storage;
                    char ip[16], acc[32], pwd[32];
                    //nvr=通道号，ip地址，端口，账号，密码，设备类型，存储类型
                    if(sscanf(buf+4, "%d,%[^,],%d,%[^,],%[^,],%u,%u",
                     &p.pos, p.ip, &port, p.acc, p.pwd, &type, &storage) < 7)
                    {
                         ShowMessage("NVR参数错误");
                         break;
                    }
                    p.port = port;
                    p.type = type;
                    p.stroage = storage;
                    Client->SetNvr(p.pos, &p);
                }
                else
                if(!strncmp(buf, "net=", 4))
                {
                    MNG_DYNAMIC_NET_INFO p;
                    memset((char *)&p, 0, sizeof(p));

                    if(sscanf(buf+4, "%d,%d,%d,%d,%d,%d,%d,%[^,],",
                     &p.dhcpEn, &p.upnpEn, &p.portMapEn, &p.dhcpdEn,
                     &p.routerEn, &p.routerFlow, &p.mobileEn, p.APN
                     ) < 8)
                    {
                         ShowMessage("NVR参数错误");
                         break;
                    }
                    Client->SetDynamicNetInfo(&p);
                }
            }
            fclose(f);
            Client->SendGetBaseInfo();
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::PageControl4Change(TObject *Sender)
{
    if(!Client)
        return;

    if(PageControl4->ActivePage == TabSheet7)
    {
        Client->SendMsg(MNG_CMD_GET_ENV, 0, 0);
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Button44Click(TObject *Sender)
{
    int i,j=0;     
    if(!Client)
        return;
    DEV_ENV_SET * e = Client->m_env;
    for(i = 0; i < CheckListBox1->Count; i++)
    {
        if(CheckListBox1->Checked[i])
            e[j++].type = i+1;
       // else
       //     e[i].type = 0;
        e[i].chnNum = Zones->Value;
    }
    for(i = j; i < 20;/*CheckListBox1->Count;*/ i++)
        e[i].type = 0;
	
    Client->SendMsg(MNG_CMD_SET_ENV, e, sizeof(DEV_ENV_SET)*20);	//CheckListBox1->Count);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IR1Click(TObject *Sender)
{
    MNG_IR v= {0};
    TSpeedButton *b = (TSpeedButton *)Sender;

    v.mode = IRStatus->Checked;
    v.type = (b->Tag >> 8);
    v.action[0] = b->Tag&0xff;
    Client->SendMsg(MNG_CMD_SET_IR, &v, sizeof(v));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button45Click(TObject *Sender)
{
     TListItem *item = SwitchChnList->Selected;
     if(!item)
     {
         ShowMessage("请选择开关");
         return;
     }
     if(MessageBox(Handle, "确定删除？", "确认", MB_OKCANCEL) == IDOK)
         SwitchChnList->DeleteSelected();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button47Click(TObject *Sender)
{
    TListItem *item;

    if(SwitchDevList->Items->Count <= 0)
    {
        ShowMessage("请先增加控制器");
        return;
    }

    TSwitchForm *fm = new TSwitchForm(this);
    if(!fm)
        return;
    if(fm->ShowModal() == mrOk)
    {
        SWITCH_CHN p = {0};
        char buf[16];
        char *st[] = SWITCH_TYPE_STRs;
        char *mt[] = SWITCH_MODE_STRs;
        fm->GetPara(&p);
        item = SwitchChnList->Items->Add();
        item->Data = (void *)p.type;
        item->Caption = st[p.type];
        sprintf(buf, "%d", p.chn+1);
        item->SubItems->Add(buf);
        item->SubItems->Add(mt[p.mode]);
        sprintf(buf, "%d", p.tim);
        item->SubItems->Add(buf);
        sprintf(buf, "%d", p.sid+1);
        item->SubItems->Add(buf);
        sprintf(buf, "%d", p.schn+1);
        item->SubItems->Add(buf);
        sprintf(buf, "%d", p.limit);
        item->SubItems->Add(buf);   
        sprintf(buf, "%d", p.delay);
        item->SubItems->Add(buf);
    }
    delete fm;
}
//---------------------------------------------------------------------------
void TMainForm::SwitchChnBuild(SWITCH_CHN *p, TListItem *item)
{
    p->type = (_UINT16)item->Data;
    p->chn = item->SubItems->Strings[0].ToInt()-1;
    if(item->SubItems->Strings[1] == "普通")
        p->mode = 0;
    else
        p->mode = 1;
    p->tim = item->SubItems->Strings[2].ToInt();
    p->sid = item->SubItems->Strings[3].ToInt()-1;
    p->schn = item->SubItems->Strings[4].ToInt()-1;
    p->limit = item->SubItems->Strings[5].ToInt();
    p->delay = item->SubItems->Strings[6].ToInt();
}

void __fastcall TMainForm::Button48Click(TObject *Sender)
{
     SWITCH_CHN p = {0};
     TListItem *item = SwitchChnList->Selected;
     if(!item)
     {
         ShowMessage("请选择开关");
         return;
     }
    TSwitchForm *fm = new TSwitchForm(this);
    if(!fm)
        return;
    SwitchChnBuild(&p, item);
    fm->SetPara(&p);
    if(fm->ShowModal() == mrOk)
    {
        char buf[16];
        char *st[] = SWITCH_TYPE_STRs;
        char *mt[] = SWITCH_MODE_STRs;
        fm->GetPara(&p);
        item->Data = (void *)p.type;
        item->Caption = st[p.type];
        sprintf(buf, "%d", p.chn+1);
        item->SubItems->Strings[0] = buf;
        item->SubItems->Strings[1] = mt[p.mode];
        sprintf(buf, "%d", p.tim);
        item->SubItems->Strings[2] = buf;
        sprintf(buf, "%d", p.sid+1);
        item->SubItems->Strings[3] = buf;
        sprintf(buf, "%d", p.schn+1);
        item->SubItems->Strings[4] = buf;
        sprintf(buf, "%d", p.limit);
        item->SubItems->Strings[5] = buf;   
        sprintf(buf, "%d", p.delay);
        item->SubItems->Strings[6] = buf;
    }
    delete fm;
}
//---------------------------------------------------------------------------
void TMainForm::SwitchDevBuild(SWITCH_DEV *p, TListItem *item)
{
    p->type = (_UINT8)item->Data;
    p->id = item->SubItems->Strings[1].ToInt();
    p->chnNum = item->SubItems->Strings[2].ToInt();
}

void __fastcall TMainForm::Button51Click(TObject *Sender)
{
    SWITCH_DEV p = {0};
    TListItem *item = SwitchDevList->Selected;
    TSwitchDevForm *fm = new TSwitchDevForm(this);
    if(item)
    {
        SwitchDevBuild(&p, item);
        fm->SetPara(&p);
    }
    if(fm->ShowModal() == mrOk)
    {
        char *sdt[] = SWITCH_DEV_TYPE_STRs;
        fm->GetPara(&p);
        if(!item) 
        {
            item = SwitchDevList->Items->Add();
            item->Caption = SwitchDevList->Items->Count;   
            item->SubItems->Add(sdt[p.type]);
            item->SubItems->Add(p.id);
            item->SubItems->Add(p.chnNum);
        }
        else
        {
            item->SubItems->Strings[0] = sdt[p.type];
            item->SubItems->Strings[1] = p.id;
            item->SubItems->Strings[2] = p.chnNum;
        }
        item->Data = (void *)p.type;
    }
    delete fm;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PageControl5Change(TObject *Sender)
{
    if(PageControl5->ActivePage == SwitchSheet)
    {    
        Client->SendMsg(MNG_CMD_GET_SWITCH, 0, 0);
    }
    else if(PageControl5->ActivePage == tbsheetflogctrl)
    {
        Client->SendMsg(MNG_CMD_GET_ENV, 0, 0);
    }
    else
    {
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button46Click(TObject *Sender)
{
    int chnNum = SwitchChnList->Items->Count;
    int i, len = sizeof(SWITCH_INFO)+sizeof(SWITCH_CHN)*(chnNum-1);
    SWITCH_INFO *s = (SWITCH_INFO *)(new BYTE[len]);
    if(!s)
        return;
    memset(s, 0, len);
    s->devNum = SwitchDevList->Items->Count;
    s->chnNum = chnNum;

    for(i = 0; i < s->devNum; i++)
    {
        SwitchDevBuild(&s->devs[i], SwitchDevList->Items->Item[i]);
    }
    for(i = 0; i < chnNum; i++)
    {
        SwitchChnBuild(&s->chns[i], SwitchChnList->Items->Item[i]);
    }                              
    Client->SendMsg(MNG_CMD_SET_SWITCH, s, len);
    delete s;
}

void TMainForm::setFlogBtnEnable(bool bst)
{
      rbtn_flogStart1->Enabled = bst;
      rbtn_flogStop1->Enabled = bst;
      rbtn_flogStart2->Enabled = bst;
      rbtn_flogStop2->Enabled = bst;
      rbtn_flogStart3->Enabled = bst;
      rbtn_flogStop3->Enabled = bst;
      rbtn_flogStart4->Enabled = bst;
      rbtn_flogStop4->Enabled = bst;
      rbtn_flogStart5->Enabled = bst;
      rbtn_flogStop5->Enabled = bst;
      rbtn_flogStart6->Enabled = bst;
      rbtn_flogStop6->Enabled = bst;
      rbtn_flogStart7->Enabled = bst;
      rbtn_flogStop7->Enabled = bst;
      rbtn_flogStart8->Enabled = bst;
      rbtn_flogStop8->Enabled = bst;
      rbtn_flogStart9->Enabled = bst;
      rbtn_flogStop9->Enabled = bst;
      rbtn_flogStart10->Enabled = bst;
      rbtn_flogStop10->Enabled = bst;
      rbtn_flogStart11->Enabled = bst;
      rbtn_flogStop11->Enabled = bst;
      rbtn_flogStart12->Enabled = bst;
      rbtn_flogStop12->Enabled = bst;
      rbtn_flogStart13->Enabled = bst;
      rbtn_flogStop13->Enabled = bst;
      rbtn_flogStart14->Enabled = bst;
      rbtn_flogStop14->Enabled = bst;
      rbtn_flogStart15->Enabled = bst;
      rbtn_flogStop15->Enabled = bst;
      rbtn_flogStart16->Enabled = bst;
      rbtn_flogStop16->Enabled = bst;
      rbtn_flogStart17->Enabled = bst;
      rbtn_flogStop17->Enabled = bst;
      rbtn_flogStart18->Enabled = bst;
      rbtn_flogStop18->Enabled = bst;
      rbtn_flogStart19->Enabled = bst;
      rbtn_flogStop19->Enabled = bst;
      rbtn_flogStart20->Enabled = bst;
      rbtn_flogStop20->Enabled = bst;

}

void TMainForm::setParmToFlogCtrl(DEV_ENV_SET env)
{
    cbox_flogcltlauto->Checked = env.actionSt;
    et_pm10maxval->Text = IntToStr(env.highLimit);
    et_pm10minval->Text = IntToStr(env.lowLimit);
    et_timeout1->Text   = IntToStr(env.timing);
    et_timeout2->Text   = IntToStr(env.timing2);

    cbox_flog1->Checked = env.action & (0x01 << 0);
    cbox_flog2->Checked = env.action & (0x01 << 1);
    cbox_flog3->Checked = env.action & (0x01 << 2);
    cbox_flog4->Checked = env.action & (0x01 << 3);
    cbox_flog5->Checked = env.action & (0x01 << 4);
    cbox_flog6->Checked = env.action & (0x01 << 5);
    cbox_flog7->Checked = env.action & (0x01 << 6);
    cbox_flog8->Checked = env.action & (0x01 << 7);
    cbox_flog9->Checked = env.action & (0x01 << 8);
    cbox_flog10->Checked = env.action & (0x01 << 9);
    cbox_flog11->Checked = env.action & (0x01 << 10);
    cbox_flog12->Checked = env.action & (0x01 << 11);
    cbox_flog13->Checked = env.action & (0x01 << 12);
    cbox_flog14->Checked = env.action & (0x01 << 13);
    cbox_flog15->Checked = env.action & (0x01 << 14);
    cbox_flog16->Checked = env.action & (0x01 << 15);
    cbox_flog17->Checked = env.action & (0x01 << 16);
    cbox_flog18->Checked = env.action & (0x01 << 17);
    cbox_flog19->Checked = env.action & (0x01 << 18);
    cbox_flog20->Checked = env.action & (0x01 << 19);

    if(env.haction & (0x01 << 0))
       rbtn_flogStart1->Checked = 1;
    else
       rbtn_flogStop1->Checked = 1;

    if(env.haction & (0x01 << 1))
       rbtn_flogStart2->Checked = 1;
    else
       rbtn_flogStop2->Checked = 1;

    if(env.haction & (0x01 << 2))
       rbtn_flogStart3->Checked = 1;
    else
       rbtn_flogStop3->Checked = 1;

    if(env.haction & (0x01 << 3))
       rbtn_flogStart4->Checked = 1;
    else
       rbtn_flogStop4->Checked = 1;

    if(env.haction & (0x01 << 4))
       rbtn_flogStart5->Checked = 1;
    else
       rbtn_flogStop5->Checked = 1;

    if(env.haction & (0x01 << 5))
       rbtn_flogStart6->Checked = 1;
    else
       rbtn_flogStop6->Checked = 1;

    if(env.haction & (0x01 << 6))
       rbtn_flogStart7->Checked = 1;
    else
       rbtn_flogStop7->Checked = 1;

    if(env.haction & (0x01 << 7))
       rbtn_flogStart8->Checked = 1;
    else
       rbtn_flogStop8->Checked = 1;
    if(env.haction & (0x01 << 8))
       rbtn_flogStart9->Checked = 1;
    else
       rbtn_flogStop9->Checked = 1;
    if(env.haction & (0x01 << 9))
       rbtn_flogStart10->Checked = 1;
    else
       rbtn_flogStop10->Checked = 1;
    if(env.haction & (0x01 << 10))
       rbtn_flogStart11->Checked = 1;
    else
       rbtn_flogStop11->Checked = 1;
    if(env.haction & (0x01 << 11))
       rbtn_flogStart12->Checked = 1;
    else
       rbtn_flogStop12->Checked = 1;
    if(env.haction & (0x01 << 12))
       rbtn_flogStart13->Checked = 1;
    else
       rbtn_flogStop13->Checked = 1;
    if(env.haction & (0x01 << 13))
       rbtn_flogStart14->Checked = 1;
    else
       rbtn_flogStop14->Checked = 1;
    if(env.haction & (0x01 << 14))
       rbtn_flogStart15->Checked = 1;
    else
       rbtn_flogStop15->Checked = 1;
    if(env.haction & (0x01 << 15))
       rbtn_flogStart16->Checked = 1;
    else
       rbtn_flogStop16->Checked = 1;
    if(env.haction & (0x01 << 16))
       rbtn_flogStart17->Checked = 1;
    else
       rbtn_flogStop17->Checked = 1;
    if(env.haction & (0x01 << 17))
       rbtn_flogStart18->Checked = 1;
    else
       rbtn_flogStop18->Checked = 1;
    if(env.haction & (0x01 << 18))
       rbtn_flogStart19->Checked = 1;
    else
       rbtn_flogStop19->Checked = 1;
       
    if(env.haction & (0x01 << 19))
       rbtn_flogStart20->Checked = 1;
    else
       rbtn_flogStop20->Checked = 1;

     setFlogBtnEnable(!cbox_flogcltlauto->Checked);

}
//---------------------------------------------------------------------------
int TMainForm::GetFlogSt()
{
    int flogBtnSt = 0x00;

    flogBtnSt |= ((0x01 & rbtn_flogStart1->Checked) << 0);
    flogBtnSt |= ((0x01 & rbtn_flogStart2->Checked) << 1);
    flogBtnSt |= ((0x01 & rbtn_flogStart3->Checked) << 2);
    flogBtnSt |= ((0x01 & rbtn_flogStart4->Checked) << 3);
    flogBtnSt |= ((0x01 & rbtn_flogStart5->Checked) << 4);
    flogBtnSt |= ((0x01 & rbtn_flogStart6->Checked) << 5);
    flogBtnSt |= ((0x01 & rbtn_flogStart7->Checked) << 6);
    flogBtnSt |= ((0x01 & rbtn_flogStart8->Checked) << 7);
    flogBtnSt |= ((0x01 & rbtn_flogStart9->Checked) << 8);
    flogBtnSt |= ((0x01 & rbtn_flogStart10->Checked) << 9);
    flogBtnSt |= ((0x01 & rbtn_flogStart11->Checked) << 10);
    flogBtnSt |= ((0x01 & rbtn_flogStart12->Checked) << 11);
    flogBtnSt |= ((0x01 & rbtn_flogStart13->Checked) << 12);
    flogBtnSt |= ((0x01 & rbtn_flogStart14->Checked) << 13);
    flogBtnSt |= ((0x01 & rbtn_flogStart15->Checked) << 14);
    flogBtnSt |= ((0x01 & rbtn_flogStart16->Checked) << 15);
    flogBtnSt |= ((0x01 & rbtn_flogStart17->Checked) << 16);
    flogBtnSt |= ((0x01 & rbtn_flogStart18->Checked) << 17);
    flogBtnSt |= ((0x01 & rbtn_flogStart19->Checked) << 18);
    flogBtnSt |= ((0x01 & rbtn_flogStart20->Checked) << 19);

    return flogBtnSt;
}
//---------------------------------------------------------------------------
int TMainForm::flogSel()
{
    int selSt = 0x00;

    selSt |= ((0x01 & cbox_flog1->Checked) << 0);
    selSt |= ((0x01 & cbox_flog2->Checked) << 1);
    selSt |= ((0x01 & cbox_flog3->Checked) << 2);
    selSt |= ((0x01 & cbox_flog4->Checked) << 3);
    selSt |= ((0x01 & cbox_flog5->Checked) << 4);
    selSt |= ((0x01 & cbox_flog6->Checked) << 5);
    selSt |= ((0x01 & cbox_flog7->Checked) << 6);
    selSt |= ((0x01 & cbox_flog8->Checked) << 7);
    selSt |= ((0x01 & cbox_flog9->Checked) << 8);
    selSt |= ((0x01 & cbox_flog10->Checked) << 9);
    selSt |= ((0x01 & cbox_flog11->Checked) << 10);
    selSt |= ((0x01 & cbox_flog12->Checked) << 11);
    selSt |= ((0x01 & cbox_flog13->Checked) << 12);
    selSt |= ((0x01 & cbox_flog14->Checked) << 13);
    selSt |= ((0x01 & cbox_flog15->Checked) << 14);
    selSt |= ((0x01 & cbox_flog16->Checked) << 15);
    selSt |= ((0x01 & cbox_flog17->Checked) << 16);
    selSt |= ((0x01 & cbox_flog18->Checked) << 17);
    selSt |= ((0x01 & cbox_flog19->Checked) << 18);
    selSt |= ((0x01 & cbox_flog20->Checked) << 19);

    return selSt;
}

void __fastcall TMainForm::btn_flogctrlClick(TObject *Sender)
{
    int i,j=0;
    if(!Client)
        return;
    DEV_ENV_SET * e = Client->m_env;
    for(i = 0; i < 20; i++)
    {
        if(e[i].type == 7) //找到PM10项
        {
            break;
        }
    }

    if(i >= 20)
        return;

    e[i].actionSt = cbox_flogcltlauto->Checked;
    e[i].highLimit = StrToInt(et_pm10maxval->Text);
    e[i].lowLimit = StrToInt(et_pm10minval->Text);
    e[i].timing   = StrToInt(et_timeout1->Text);
    e[i].timing2  = StrToInt(et_timeout2->Text);

    e[i].action   = flogSel();
    e[i].haction  = GetFlogSt();
	
    Client->SendMsg(MNG_CMD_SET_ENV, e, sizeof(DEV_ENV_SET)*20);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::cbox_flogcltlautoClick(TObject *Sender)
{
        setFlogBtnEnable(!cbox_flogcltlauto->Checked);
}
//---------------------------------------------------------------------------

