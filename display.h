//---------------------------------------------------------------------------

#ifndef displayH
#define displayH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>        
#include "IBaseNetView.h"
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include "ZTrackBar.h"
#include "CSPIN.h"
#include "mngmsg.h"
//---------------------------------------------------------------------------

typedef long (__stdcall *ENCODE_CALLBACK)(long lType, long data, long length);

class TDisplayForm : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *PopupMenu1;
        TMenuItem *N1;
    TPanel *Panel1;
    TStatusBar *StatusBar1;
        TPanel *Panel2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TButton *Button1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        TLabel *Label5;
        TLabel *Label6;
        ZTrackBar *TrackBar1;
        ZTrackBar *TrackBar2;
        ZTrackBar *TrackBar3;
        ZTrackBar *TrackBar4;
        TButton *Button2;
        TButton *Button3;
        TGroupBox *GroupBox1;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton6;
        TSpeedButton *SpeedButton7;
        TSpeedButton *SpeedButton8;
        TSpeedButton *SpeedButton9;
        TSpeedButton *SpeedButton10;
        TCheckBox *CheckBox1;
        TLabel *Label7;
        TCSpinEdit *Preset;
        TSpeedButton *SpeedButton11;
        TLabel *Label8;
        TLabel *Label9;
        ZTrackBar *PhoneVolum;
        ZTrackBar *MicVolum;
        TButton *Button4;
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall N1Click(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall SpeedButton1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall SpeedButton1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall TrackBar1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall SpeedButton5Click(TObject *Sender);
        void __fastcall SpeedButton11Click(TObject *Sender);
        void __fastcall VolumMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Button4Click(TObject *Sender);
private:	// User declarations  
       IBaseNetView *m_Base;
       long m_player;
       long m_talker;
       int m_chn;
       int m_PtzMode;
       
public:		// User declarations
        __fastcall TDisplayForm(TComponent* Owner);
        int Init(int chn);
        int AddData(void *data, int len);
        int RefreshRate(int rate);
        void AddInfo(int n, AnsiString Info);
        void SetDisplay(int bright, int contrast, int saturation, int hue);
        int StopTalk();
        int StartTalk(ENCODE_CALLBACK CallBk);
        void SetVolum(VOLUME *volum);
};
//---------------------------------------------------------------------------
extern PACKAGE TDisplayForm *DisplayForm;
//---------------------------------------------------------------------------
#endif
