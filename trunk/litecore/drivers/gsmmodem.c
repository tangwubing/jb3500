#if MODEM_ENABLE
#include <drivers/gsmmodem.h>








buffer *g_pbufModem = NULL;
SM_PARAM g_stuSmsTxBuf;

void modem_RecData()
{
	UART_RXBUF *p = &g_stuUart1RxBuf;

	if (p->nIn > p->nOut) {
		buffer_Push(g_pbufModem, &p->arrBuf[p->nOut], p->nIn - p->nOut);
	} else if (p->nIn < p->nOut) {
		buffer_Push(g_pbufModem, &p->arrBuf[p->nOut], sizeof(p->arrBuf) - p->nOut);
		buffer_Push(g_pbufModem, &p->arrBuf[0], p->nIn);
	}
	p->nOut = p->nIn;
}

int modem_FindStr(BUFFER *pbuf, int nOffset, char *pStr)
{
	int nTemp = 0;
	U8 *ptr;
	
	//����ַ����������Ա����
	buffer_Push(pbuf, (U8 *)&nTemp, 1);
	if ((ptr = (U8 *)rt_strstr((char *)(pbuf->p + nOffset), pStr)) != NULL) {
		nTemp = ptr - pbuf->p;
	} else {
		nTemp = -1;
	}
	pbuf->nLen -= 1;
	return nTemp;
}

int modem_SendCmd(const char *pCmd, const char *pResult, buf b)
{
	U32 i = 0;

	buf_Release(b);
	g_pbufModem = pbuf;
	uart_Send(UART_MODEM, (U8 *)pCmd, rt_strlen(pCmd));
	for (i = 50; i; --i) {
		os_dly_wait(10);
		modem_RecData();
		if (modem_FindStr(pbuf, 0, pResult) != -1) {
			break;
		}
	}
	return i;
}

void modem_BufFree(BUFFER *pbuf)
{
	g_pbufModem = NULL;
	buffer_Free(pbuf);
}

int modem_Initial()
{
	U32 i, nState = 0;
	int nResult = 0;
	BUFFER buf = {0};

	TC35I_IGT_SET();
	TC35I_PWER_DOWN();
	os_dly_wait(50);
	TC35I_PWER_ON();
	os_dly_wait(10);
	TC35I_IGT_CLR();
	os_dly_wait(10);
	TC35I_IGT_SET();
	os_dly_wait(500);
	for (i = 20; i; --i) {
		switch (nState) {
			case 0:
				//�������� 
				if (modem_SendCmd("ATE0\r\n", "OK", &buf)) {
					nState++;
				}
				break;
			case 1:
				//���ö��Ŵ洢��SIM��
				if (modem_SendCmd("AT+CPMS=SM,SM,SM\r\n", "OK", &buf)) {
					nState++;
				}
				break;
			case 2:
				//������ö��Ŵ洢��SIM������뷢����ָ��
				if (modem_SendCmd("AT^SSMSS=1\r\n", "OK", &buf)) {
					nState++;
				}
				break;
			case 3:
				//PDUģʽ
				if (modem_SendCmd("AT+CMGF=0\r\n", "OK", &buf)) {
					nState++;
				}
				break;
			case 4:
				//������
				nState++;
				break;
			case 5:
				//�������Ϣ���ĺ���
				if (modem_SendCmd("AT+CSCA?\r\n", "OK", &buf)) {
					int k;
					U8 *pNum;
					
					if ((k = modem_FindStr(&buf, 0, "+86")) != -1) {
						pNum = buf.p + k + 1;
						memcpy(g_stuSmsTxBuf.SCA, pNum, 13);
						g_stuSmsTxBuf.SCA[13] = 'F';
					}
					nResult = 1;
				}
				break;
			default:
				break;
		}
		modem_BufFree(&buf);
		if (nResult) {
			break;
		}
		os_dly_wait(10);
	}
	return nResult;
}

//�ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
//�磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
//����: pSrc - Դ�ַ���ָ��
//     nSrcLength - Դ�ַ�������
//���: pDst - Ŀ������ָ��
//����: Ŀ�����ݳ���
int gsmString2Bytes(const char* pSrc, U8* pDst, int nSrcLength)
{
	int i;

	for (i = 0; i < nSrcLength; i += 2)
	{
		//�����4λ
		if ((*pSrc >= '0') && (*pSrc <= '9'))
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		//�����4λ
		if ((*pSrc>='0') && (*pSrc<='9'))
		{
			*pDst |= *pSrc - '0';
		}
		else
		{
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
		pDst++;
	}

	//����Ŀ�����ݳ���
	return (nSrcLength / 2);
}

//�ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
//�磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
//����: pSrc - Դ����ָ��
//     nSrcLength - Դ���ݳ���
//���: pDst - Ŀ���ַ���ָ��
//����: Ŀ���ַ�������
const char tab[]="0123456789ABCDEF";	//0x0-0xf���ַ����ұ�
int gsmBytes2String(const U8* pSrc, char* pDst, int nSrcLength)
{
	int i;

	for (i = 0; i < nSrcLength; i++)
	{
		*pDst++ = tab[*pSrc >> 4];		//�����4λ
		*pDst++ = tab[*pSrc & 0x0f];	//�����4λ
		pSrc++;
	}

	//����ַ����Ӹ�������
	*pDst = '\0';

	//����Ŀ���ַ�������
	return (nSrcLength * 2);
}

//7bit����
//����: pSrc - Դ�ַ���ָ��
//     nSrcLength - Դ�ַ�������
//���: pDst - Ŀ����봮ָ��
//����: Ŀ����봮����
int gsmEncode7bit(const char* pSrc, U8* pDst, int nSrcLength)
{
	int nSrc;		//Դ�ַ����ļ���ֵ
	int nDst;		//Ŀ����봮�ļ���ֵ
	int nChar;		//��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
	U8 nLeft = 0; 	//��һ�ֽڲ��������

	//����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;

	//��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
	//ѭ���ô�����̣�ֱ��Դ����������
	//������鲻��8�ֽڣ�Ҳ����ȷ����
	while (nSrc < nSrcLength)
	{
		//ȡԴ�ַ����ļ���ֵ�����3λ
		nChar = nSrc & 7;

		//����Դ����ÿ���ֽ�
		if(nChar == 0)
		{
			//���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
			nLeft = *pSrc;
		}
		else
		{
			//���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
			*pDst = (*pSrc << (8-nChar)) | nLeft;

			//�����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
			nLeft = *pSrc >> nChar;

			//�޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;
		}

		//�޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	//����Ŀ�괮����
	return nDst;
}

//7bit����
//����: pSrc - Դ���봮ָ��
//     nSrcLength - Դ���봮����
//���: pDst - Ŀ���ַ���ָ��
//����: Ŀ���ַ�������
int gsmDecode7bit(const U8* pSrc, char* pDst, int nSrcLength)
{
	int nSrc;		//Դ�ַ����ļ���ֵ
	int nDst;		//Ŀ����봮�ļ���ֵ
	int nByte;		//��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	U8 nLeft;	//��һ�ֽڲ��������

	//����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;
	
	//�����ֽ���źͲ������ݳ�ʼ��
	nByte = 0;
	nLeft = 0;

	//��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	//ѭ���ô�����̣�ֱ��Դ���ݱ�������
	//������鲻��7�ֽڣ�Ҳ����ȷ����
	while(nSrc<nSrcLength)
	{
		//��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;

		//�����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		nLeft = *pSrc >> (7-nByte);

		//�޸�Ŀ�괮��ָ��ͼ���ֵ
		pDst++;
		nDst++;

		//�޸��ֽڼ���ֵ
		nByte++;

		//����һ������һ���ֽ�
		if(nByte == 7)
		{
			//����õ�һ��Ŀ������ֽ�
			*pDst = nLeft;

			//�޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;

			//�����ֽ���źͲ������ݳ�ʼ��
			nByte = 0;
			nLeft = 0;
		}

		//�޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	//����ַ����Ӹ�������
	*pDst = '\0';

	//����Ŀ�괮����
	return nDst;
}

//8bit����
//����: pSrc - Դ�ַ���ָ��
//     nSrcLength - Դ�ַ�������
//���: pDst - Ŀ����봮ָ��
//����: Ŀ����봮����
int gsmEncode8bit(const char* pSrc, U8* pDst, int nSrcLength)
{
	//�򵥸���
	memcpy(pDst, pSrc, nSrcLength);

	return nSrcLength;
}

//8bit����
//����: pSrc - Դ���봮ָ��
//     nSrcLength -  Դ���봮����
//���: pDst -  Ŀ���ַ���ָ��
//����: Ŀ���ַ�������
int gsmDecode8bit(const U8* pSrc, char* pDst, int nSrcLength)
{
	//�򵥸���
	memcpy(pDst, pSrc, nSrcLength);

	//����ַ����Ӹ�������
	*(pDst + nSrcLength) = '\0';

	return nSrcLength;
}


#if GSM_UNICODE
//UCS2����
//����: pSrc - Դ�ַ���ָ��
//     nSrcLength - Դ�ַ�������
//���: pDst - Ŀ����봮ָ��
//����: Ŀ����봮����
int gsmEncodeUcs2(const char* pSrc, U8* pDst, int nSrcLength)
{
	int i, nDstLength;		//UNICODE���ַ���Ŀ
	WCHAR wchar[128];	//UNICODE��������

	//�ַ���-->UNICODE��
	nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);

	//�ߵ��ֽڶԵ������
	for(i=0; i<nDstLength; i++)
	{
		*pDst++ = wchar[i] >> 8;		//�������λ�ֽ�
		*pDst++ = wchar[i] & 0xff;		//�������λ�ֽ�
	}

	//����Ŀ����봮����
	return nDstLength * 2;
}

//UCS2����
//����: pSrc - Դ���봮ָ��
//     nSrcLength -  Դ���봮����
//���: pDst -  Ŀ���ַ���ָ��
//����: Ŀ���ַ�������
int gsmDecodeUcs2(const U8* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;		//UNICODE���ַ���Ŀ
	WCHAR wchar[128];	//UNICODE��������

	//�ߵ��ֽڶԵ���ƴ��UNICODE
	for(int i=0; i<nSrcLength/2; i++)
	{
		wchar[i] = *pSrc++ << 8;	//�ȸ�λ�ֽ�
		wchar[i] |= *pSrc++;		//���λ�ֽ�
	}

	//UNICODE��-->�ַ���
	nDstLength = WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);

	//����ַ����Ӹ�������
	pDst[nDstLength] = '\0';

	//����Ŀ���ַ�������
	return nDstLength;
}
#endif


//����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
//�磺"8613851872468" --> "683158812764F8"
//����: pSrc - Դ�ַ���ָ��
//     nSrcLength - Դ�ַ�������
//���: pDst - Ŀ���ַ���ָ��
//����: Ŀ���ַ�������
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int i, nDstLength;		//Ŀ���ַ�������
	char ch;			//���ڱ���һ���ַ�

	//���ƴ�����
	nDstLength = nSrcLength;

	//�����ߵ�
	for(i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;		//�����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++;	//���ƺ���ֵ��ַ�
		*pDst++ = ch;		//�����ȳ��ֵ��ַ�
	}

	//Դ��������������
	if(nSrcLength & 1)
	{
		*(pDst-2) = 'F';	//��'F'
		nDstLength++;		//Ŀ�괮���ȼ�1
	}

	//����ַ����Ӹ�������
	*pDst = '\0';

	//����Ŀ���ַ�������
	return nDstLength;
}

