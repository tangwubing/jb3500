#include <math.h>
#include <string.h>
#include <litecore.h>
#include <drivers/tdk6515.h>
#include "acm.h"
#include "data.h"


//Private Defines


//Pirvate Structs


//Pivate Consts



//Private Variables
static t_tdk6515 acm_xTdk6515;




//Public Variables
t_acm_rtdata acm_rtd;
t_acm_xbdata acm_uxb[3];
t_acm_xbdata acm_ixb[3];

//Internal Functions



//External Functions
void acm_Init()
{
	t_tdk6515 *p = &acm_xTdk6515;

	memset(&acm_rtd, 0, sizeof(acm_rtd));
	memset(&acm_uxb, 0, sizeof(acm_uxb));
	memset(&acm_ixb, 0, sizeof(acm_ixb));
	p->ste = 0;
	p->uart = uart_Get(TDK6515_COMID, OS_TMO_FOREVER);
	uart_Config(p->uart, 9600, UART_PARI_EVEN, UART_DATA_8D, UART_STOP_1D);
	tdk6515_Reset(p);
	p->ste = 1;
}

void acm_JLRead()
{
	t_tdk6515 *p = &acm_xTdk6515;
	t_acm_rtdata *pa = &acm_rtd;
	t_tdk6515_rtdata *pT;
	uint8_t *pTemp;
	uint_t i, nCRC;
	float fTemp;
	buf b = {0};

	if (tdk6515_IsJLReady() == SYS_R_OK) {
		tdk6515_CmdSend(p, 0, 0x0000, 48);
		uart_RecLength(p->uart, b, 48 * 4 + 2, 2000);
		nCRC = crc16(b->p, 48 * 4);
		if (memcmp(&nCRC, &b->p[48 * 4], 2) == 0) {
			buf_Unpush(b, 2);
			for (pTemp = b->p; pTemp < &b->p[b->len]; pTemp += 4)
				reverse(pTemp, 4);
			pT = (t_tdk6515_rtdata *)(b->p);
			//Ƶ��
			pa->freq = pT->freq / 100.0f;
			//��ѹ�Ƕ�
			pa->ua[0] = 0;
			pa->ua[1] = pT->caangle / 10.0f;
			pa->ua[2] = pT->cbangle / 10.0f;
			for (i = 0; i < 3; i++) {
				//��ѹ
				pa->u[i] = pT->u[i];
				//�����Ƕ�
				pa->ia[i] = pT->viangle[i + 1] + pa->ua[i];
				//�Ƕ�У��
				if (pa->ua[i] >= 360)
					pa->ua[i] -= 360;
				else if (pa->ua[i] <= -360)
					pa->ua[i] += 360;
				if (pa->ua[i] < 0)
					pa->ua[i] += 360;
				if (pa->ia[i] >= 360)
					pa->ia[i] -= 360;
				else if (pa->ia[i] <= -360)
					pa->ia[i] += 360;
				if (pa->ia[i] < 0)
					pa->ia[i] += 360;
			}
			for (i = 0; i < 4; i++) {
				//����
				pa->i[i] = pT->i[i];
				//����
				pa->pp[i] = pT->p[i] / 1000.0f;
				pa->pq[i] = pT->q[i] / 1000.0f;
				pa->ui[i] = pT->ui[i] / 1000.0f;
				//��������
				pa->cos[i] = pT->cos[i];
			}
			//��ƽ���
			fTemp = (pa->u[0] + pa->u[1] + pa->u[2]) / 3;
			pa->ub = (fabs(fTemp - pa->u[0]) + fabs(fTemp - pa->u[1]) + fabs(fTemp - pa->u[2])) / fTemp / 3;
			fTemp = (pa->i[0] + pa->i[1] + pa->i[2]) / 3;
			pa->ib = (fabs(fTemp - pa->i[0]) + fabs(fTemp - pa->i[1]) + fabs(fTemp - pa->i[2])) / fTemp / 3;
		}
		buf_Release(b);
	}		 
}

