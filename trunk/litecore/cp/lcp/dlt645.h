#ifndef __LCP_DLT645_H__
#define __LCP_DLT645_H__

#ifdef __cplusplus
extern "C" {
#endif


//Public Defines
#define DLT645_HEADER_SIZE			10

#define DLT645_CODE_READ97			0x01
#define DLT645_CODE_WRITE97			0x04
#define DLT645_CODE_READ07			0x11
#define DLT645_CODE_CTRL07			0x1C
#define DLT645_CODE_BROADCAST		0x08








//External Functions
void dlt645_Packet2Buf(buf b, const void *pAdr, uint_t nC, const void *pData, uint_t nLen);
uint8_t *dlt645_PacketAnalyze(const uint8_t *p, uint_t nLen);
sys_res dlt645_Meter(chl c, buf b, uint_t nTmo);
sys_res dlt645_Transmit(chl c, buf b, uint_t nTmo);


#ifdef __cplusplus
}
#endif

#endif

