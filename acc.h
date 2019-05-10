//---------------------------------------------------------------------------

#ifndef accH
#define accH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "mngmsg.h"
//---------------------------------------------------------------------------
class TAccForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label213;
        TComboBox *Level;
        TEdit *Acc;
        TEdit *Pwd;
        TEdit *Des;
        TLabel *Label206;
        TLabel *Label179;
        TLabel *Label177;
        TButton *Button1;
        TButton *Button2;
private:	// User declarations

public:		// User declarations
        __fastcall TAccForm(TComponent* Owner);
        void SetAcc(MNG_ACC *acc);
        void GetAcc(MNG_ACC *acc);
};
//---------------------------------------------------------------------------
extern PACKAGE TAccForm *AccForm;
//---------------------------------------------------------------------------
#endif
