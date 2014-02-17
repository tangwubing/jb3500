//File: fs/sfs/sfs.c 
//Simple File System
//
//
//History:
//V1.0	First Release
//		likazhou 2009-02-06
//V1.1	Length 8 supported
//		likazhou 2012-03-29

//Include Header Files



//Private Defines
#define SFS_LOCK_ENABLE			1

#define SFS_RECORD_MASK			0xFFFFFFFF

#if SFS_LOCK_ENABLE
#define sfs_Lock()				os_thd_Lock()
#define sfs_Unlock()			os_thd_Unlock()
#else
#define sfs_Lock()
#define sfs_Unlock()
#endif


#define SFS_BLK_IDLE			0xFFFFFFFF
#define SFS_BLK_ACTIVE			0xFFFFFF00
#define SFS_BLK_FULL			0xFFFF0000


#define SFS_S_IDLE				0xFFFF
#define SFS_S_VALID				0xFF00
#define SFS_S_DUMPING			0x0000
#define SFS_S_INVALID			0x0000


//Private Typedefs
typedef struct {
	uint32_t ste;
}t_sfs_ste, *p_sfs_ste;

typedef struct {
	uint16_t ste;
	uint16_t len;
	t_sfs_id id;
}t_sfs_idx, *p_sfs_idx;










//Internal Functions
static adr_t _sfs_Find(sfs_dev p, uint32_t nAnd, uint32_t nRecord, p_sfs_idx pIdx)
{
	adr_t nBlk, nBEnd, nIdx, nEnd, nAdr = 0;
	t_sfs_ste blk;
	uint_t nSize = flash_BlkSize(p->dev);

	nRecord &= nAnd;
	nBlk = p->start;
	nBEnd = nBlk + nSize * p->blk;
	for (; nBlk < nBEnd; nBlk += nSize) {
		memcpy(&blk, (uint8_t *)nBlk, sizeof(blk));
		if (blk.ste != SFS_BLK_IDLE) {
			nIdx = nBlk + sizeof(blk);
			nEnd = nBlk + nSize;
			for (; nIdx < nEnd; nIdx = ALIGN4(nIdx + sizeof(t_sfs_idx) + pIdx->len)) {
				memcpy(pIdx, (uint8_t *)nIdx, sizeof(t_sfs_idx));
				if (pIdx->ste == SFS_S_VALID) {
					if ((pIdx->id & nAnd) == nRecord) {
						nAdr = nIdx;
						break;
					}
				}
			}
			if (nAdr)
				break;
		}
	}
	return nAdr;
}

static int _sfs_Free(const adr_t nBlk, uint_t nSize, adr_t *pAdr)
{
	adr_t nEnd;
	t_sfs_idx xIdx;

	*pAdr = nBlk + sizeof(t_sfs_ste);
	nEnd = nBlk + nSize;
	for (; *pAdr < nEnd; *pAdr = ALIGN4(*pAdr + sizeof(t_sfs_idx) + xIdx.len)) {
		memcpy(&xIdx, (uint8_t *)*pAdr, sizeof(t_sfs_idx));
		if (xIdx.ste == SFS_S_IDLE)
			return (nEnd - (*pAdr + sizeof(t_sfs_idx)));
	}
	return 0;
}

