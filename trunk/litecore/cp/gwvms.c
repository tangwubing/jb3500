#if RTC_ENABLE


//Private Defines
//��������򳤶�
#define GWVMS_DATA_SIZE					1453

//����״̬
#define GWVMS_CMD_SUCC				0xFF	//�ɹ�
#define GWVMS_CMD_FAIL				0x00	//ʧ��

//֡���Ͷ���
#define GWVMS_FTYPE_KEEPALIVE			0x01	//����
#define GWVMS_FTYPE_KEEPALIVE_R			0x02	//����ȷ��
#define GWVMS_FTYPE_MEASURE				0x03	//�������
#define GWVMS_FTYPE_MEASURE_R			0x04	//�������ȷ��
#define GWVMS_FTYPE_DATEREQUEST			0x05	//��������
#define GWVMS_FTYPE_DATEREQUEST_R		0x06	//��������ȷ��
#define GWVMS_FTYPE_PARASET				0x07	//������ѯ����
#define GWVMS_FTYPE_PARASET_R			0x08	//������ѯ������Ӧ
#define GWVMS_FTYPE_FLOW				0x09	//��������
#define GWVMS_FTYPE_FLOW_R				0x10	//��������ȷ��
#define GWVMS_FTYPE_EVENT				0x11	//�¼���Ϣ
#define GWVMS_FTYPE_EVENT_R				0x12	//�¼���Ϣȷ��
#define GWVMS_FTYPE_UPDATE				0x13	//Զ����������
#define GWVMS_FTYPE_UPDATE_R			0x14	//Զ����������ȷ��


//�������Ͷ���
#define GWVMS_PTYPE_KEEPALIVE			0x01	//���� 0x02 -- 0x03	Ԥ��
#define GWVMS_PTYPE_MEASURE_VOL			0x04	//��ѹ���ݱ�
#define GWVMS_PTYPE_MEASURE_DAY			0x05	//��ͳ�����ݱ� 
#define GWVMS_PTYPE_MEASURE_MON			0x06	//��ͳ�����ݱ� 0x07 -- 0xA0 	Ԥ�� 
#define GWVMS_PTYPE_DATEREQUEST			0xA1	//�������� 0xA2 -- 0xA3 	Ԥ�� 
#define GWVMS_PTYPE_TIME				0xA4	//ʱ���ѯ/����
#define GWVMS_PTYPE_PARA_COM			0xA6	//װ��ͨ�Ų�����ѯ/����
#define GWVMS_PTYPE_PARA_MSP			0xA7	//���������ѯ/����
#define GWVMS_PTYPE_PARA_EVENT			0xA8	//װ���¼�������ѯ/���� 
#define GWVMS_PTYPE_PARA_CAC			0xA9	//װ������CAC����Ϣ��ѯ/����
#define GWVMS_PTYPE_DEVICE_INFO			0xAA	//װ�û�����Ϣ��ѯ 
#define GWVMS_PTYPE_DEVICE_WORKSTATUS	0xAB	//װ�ù���״̬��Ϣ��ѯ
#define GWVMS_PTYPE_COMFLOW				0xAC	//װ��ͨ��������Ϣ��ѯ
#define GWVMS_PTYPE_ORIGINAL_ID			0xAD	//װ��ID��ѯ/����
#define GWVMS_PTYPE_RESET				0xAE	//װ�ø�λ
#define GWVMS_PTYPE_FLOWDATA			0xC1	//�������� 0xC2 -- 0xC3 	Ԥ�� 
#define GWVMS_PTYPE_EVENT				0xC4	//�¼���Ϣ�� 0xC5 -- 0xC6 	Ԥ�� 
#define GWVMS_PTYPE_UPDATE_START		0xC7	//�����������ݱ�
#define GWVMS_PTYPE_UPDATE				0xC8	//�����������ݱ�
#define GWVMS_PTYPE_UPDATE_END			0xC9	//�����������ݱ� 0xCA -- 0xCF 	Ԥ�� 

//Private Typedef
typedef __packed struct {
	uint16_t	sc1;			//0x5AA5
//	uint8_t		sc2;			//
	uint16_t	len;			//���ݳ���
	uint8_t		adr[GWVMS_ADR_SIZE];
	uint8_t		ftype;			//֡����
	uint8_t		ptype;			//��������
	uint8_t		fseq;			//֡���
	
}t_gwvms_header, *p_gwvms_header;


//Internal Functions


