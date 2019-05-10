//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SwitchDev.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TSwitchDevForm *SwitchDevForm;
//---------------------------------------------------------------------------
__fastcall TSwitchDevForm::TSwitchDevForm(TComponent* Owner)
        : TForm(Owner)
{    
    int i;
    char *st[] = SWITCH_DEV_TYPE_STRs;
    for(i = 0; i < e_SWITCH_DEV_TYPE_TOTAL; i++)
    {
         Type->Items->Add(st[i]);
    }
}
//---------------------------------------------------------------------------

int TSwitchDevForm::GetPara(SWITCH_DEV *para)
{
    para->type = Type->ItemIndex;
    para->chnNum = ChnNum->Value;
    para->id = Id->Value;
}

int TSwitchDevForm::SetPara(SWITCH_DEV *para)
{
    ChnNum->Value = para->chnNum;
    Id->Value = para->id;
    Type->ItemIndex = para->type;
}