static sys_res _sfs_Write(sfs_dev p, uint32_t nRecord, const void *pData, uint_t nLen)
{
	sys_res res;
	adr_t nAdrOld = 0, nIdx, nEnd, nIdxNext, nBlk, nBEnd, nAct = NULL;
	t_sfs_ste xBlk;
	t_sfs_idx xIdx;
	uint_t i, nSize, nQty, nIsFull = 1;

	nSize = flash_BlkSize(p->dev);
	//�ָ�����ԭ����Ч����ʱ�����ж�
	nBlk = p->start;
	nBEnd = nBlk + nSize * (p->blk - 1);
	memcpy(&xBlk, (uint8_t *)nBlk, sizeof(t_sfs_ste));
	if (xBlk.ste == SFS_BLK_ACTIVE) {
		memcpy(&xBlk, (uint8_t *)nBEnd, sizeof(t_sfs_ste));
		if (xBlk.ste == SFS_BLK_ACTIVE)
			nAct = nBEnd;
	}
	//����ԭ��¼�ͼ���Ŀ�
	for (; nBlk <= nBEnd; nBlk += nSize) {
		memcpy(&xBlk, (uint8_t *)nBlk, sizeof(t_sfs_ste));
		if ((nAct == NULL) && (xBlk.ste == SFS_BLK_ACTIVE))
			nAct = nBlk;
		if ((nAdrOld == 0) && (xBlk.ste != SFS_BLK_IDLE)) {
			nIdx = nBlk + sizeof(t_sfs_ste);
			nEnd = nBlk + nSize;
			for (; nIdx < nEnd; nIdx = ALIGN4(nIdx + sizeof(t_sfs_idx) + xIdx.len)) {
				memcpy(&xIdx, (uint8_t *)nIdx, sizeof(t_sfs_idx));
				if ((xIdx.id == nRecord) && (xIdx.ste == SFS_S_VALID)) {
					//�ҵ�ԭ��¼
					nAdrOld = nIdx;
					break;
				}
				//�ѵ�ĩβ
				if (xIdx.ste == SFS_S_IDLE)
					break;
			}
		}
		if (nAdrOld && (nAct != NULL))
			break;
	}
	if (nAct == NULL) {
		//δ�ҵ�����Ŀ�
		nAct = p->start;
 		if ((res = flash_nolockErase(p->dev, nAct))!= SYS_R_OK)
			return res;
		//�õ�һ��BlockΪ����״̬
		xBlk.ste = SFS_BLK_ACTIVE;
 		if ((res = flash_nolockProgram(p->dev, nAct, (uint8_t *)&xBlk, sizeof(t_sfs_ste))) != SYS_R_OK)
			return res;
	}
	//���ҿռ�
	if (_sfs_Free(nAct, nSize, &nIdx) >= (int)(nLen + sizeof(t_sfs_idx)))
		//�ռ���,ֱ��д������
		nIsFull = 0;
	//��ǰ��ID
	i = (nAct - p->start) / nSize;
	for (nQty = p->blk - 1; nIsFull && nQty; nQty--) {
		//�ÿ�ռ䲻��,��Ҫ�����¿�
		nAct = p->start + nSize * i;
		i = cycle(i, 0, p->blk - 1, 1);
		nBEnd = p->start + nSize * i;
		nBlk = p->start + nSize * cycle(i, 0, p->blk - 1, 1);
		memcpy(&xBlk, (uint8_t *)nBEnd, sizeof(t_sfs_ste));
		if (xBlk.ste != SFS_BLK_IDLE) {
			//����NextBlk
 			if ((res = flash_nolockErase(p->dev, nBEnd)) != SYS_R_OK)
				return res;
		}
		//��NextBlkΪ��Act
		xBlk.ste = SFS_BLK_ACTIVE;
 		if ((res = flash_nolockProgram(p->dev, nBEnd, (uint8_t *)&xBlk, sizeof(t_sfs_ste))) != SYS_R_OK)
			return res;
		//����ԭ�е���Ч��¼
		memcpy(&xBlk, (uint8_t *)nBlk, sizeof(t_sfs_ste));
		if (xBlk.ste != SFS_BLK_IDLE) {
			nIdx = nBlk + sizeof(t_sfs_ste);
			nEnd = nBlk + nSize;
			nIdxNext = nBEnd + sizeof(t_sfs_ste);
			for (; nIdx < nEnd; nIdx = ALIGN4(nIdx + sizeof(t_sfs_idx) + xIdx.len)) {
				memcpy(&xIdx, (uint8_t *)nIdx, sizeof(t_sfs_idx));
				if (xIdx.ste == SFS_S_VALID) {
					//�ҵ���Ч��¼
					if (nAdrOld != nIdx) {
 						if ((res = flash_nolockProgram(p->dev, nIdxNext, (uint8_t *)nIdx, sizeof(t_sfs_idx) + xIdx.len)) != SYS_R_OK)
							return res;
						nIdxNext += ALIGN4(sizeof(t_sfs_idx) + xIdx.len);
					} else
						nAdrOld = 0;
				}
			}
		}
		//��ԭActBlkΪFull
		xBlk.ste = SFS_BLK_FULL;
 		if ((res = flash_nolockProgram(p->dev, nAct, (uint8_t *)&xBlk, sizeof(t_sfs_ste))) != SYS_R_OK)
			return res;
		//����OldBlk
 		if ((res = flash_nolockErase(p->dev, nBlk)) != SYS_R_OK)
			return res;
		if (_sfs_Free(nBEnd, nSize, &nIdx) >= nLen)
			//�ռ���,д������
			nIsFull = 0;
	}
	if (nQty == 0)
		return SYS_R_FULL;
	//��ԭ��¼Ϊ��ɾ��״̬
	//if (nAdrOld) {
	//	_sfs_Read(p, nAdrOld, (uint8 *)&xIdx, sizeof(t_sfs_idx));
	//	xIdx.ste = SFS_S_DUMPING;
	//	flash_nolockProgram(p, nAdrOld, (uint8 *)&xIdx, sizeof(t_sfs_idx));
	//}
	//д�¼�¼
	xIdx.ste = SFS_S_VALID;
	xIdx.id = nRecord;
	xIdx.len = nLen;
	if ((res = flash_nolockProgram(p->dev, nIdx, (uint8_t *)&xIdx, sizeof(t_sfs_idx))) != SYS_R_OK)
		return res;
	if ((res = flash_nolockProgram(p->dev, nIdx + sizeof(t_sfs_idx), (uint8_t *)pData, nLen)) != SYS_R_OK)
		return res;
	//ɾ��ԭ��¼
	if (nAdrOld) {
		memcpy(&xIdx, (uint8_t *)nAdrOld, sizeof(t_sfs_idx));
		xIdx.ste = SFS_S_INVALID;
 		if ((res = flash_nolockProgram(p->dev, nAdrOld, (uint8_t *)&xIdx, sizeof(t_sfs_idx))) != SYS_R_OK)
			return res;
	}
	return SYS_R_OK;
}




