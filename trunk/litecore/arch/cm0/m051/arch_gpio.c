


//Private Defines


//Private Variables


//Internal Functions




//External Functions
void arch_GpioConf(uint_t nPort, uint_t nPin, uint_t nMode, uint_t nInit)
{
	uint_t nMove, nValue;
	uint32_t nPMD, nPOUT;

	nPMD = PORT0_BASE + 0x40 * nPort;
	nPOUT = nPMD + 0x08;

	//Set Init Status
	if (nMode & GPIO_M_OUT_MASK) {
		switch (nInit) {
		case GPIO_INIT_HIGH:
			__raw_writel(__raw_readl(nPOUT) | (1 << nPin), nPOUT);
			break;
		case GPIO_INIT_LOW:
			__raw_writel(__raw_readl(nPOUT) & ~(1 << nPin), nPOUT);
			break;
		default:
			break;
		}
	}
	//Set as GPIO function
	nMove = nPin << 1;
	nValue = __raw_readl(nPMD) & ~(3 << nMove);
	switch (nMode) {
	case GPIO_M_OUT_PP:
	case GPIO_M_AF_PP:
		nValue |= (1 << nMove);
		break;
	case GPIO_M_OUT_OD:
	case GPIO_M_AF_OD:
		nValue |= (2 << nMove);
		break;
	default:
		break;
	}
	__raw_writel(nValue, nPMD);
}

void arch_GpioSet(uint_t nPort, uint_t nPin, uint_t nHL)
{
	uint32_t nPOUT = PORT_BIT_DOUT_BASE + nPort * 0x20 + nPin * 0x04;

	if (nHL)
		nHL = 1;
	__raw_writel(nHL, nPOUT);
}

int arch_GpioRead(uint_t nPort, uint_t nPin)
{
	uint32_t nPOUT = PORT_BIT_DOUT_BASE + nPort * 0x20 + nPin * 0x04;

	return __raw_readl(nPOUT);
}