//�����ߵ����ַ���ת��Ϊ����˳����ַ���
//�磺"683158812764F8" --> "8613851872468"
//����: pSrc - Դ�ַ���ָ��
//     nSrcLength - Դ�ַ�������
//���: pDst - Ŀ���ַ���ָ��
//����: Ŀ���ַ�������
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int i, nDstLength;		//Ŀ���ַ�������
	char ch;			//���ڱ���һ���ַ�

	//���ƴ�����
	nDstLength = nSrcLength;

	//�����ߵ�
	for(i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;		//�����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++;	//���ƺ���ֵ��ַ�
		*pDst++ = ch;		//�����ȳ��ֵ��ַ�
	}

	//�����ַ���'F'��
	if(*(pDst-1) == 'F')
	{
		pDst--;
		nDstLength--;		//Ŀ���ַ������ȼ�1
	}

	//����ַ����Ӹ�������
	*pDst = '\0';

	//����Ŀ���ַ�������
	return nDstLength;
}

//PDU���룬���ڱ��ơ����Ͷ���Ϣ
//����: pSrc - ԴPDU����ָ��
//���: pDst - Ŀ��PDU��ָ��
//����: Ŀ��PDU������
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst)
{
	int nLength;			//�ڲ��õĴ�����
	int nDstLength;			//Ŀ��PDU������
	U8 pBuf[256];	//�ڲ��õĻ�����

	//SMSC��ַ��Ϣ��
	nLength = rt_strlen(pSrc->SCA);	//SMSC��ַ�ַ����ĳ���	
	pBuf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;	//SMSC��ַ��Ϣ����
	pBuf[1] = 0x91;		//�̶�: �ù��ʸ�ʽ����
	nDstLength = gsmBytes2String(pBuf, pDst, 2);		//ת��2���ֽڵ�Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);	//ת��SMSC���뵽Ŀ��PDU��

	//TPDU�λ���������Ŀ���ַ��
	nLength = rt_strlen(pSrc->TPA);	//TP-DA��ַ�ַ����ĳ���
	pBuf[0] = 0x11;					//�Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
	pBuf[1] = 0;						//TP-MR=0
	pBuf[2] = (char)nLength;			//Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
	pBuf[3] = 0x91;					//�̶�: �ù��ʸ�ʽ����
	nDstLength += gsmBytes2String(pBuf, &pDst[nDstLength], 4);		//ת��4���ֽڵ�Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);	//ת��TP-DA��Ŀ��PDU��

	//TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	if (pSrc->TP_DCS == GSM_UCS2) {
		nLength = rt_strlen(pSrc->TP_UD);
	} else {
		nLength = pSrc->nUD;
	}
	pBuf[0] = pSrc->TP_PID;			//Э���ʶ(TP-PID)
	pBuf[1] = pSrc->TP_DCS;			//�û���Ϣ���뷽ʽ(TP-DCS)
	pBuf[2] = 0;						//��Ч��(TP-VP)Ϊ5����
	if(pSrc->TP_DCS == GSM_7BIT)	
	{
		//7-bit���뷽ʽ
		pBuf[3] = nLength;			//����ǰ����
		nLength = gsmEncode7bit(pSrc->TP_UD, &pBuf[4], nLength+1) + 4;	//ת��TP-DA��Ŀ��PDU��
	}
