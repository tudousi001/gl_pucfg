/***********************************************************
*iotrl.h - io control head file
*
* Copyright(c) 2018~  , 
*
*$Date: $ 
*$Revision: $
*
*-----------------------
*$Log: $
*
*18-01-21, zhushuchao created
************************************************************/ 

#ifndef __IO_CTRL_H__
#define __IO_CTRL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "defs.h"

#define MAX_SWITCH_NUM 8

typedef enum
{
    e_SWITCH_TYPE_CURTAIN,
    e_SWITCH_TYPE_VENTILATE,
    e_SWITCH_TYPE_WATER,
    e_SWITCH_TYPE_SPRAY,
    e_SWITCH_TYPE_LIGHT,
    e_SWITCH_TYPE_CO2,
    e_SWITCH_TYPE_HEAT,
    e_SWITCH_TYPE_WIND,
    e_SWITCH_TYPE_WATER_PRESSURE,
    e_SWITCH_TYPE_FERTILIZER_IN,
    e_SWITCH_TYPE_FERTILIZER_OUT,
    e_SWITCH_TYPE_MIX,
    e_SWITCH_TYPE_WATER_IN,
    e_SWITCH_TYPE_WATER_CURTAIN,
    e_SWITCH_TYPE_MAX
}SWITCH_TYPEs;

typedef enum
{
    e_SWITCH_MODE_OFF,
    e_SWITCH_MODE_ON = 1,
    e_SWITCH_MODE_POSITIVE = 1,
    e_SWITCH_MODE_NEGATIVE
}SWITCH_MODEs;

typedef struct
{
    _UINT8 type;
    _UINT8 id;
    _UINT8 chnNum;
    _UINT8 res;
}SWITCH_DEV;

typedef struct
{
    _UINT16 type; /*SWITCH_TYPEs*/
    _UINT8 chn; /*ͨ����*/
    _UINT8 mode; /*0 = ��ͨ����, 1 = ��˳����*/
    _UINT8 sid; /*������id*/
    _UINT8 schn; /*�õĿ�����ͨ��*/
    _UINT16 tim; /*�г�*/
    _UINT16 delay; /*������ʱ*100ms*/
    _UINT8 st;  /*����״̬SWITCH_MODEs*/
    _UINT8 pos; /*��ǰλ��*/
    _UINT8 limit; /*��λʹ��*/
    _UINT8 res[3];
}SWITCH_CHN;

typedef struct
{
    _UINT8 devNum;  /*����������*/
    _UINT8 chnNum; /*��������*/
    _UINT8 res[2];
    SWITCH_DEV devs[MAX_SWITCH_NUM];
    SWITCH_CHN chns[1];
}SWITCH_INFO;

typedef enum
{
    e_SWITCH_DEV_TYPE_NONE,
    e_SWITCH_DEV_TYPE_IO,    
    e_SWITCH_DEV_TYPE_MODBUS, 
    e_SWITCH_DEV_TYPE_TOTAL
}SWITCH_DEV_TYPEs;

#define SWITCH_DEV_TYPE_STRs {"��","IO","MODBUS"}
#define SWITCH_TYPE_STRs {"����","��Ĥ","���","����","����","CO2","����","���","��ѹ","ˮ�ʽ�","ˮ�ʳ�","����","����ˮ","ˮ��","δ֪"}
#define SWITCH_MODE_STRs {"��ͨ","��˳"}

#ifdef __cplusplus
}
#endif

#endif

