/***********************************************************
*defs.h - defines
*
* Copyright(c) 2007~  , HCMIND
*
*
*modification history
*-----------------------
*
*01a, 07-05-03, Zhushuchao created
*
************************************************************/
#ifndef __MAIN_DEFS_H_
#define __MAIN_DEFS_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define DLL_API __declspec(dllexport)
#define DLL_CALL __stdcall
#else
#define DLL_API 
#define DLL_CALL
#endif

#ifdef WIN32
typedef __int64  _INT64;
typedef unsigned __int64 _UINT64;
#else
typedef long long  _INT64;
typedef unsigned long long _UINT64;
#endif

typedef long _LONG;
typedef unsigned long _ULONG;

typedef int _INT32;
typedef unsigned int _UINT32;

typedef short _INT16;
typedef unsigned short _UINT16;

typedef char _INT8;
typedef unsigned char _UINT8;

typedef unsigned int _UINT;

typedef int _BOOL;

typedef union
{
    _UINT8 b[4];
    _LONG l;
}UNION32;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FLASE 0
#endif

#ifdef OK
#undef OK
#endif
#define OK 0

#ifdef ERROR
#undef ERROR
#endif
#define ERROR -1

#ifndef NOTSUPPORT
#define NOTSUPPORT -2
#endif

#ifdef __cplusplus
}
#endif
#endif

