#ifndef __LCP_DLT645_H__
#define __LCP_DLT645_H__

#ifdef __cplusplus
extern "C" {
#endif


//�����붨��
#define DLT645_CODE_READ97		0x01
#define DLT645_CODE_READ07		0x11
#define DLT645_CODE_BROADCAST	0x08








//External Functions
void dlt645_Packet2Buf(buf b, const void *pAdr, uint_t nC, const void *pData, uint_t nLen);
uint8_t *dlt645_PacketAnalyze(uint8_t *p, uint_t nLen);
sys_res dlt645_Transmit2Meter(chl c, buf bRx, const void *pAdr, const void *pBuf, uint_t nLen, uint_t nTmo);


#ifdef __cplusplus
}
#endif

#endif
