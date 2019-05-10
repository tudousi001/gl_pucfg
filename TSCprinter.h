/***********************************************************
*printer.h -  printer head file
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

#define SOH  0x01
#define STX  0x02
#define ESC  0x1b 
#define XON	 0X11
#define XOFF 0X13
#define CR   0X0D
#define LF	 0X0A
#define FF   0X0C

int LoadTSCDll();
int UnLoadTSCDll();

typedef int (__stdcall *mTSC_openport)(char*);
typedef int (__stdcall *mTSC_closeport)();

typedef int (__stdcall *mTSC_sendcommand)(char*);
typedef int (__stdcall *mTSC_setup)(char *,char *,char *,char *,char *,char *,char *);
typedef int (__stdcall *mTSC_downloadpcx)(char *filename,char *image_name);
typedef int (__stdcall *mTSC_barcode)(char *,char *,char *,char *,char *,char *,char *,char *,char *);
typedef int (__stdcall *mTSC_printerfont)(char *,char *,char *,char *,char *,char *,char *);
typedef int (__stdcall *mTSC_clearbuffer)();
typedef int (__stdcall *mTSC_printlabel)(char *, char *);
typedef int (__stdcall *mTSC_formfeed)();
typedef int (__stdcall *mTSC_nobackfeed)();


typedef int (__stdcall *mTSC_isready)();
typedef int (__stdcall *mTSC_windowsfont)(int x, int y, int fontheight, int rotation, int fontstyle, int fontunderline, char *szFaceName, char *content);  //接收指令
typedef int (__stdcall *mTSC_about)();                 //发送指令文件
typedef int (__stdcall *mTSC_ReceiveFile)(int,char*, int*,int);



extern mTSC_openport		        TSC_openport			     ;
extern mTSC_closeport   		 TSC_closeport			     ;
extern mTSC_sendcommand   		 TSC_sendcommand				     ;
extern mTSC_setup   		        TSC_setup		            ;
extern mTSC_downloadpcx			 TSC_downloadpcx				     ;
extern mTSC_barcode  			 TSC_barcode				    ;
extern mTSC_printerfont                   TSC_printerfont				     ;
extern mTSC_clearbuffer			 TSC_clearbuffer					 ;
extern mTSC_printlabel  		 TSC_printlabel				 ;
extern mTSC_formfeed   		        TSC_formfeed					 ;
extern mTSC_nobackfeed                  TSC_nobackfeed				 ;
extern mTSC_isready	                TSC_isready			 ;
extern mTSC_windowsfont                 TSC_windowsfont		        ;
extern mTSC_about   		        TSC_about					 ;

