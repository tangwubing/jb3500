#include <string.h>
#include <stdio.h>
#include <litecore.h>
#include "iap.h"

//Private Typedefs
typedef __packed struct {
	uint8_t 	flag;		// �ļ���ʶ
	uint8_t 	attrib;		// �ļ�����
	uint8_t 	cmd;		// �ļ�ָ��
	uint16_t	sector;		// �ܶ���n 
	uint32_t	offset;		// ��i �α�ʶ��ƫ�ƣ�i=0��n��
	uint16_t	size;		// ��i �����ݳ���
	uint8_t		data[];		// �ļ�����
}t_afn0F_f1;




//Internal Functions
static void iap_SetFlag(uint32_t nSize)
{
	struct iap_info iap;
	uint8_t aBuf[256];
	uint_t i, nLen;

	//��鴫������Ƿ���©��(����256���ֽ�0xFF)
	for (nLen = 0; nLen < nSize; i++, nLen += sizeof(aBuf)) {
		spif_Read((UPDATE_SECTOR_START + 1) * SPIF_SEC_SIZE + nLen, aBuf, sizeof(aBuf));
		for (i = 0; i < sizeof(aBuf); i++)
			if (aBuf[i] != 0xFF)
				break;
		if (i >= sizeof(aBuf))
			break;
	}
	if (nLen >= nSize) {
		iap.magic = IAP_MAGIC_WORD;
		iap.size = nSize;
		spif_Write(UPDATE_SECTOR_START * SPIF_SEC_SIZE, &iap, sizeof(struct iap_info));
		flash_Flush(0);
	}
}


//External Functions
int gw3761_ResponseFileTrans(p_gw3761 p, buf b, u_word2 *pDu, uint8_t **ppData)
{
	static uint8_t nUpdateVer = 0;				//�����汾
	static uint32_t nUpdateOffset = 0;			//����ƫ��
	t_afn0F_f1 f01, *pft;
	char str[6];

	if ((pDu->word[0] != TERMINAL) || (pDu->word[1] != 1))
		return 0;

	pft = (t_afn0F_f1 *)*ppData;
	*ppData += 11;
	switch(pft->cmd) {
	case 'R':			//���ļ�
	case 'D':			//ɾ���ļ�
		break;
	default:			//����
		if (pft->offset > pft->sector)
			break;
		switch (pft->offset) {
		case 0:
			// ѯ��
			memset(&f01, 0, sizeof(f01));
			snprintf(str, sizeof(str), "%04X", VER_SOFT);
			memcpy((void *)&f01.sector, &str[0], 2);
			memcpy((void *)&f01.size, &str[2], 2);
			if (nUpdateVer != pft->flag) {
				nUpdateVer = pft->flag;
				nUpdateOffset = 0;
			}
			f01.offset = nUpdateOffset;
			buf_Push(b, &f01, sizeof(t_afn0F_f1));
			break;
		case 1:
			nUpdateOffset = 0;
		default:
			if (pft->sector != pft->offset)
				nUpdateOffset = (pft->offset - 1) * pft->size;
			spif_Write((UPDATE_SECTOR_START + 1) * SPIF_SEC_SIZE + nUpdateOffset, pft->data, pft->size);
			nUpdateOffset += pft->size;
			*ppData += pft->size;
			if (pft->sector == pft->offset) {
				gw3761_TmsgConfirm(p);
				// do.. �ñ�־,�˳�Ӧ�ó���,����IAP
				iap_SetFlag(nUpdateOffset);
				os_thd_Sleep(10000);
				sys_Reset();
			}
			break;
		}
		break;
	}	
	return 1;
}



