#if SPI_ENABLE & !SPI_SOFTWARE

//Private Variables
static SPI_TypeDef * const stm32_tblSpiId[] = {SPI1, SPI2, SPI3};



//Private Functions
static void stm32_SpiApbClockCmd(SPI_TypeDef *pSpi, FunctionalState ste)
{

	switch ((u32)pSpi) {
	case SPI1_BASE:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ste);
		break;
	case SPI2_BASE:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ste);
		break;
	case SPI3_BASE:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ste);
		break;
	default:
		break;
	}
}





//External Functions
void arch_SpiInit(p_dev_spi p)
{
	p_spi_def pDef = &tbl_bspSpiDef[p->parent->id];
	SPI_TypeDef *pSpi = stm32_tblSpiId[pDef->chipid];
	GPIO_InitTypeDef xGpio;

	//Enable SPI Clock
	stm32_SpiApbClockCmd(pSpi, ENABLE);

	xGpio.GPIO_Speed = GPIO_Speed_10MHz;
	if (pDef->outmode == DEV_PIN_OD)
		xGpio.GPIO_Mode = GPIO_Mode_AF_OD;
	else
		xGpio.GPIO_Mode = GPIO_Mode_AF_PP;
	//SCK
	xGpio.GPIO_Pin = BITMASK(pDef->sckpin);
	stm32_GpioClockEnable(pDef->sckport);
	GPIO_SetBits(arch_GpioPortBase(pDef->sckport), BITMASK(pDef->sckpin));
	GPIO_Init(arch_GpioPortBase(pDef->sckport), &xGpio);
	//MOSI
	xGpio.GPIO_Pin = BITMASK(pDef->mosipin);
	stm32_GpioClockEnable(pDef->mosiport);
	GPIO_SetBits(arch_GpioPortBase(pDef->mosiport), BITMASK(pDef->mosipin));
	GPIO_Init(arch_GpioPortBase(pDef->mosiport), &xGpio);
	//MISO
	xGpio.GPIO_Pin = BITMASK(pDef->misopin);
	stm32_GpioClockEnable(pDef->misoport);
	GPIO_SetBits(arch_GpioPortBase(pDef->misoport), BITMASK(pDef->misopin));
	GPIO_Init(arch_GpioPortBase(pDef->misoport), &xGpio);
	//NSS
	xGpio.GPIO_Pin = BITMASK(pDef->nsspin);
	stm32_GpioClockEnable(pDef->nssport);
	GPIO_SetBits(arch_GpioPortBase(pDef->nssport), BITMASK(pDef->nsspin));
	GPIO_Init(arch_GpioPortBase(pDef->nssport), &xGpio);
}

sys_res arch_SpiConfig(p_dev_spi p)
{
	p_spi_def pDef = &tbl_bspSpiDef[p->parent->id];
	SPI_TypeDef *pSpi = stm32_tblSpiId[pDef->chipid];
 	SPI_InitTypeDef xSpi;

	xSpi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	xSpi.SPI_Mode = SPI_Mode_Master;
	xSpi.SPI_DataSize = SPI_DataSize_8b;
	if (p->sckmode == SPI_SCKIDLE_HIGH)
		xSpi.SPI_CPOL = SPI_CPOL_High;
	else
		xSpi.SPI_CPOL = SPI_CPOL_Low;
	if (p->latchmode == SPI_LATCH_1EDGE)
		xSpi.SPI_CPHA = SPI_CPHA_1Edge;
	else
		xSpi.SPI_CPHA = SPI_CPHA_2Edge;
	xSpi.SPI_NSS = SPI_NSS_Soft;
	xSpi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	xSpi.SPI_FirstBit = SPI_FirstBit_MSB;
	xSpi.SPI_CRCPolynomial = 7;
	SPI_Init(pSpi, &xSpi);
	SPI_Cmd(pSpi, ENABLE);
	return SYS_R_OK;
}

sys_res arch_SpiSend(p_dev_spi p, const void *p, uint_t nLen)
{
	SPI_TypeDef *pSpi = stm32_tblSpiId[tbl_bspSpiDef[p->parent->id].chipid];
	uint8_t *pData = (uint8_t *)p;

	for (; nLen; nLen--) {
		while (SPI_I2S_GetFlagStatus(pSpi, SPI_I2S_FLAG_TXE) == RESET);
		pSpi->DR = *pData++;
	}
	return SYS_R_OK;
}

sys_res arch_SpiRecv(p_dev_spi p, uint8_t *pRec, uint_t nLen)
{
	SPI_TypeDef *pSpi = stm32_tblSpiId[tbl_bspSpiDef[p->parent->id].chipid];

	for (; nLen; nLen--) {
		while (SPI_I2S_GetFlagStatus(pSpi, SPI_I2S_FLAG_RXNE) == RESET);
		*pRec++ = pSpi->DR;
	}
	return SYS_R_OK;
}

sys_res arch_SpiTransce(p_dev_spi p, uint_t nCmd, uint8_t *pRec, uint_t nLen)
{
	SPI_TypeDef *pSpi = stm32_tblSpiId[tbl_bspSpiDef[p->parent->id].chipid];

	while (SPI_I2S_GetFlagStatus(pSpi, SPI_I2S_FLAG_TXE) == RESET);
	pSpi->DR = nCmd;
	for (; nLen; nLen--) {
		while (SPI_I2S_GetFlagStatus(pSpi, SPI_I2S_FLAG_RXNE) == RESET);
		*pRec++ = pSpi->DR;
	}
	return SYS_R_OK;
}


#endif



