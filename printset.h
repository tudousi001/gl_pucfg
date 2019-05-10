//---------------------------------------------------------------------------

#ifndef printsetH
#define printsetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPrintForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TGroupBox *GroupPortSel;
        TRadioButton *RadioButtonUsb;
        TRadioButton *RadioButtonCom;
        TRadioButton *RadioButtonLpt;
        TButton *Button1;
        TButton *Button2;
        TGroupBox *GroupPortSet;
        TLabel *Label1;
        TComboBox *cbPrinter;
        TLabel *Label2;
        TComboBox *cbComPortName;
        TLabel *Label3;
        TComboBox *cbBitRate;
        TLabel *Label4;
        TComboBox *cbDataBit;
        TLabel *Label5;
        TComboBox *cbCheck;
        TLabel *Label6;
        TComboBox *cbStopBit;
        TLabel *Label7;
        TComboBox *cbHardCtrl;
        TComboBox *cbUsbSel;
        TComboBox *cbLptSel;
        TLabel *Label8;
        TComboBox *cbSize;
        TComboBox *cbPaperType;
        TComboBox *cbPrintRever;
        TComboBox *cbParperOut;
        TComboBox *cbPrintSpeed;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TComboBox *cbPrintDensity;
        TComboBox *cbPrintMode;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall RadioButtonComClick(TObject *Sender);
        void __fastcall RadioButtonUsbClick(TObject *Sender);
        void __fastcall RadioButtonLptClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TPrintForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPrintForm *PrintForm;
//---------------------------------------------------------------------------
#endif
