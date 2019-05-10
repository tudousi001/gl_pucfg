//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>
#pragma hdrstop

#include "ledareaconfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TareaConfig *areaConfig;
//---------------------------------------------------------------------------
__fastcall TareaConfig::TareaConfig(TComponent* Owner)
        : TForm(Owner)
{
}

int TareaConfig::GetParam(STRAREAINFOS *parea)
{
        parea->areaNo = CmbAreaNo->ItemIndex;
        parea->rowcount = CmbTextRows->ItemIndex+1;
        parea->areaRect.x =  StrToInt(xPos->Text);
        parea->areaRect.y = StrToInt(yPos->Text);
        parea->areaRect.aw = StrToInt(areaW->Text);
        parea->areaRect.ah = StrToInt(areaH->Text);
        return 0;
}

int TareaConfig::SetParam(STRAREAINFOS *parea)
{
        CmbAreaNo->ItemIndex = parea->areaNo;
        CmbTextRows->ItemIndex = parea->rowcount-1;

        xPos->Text = IntToStr(parea->areaRect.x);
        yPos->Text = IntToStr(parea->areaRect.y);
        areaW->Text = IntToStr(parea->areaRect.aw);
        areaH->Text = IntToStr(parea->areaRect.ah);
        
        return 0;
}
//---------------------------------------------------------------------------


