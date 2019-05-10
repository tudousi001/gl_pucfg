//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "display.h"
#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZTrackBar"
#pragma link "CSPIN"
#pragma resource "*.dfm"

extern CMClient *Client;

TDisplayForm *DisplayForm;


//---------------------------------------------------------------------------
__fastcall TDisplayForm::TDisplayForm(TComponent* Owner)
        : TForm(Owner)
{
    m_PtzMode = -1;
    m_Base = 0;
}
//---------------------------------------------------------------------------
int TDisplayForm::Init(int chn)
{
    const IID IID_IBaseNetView = {0xF3C4C7BB,0x1D4B,0x4983,{0x97,0x3C,0x88,0xF3,0x97,0x87,0xB8,0xAD}};
    //const IID Clsid = {0xCC895CDD, 0x0EDB, 0x4E46, {0xB6, 0xB4, 0x46, 0x4E, 0xE2, 0x84, 0xBB, 0xAC}};
    const IID Clsid = {0x81468EDD, 0x0A8F, 0x4F35, {0x88, 0xA5, 0x1A, 0xAF, 0x50, 0x26, 0xB7, 0xC2}};

    m_chn = chn;
    HRESULT hr = CoInitialize(NULL);
    if(FAILED(hr))
    {
     	ShowMessage("Com库初始化失败");
        return -1;
    }

    hr = CoCreateInstance(Clsid,NULL,CLSCTX_INPROC_SERVER,IID_IBaseNetView,(void **)&m_Base);
    if (FAILED(hr))
    {
        ShowMessage("调用Com接口失败!");
        return -1;
    }
    m_Base->Initial((long)Panel1->Handle);
    m_Base->OpenStream(&m_player, 0, 0);
    m_Base->PlaySound(m_player);

    return 0;
}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
    if(m_Base)
    {
        m_Base->Free();
        m_Base->Release();
        m_Base = NULL;
    }
    CoUninitialize();
    PostMessage(MainForm->Handle, WM_MNG_DISPLAY_END, m_chn, 0);
}
//---------------------------------------------------------------------------

int TDisplayForm::AddData(void *data, int len)
{
    if(m_Base)
    {
        m_Base->InputData(m_player, (long *)data, len);
    }
    return 0;
}

int TDisplayForm::RefreshRate(int rate)
{
    //Caption = "视频(K):" + IntToStr(rate);
    AddInfo(1, "码流:"+ IntToStr(rate)+"K");
    return 0;
}

void __fastcall TDisplayForm::N1Click(TObject *Sender)
{
    if(m_Base)
    {               
        WideString w = "d:\\cap.bmp";

        m_Base->CapPic(m_player, w.c_bstr());
    }
}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int i, b;
    HRGN rgn;
    int mH = ClientHeight/2, mW = ClientWidth/2;
    POINT p[4][3] = {
      {{0, 0}, {ClientWidth, 0}, {mW, mH}},
      {{0, 0}, {0, ClientHeight}, {mW, mH}},
      {{ClientWidth, ClientHeight}, {0, ClientHeight}, {mW, mH}},
      {{ClientWidth, ClientHeight}, {ClientWidth, 0}, {mW, mH}},
      };

    for(i = 0; i < 4; i++)
    {
        rgn = CreatePolygonRgn(p[i], 3, WINDING);
        if(rgn)
        {
        b = PtInRegion(rgn, X, Y);
        DeleteObject(rgn);
        if(b)
            break;
        }
    }

    switch(i)
    {
        case 0:
            m_PtzMode = 1;
            break;
        case 1:
            m_PtzMode = 3;
            break;   
        case 2:
            m_PtzMode = 5;
            break;
        case 3:
            m_PtzMode = 7;
            break;
        default:
            m_PtzMode = -1;
            return;
    }
    if((m_PtzMode > 0) && Client)
    {
        Client->SendPtzCmd(m_chn, m_PtzMode, 0, 8);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if((m_PtzMode > 0) && Client)
    {
        Client->SendPtzCmd(m_chn, m_PtzMode, 1, 8);
    }
}
//---------------------------------------------------------------------------

void TDisplayForm::AddInfo(int n, AnsiString Info)
{
    if(n < 0)
    {
        Caption = Info;
    }
    else
    {
        StatusBar1->Panels->Items[n]->Text = Info;
    }
}

void TDisplayForm::SetDisplay(int bright, int contrast, int saturation, int hue)
{
    TrackBar1->Position = bright;
    TrackBar2->Position = contrast;
    TrackBar3->Position = saturation;
    TrackBar4->Position = hue;
}

void TDisplayForm::SetVolum(VOLUME *volum)
{
    MicVolum->Position = volum->micVol;
    PhoneVolum->Position = volum->phoneVol;
}

void __fastcall TDisplayForm::SpeedButton1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int mode = ((TSpeedButton*)Sender)->Tag;
    if(Client)
    {
        Client->SendPtzCmd(m_chn, mode, 0, 8);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::SpeedButton1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int mode = ((TSpeedButton*)Sender)->Tag;
    if(Client)
    {
        Client->SendPtzCmd(m_chn, mode, 1, 8);
    }
}
//---------------------------------------------------------------------------


void __fastcall TDisplayForm::Button1Click(TObject *Sender)
{
    if(Client)
    {
        Client->SetDisplay(m_chn, -1, -1, -1, -1);
        Client->SendParaGetCmd(m_chn, MNG_CMD_GET_DISPLAY);
    }
}
//---------------------------------------------------------------------------




void __fastcall TDisplayForm::TrackBar1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Client)
    {
        Client->SetDisplay(m_chn, TrackBar1->Position, TrackBar2->Position,
            TrackBar3->Position, TrackBar4->Position);
    }
}
//---------------------------------------------------------------------------

int TDisplayForm::StopTalk()
{
    if(!m_Base || !m_talker)
        return -1;

    m_Base->CloseAudio(m_talker);
    m_talker = 0;

    return 0;
}

int TDisplayForm::StartTalk(ENCODE_CALLBACK CallBk)
{
    if(!m_Base || !CallBk)
        return -1;

    if(m_talker)
    {
        StopTalk();
    }
    
    /*if(PlaySound(1))
    {
        return -1;
    } */

    if(m_Base->OpenAudio(&m_talker, 1, 0))
    {
        return -1;
    }

    m_Base->SetCallBack(m_talker, (long)CallBk);

    return 0;
}

void __fastcall TDisplayForm::Button2Click(TObject *Sender)
{
    MainForm->ControlTalk();
}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::Button3Click(TObject *Sender)
{
    MainForm->StopTalk();         
}
//---------------------------------------------------------------------------


void __fastcall TDisplayForm::SpeedButton5Click(TObject *Sender)
{
    int mode = CheckBox1->Checked ? 0:2;
    int id = ((TSpeedButton*)Sender)->Tag;
    if(Client)
    {
        Client->SendPresetCmd(m_chn, mode, id, 8);
    }

}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::SpeedButton11Click(TObject *Sender)
{
    int id = Preset->Value;
    if(Client)
    {
        Client->SendPresetCmd(m_chn, 2, id, 8);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::VolumMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Client)
    {
        VOLUME volume;
        volume.phoneVol = PhoneVolum->Position;
        volume.micVol = MicVolum->Position;
        Client->SetVolume(&volume);
        //Client->SetVolum(MicVolum->Position, PhoneVolum->Position);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDisplayForm::Button4Click(TObject *Sender)
{
    if(Client)
    {
        Client->SendMsg(MNG_CMD_SET_VIDEO_FLIP, (char *)&m_chn,
            sizeof(m_chn));
    }
}
//---------------------------------------------------------------------------