//-------------------------------------------------------------------------
//sfs_Init - ��ʼ��һ��SFS�豸
//
//@sfsDev:�������豸���
//
//Note:
//
//Return: SYS_R_OK on success, errno otherwise
//-------------------------------------------------------------------------
sys_res sfs_Init(sfs_dev p)
{
	sys_res res;
	adr_t nAdr, nEnd;
	t_sfs_ste blk;
	uint_t nSize = flash_BlkSize(p->dev);

	sfs_Lock();
	//����Flash,���
	blk.ste = SFS_BLK_IDLE;
	nAdr = p->start;
	nEnd = nAdr + nSize * p->blk;
	for (; nAdr < nEnd; nAdr += nSize) {
		res = flash_nolockErase(p->dev, nAdr);
		if (res != SYS_R_OK) {
			sfs_Unlock();
			return res;
		}
	}
	//�õ�һ��BlockΪ����״̬
	blk.ste = SFS_BLK_ACTIVE;
	res = flash_nolockProgram(p->dev, p->start, (uint8_t *)&blk, sizeof(blk));
	sfs_Unlock();
	return res;
}

//-------------------------------------------------------------------------
//sfs_Write - д��һ����¼
//
//@sfsDev: �������豸���
//@nParam: ��¼��ʶ��
//@buf: ����ָ��
//@nDataLen: ���ݳ���
//
//Note: ���ݳ��ȷ�Χ0 ~ 655535
//
//Return: SYS_R_OK on success, errno otherwise
//-------------------------------------------------------------------------
sys_res sfs_Write(sfs_dev p, t_sfs_id nRecord, const void *pData, uint_t nLen)
{
	sys_res res;

	sfs_Lock();
	res = _sfs_Write(p, nRecord, pData, nLen);
	sfs_Unlock();
	return res;
}

//-------------------------------------------------------------------------
//_sfs_Read - ����һ����¼
//
//@sfsDev: �������豸���
//@nParam: ��¼��ʶ��
//@pData: ����ָ��
//
//Note:
//
//Return: �ɹ���ȡ�����ݳ���, errno otherwise
//-------------------------------------------------------------------------
sys_res sfs_Read(sfs_dev p, t_sfs_id nRecord, void *pData, uint_t nLen)
{
	sys_res res = SYS_R_ERR;
	adr_t nIdx;
	t_sfs_idx xIdx;

	sfs_Lock();
	if ((nIdx = _sfs_Find(p, SFS_RECORD_MASK, nRecord, &xIdx)) != 0) {
		//�ҵ���¼,��ȡ
		memcpy(pData, (uint8_t *)(nIdx + sizeof(t_sfs_idx)), MIN(nLen, xIdx.len));
		res = SYS_R_OK;
	}
	sfs_Unlock();
	return res;
}

