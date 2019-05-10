/***********************************************************
*printer.c -  printer source file
*
* Copyright(c) 2010, HCTel
*
*
*modification history
*-----------------------
*
*01a, 10-07-12, Guoqingli created
*
************************************************************/
#include "TSCprinter.h"
#include "windows.h"
#include <vcl.h>
#ifndef NULL
#define NULL (void *)0
#endif

HINSTANCE hlib;

mTSC_openport		        TSC_openport			     = NULL;
mTSC_closeport   		 TSC_closeport			     = NULL;
mTSC_sendcommand   		 TSC_sendcommand				     = NULL;
mTSC_setup   		        TSC_setup		            = NULL;
mTSC_downloadpcx		 TSC_downloadpcx				     = NULL;
mTSC_barcode  			 TSC_barcode				     = NULL;
mTSC_printerfont                 TSC_printerfont				     = NULL;
mTSC_clearbuffer		 TSC_clearbuffer					 = NULL;
mTSC_printlabel  		 TSC_printlabel				 = NULL;
mTSC_formfeed   		 TSC_formfeed					 = NULL;
mTSC_nobackfeed                  TSC_nobackfeed				 = NULL;
mTSC_isready	                 TSC_isready			 = NULL;
mTSC_windowsfont                 TSC_windowsfont		        = NULL;
mTSC_about   		         TSC_about					 = NULL;

int LoadTSCDll()
{
   hlib = LoadLibrary("TSCLib.dll");
   if(hlib == NULL)
   {
       return FALSE;
   }

   TSC_openport = (mTSC_openport) GetProcAddress(hlib,"openport");
   if(TSC_openport == NULL)
   {
	   //ShowMessage("BPLA_SetSaveFile Funcation Error!");
	   return FALSE;
   }

   TSC_closeport = (mTSC_closeport) GetProcAddress(hlib,"closeport");
   if(TSC_closeport == NULL)
   {
	   //ShowMessage("BPLA_GetVersion Funcation Error!");
	   return FALSE;
   }

   TSC_sendcommand = (mTSC_sendcommand) GetProcAddress(hlib,"sendcommand");
   if(TSC_sendcommand == NULL)
   {
	   //ShowMessage("BPLA_OpenCom Funcation Error!");
	   return FALSE;
   }

   TSC_setup = (mTSC_setup) GetProcAddress(hlib,"setup");
   if(TSC_setup == NULL)
   {
	   //ShowMessage("BPLA_OpenComEx Funcation Error!");
	   return FALSE;
   }

   TSC_downloadpcx = (mTSC_downloadpcx) GetProcAddress(hlib,"downloadpcx");
   if(TSC_downloadpcx == NULL)
   {
	   //ShowMessage("BPLA_CloseCom Funcation Error!");
	   return FALSE;
   }

   TSC_barcode = (mTSC_barcode) GetProcAddress(hlib,"barcode");
   if(TSC_barcode == NULL)
   {
	   //ShowMessage("BPLA_OpenLpt Funcation Error!");
	   return FALSE;
   }

   TSC_printerfont = (mTSC_printerfont) GetProcAddress(hlib,"printerfont");
   if(TSC_printerfont == NULL)
   {
	   //ShowMessage("BPLA_CloseLpt Funcation Error!");
	   return FALSE;
   }

   TSC_clearbuffer = (mTSC_clearbuffer) GetProcAddress(hlib,"clearbuffer");
   if(TSC_clearbuffer == NULL)
   {
	   //ShowMessage("BPLA_OpenUsb Funcation Error!");
	   return FALSE;
   }

   TSC_printlabel = (mTSC_printlabel) GetProcAddress(hlib,"printlabel");
   if(TSC_printlabel == NULL)
   {
	   //ShowMessage("BPLA_OpenUsbByID Funcation Error!");
	   return FALSE;
   }

   TSC_formfeed = (mTSC_formfeed) GetProcAddress(hlib,"formfeed");
   if(TSC_formfeed == NULL)
   {
	   //ShowMessage("BPLA_CloseUsb Funcation Error!");
	   return FALSE;
   }

   TSC_nobackfeed = (mTSC_nobackfeed) GetProcAddress(hlib,"nobackfeed");
   if(TSC_nobackfeed == NULL)
   {
	   //ShowMessage("BPLA_SetTimeOut Funcation Error!");
	   return FALSE;
   }

   TSC_isready = (mTSC_isready) GetProcAddress(hlib,"isready");
   if(TSC_isready == NULL)
   {
	   //ShowMessage("BPLA_OpenPrinterDriver Funcation Error!");
	   return FALSE;
   }

   TSC_windowsfont = (mTSC_windowsfont) GetProcAddress(hlib,"windowsfont");
   if(TSC_windowsfont == NULL)
   {
	   //ShowMessage("BPLA_ClosePrinterDriver Funcation Error!");
	   return FALSE;
   }

   TSC_about = (mTSC_about) GetProcAddress(hlib,"about");
   if(TSC_about == NULL)
   {
	   //ShowMessage("BPLA_StartDoc Funcation Error!");
	   return FALSE;
   }

   return TRUE;
}

int UnLoadTSCDll()
{
	return FreeLibrary(hlib);	 
}


 