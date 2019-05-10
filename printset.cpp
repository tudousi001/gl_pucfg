//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "stdio.h"

#include "printset.h"
#include "printer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPrintForm *PrintForm;
//---------------------------------------------------------------------------
__fastcall TPrintForm::TPrintForm(TComponent* Owner)
        : TForm(Owner)
{
    if(BPLA_CloseCom)
        BPLA_CloseCom();
    if(BPLA_CloseLpt)
        BPLA_CloseLpt();
    if(BPLA_CloseUsb)
        BPLA_CloseUsb();
    UnLoadDll();
    LoadDll();
    RadioButtonUsb->Checked = TRUE;
    cbSize->Enabled = FALSE;
    if(RadioButtonCom->Checked != TRUE)
    {
        cbComPortName->Enabled = FALSE;
        cbBitRate->Enabled = FALSE;
        cbDataBit->Enabled = FALSE;
        cbCheck->Enabled = FALSE;
        cbStopBit->Enabled = FALSE;
        cbHardCtrl->Enabled = FALSE;
    }
    if(RadioButtonUsb->Checked != TRUE)
        cbUsbSel->Enabled = FALSE;
    if(RadioButtonLpt->Checked != TRUE)
        cbLptSel->Enabled = FALSE;     
}

//---------------------------------------------------------------------------



void __fastcall TPrintForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{

    //UnLoadDll();        
}
//---------------------------------------------------------------------------


void __fastcall TPrintForm::RadioButtonComClick(TObject *Sender)
{
    RadioButtonCom->Checked = TRUE;
    cbComPortName->Enabled = TRUE;
    cbBitRate->Enabled = TRUE;
    cbDataBit->Enabled = TRUE;
    cbCheck->Enabled = TRUE;
    cbStopBit->Enabled = TRUE;
    cbHardCtrl->Enabled = TRUE;
    cbUsbSel->Enabled = FALSE;
    cbLptSel->Enabled = FALSE;
}
//---------------------------------------------------------------------------

void __fastcall TPrintForm::RadioButtonUsbClick(TObject *Sender)
{
        cbComPortName->Enabled = FALSE;
        cbBitRate->Enabled = FALSE;
        cbDataBit->Enabled = FALSE;
        cbCheck->Enabled = FALSE;
        cbStopBit->Enabled = FALSE;
        cbHardCtrl->Enabled = FALSE;
    cbUsbSel->Enabled = TRUE;
    cbLptSel->Enabled = FALSE;
    
}
//---------------------------------------------------------------------------

void __fastcall TPrintForm::RadioButtonLptClick(TObject *Sender)
{
        cbComPortName->Enabled = FALSE;
        cbBitRate->Enabled = FALSE;
        cbDataBit->Enabled = FALSE;
        cbCheck->Enabled = FALSE;
        cbStopBit->Enabled = FALSE;
        cbHardCtrl->Enabled = FALSE;
    cbUsbSel->Enabled = FALSE;
    cbLptSel->Enabled = TRUE;
}
//---------------------------------------------------------------------------

void __fastcall TPrintForm::Button1Click(TObject *Sender)
{
    char comx[16];
    int comBateRate;
    int lptAddr;
    int state = 0;
    
    if(BPLA_CloseCom)
        BPLA_CloseCom();
    if(BPLA_CloseLpt)
        BPLA_CloseLpt();
    if(BPLA_CloseUsb)
        BPLA_CloseUsb();
        
    if(RadioButtonCom->Checked)
    {
        sprintf(comx, "com%d", cbComPortName->ItemIndex+1);
        switch(cbBitRate->ItemIndex)
        {
            case 0:comBateRate = 2400;	break;
            case 1:comBateRate = 9600;	break;
            case 2:comBateRate = 19200;	break;
            case 3:comBateRate = 38400;	break;
            case 4:comBateRate = 56000;	break;
            case 5:comBateRate = 57600;	break;
            case 6:comBateRate = 115200;	break;
        }
        state = BPLA_OpenComEx(comx, comBateRate, 1, 2000);
    }
    if(RadioButtonUsb->Checked)
    {
        state = BPLA_OpenUsb();
        if(state == BPLA_OK)
        {
            if(BPLA_SetTimeOut(2000) != BPLA_OK)
            {
                ShowMessage("USB 超时设置错误!");
                return;
            }
        }
    }
    if(RadioButtonLpt->Checked)
    {
        switch(cbLptSel->ItemIndex)
        {
          case 0:lptAddr = 0x378;	break;
          case 1:lptAddr = 0x278;	break;
	}
        state = BPLA_OpenLpt(lptAddr, 2000);
    }
    if(state != BPLA_OK)
    {
        ShowMessage("打开端口失败!");
    }
    int outmode;
    int papermode;
    int printmode;
    outmode = cbParperOut->ItemIndex;
    papermode =  cbPaperType->ItemIndex;
    printmode =  cbPrintMode->ItemIndex?1:0;
    //state = BPLA_Set(outmode, papermode, printmode);
    state = BPLA_Set(0, 0, 1);
    ShowMessage("设置成功!"); 
    return;
}
//---------------------------------------------------------------------------





void __fastcall TPrintForm::Button2Click(TObject *Sender)
{
    Close();        
}
//---------------------------------------------------------------------------

