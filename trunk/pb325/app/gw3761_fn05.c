#include <litecore.h>
#include "meter.h"


//Internal Functions


//External Functions
int gw3761_ResponseCtrlCmd(p_gw3761 p, u_word2 *pDu, uint8_t **ppData)
{
	int res = 0;
	uint_t i, nFn;
	
	for (i = 0; i < 8; i++) {
		if ((pDu->word[1] & BITMASK(i)) == 0)
			continue;
		nFn = gw3761_ConvertDt2Fn((pDu->word[1] & 0xFF00) | BITMASK(i));
		switch (nFn) {
		case 1:
			//ң����բ
			res += 1;
			break;
		case 31:
			//��ʱ����
			rtc_SetTimet(bin2timet((*ppData)[0], (*ppData)[1], (*ppData)[2], (*ppData)[3], (*ppData)[4] & 0x1F, (*ppData)[5], 1));
			*ppData += 6;
			res += 1;
			break;
		case 33:
			//������
			res += 1;
			break;
		case 37:
			//�����澯
			res += 1;
			break;
		default:
			break;
		}
	}
	return res;
}

