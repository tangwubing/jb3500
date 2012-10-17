

#ifndef _CCEMAN_H_
#define _CCEMAN_H_


	
#define MAXSBUFLEN 4096

#define EXE_COMPRESS_NEW	0x01//�µ�ѹ�� 
#define EXE_SHA			0x02//��������֤ 
#define EXE_ENCRYPT		0x04//����

#include "CrypFun.h"
#include "CompressFun.h"
#include "CompressFunNew.h"


void CCEManInit(void);

int SetKey(BYTE MainKey[4][4],int keylen);
int EnData(BYTE * DataBuf, int DataLen, unsigned char Oper);
int DeData(BYTE * DataBuf, int DataLen);



/*
int FormFrame(unsigned char Oper, unsigned char * buf,int buflen);
int CheckFrame(unsigned char * buf,int buflen);
int Expand(DATA * temp);
int Compress(DATA * temp);
*/



#endif //(_CCEMAN_H_)
