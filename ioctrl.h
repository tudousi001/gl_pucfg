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
    _UINT8 chn; /*通道号*/
    _UINT8 mode; /*0 = 普通开关, 1 = 到顺开关*/
    _UINT8 sid; /*控制器id*/
    _UINT8 schn; /*用的控制器通道*/
    _UINT16 tim; /*行程*/
    _UINT16 delay; /*开关延时*100ms*/
    _UINT8 st;  /*开关状态SWITCH_MODEs*/
    _UINT8 pos; /*当前位置*/
    _UINT8 limit; /*限位使能*/
    _UINT8 res[3];
}SWITCH_CHN;

typedef struct
{
    _UINT8 devNum;  /*控制器数量*/
    _UINT8 chnNum; /*开关数量*/
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

#define SWITCH_DEV_TYPE_STRs {"无","IO","MODBUS"}
#define SWITCH_TYPE_STRs {"卷帘","卷膜","灌溉","喷淋","补光","CO2","加热","风机","增压","水肥进","水肥出","搅拌","主进水","水帘","未知"}
#define SWITCH_MODE_STRs {"普通","到顺"}

#ifdef __cplusplus
}
#endif

#endif

