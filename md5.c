/***********************************************************
*md5.h -  MD5 source file
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
 
#include "md5.h"  

#define S11 7  
#define S12 12  
#define S13 17  
#define S14 22  
#define S21 5  
#define S22 9  
#define S23 14  
#define S24 20  
#define S31 4  
#define S32 11  
#define S33 16  
#define S34 23  
#define S41 6  
#define S42 10  
#define S43 15  
#define S44 21  

/*www.cmd5.com
MD5(12345,32) = 827ccb0eea8a706c4c34a16891f84e7b
MD5(12345,16) = ea8a706c4c34a168
MD5(123456,32) = e10adc3949ba59abbe56e057f20f883e
MD5(123456,16) = 49ba59abbe56e057
MD5(1234567,32) = fcea920f7412b5da7be0cf42b8c93759
MD5(1234567,16) = 7412b5da7be0cf42
MD5(12345678,32) = 25d55ad283aa400af464c76d713c07ad
MD5(12345678,16) = 83aa400af464c76d   
*/
/* 
MD5的基本函数.  
*/  
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/*
ROT 将x 循环左移n bits.
*/  
#define ROT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/*
FF, GG, HH, and II 代表1, 2, 3, 4 轮计算
*/  
#define FF(a, b, c, d, i, s, ac) (a) = ROT ((a+F ((b), (c), (d)) + (x[i]) + (u_longint)(ac)), (s)) + b
#define GG(a, b, c, d, i, s, ac) (a) = ROT ((a+G ((b), (c), (d)) + (x[i]) + (u_longint)(ac)), (s)) + b
#define HH(a, b, c, d, i, s, ac) (a) = ROT ((a + H ((b), (c), (d)) + (x[i]) + (u_longint)(ac)), (s))+b 
#define II(a, b, c, d, i, s, ac)  (a) = ROT ((a +I ((b), (c), (d)) + (x[i]) + (u_longint)(ac)), (s)) + b

static unsigned char PADDING[64] = 
{  
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
}; 

/* 编码 input (u_longint) into output (unsigned char). 
Assumes len is a multiple of 4.*/
static void MD5_Encode (unsigned char *output, u_longint *input, unsigned int len)
{
    unsigned int i, j;

    for (i = 0, j = 0; j < len; i++, j += 4) 
    {
        output[j] = (unsigned char)(input[i] & 0xff);
        output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
        output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
        output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
    }
}

/* 解码 input (unsigned char) into output (UINT4).
Assumes len is a multiple of 4. */  
static void MD5_Decode (u_longint *output, unsigned char *input, unsigned int len)
{
    unsigned int i, j;

    for (i = 0, j = 0; j < len; i++, j += 4)
    {   
        output[i] =   ((u_longint)input[j])  
                          |(((u_longint)input[j+1]) << 8)
                          |(((u_longint)input[j+2]) << 16)
                          |(((u_longint)input[j+3]) << 24);
    }
}

static void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
        output[i] = input[i];
}

static void MD5_memset (unsigned char* output,int value,unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
        ((char *)output)[i] = (char)value;
}

