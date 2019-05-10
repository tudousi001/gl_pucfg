//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "acc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAccForm *AccForm;
//---------------------------------------------------------------------------
__fastcall TAccForm::TAccForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void TAccForm::SetAcc(MNG_ACC *acc)
{
    Acc->Text = acc->name;
    Acc->Enabled = 0;
    Des->Text = acc->description;
    Level->ItemIndex = acc->level;
}

void TAccForm::GetAcc(MNG_ACC *acc)
{                  
    strcpy(acc->name, Acc->Text.c_str());
    strcpy(acc->description, Des->Text.c_str());   
    strcpy(acc->pwd, Pwd->Text.c_str());
    acc->level = Level->ItemIndex;
}
