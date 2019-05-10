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

#define BPLA_OK             1000     //一切正常
#define BPLA_COMERROR       1001     //通讯错或者未联接打印机
#define BPLA_PARAERROR      1002     //参数错误
#define BPLA_FILEOPENERROR  1003     //文件打开错误
#define BPLA_FILEREADERROR  1004     //文件读错误
#define BPLA_FILEWRITEERROR 1005     //文件写错误
#define BPLA_FILEERROR      1006     //文件不合要求
#define BPLA_NUMBEROVER     1007     //指定的接收信息数量过大
#define BPLA_IMAGETYPEERROR 1008     //图象文件格式不正确 

int LoadDll();
int UnLoadDll();

//***************测试函数
typedef int (__stdcall *mBPLA_SetSaveFile)(bool,char*,bool);	//将指令保存到文件
typedef int (__stdcall *mBPLA_GetVersion)();					//获取版本

//***************端口控制
typedef int (__stdcall *mBPLA_OpenCom)(char*,int,int);			//打开配置串口
typedef int (__stdcall *mBPLA_OpenComEx)(char*,int,int,int);	//打开配置串口,不检测
typedef int (__stdcall *mBPLA_CloseCom)();						//关闭串口
typedef int (__stdcall *mBPLA_OpenLpt)(int, int);				//打开并口
typedef int (__stdcall *mBPLA_CloseLpt)();						//关闭并口
typedef int (__stdcall *mBPLA_OpenUsb)();						//打开USB口
typedef int (__stdcall *mBPLA_OpenUsbByID)(int);				//通过内部ID号打开USB口
typedef int (__stdcall *mBPLA_CloseUsb)();						//关闭USB口
typedef int (__stdcall *mBPLA_SetTimeOut)(int);					//设置端口超时

//***************数据收发
typedef int (__stdcall *mBPLA_SendCommand)(char*,int);           //发送指令
typedef int (__stdcall *mBPLA_ReceiveInfo)(int,char*,int*,int);  //接收指令
typedef int (__stdcall *mBPLA_SendFile)(char*);                  //发送指令文件
typedef int (__stdcall *mBPLA_ReceiveFile)(int,char*, int*,int); //接收指令到文件

//***************驱动控制
typedef int (__stdcall *mBPLA_OpenPrinterDriver)(char* );		 //打开驱动程序
typedef int (__stdcall *mBPLA_ClosePrinterDriver)();             //关闭驱动程序
typedef int (__stdcall *mBPLA_StartDoc)();                       //开启打印作业
typedef int (__stdcall *mBPLA_EndDoc)();                         //关闭打印作业

//***************设备设置
typedef int (__stdcall *mBPLA_DownloadImage)(char*,int,int,char*);   //下载图象
typedef int (__stdcall *mBPLA_DownloadFont)(char*,int,int,int,int,int,int,int,int,char*);//下载字体
typedef int (__stdcall *mBPLA_DownErase)(int,int,char*);             //删除指定的文件
typedef int (__stdcall *mBPLA_DownEraseAll)(int);                    //删除指定模块中的全部文件
typedef int (__stdcall *mBPLA_Reset)();                              //复位打印机
typedef int (__stdcall *mBPLA_ForBack)(int,int);                     //进/退标签
typedef int (__stdcall *mBPLA_Set)(int,int,int);                     //出纸方式、纸张类型、工作模式
typedef int (__stdcall *mBPLA_SetSensor)(int);                       //选择传感器
typedef int (__stdcall *mBPLA_SetPaperLength)(int,int);              //设置纸长度
typedef int (__stdcall *mBPLA_SetEnd)(int);	                         //设置打印停止位置

//***************标签格式
typedef int (__stdcall *mBPLA_StartArea)(int,int,int,int,int,int,int,int);//进入标签模式
typedef int (__stdcall *mBPLA_SetMirror)();                               //对条码无效
//typedef int (__stdcall *mBPLA_SetZero)();
typedef int (__stdcall *mBPLA_SetCopy)(int pieces,int gap);               //横向复制
typedef int (__stdcall *mBPLA_SetAllRotate)(int);                         //整体翻转
typedef int (__stdcall *mBPLA_SetAllMirror)();                            //整体镜象  
typedef int (__stdcall *mBPLA_PrintSaveLabel)(int);                       //打印已经保存的标签，不支持连续域设置
typedef int (__stdcall *mBPLA_Print)(int,int,int);                        //打印标签
typedef int (__stdcall *mBPLA_SaveLabel)();                               //保存标签不打印  

//***************内容设置
//图形打印
typedef int (__stdcall *mBPLA_PrintLine)(int,int,int,int,int);        //直线
typedef int (__stdcall *mBPLA_PrintBox)(int,int,int,int,int,int,int); //矩形
typedef int (__stdcall *mBPLA_PrintCircle)(int,int,int,int,int);      //圆

//图象打印
typedef int (__stdcall *mBPLA_LoadImage)(char*,int,int,int,int,int);  //载入已经下载的图象
typedef int (__stdcall *mBPLA_PrintImage)(char*,int,int,int);         //直接下载图象到打印位置

