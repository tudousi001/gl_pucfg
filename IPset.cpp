//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IPset.h"
#include "winsock2.h"
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIPSetForm *IPSetForm;

extern void ip2ip(UINT32 ip, char *ipstring);
//---------------------------------------------------------------------------
__fastcall TIPSetForm::TIPSetForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TIPSetForm::IPKeyPress(TObject *Sender, char &Key)
{
    char ipbuf[16];
    long ip;
    ip = inet_addr(IP->Text.c_str());
    if(ip == -1)
        return;
    ip = (ip&0x00ffffff) | 0x01000000;
    ip2ip(ip, ipbuf);
    Gataway->Text = ipbuf;
}
//---------------------------------------------------------------------------