void acm_XBRead()
{
	t_tdk6515 *p = &acm_xTdk6515;
	t_acm_xbdata *pD;
	t_tdk6515_xbdata *pT;
	uint8_t *pTemp;
	uint_t i, j, nCRC;
	buf b = {0};

	if (tdk6515_IsXBReady() == SYS_R_OK)	  {
		tdk6515_CmdSend(p, 0, 0x011C, 24);		//��ʼ��ַΪ0x011C ����24
		uart_RecLength(p->uart, b, 24 * 4 + 2, 2000);	//���24*4+2���ֽ�
		nCRC = crc16(b->p, 24 * 4); 			//����24*4���ֽ�crc16ֵ ��0x0011C��ʼ
		if (memcmp(&nCRC, &b->p[24 * 4], 2) == 0) {
			buf_Unpush(b, 2);
			for (pTemp = b->p; pTemp < &b->p[b->len]; pTemp += 4)
				reverse(pTemp, 4);
			pT = (t_tdk6515_xbdata *)b->p;
			i = pT->curxbno;
			if (i < 6) {
				if (i & 1) {
					switch (i) {
					case 3:
						pD = &acm_uxb[1];
						break;
					case 5:
						pD = &acm_uxb[2];
						break;
					default:
						pD = &acm_uxb[0];
						break;
					}
				} else
					pD = &acm_ixb[i >> 1];
				pD->base = pT->xbbase;
				for (j = 0; j <= 10; j++)
					pD->xbrate[j] = pT->xbrate[j * 2];
			}
		}
		buf_Release(b);
	}
}

void acm_MinSave(const uint8_t *pTime)
{
	t_acm_rtdata *pD = &acm_rtd;
	uint_t i;
	uint8_t *pTemp;
	t_data_min xData = {0};

	pTemp = xData.data;
	xData.time = rtc_GetTimet();
	for (i = 0; i < 3; i++)
		gw3761_ConvertData_07(&pTemp[ACM_MSAVE_VOL + i * 2], FLOAT2FIX(pD->u[i]));
	for (i = 0; i < 4; i++) {
		gw3761_ConvertData_25(&pTemp[ACM_MSAVE_CUR + i * 3], FLOAT2FIX(pD->i[i]), 1);
		gw3761_ConvertData_09(&pTemp[ACM_MSAVE_PP + i * 3], FLOAT2FIX(pD->pp[i]), 1);
		gw3761_ConvertData_09(&pTemp[ACM_MSAVE_PQ + i * 3], FLOAT2FIX(pD->pq[i]), 1);
		gw3761_ConvertData_05_Percent(&pTemp[ACM_MSAVE_COS + i * 2], FLOAT2FIX(pD->cos[i]), 1);
	}
	data_MinWrite(&pTime[1], &xData);
}

void acm_QuarterSave(const uint8_t *pTime)
{
	t_acm_rtdata *pD = &acm_rtd;
	t_acm_xbdata *pX;
	uint_t i, j;
	uint8_t *pTemp, aBuf[4];
	t_data_quarter xData = {0};

	pTemp = xData.data;
	xData.time = rtc_GetTimet();
	for (i = 0; i < 6; i++) {
		if (i < 3)
			pX = &acm_uxb[i];
		else
			pX = &acm_ixb[i - 3];
		for (j = 0; j < 11; j++) {
			gw3761_ConvertData_05(aBuf, FLOAT2FIX(pX->xbrate[j]), 0);
			memcpy(pTemp, aBuf, 2);
			pTemp += 2;
		}
	}
	for (i = 0; i < 3; i++) {
		gw3761_ConvertData_05(aBuf, FLOAT2FIX(pD->ua[i]), 0);
		memcpy(pTemp, aBuf, 2);
		pTemp += 2;
	}
	for (i = 0; i < 3; i++) {
		gw3761_ConvertData_05(aBuf, FLOAT2FIX(pD->ia[i]), 0);
		memcpy(pTemp, aBuf, 2);
		pTemp += 2;
	}
	gw3761_ConvertData_06(aBuf, FLOAT2FIX(pD->freq), 0);
	memcpy(pTemp, aBuf, 2);
	data_QuarterWrite(&pTime[1], &xData);
}



int acm_IsReady()
{

	return acm_xTdk6515.ste;
}










