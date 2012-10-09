#include <lib/bcd.c>
#include <lib/ecc.c>
#include <lib/math.c>
#include <lib/string.c>
#include <lib/time.c>

#if OS_TYPE
#include <lib/memory.c>
#include <lib/buffer.c>
#endif

#include <stdlib.h>


//-------------------------------------------------------------------------
//�õ�uint32�����м�λ���ɵ�ֵ
//-------------------------------------------------------------------------
uint32_t int_getbits(uint32_t nData, uint_t nStartBit, uint_t nBits)
{
	uint32_t nMask;

	nMask = (0xFFFFFFFF >> (32 - nBits)) << nStartBit;
	return ((nData & nMask) >> nStartBit);
}

//-------------------------------------------------------------------------
//����uint32�����м�λ���ɵ�ֵ
//-------------------------------------------------------------------------
uint32_t int_setbits(uint32_t nData, uint_t nStartBit, uint_t nBits)
{
	uint32_t nMask;

	nMask = 0xFFFFFFFF >> (32 - nBits);
	return ((nData & nMask) << nStartBit);
}

//-------------------------------------------------------------------------
//���ָ��λ��ֵ
//-------------------------------------------------------------------------
uint_t getbit(void *pData, uint_t i)
{
	uint8_t *p = pData;
	uint_t j;

	j = i & 7;
	i >>= 3;
	if (p[i] & BITMASK(j))
		return 1;
	return 0;
}

//-------------------------------------------------------------------------
//��ָ��λ��ֵ
//-------------------------------------------------------------------------
void setbit(void *pData, uint_t i, uint_t nValue)
{
	uint8_t *p = pData;
	uint_t j;

	j = i & 7;
	i >>= 3;
	if (nValue)
		SETBIT(p[i], j);
	else
		CLRBIT(p[i], j);
}

//-------------------------------------------------------------------------
//����uint���ж���λ1
//-------------------------------------------------------------------------
uint_t bits1(uint_t i)
{
	uint_t j;
	
	for (j = 0; i; i &= (i - 1)) {
		j += 1;
	}
	return j;
}

//-------------------------------------------------------------------------
//��λ����
//-------------------------------------------------------------------------
uint_t invert_bits(uint_t nData, uint_t nBits)
{
	uint_t i, j, nTemp;

	for (i = 0, j = nBits - 1; i < j; i++, j--) {
		if (nData & BITMASK(i))
			nTemp = 1;
		else
			nTemp = 0;
		if (nData & BITMASK(j))
			SETBIT(nData, i);
		else
			CLRBIT(nData, i);
		if (nTemp)
			SETBIT(nData, j);
		else
			CLRBIT(nData, j);
	}
	return nData;
}

//-------------------------------------------------------------------------
//���ֽڵ���
//-------------------------------------------------------------------------
void reverse(void *pData, uint_t nLen)
{
	uint_t nTemp;
	uint8_t *pStart = pData;
	uint8_t *pEnd = pStart + nLen - 1;

	while (pStart < pEnd) {
		nTemp = *pStart;
		*pStart++ = *pEnd;
		*pEnd-- = nTemp;
	}
}

//-------------------------------------------------------------------------
//���ֽ�����
//-------------------------------------------------------------------------
void byteadd(void *pData, int nValue, uint_t nLen)
{
	uint8_t *pStart = pData;
	uint8_t *pEnd = pStart + nLen;
	int nTemp;

	for (; pStart < pEnd; pStart++) {
		nTemp = *pStart;
		nTemp += nValue;
		*pStart = nTemp;
	}
}

//-------------------------------------------------------------------------
//����ѭ��
//-------------------------------------------------------------------------
uint_t cycle(uint_t n, uint_t nStart, uint_t nEnd, int nStepping)
{
	int nResult;
	uint32_t nSpan;

	if (nStart > nEnd)
		return n;
	nSpan = nEnd - nStart + 1;
	if (n > nEnd)
		nResult = nEnd;
	else if (n < nStart)
		nResult = nStart;
	else
		nResult = n;
	//����
	if (nStepping < 0)
		nStepping = -(abs(nStepping) % nSpan);
	else
		nStepping = abs(nStepping) % nSpan;
	nResult = ((nResult - nStart + nSpan + nStepping) % nSpan) + nStart;
	return nResult;
}