sys_res sfs_ReadRandom(sfs_dev p, t_sfs_id nRecord, void *pData, uint_t nOffset, uint_t nLen)
{
	sys_res res = SYS_R_ERR;
	adr_t nIdx;
	t_sfs_idx xIdx;

	sfs_Lock();
	if ((nIdx = _sfs_Find(p, SFS_RECORD_MASK, nRecord, &xIdx)) != 0) {
		//�ҵ���¼,��ȡ
		if (nOffset < xIdx.len) {
			memcpy(pData, (uint8_t *)(nIdx + sizeof(t_sfs_idx) + nOffset), MIN(nLen, xIdx.len - nOffset));
			res = SYS_R_OK;
		}
	}
	sfs_Unlock();
	return res;
}

sys_res sfs_Read2Buf(sfs_dev p, t_sfs_id nRecord, buf b)
{
	sys_res res = SYS_R_ERR;
	adr_t nIdx;
	t_sfs_idx xIdx;

	sfs_Lock();
	if ((nIdx = _sfs_Find(p, SFS_RECORD_MASK, nRecord, &xIdx)) != 0) {
		//�ҵ���¼,��ȡ
		if (buf_Push(b, (uint8_t *)(nIdx + sizeof(t_sfs_idx)), xIdx.len) == SYS_R_OK)
			res = SYS_R_OK;
		else
			res = SYS_R_EMEM;
	}
	sfs_Unlock();
	return res;
}

sys_res sfs_Find(sfs_dev p, t_sfs_id nRecord, buf b, uint_t nLen)
{
	sys_res res = SYS_R_ERR;
	adr_t nIdx, nEnd, nBlk, nBEnd;
	t_sfs_ste blk;
	t_sfs_idx xIdx;
	uint_t nSize = flash_BlkSize(p->dev);

	sfs_Lock();
	nBlk = p->start;
	nBEnd = nBlk + nSize * p->blk;
	for (; nLen && (nBlk < nBEnd); nBlk += nSize) {
		memcpy(&blk, (uint8_t *)nBlk, sizeof(blk));
		if (blk.ste != SFS_BLK_IDLE) {
			nIdx = nBlk + sizeof(blk);
			nEnd = nBlk + nSize;
			for (; nLen && (nIdx < nEnd); nIdx = ALIGN4(nIdx + sizeof(t_sfs_idx) + xIdx.len)) {
				memcpy(&xIdx, (uint8_t *)nIdx, sizeof(t_sfs_idx));
				if (((xIdx.id & nRecord) == nRecord) && (xIdx.ste == SFS_S_VALID)) {
					buf_Push(b, &xIdx.id, sizeof(xIdx.id));
					nLen -= 1;
					res = SYS_R_OK;
				}
			}
		}
	}
	sfs_Unlock();
	return res;
}


//-------------------------------------------------------------------------
//sfs_Info - ��ȡһ����¼����Ϣ
//
//@sfsDev:�������豸���
//@nParam: ��¼��ʶ��
//@info:sfs_info�ṹ��ָ��
//
//Note:
//
//Return: SYS_R_OK on success, errno otherwise
//-------------------------------------------------------------------------
sys_res sfs_Info(sfs_dev p, t_sfs_id nRecord, sfs_info info)
{
	sys_res res = SYS_R_ERR;
	t_sfs_idx xIdx;

	sfs_Lock();
	if (_sfs_Find(p, SFS_RECORD_MASK, nRecord, &xIdx) != 0) {
		//�ҵ���¼
		info->len = xIdx.len;
		res = SYS_R_OK;
	}
	sfs_Unlock();
	return res;
}


//-------------------------------------------------------------------------
//sfs_Delete - ɾ��һ����¼
//
//@sfsDev:�������豸���
//@nParam: ��¼��ʶ��
//
//Note:
//
//Return: SYS_R_OK on success, errno otherwise
//-------------------------------------------------------------------------
sys_res sfs_Delete(sfs_dev p, t_sfs_id nRecord)
{
	sys_res res = SYS_R_ERR;
	adr_t nIdx;
	t_sfs_idx xIdx;

	sfs_Lock();
	if ((nIdx = _sfs_Find(p, SFS_RECORD_MASK, nRecord, &xIdx)) != 0) {
		//�ҵ���¼,���Ϊ��Ч
		xIdx.ste = SFS_S_INVALID;
		res = flash_nolockProgram(p->dev, nIdx, (uint8_t *)&xIdx, sizeof(t_sfs_idx));
	}
	sfs_Unlock();
	return res;
}

