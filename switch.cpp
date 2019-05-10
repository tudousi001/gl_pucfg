//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "switch.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TSwitchForm *SwitchForm;
//---------------------------------------------------------------------------
__fastcall TSwitchForm::TSwitchForm(TComponent* Owner)
        : TForm(Owner)
{
    
}
//---------------------------------------------------------------------------

void __fastcall TSwitchForm::FormCreate(TObject *Sender)
{
    int i;
    char *st[] = SWITCH_TYPE_STRs;
    for(i = 0; i < e_SWITCH_TYPE_MAX; i++)
    {
         Type->Items->Add(st[i]);
    }
}
//---------------------------------------------------------------------------
int TSwitchForm::GetPara(SWITCH_CHN *p)
{             
    p->type = Type->ItemIndex;
    p->mode = Mode->ItemIndex;
    p->chn = Chn->ItemIndex;
    p->sid = Sid->ItemIndex;
    p->schn = Schn->ItemIndex;
    p->tim = Tim->Value;
    p->limit = Limit->ItemIndex;
    p->delay = Delay->Value;
}

int TSwitchForm::SetPara(SWITCH_CHN *p)
{
    Type->ItemIndex = p->type;
    Mode->ItemIndex = p->mode;
    Chn->ItemIndex = p->chn;
    Sid->ItemIndex = p->sid;
    Schn->ItemIndex = p->schn;
    Tim->Value = p->tim;
    Limit->ItemIndex = p->limit;
    Delay->Value = p->delay;
}
