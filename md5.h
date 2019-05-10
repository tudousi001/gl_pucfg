/***********************************************************
*md5.h -  MD5 head file
*
* Copyright(c) 2007, HCMind
*
*
*modification history
*-----------------------
*
*01a, 07-05-11, Zhushuchao created
*
************************************************************/

#ifndef __MD5_H_
#define  __MD5_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long int u_longint;

typedef struct
{  
    u_longint state[4]; /* state (ABCD) */  
    u_longint count[2]; /* number of bits, modulo 2^64 (lsb first) */  
    unsigned char buffer[64]; /* input buffer */  
} MD5_CTX;  

void MD5_Init (MD5_CTX *context);  
void MD5_Update (MD5_CTX *context, unsigned char *input, unsigned int inputLen);  
void MD5_Final (MD5_CTX *context, unsigned char digest[16]);   
void md5Encrypt(char *string, char *out);
    

#ifdef __cplusplus
}
#endif

#endif