//-------------------------------------------------------------------------
//��������
//-------------------------------------------------------------------------
static sys_res gwvms_RmsgAnalyze(void *args)
{
	p_gwvms p = (t_gwvms *)args;
	uint8_t *pTemp;
	uint16_t nCRC;
	p_gwvms_header pH;
	p_dlrcp pRcp = &p->parent;

	chl_RecData(pRcp->chl, pRcp->rbuf, OS_TICK_MS);
	for (; ; buf_Remove(pRcp->rbuf, 1)) {
		for (; ; buf_Remove(pRcp->rbuf, 1)) {
			//���㱨��ͷ����
			if (pRcp->rbuf->len < sizeof(t_gwvms_header))
				return SYS_R_ERR;
			pH = (p_gwvms_header)pRcp->rbuf->p;
			if (pH->sc1 == 0x5AA5){
				if (pH->len > GWVMS_DATA_SIZE)
					continue;
				break;
			}
		}
		//���㳤��
		if (pRcp->rbuf->len < (sizeof(t_gwvms_header) + pH->len + 2))
			return SYS_R_ERR;
		pTemp = pRcp->rbuf->p + sizeof(t_gwvms_header) + pH->len;
		nCRC = *pTemp<<8 | *(pTemp+1);
		pTemp += 2;
		//CRC
		if (crc16(pRcp->rbuf->p + 2, sizeof(t_gwvms_header) + pH->len - 2) != nCRC)
			continue;
		//������
		if (*pTemp != 0x96)
			continue;
		//���յ�����
		p->fseq = pH->fseq;
		p->ftype = pH->ftype;
		p->ptype = pH->ptype;
		
		buf_Release(p->data);
		buf_Push(p->data, pRcp->rbuf->p + sizeof(t_gwvms_header), pH->len);
		buf_Remove(pRcp->rbuf, sizeof(t_gwvms_header) + pH->len + 2);
		return SYS_R_OK;
	}
}


//-------------------------------------------------------------------------
//����ͷ��ʼ��
//-------------------------------------------------------------------------
static void gwvms_TmsgHeaderInit(p_gwvms p, p_gwvms_header pH)
{

	pH->sc1 = 0x5AA5;
//	pH->sc2 = 0xA5;
	memcpy(pH->adr, p->adr, GWVMS_ADR_SIZE);
}


//-------------------------------------------------------------------------
//��¼
//-------------------------------------------------------------------------
 static sys_res gwvms_TmsgLinkcheck (void *p, uint_t nCmd)
 {
 	sys_res res;
 	buf b = {0};

 	res = gwvms_TmsgSend(p, 0x02,0x01, b, DLRCP_TMSG_REPORT);
 	buf_Release(b);
 	return res;
 }







//External Functions
//-------------------------------------------------------------------------
//��ʼ��
//-------------------------------------------------------------------------
void gwvms_Init(p_gwvms p)
{

	memset(p, 0, sizeof(t_gwvms));
	chl_Init(p->parent.chl);
 	p->parent.linkcheck = gwvms_TmsgLinkcheck;
	p->parent.analyze = gwvms_RmsgAnalyze;
}

//-------------------------------------------------------------------------
//���ͱ���
//-------------------------------------------------------------------------
sys_res gwvms_TmsgSend(p_gwvms p, uint_t nFType,uint_t nPType, buf b, uint_t nType)
{
	t_gwvms_header xH;
	uint16_t nCRC;
	buf bBuf = {0};

	gwvms_TmsgHeaderInit(p, &xH);
	if (nType == DLRCP_TMSG_REPORT) {
		if (p->parent.pfc == 0)
			p->parent.pfc = 1;
		else
			p->parent.pfc += 1;
		xH.fseq = p->parent.pfc;
	} else
		xH.fseq = p->fseq;
	xH.ftype = nFType;
	xH.ptype = nPType;
	xH.len = b->len;
	buf_Push(bBuf,&xH,sizeof(t_gwvms_header));
	buf_Push(bBuf,b->p,b->len);
 	nCRC = crc16(bBuf->p, bBuf->len);
	buf_Release(bBuf);
	buf_PushData(b, nCRC, 2);
	buf_PushData(b, 0x96, 1);
	return dlrcp_TmsgSend(&p->parent, &xH, sizeof(t_gwvms_header), b->p, b->len, nType);
}



//-------------------------------------------------------------------------
//��Լ����
//-------------------------------------------------------------------------
sys_res gwvms_Handler(p_gwvms p)
{

	return dlrcp_Handler(&p->parent);
}

#endif


