


//-------------------------------------------------------------------------
//数据转换
//-------------------------------------------------------------------------
uint32_t gw3761_ConvertDa2DA(uint_t nDa)
{

#if GW3761_TYPE == GW3761_T_GWJC2009
	if (nDa) {
		nDa -= 1;
		return (((nDa >> 3) + 1) << 8) | BITMASK(nDa & 7);
	}
#else
	if (nDa)
		return (0x00000100 << ((nDa - 1) & 7)) | (0x00000001 << (nDa >> 3));
#endif
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
uint32_t gw3761_ConvertFn2Du(uint_t nDa, uint_t nFn)
{

	nFn -= 1;
	return ((0x00010000 << (nFn % 8)) | ((nFn / 8) << 24) | gw3761_ConvertDa2DA(nDa));
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
uint_t gw3761_ConvertDt2Fn(uint_t nDt)
{
	uint_t i;

	for (i = 0; i < 8; i++)
		if (nDt & BITMASK(i))
			break;
	if (i < 8)
		return ((nDt >> 5) & 0x7F8) | i + 1;
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
uint_t gw3761_ConvertDa2Map(uint_t nDA, void *pData)
{
#if GW3761_TYPE == GW3761_T_GWJC2009
	uint16_t *p = (uint16_t *)pData;
#else
	uint_t i;
	uint8_t *p = (uint8_t *)pData;
#endif
	uint_t j, nQty = 0, nDa;

	if (nDA == 0xFFFF)
		return 1;
	nDa = nDA >> 8;
	if (nDa == 0) {
		*p = TERMINAL;
		return 1;
	}
	nDa -= 1;
#if GW3761_TYPE == GW3761_T_GWJC2009
	for (j = 0; j < 8; j++)
		if (nDA & BITMASK(j))
			p[nQty++] = nDa * 8 + j + 1;
#else
	for (i = 0; i < 8; i++)
		if (nDa & BITMASK(i))
			for (j = 0; j < 8; j++)
				if (nDA & BITMASK(j))
					p[nQty++] = i * 8 + j;
#endif
	return nQty;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData(void *p, uint32_t nData, uint_t nDec, uint32_t nMark, uint_t nSignBit, uint_t nSize, uint_t nSign)
{
	uint32_t nResult;
	float fData;

	if (nSign) {
		if ((fixpoint)nData < 0) {
			nData = abs((sint32_t)nData);
			nSign = BITMASK(nSignBit);
		} else {
			nSign = 0;
		}
	}
	fData = (float)nData * math_pow10[nDec] * FIXP_EPSILON + 0.5f;
	nResult = bin2bcd32(fData) & nMark;
	nResult |= nSign;
	memcpy(p, &nResult, nSize);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_Time(uint8_t *p, time_t tTime, uint_t nType)
{
	struct tm tmTime;
	uint_t nFlag;

	switch (nType) {
	case GW3761_DATA_T_01:
		nFlag = 0x7F;
		break;
	case GW3761_DATA_T_15:
		nFlag = 0x3E;
		break;
	case GW3761_DATA_T_17:
		nFlag = 0x1E;
		break;
	case GW3761_DATA_T_18:
		nFlag = 0x0E;
		break;
	default:
		return;
	}
	localtime_r(&tTime, &tmTime);
	if (nFlag & BITMASK(0))
		*p++ = bin2bcd8(tmTime.tm_sec);
	if (nFlag & BITMASK(1))
		*p++ = bin2bcd8(tmTime.tm_min);
	if (nFlag & BITMASK(2))
		*p++ = bin2bcd8(tmTime.tm_hour);
	if (nFlag & BITMASK(3))
		*p++ = bin2bcd8(tmTime.tm_mday);
	if (nFlag & BITMASK(4)) {
		*p = bin2bcd8(tmTime.tm_mon + 1);
		if (nFlag & BITMASK(6)) {
			if (tmTime.tm_wday == 0)
				tmTime.tm_wday = 7;
			*p |= (tmTime.tm_wday << 5);
		}
		p += 1;
	}
	if (nFlag & BITMASK(5))
		*p++ = bin2bcd8(tmTime.tm_year - 100);
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_01(void *p, time_t tTime)
{

	gw3761_ConvertData_Time(p, tTime, 1);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_03(void *p, uint32_t nData, uint_t nSign)
{

	gw3761_ConvertData(p, nData, 0, 0x0FFFFFFF, 28, 4, nSign);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_05(void *p, uint32_t nData, uint_t nSign)
{

	gw3761_ConvertData(p, nData, 1, 0x00007FFF, 15, 2, nSign);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_05_Percent(void *p, uint32_t nData, uint_t nSign)
{

	gw3761_ConvertData(p, nData, 3, 0x00007FFF, 15, 2, nSign);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_06(void *p, uint32_t nData, uint_t nSign)
{

	gw3761_ConvertData(p, nData, 2, 0x00007FFF, 15, 2, nSign);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_07(void *p, uint32_t nData)
{

	gw3761_ConvertData(p, nData, 1, 0x0000FFFF, 0, 2, 0);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_09(void *p, uint32_t nData, uint_t nSign)
{

	gw3761_ConvertData(p, nData, 4, 0x007FFFFF, 23, 3, nSign);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_11(void *p, uint32_t nData)
{

	gw3761_ConvertData(p, nData, 2, 0xFFFFFFFF, 0, 4, 0);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_13(void *p, uint32_t nData)
{

	gw3761_ConvertData(p, nData, 4, 0xFFFFFFFF, 0, 4, 0);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_14(uint8_t *p, float fData)
{
	uint64_t nResult = 0;
 
	fData = fData * 100.0f + 0.5f;
	nResult = bin2bcd64(fData) & (uint64_t)0x000000FFFFFFFFFF;
	*p++ = 0;
	memcpy(p, &nResult, 4);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_15(void *p, time_t tTime)
{

	gw3761_ConvertData_Time(p, tTime, GW3761_DATA_T_15);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_17(void *p, time_t tTime)
{

	gw3761_ConvertData_Time(p, tTime, GW3761_DATA_T_17);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_18(void *p, time_t tTime)
{

	gw3761_ConvertData_Time(p, tTime, GW3761_DATA_T_18);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_22(void *p, uint32_t nData)
{

	gw3761_ConvertData(p, nData, 1, 0x000000FF, 0, 1, 0);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_23(void *p, uint32_t nData)
{

	gw3761_ConvertData(p, nData, 4, 0x00FFFFFF, 0, 3, 0);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_25(void *p, uint32_t nData, uint_t nSign)
{

	gw3761_ConvertData(p, nData, 3, 0x007FFFFF, 23, 3, nSign);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void gw3761_ConvertData_26(void *p, uint32_t nData)
{

	gw3761_ConvertData(p, nData, 3, 0x0000FFFF, 0, 2, 0);
}


//-------------------------------------------------------------------------
//谐波含有率到有效值的转换
//-------------------------------------------------------------------------
#if REGISTER_ENABLE
void gw3761_ConvertData_THD(uint_t nDa, uint_t nPulse, void *pBuf)
{
	float fBase, fTemp;
	uint8_t *pData = (uint8_t *)pBuf;
	uint_t i;
	uint16_t arrThd[38];

	///////////////////////////Unfinished///////////////////////////////
	fBase = FIXP_EPSILON * reg_GetValue(nDa, 0x1211 + nPulse);	//nID_CurVolPHA		//A相电压
	reg_Get(nDa, 0x2131 + nPulse, arrThd);						//nID_HarPVolPHA	//A相电压谐波含有率
	//////////////////////////////////////////////////////////////////
	for (i = 0; i < 19; ++i, pData += 2) {
		fTemp = (float)bcd2bin32(arrThd[i]) * fBase / 1000.0f;
		if (nPulse < 3) {
			gw3761_ConvertData_07(pData, FLOAT2FIX(fTemp));
		} else {
			gw3761_ConvertData_06(pData, FLOAT2FIX(fTemp), 1);
		}
	}
}
#endif


