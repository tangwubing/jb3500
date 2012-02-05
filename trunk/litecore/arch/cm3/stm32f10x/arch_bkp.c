

//Private Defines
#define STM32_BKP_SIZE      16


/*******************************************************************************
* Function Name  : stm32_BKPinit
* Description    : ��ʼ��BKP
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void arch_BkpInit(void)
{
	/* Enable PWR and BKP clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
}

/*******************************************************************************
* Function Name  :stm32_WriteBKP
* Description    : ����nStartAdr��ͷ��nLen���ֽ�д��BKP�ļĴ�����
				   ÿ��дһ���ֽڣ�����Ϊ*P
* Input          : -nStartAdr:0~19������ ��BKP��10��Ĵ�����2���ֽ�Ϊһ��
				-*p:д��BKP�����ݣ�һ��дһ���ֽ�
				-nLen:1~(20-nStartAdr)
* Output         : Non
* Return         : SYS_R_ERR ��SYS_R_OK
*******************************************************************************/
sys_res arch_BkpWrite(uint_t nStartAdr, uint8_t *p, uint_t nLen)
{
	uint_t nReg;

	if ((int)nLen > (STM32_BKP_SIZE - nStartAdr))
		return SYS_R_ERR;
	nReg = ((nStartAdr >> 1) << 2) + 4;
	if (nStartAdr & 1) {
		BKP_WriteBackupRegister(nReg, (BKP_ReadBackupRegister(nReg) & 0x00FF) | (*p++ << 8));
		nLen -= 1;
	}
	for (; nLen; nReg += 4) {
		if (--nLen) {
			BKP_WriteBackupRegister(nReg, *p | *(p + 1) << 8);
			p += 2;
			nLen -= 1;
		} else
			BKP_WriteBackupRegister(nReg, (BKP_ReadBackupRegister(nReg) & 0xFF00) | *p++);
	}
	return SYS_R_OK;
}

/*******************************************************************************
* Function Name  :stm32_ReadBKP
* Description    : ����nStartAdr��ͷ��nLen���ֽڴ�BKP�Ĵ���  ����
				   ÿ�ζ�һ���ֽڣ�����Ϊ*P
* Input          : -nStartAdr:0~19������ ��BKP��10��Ĵ�����2���ֽ�Ϊһ��
				-*p: ��������BKP�Ĵ��������ݣ�һ�ζ�һ���ֽ�
				-nLen:1~(20-nStartAdr)
* Output         : None
* Return         : SYS_R_ERR ��SYS_R_OK
*******************************************************************************/
sys_res arch_BkpRead(uint_t nStartAdr, uint8_t *p, uint_t nLen)
{
	uint16_t nTemp;
	uint_t nReg;

	if ((int)nLen > (STM32_BKP_SIZE - nStartAdr))
		return SYS_R_ERR;
	nReg = ((nStartAdr >> 1) << 2) + 4;
	if (nStartAdr & 1) {
		*p++ = BKP_ReadBackupRegister(nReg) >> 8;
		nLen--;
	}
	for (; nLen; nReg += 4) {
		nTemp = BKP_ReadBackupRegister(nReg);
		*p++ = nTemp;  //�ѵ�λ����*p
		if (--nLen)
			*p++ = nTemp >> 8, nLen -= 1;
	}
	return SYS_R_ERR;
}