#if GSM_UNICODE
	else if(pSrc->TP_DCS == GSM_UCS2)
	{
		//UCS2���뷽ʽ
		pBuf[3] = gsmEncodeUcs2(pSrc->TP_UD, &pBuf[4], nLength);	//ת��TP-DA��Ŀ��PDU��
		nLength = pBuf[3] + 4;		//nLength���ڸö����ݳ���
	}
#endif
	else
	{
		//8-bit���뷽ʽ
		pBuf[3] = gsmEncode8bit(pSrc->TP_UD, &pBuf[4], nLength);	//ת��TP-DA��Ŀ��PDU��
		nLength = pBuf[3] + 4;		//nLength���ڸö����ݳ���
	}
	nDstLength += gsmBytes2String(pBuf, &pDst[nDstLength], nLength);		//ת���ö����ݵ�Ŀ��PDU��

	//����Ŀ���ַ�������
	return nDstLength;
}

//PDU���룬���ڽ��ա��Ķ�����Ϣ
//����: pSrc - ԴPDU��ָ��
//���: pDst - Ŀ��PDU����ָ��
//����: �û���Ϣ������
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nDstLength;			//Ŀ��PDU������
	U8 tmp;		//�ڲ��õ���ʱ�ֽڱ���
	U8 pBuf[256];	//�ڲ��õĻ�����

	//SMSC��ַ��Ϣ��
	gsmString2Bytes(pSrc, &tmp, 2);	//ȡ����
	tmp = (tmp - 1) * 2;	//SMSC���봮����
	pSrc += 4;			//ָ����ƣ�������SMSC��ַ��ʽ
	gsmSerializeNumbers(pSrc, pDst->SCA, tmp);	//ת��SMSC���뵽Ŀ��PDU��
	pSrc += tmp;		//ָ�����

	//TPDU�λ�������
	gsmString2Bytes(pSrc, &tmp, 2);	//ȡ��������
	pSrc += 2;		//ָ�����

	//ȡ�ظ�����
	gsmString2Bytes(pSrc, &tmp, 2);	//ȡ����
	if(tmp & 1) tmp += 1;	//������ż��
	pSrc += 4;			//ָ����ƣ������˻ظ���ַ(TP-RA)��ʽ
	gsmSerializeNumbers(pSrc, pDst->TPA, tmp);	//ȡTP-RA����
	pSrc += tmp;		//ָ�����

	//TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	gsmString2Bytes(pSrc, (U8*)&pDst->TP_PID, 2);	//ȡЭ���ʶ(TP-PID)
	pSrc += 2;		//ָ�����
	gsmString2Bytes(pSrc, (U8*)&pDst->TP_DCS, 2);	//ȡ���뷽ʽ(TP-DCS)
	pSrc += 2;		//ָ�����
	gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);		//����ʱ����ַ���(TP_SCTS) 
	pSrc += 14;		//ָ�����
	gsmString2Bytes(pSrc, &tmp, 2);	//�û���Ϣ����(TP-UDL)
	pSrc += 2;		//ָ�����
	if(pDst->TP_DCS == GSM_7BIT)
	{
		//7-bit����
		nDstLength = gsmString2Bytes(pSrc, pBuf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);	//��ʽת��
		gsmDecode7bit(pBuf, pDst->TP_UD, nDstLength);	//ת����TP-DU
		nDstLength = tmp;
	}
