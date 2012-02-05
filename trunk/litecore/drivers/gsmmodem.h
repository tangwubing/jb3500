#ifndef __GSMMODEM_H__
#define __GSMMODEM_H__


#ifdef __cplusplus
extern "C" {
#endif






//�Ƿ�֧��Unicode��
#define GSM_UNICODE		0

//�û���Ϣ���뷽ʽ
#define GSM_7BIT		0
#define GSM_8BIT		4
#define GSM_UCS2		8

//Ӧ��״̬
#define GSM_WAIT		0		//�ȴ�����ȷ��
#define GSM_OK			1		//OK
#define GSM_ERR			(-1)	//ERROR

//����Ϣ�����ṹ������/���빲��
//���У��ַ�����'\0'��β
typedef struct {
	char SCA[16];				//����Ϣ�������ĺ���(SMSC��ַ)
	char TPA[16];				//Ŀ������ظ�����(TP-DA��TP-RA)
	char TP_PID;				//�û���ϢЭ���ʶ(TP-PID)
	char TP_DCS;				//�û���Ϣ���뷽ʽ(TP-DCS)
	char TP_SCTS[16];			//����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char TP_UD[160];			//ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	short nUD;					//�û����ݳ���,����ʱ�õ�
	short nIndex;				//����Ϣ���,��ȡʱ�õ�
} SM_PARAM;

//��ȡӦ��Ļ�����
typedef struct {
	int len;
	char data[16384];
} SM_BUFF;

int gsmBytes2String(const U8* pSrc, char* pDst, int nSrcLength);
int gsmString2Bytes(const char* pSrc, U8* pDst, int nSrcLength);
int gsmEncode7bit(const char* pSrc, U8* pDst, int nSrcLength);
int gsmDecode7bit(const U8* pSrc, char* pDst, int nSrcLength);
int gsmEncode8bit(const char* pSrc, U8* pDst, int nSrcLength);
int gsmDecode8bit(const U8* pSrc, char* pDst, int nSrcLength);
#if GSM_UNICODE
int gsmEncodeUcs2(const char* pSrc, U8* pDst, int nSrcLength);
int gsmDecodeUcs2(const U8* pSrc, char* pDst, int nSrcLength);
#endif
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);

#ifdef __cplusplus
}
#endif

#endif

