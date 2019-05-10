#ifndef EMLED_HXXX
#define EMLED_HXXX
#ifdef __cplusplus
extern "C" {
#endif

//int emLedShowInit();

//int emLedShowRTData(char* text, int size,int color);

#define MAX_AREATXLINES 16  //ÿ���������16��
#define MAX_AREACOUNTS  4   //ÿ����Ļ����ĸ�����

typedef struct 
{
    char rowNo;
    char areaNo;
    char fontType;
    char fontSize;
    char wordsIntal;    //�ּ�࣬����λ��λ
    char aglian;        //���뷽ʽ��1 = �� 0= �� 2 = ��
    char prelineintal;  //����һ�е��м��
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
    char rowIntal;   //�м��
    char enable;    //�����Ƿ����
    char showUpdateFlag;    //ѭ����ʾʱ���Ƿ���Ҫ���´�����
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

