#if NANDFLASH_ENABLE

#if NAND_BASE_ADR != FSMC_BASEADR_NAND2 && NAND_BASE_ADR != FSMC_BASEADR_NAND3
#error Nand Flash Base Address Error!!!!
#endif



__inline uint32_t arch_NandGetEcc()
{
	volatile uint32_t nEcc;

#if NAND_BASE_ADR == FSMC_BASEADR_NAND2
	nEcc = FSMC_GetECC(FSMC_Bank2_NAND);
	FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);
#elif NAND_BASE_ADR == FSMC_BASEADR_NAND3
	nEcc = FSMC_GetECC(FSMC_Bank3_NAND);
	FSMC_NANDECCCmd(FSMC_Bank3_NAND, DISABLE);
#endif
	return nEcc;
}

__inline void arch_NandEccStart()
{

#if NAND_BASE_ADR == FSMC_BASEADR_NAND2
	FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE);
#elif NAND_BASE_ADR == FSMC_BASEADR_NAND3
	FSMC_NANDECCCmd(FSMC_Bank3_NAND, ENABLE);
#endif
}

#endif



