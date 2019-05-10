#ifndef EMLED_HXXX
#define EMLED_HXXX
#ifdef __cplusplus
extern "C" {
#endif

//int emLedShowInit();

//int emLedShowRTData(char* text, int size,int color);

#define MAX_AREATXLINES 16  //每个区域最大16行
#define MAX_AREACOUNTS  4   //每个屏幕最多四个区域

typedef struct 
{
    char rowNo;
    char areaNo;
    char fontType;
    char fontSize;
    char wordsIntal;    //字间距，像素位单位
    char aglian;        //对齐方式，1 = 左 0= 中 2 = 右
    char prelineintal;  //与上一行的行间距
    char enable;
    char text[128];
    char ret[8];
}STRTXLINEINFO;

typedef struct
{
    int x;
    int y;
    int aw;
    int ah;
}STRAREARECT;

typedef struct
{
    STRAREARECT areaRect;
    STRTXLINEINFO lineInfos[MAX_AREATXLINES];
    char areaNo;
    char rowcount;
    char rowIntal;   //行间距
    char enable;    //区域是否可用
    char showUpdateFlag;    //循环显示时，是否需要更新此区域
    char ret[7];
}STRAREAINFOS;

typedef struct
{
    int screenW;
    int screenH;
    char screenIp[16];
    int screenPort;
    STRAREAINFOS areaInfos[MAX_AREACOUNTS];
    char areaCount;
    char ret[7];
}STRSCREENINFOS;

int emledShowInit();

int emledShowAreaText(int areaNo);

int emledShowTextByDyc(int areaNo,const char* text,int len);

#ifdef __cplusplus
}
#endif

#endif

