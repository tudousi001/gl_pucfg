//---------------------------------------------------------------------------

#ifndef switchH
#define switchH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"     
#include "ioctrl.h"
//---------------------------------------------------------------------------
class TSwitchForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label60;
        TComboBox *Sid;
        TComboBox *Mode;
        TLabel *Label35;
        TComboBox *Chn;
        TLabel *Label73;
        TLabel *Label2;
        TLabel *Label1;
        TCSpinEdit *Tim;
        TButton *Button1;
        TButton *Button2;
        TLabel *Label6;
        TComboBox *Type;
        TComboBox *Schn;
        TLabel *Label3;
        TComboBox *Limit;
        TLabel *Label4;
        TCSpinEdit *Delay;
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSwitchForm(TComponent* Owner);
        int GetPara(SWITCH_CHN *para);
        int SetPara(SWITCH_CHN *para);
};
//---------------------------------------------------------------------------
extern PACKAGE TSwitchForm *SwitchForm;
//---------------------------------------------------------------------------
#endif