//文字打印
typedef int (__stdcall *mBPLA_PrintText)(char*,int,int,int,int,int,int,char*,int,int);//内部字体0-8、000-007和P06-P18
typedef int (__stdcall *mBPLA_PrintOut)(char*,int,int,int,char*,int,int,char*,int,int);//已经下载的外部字体打印
typedef int (__stdcall *mBPLA_PrintTruetype)(char*,int,int,char*,int,int);//打印TRUETYPE字体
typedef int (__stdcall *mBPLA_PrintTruetypeEx)(char*,int,int,char*,int,int,int);//打印TRUETYPE字体，允许选择字体旋转方向和行列方向。
//中英文混排打印
typedef int (__stdcall *mBPLA_PrintMixText)(char*,int,int,int,int, int,char*,int,int, int,int,char*,int,int);
//中英文混排打印（指令格式）
typedef int (__stdcall *mBPLA_PrintMixTextEx)(char*,int,int,int,int, char*,char*,int,int,char*,int,int);

//条码打印
//一维条码
typedef int (__stdcall *mBPLA_PrintBarcode)(char*,int,int,int,int,int,int,int,char*);
//PDF417码
typedef int (__stdcall *mBPLA_PrintPDF)(char*,int,int,int,int,int,int,int,int,int,int,int,int,char*);
//MAXICODE码
typedef int (__stdcall *mBPLA_PrintMaxi)(char*,int,int,char*);

//***************查询
typedef int (__stdcall *mBPLA_CheckCom)();  //测试串口
typedef int (__stdcall *mBPLA_CheckStatus)(char*,char*,char*,char*,char*,char*,char*,char*);//测试状态
typedef int (__stdcall *mBPLA_CheckCut)();  //测试切刀


extern mBPLA_SetSaveFile		 BPLA_SetSaveFile			     ;
extern mBPLA_GetVersion   		 BPLA_GetVersion			     ;
extern mBPLA_OpenCom   		 BPLA_OpenCom				     ;
extern mBPLA_OpenComEx   		 BPLA_OpenComEx		            ;
extern mBPLA_CloseCom			 BPLA_CloseCom				     ;
extern mBPLA_OpenLpt  			 BPLA_OpenLpt				    ;
extern mBPLA_CloseLpt                   BPLA_CloseLpt				     ;
extern mBPLA_OpenUsb			 BPLA_OpenUsb					 ;
extern mBPLA_OpenUsbByID  		 BPLA_OpenUsbByID				 ;
extern mBPLA_CloseUsb   		 BPLA_CloseUsb					 ;
extern mBPLA_SetTimeOut               BPLA_SetTimeOut				 ;
extern mBPLA_OpenPrinterDriver	 BPLA_OpenPrinterDriver			 ;
extern mBPLA_ClosePrinterDriver     BPLA_ClosePrinterDriver		        ;
extern mBPLA_StartDoc   		        BPLA_StartDoc					 ;
extern mBPLA_EndDoc                     BPLA_EndDoc				;
extern mBPLA_SendCommand  	 BPLA_SendCommand		        ;
extern mBPLA_ReceiveInfo  		 BPLA_ReceiveInfo				 ;
extern mBPLA_SendFile			 BPLA_SendFile					 ;
extern mBPLA_ReceiveFile		        BPLA_ReceiveFile			;
extern mBPLA_DownloadImage        BPLA_DownloadImage			 ;
extern mBPLA_DownloadFont		 BPLA_DownloadFont				;
extern mBPLA_DownErase			 BPLA_DownErase				 ;
extern mBPLA_DownEraseAll		 BPLA_DownEraseAll				;
extern mBPLA_Set				 BPLA_Set				;
extern mBPLA_SetSensor			 BPLA_SetSensor					;
extern mBPLA_SetPaperLength	        BPLA_SetPaperLength			 ;
extern mBPLA_SetEnd			 BPLA_SetEnd					;
extern mBPLA_ForBack			 BPLA_ForBack					;
extern mBPLA_Reset				 BPLA_Reset				;
extern mBPLA_StartArea			 BPLA_StartArea					;
extern mBPLA_SetMirror			 BPLA_SetMirror					;
extern mBPLA_Print				 BPLA_Print				;
extern mBPLA_SaveLabel			 BPLA_SaveLabel					;
extern mBPLA_PrintSaveLabel	        BPLA_PrintSaveLabel			 ;
extern mBPLA_SetCopy			 BPLA_SetCopy					;
extern mBPLA_SetAllMirror		 BPLA_SetAllMirror				;
extern mBPLA_SetAllRotate		 BPLA_SetAllRotate				;
extern mBPLA_PrintLine			 BPLA_PrintLine					;
extern mBPLA_PrintBox			 BPLA_PrintBox					;
extern mBPLA_PrintCircle		        BPLA_PrintCircle			;
extern mBPLA_LoadImage			 BPLA_LoadImage			        ;
extern mBPLA_PrintImage		        BPLA_PrintImage			        ;
extern mBPLA_PrintText			 BPLA_PrintText					;
extern mBPLA_PrintOut			 BPLA_PrintOut					;
extern mBPLA_PrintTruetype		 BPLA_PrintTruetype				;
extern mBPLA_PrintTruetypeEx	        BPLA_PrintTruetypeEx			 ;
extern mBPLA_PrintMixText		 BPLA_PrintMixText				 ;
extern mBPLA_PrintMixTextEx	        BPLA_PrintMixTextEx			 ;
extern mBPLA_PrintBarcode		 BPLA_PrintBarcode				;
extern mBPLA_PrintPDF			 BPLA_PrintPDF					;
extern mBPLA_PrintMaxi			 BPLA_PrintMaxi					;
extern mBPLA_CheckCom			 BPLA_CheckCom					;
extern mBPLA_CheckStatus		 BPLA_CheckStatus				;
extern mBPLA_CheckCut			 BPLA_CheckCut					;