#if GSM_UNICODE
	else if(pDst->TP_DCS == GSM_UCS2)
	{
		//UCS2����
		nDstLength = gsmString2Bytes(pSrc, pBuf, tmp * 2);			//��ʽת��
		nDstLength = gsmDecodeUcs2(pBuf, pDst->TP_UD, nDstLength);	//ת����TP-DU
	}
#endif
	else
	{
		//8-bit����
		nDstLength = gsmString2Bytes(pSrc, pBuf, tmp * 2);			//��ʽת��
		nDstLength = gsmDecode8bit(pBuf, pDst->TP_UD, nDstLength);	//ת����TP-DU
	}

	//����Ŀ���ַ�������
	return nDstLength;
}


int modem_GetSms(SM_PARAM *pMsg)
{
	U32 i, j;
	int k, nResult = 0;
	BUFFER buf = {0};

	buffer_Free(&buf);
	g_pbufModem = &buf;
	uart_Send(UART_MODEM, "AT+CMGL=4\r\n", 11);
	for (i = 60; i; --i) {
		os_dly_wait(5);
		modem_RecData();
		if ((k = modem_FindStr(&buf, 0, "+CMGL:")) != -1) {
			for (j = 100; j; --j) {
				os_dly_wait(5);
				modem_RecData();
				if ((modem_FindStr(&buf, k, "+CMGL:") != -1) || (modem_FindStr(&buf, k, "OK") != -1)) {
					break;
				}
			}
			//����"+CMGL:",��ȡ���
			k += 6;
			sscanf((char *)buf.p + k, "%d", (int *)&pMsg->nIndex);
			//����һ��
			if ((k = modem_FindStr(&buf, k, "\r\n")) != -1) {
				nResult = gsmDecodePdu((char *)buf.p + k + 2, pMsg);	//PDU������
			}
			break;
		} else if (modem_FindStr(&buf, 0, "OK") != -1) {
			break;
		}
	}
	modem_BufFree(&buf);
	return nResult;
}

int modem_SendSms(SM_PARAM *pSrc)
{
	int nPduLength;				//PDU������
	unsigned char nSmscLength;	//SMSC������
	char cmd[16], *pPdu;		//���
	BUFFER buf = {0};

	pPdu = (char *)mem_Malloc(512);
	nPduLength = gsmEncodePdu(pSrc, pPdu);	//����PDU����������PDU��
	strcat(pPdu, "\x01a");						//��Ctrl-Z����
	gsmString2Bytes(pPdu, &nSmscLength, 2);		//ȡPDU���е�SMSC��Ϣ����
	//�����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
	nSmscLength++;								//���ϳ����ֽڱ���
	sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);	//��������
	if (modem_SendCmd(cmd, "\r\n> ", &buf)) {
		//�õ��϶��ش𣬼������PDU��
		nPduLength = modem_SendCmd(pPdu, "OK", &buf);
	}
	mem_Free((U8 **)&pPdu);
	modem_BufFree(&buf);
	return nPduLength;
}

int modem_DeleteSms(int nIndex)
{
	int nResult = 0;
	char arrCmd[48];
	BUFFER buf = {0};

	sprintf(arrCmd, "AT+CMGD=%d\r\n", nIndex);	//��������
	nResult = modem_SendCmd(arrCmd, "OK", &buf);
	modem_BufFree(&buf);
	return nResult;
}
#endif


#endif


