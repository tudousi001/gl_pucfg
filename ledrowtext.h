//---------------------------------------------------------------------------

#ifndef ledrowtextH
#define ledrowtextH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ledshowCtrl.h"
//---------------------------------------------------------------------------
class TrowTextInfo : public TForm
{
__published:	// IDE-managed Components
        TComboBox *inAreaNo;
        TLabel *Label1;
        TComboBox *rowNo;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TComboBox *fontNo;
        TComboBox *alignmode;
        TEdit *Edit1;
        TEdit *defText;
        TButton *Button1;
        TButton *Button2;
private:	// User declarations
public:		// User declarations
        __fastcall TrowTextInfo(TComponent* Owner);
        int GetParam(STRTXLINEINFO *prowinfo);
        int SetParam(STRTXLINEINFO *prowinfo);
};
//---------------------------------------------------------------------------
extern PACKAGE TrowTextInfo *rowTextInfo;
//---------------------------------------------------------------------------
#endif
