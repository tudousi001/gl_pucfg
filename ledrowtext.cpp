//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ledrowtext.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TrowTextInfo *rowTextInfo;
//---------------------------------------------------------------------------
__fastcall TrowTextInfo::TrowTextInfo(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TrowTextInfo::GetParam(STRTXLINEINFO *prowinfo)
{
        prowinfo->areaNo = inAreaNo->ItemIndex;
        prowinfo->rowNo =  rowNo->ItemIndex;
        prowinfo->fontType = fontNo->ItemIndex;
        prowinfo->aglian =  alignmode->ItemIndex;
        prowinfo->prelineintal = StrToInt(Edit1->Text);
        strcpy(prowinfo->text,defText->Text.c_str());
        return 0;
}

int TrowTextInfo::SetParam(STRTXLINEINFO *prowinfo)
{
       inAreaNo->ItemIndex = prowinfo->areaNo;
       rowNo->ItemIndex = prowinfo->rowNo;
       fontNo->ItemIndex = prowinfo->fontType;
       alignmode->ItemIndex = prowinfo->aglian;
       Edit1->Text = IntToStr(prowinfo->prelineintal);
       defText->Text = prowinfo->text;
        
       return 0;
}