/* 对一个消息块的16个子块进行MD5 基本计算.  */  
static void MD5_Transform (MD5_CTX *context, unsigned char block[64])
{
    u_longint a, b, c, d, x[16]; 

    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];        

    MD5_Decode (x, block, 64);

    /* Round 1 */
    FF (a, b, c, d, 0, S11, 0xd76aa478); /* 1 */
    FF (d, a, b, c, 1, S12, 0xe8c7b756); /* 2 */
    FF (c, d, a, b, 2, S13, 0x242070db); /* 3 */
    FF (b, c, d, a, 3, S14, 0xc1bdceee); /* 4 */
    FF (a, b, c, d, 4, S11, 0xf57c0faf); /* 5 */
    FF (d, a, b, c, 5, S12, 0x4787c62a); /* 6 */
    FF (c, d, a, b, 6, S13, 0xa8304613); /* 7 */
    FF (b, c, d, a, 7, S14, 0xfd469501); /* 8 */
    FF (a, b, c, d, 8, S11, 0x698098d8); /* 9 */
    FF (d, a, b, c, 9, S12, 0x8b44f7af); /* 10 */
    FF (c, d, a, b, 10, S13, 0xffff5bb1); /* 11 */
    FF (b, c, d, a, 11, S14, 0x895cd7be); /* 12 */
    FF (a, b, c, d, 12, S11, 0x6b901122); /* 13 */
    FF (d, a, b, c, 13, S12, 0xfd987193); /* 14 */
    FF (c, d, a, b, 14, S13, 0xa679438e); /* 15 */
    FF (b, c, d, a, 15, S14, 0x49b40821); /* 16 */

    /* Round 2 */
    GG (a, b, c, d, 1, S21, 0xf61e2562); /* 17 */
    GG (d, a, b, c, 6, S22, 0xc040b340); /* 18 */
    GG (c, d, a, b, 11, S23, 0x265e5a51); /* 19 */
    GG (b, c, d, a, 0, S24, 0xe9b6c7aa); /* 20 */
    GG (a, b, c, d, 5, S21, 0xd62f105d); /* 21 */
    GG (d, a, b, c, 10, S22,  0x2441453); /* 22 */
    GG (c, d, a, b, 15, S23, 0xd8a1e681); /* 23 */
    GG (b, c, d, a, 4, S24, 0xe7d3fbc8); /* 24 */
    GG (a, b, c, d, 9, S21, 0x21e1cde6); /* 25 */
    GG (d, a, b, c, 14, S22, 0xc33707d6); /* 26 */
    GG (c, d, a, b, 3, S23, 0xf4d50d87); /* 27 */
    GG (b, c, d, a, 8, S24, 0x455a14ed); /* 28 */
    GG (a, b, c, d, 13, S21, 0xa9e3e905); /* 29 */
    GG (d, a, b, c, 2, S22, 0xfcefa3f8); /* 30 */
    GG (c, d, a, b, 7, S23, 0x676f02d9); /* 31 */
    GG (b, c, d, a, 12, S24, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH (a, b, c, d, 5, S31, 0xfffa3942); /* 33 */
    HH (d, a, b, c, 8, S32, 0x8771f681); /* 34 */
    HH (c, d, a, b, 11, S33, 0x6d9d6122); /* 35 */
    HH (b, c, d, a, 14, S34, 0xfde5380c); /* 36 */
    HH (a, b, c, d, 1, S31, 0xa4beea44); /* 37 */
    HH (d, a, b, c, 4, S32, 0x4bdecfa9); /* 38 */
    HH (c, d, a, b, 7, S33, 0xf6bb4b60); /* 39 */
    HH (b, c, d, a, 10, S34, 0xbebfbc70); /* 40 */
    HH (a, b, c, d, 13, S31, 0x289b7ec6); /* 41 */
    HH (d, a, b, c, 0, S32, 0xeaa127fa); /* 42 */
    HH (c, d, a, b, 3, S33, 0xd4ef3085); /* 43 */
    HH (b, c, d, a, 6, S34,  0x4881d05); /* 44 */
    HH (a, b, c, d, 9, S31, 0xd9d4d039); /* 45 */
    HH (d, a, b, c, 12, S32, 0xe6db99e5); /* 46 */
    HH (c, d, a, b, 15, S33, 0x1fa27cf8); /* 47 */
    HH (b, c, d, a,  2, S34, 0xc4ac5665); /* 48 */

    /* Round 4 */
    II (a, b, c, d,  0, S41, 0xf4292244); /* 49 */
    II (d, a, b, c,  7, S42, 0x432aff97); /* 50 */
    II (c, d, a, b, 14, S43, 0xab9423a7); /* 51 */
    II (b, c, d, a,  5, S44, 0xfc93a039); /* 52 */
    II (a, b, c, d, 12, S41, 0x655b59c3); /* 53 */
    II (d, a, b, c,  3, S42, 0x8f0ccc92); /* 54 */
    II (c, d, a, b, 10, S43, 0xffeff47d); /* 55 */
    II (b, c, d, a,  1, S44, 0x85845dd1); /* 56 */
    II (a, b, c, d,  8, S41, 0x6fa87e4f); /* 57 */
    II (d, a, b, c, 15, S42, 0xfe2ce6e0); /* 58 */
    II (c, d, a, b,  6, S43, 0xa3014314); /* 59 */
    II (b, c, d, a, 13, S44, 0x4e0811a1); /* 60 */
    II (a, b, c, d,  4, S41, 0xf7537e82); /* 61 */
    II (d, a, b, c, 11, S42, 0xbd3af235); /* 62 */
    II (c, d, a, b,  2, S43, 0x2ad7d2bb); /* 63 */
    II (b, c, d, a,  9, S44, 0xeb86d391); /* 64 */

    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;

    /*    Zeroize sensitive information.    */
    MD5_memset ((unsigned char*)x, 0, sizeof (x));
}

