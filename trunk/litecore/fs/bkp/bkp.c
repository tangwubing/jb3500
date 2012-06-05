
#if BKP_ENABLE

//Private Defines
#define EEPROM_PAGE_SIZE			4




//Internal Functions
static sys_res bkp_EepromWrite(uint_t nAdr, const uint8_t *pBuf, uint_t nLen)
{
	p_dev_i2c p;
	uint_t nEnd, nSize;
	uint8_t aBuf[EEPROM_PAGE_SIZE + 2];

	if ((p = i2c_Get(BKP_COMID, OS_TMO_FOREVER)) == NULL)
		return SYS_R_TMO;
	nEnd = nAdr + nLen;
	for (; nAdr < nEnd; nAdr += nSize, pBuf += nSize, nLen -= nSize) {
		aBuf[0] = nAdr >> 8;
		aBuf[1] = nAdr;
		nSize = nLen < EEPROM_PAGE_SIZE ? nLen : EEPROM_PAGE_SIZE;
		memcpy(&aBuf[2], pBuf, nSize);
		if (i2c_Write(p, BKP_DEVID, aBuf, nSize + 2))
			break;
		os_thd_Sleep(20);
	}
	i2c_Release(p);
	if (nAdr < nEnd)
		return SYS_R_ERR;
	return SYS_R_OK;
}








//External Functions
void bkp_Init()
{

#if BKP_TYPE == BKP_T_INT
	arch_BkpInit();
#endif
}

//-------------------------------------------------------------------------
//Function Name  :bkp_Write
//Description    : ����nStartAdr��ͷ��nLen���ֽ�д��BKP�ļĴ�����
//				   ÿ��дһ���ֽڣ�����Ϊ*P
//Input          : -nStartAdr:0~19������ ��BKP��10��Ĵ�����2���ֽ�Ϊһ��
//				-*p:д��BKP�����ݣ�һ��дһ���ֽ�
//				-nLen:1~(20-nStartAdr)
//Output         : Non
//Return         : SYS_R_ERR ��SYS_R_OK
//-------------------------------------------------------------------------
sys_res bkp_Write(uint_t nAdr, const void *pBuf, uint_t nLen)
{

#if BKP_TYPE == BKP_T_INT
	return arch_BkpWrite(nAdr, pBuf, nLen);
#else
	return bkp_EepromWrite(nAdr, pBuf, nLen);
#endif
}


sys_res bkp_WriteData(uint_t nAdr, const uint64_t nData, uint_t nLen)
{

	return bkp_Write(nAdr, &nData, nLen);
}

void bkp_Fill(uint_t nAdr, uint_t nEnd, uint_t nVal)
{
	uint_t nLen, nFill;
	uint8_t aBuf[64];

	memset(aBuf, nVal, sizeof(aBuf));
	nLen = nEnd - nAdr;
	for (; nLen; nLen -= nFill, nAdr += nFill) {
		nFill = MIN(sizeof(aBuf), nLen);
		bkp_Write(nAdr, aBuf, nFill);
	}
}


//-------------------------------------------------------------------------
//Function Name  :bkp_Read
//Description    : ����nStartAdr��ͷ��nLen���ֽڴ�BKP�Ĵ���  ����
//				   ÿ�ζ�һ���ֽڣ�����Ϊ*P
//Input          : -nStartAdr:0~19������ ��BKP��10��Ĵ�����2���ֽ�Ϊһ��
//				-*p: ��������BKP�Ĵ��������ݣ�һ�ζ�һ���ֽ�
//				-nLen:1~(20-nStartAdr)
//Output         : None
//Return         : SYS_R_ERR ��SYS_R_OK
//-------------------------------------------------------------------------
sys_res bkp_Read(uint_t nAdr, void *pBuf, uint_t nLen)
{

#if BKP_TYPE == BKP_T_INT
	return arch_BkpRead(nAdr, pBuf, nLen);
#else
	sys_res res;
	p_dev_i2c p;

	if ((p = i2c_Get(BKP_COMID, OS_TMO_FOREVER)) == NULL)
		return SYS_R_TMO;
	reverse(&nAdr, 2);
	res = i2c_Write(p, BKP_DEVID, &nAdr, 2);
	if (res == SYS_R_OK)
		res = i2c_Read(p, BKP_DEVID, pBuf, nLen);
	i2c_Release(p);
	return res;
#endif
}

#endif