/* MD5 块更新操作.不断地对消息块进行MD5 消息-摘要操作,并更新context.  */  
void MD5_Update (MD5_CTX *context, unsigned char *input, unsigned int inputLen)  
{  
    unsigned int i, index, partLen;

    /* Compute number of bytes mod 64 */
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);

    /* Update number of bits */
    if ((context->count[0] += ((u_longint)inputLen << 3))< ((u_longint)inputLen << 3))
        context->count[1]++;
    context->count[1] += ((u_longint)inputLen >> 29);

    partLen = 64 - index;

    /* Transform as many times as possible.*/
    if (inputLen >= partLen) 
    {
        MD5_memcpy((unsigned char*)&context->buffer[index],  (unsigned char*)input, partLen);
        MD5_Transform (context, context->buffer);

        for (i = partLen; i + 63 < inputLen; i += 64)
    	     MD5_Transform (context, &input[i]);

        index = 0;
    }
    else
    {
        i = 0;
    }

    /* Buffer remaining input */
    MD5_memcpy ((unsigned char*)&context->buffer[index], (unsigned char*)&input[i], inputLen-i);
}  

/* MD5 的初始化, MD5 操作的开始. */  
void MD5_Init (MD5_CTX *context)  
{  
    context->count[0] = 0;
    context->count[1] = 0;
    
    context->state[0] = 0x67452301;  
    context->state[1] = 0xefcdab89;  
    context->state[2] = 0x98badcfe;  
    context->state[3] = 0x10325476;  
}  

/* MD5 的结束操作. 输出128比特的散列值并对context重新初始化 */  
void MD5_Final (MD5_CTX *context, unsigned char digest[16])  
{  
    unsigned char bits[8];
    unsigned int index, padLen;

    /* Save number of bits */
    MD5_Encode (bits, context->count, 8);

    /* Pad out to 56 mod 64. */
    index = (unsigned int)((context->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5_Update (context, PADDING, padLen);

    /* Append length (before padding) */
    MD5_Update (context, bits, 8);
    
    /* Store state in digest */
    MD5_Encode (digest, context->state, 16);

    /* Zeroize sensitive information.*/
    MD5_memset ((unsigned char*)context, 0, sizeof (*context));
    MD5_Init(context);
}  
          
void md5Encrypt(char *string, char *out)
{
    MD5_CTX context;
    unsigned char digest[16];
    unsigned int len = strlen (string);

    MD5_Init (&context);
    MD5_Update (&context, string, len);
    MD5_Final (&context, digest);

    sprintf(out, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
             digest[0], digest[1], digest[2], digest[3],
             digest[4], digest[5],digest[6], digest[7],
             digest[8], digest[9], digest[10], digest[11],
             digest[12], digest[13],digest[14], digest[15]);
}

