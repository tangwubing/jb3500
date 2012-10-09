#if RTC_ENABLE


//Private Defines
//��������򳤶�
#define GDVMS_DATA_SIZE					4096

//�쳣��־����
#define GDVMS_CABN_NORMAL				0x00	//ȷ��֡
#define GDVMS_CABN_ABNORMAL				0x01	//��֡

//�����붨��
#define GDVMS_CCODE_LOGIN				0x21	//��¼
#define GDVMS_CCODE_LOGOUT				0x22	//��¼�˳�
#define GDVMS_CCODE_KEEPALIVE			0x24	//����



//Private Typedef
typedef __packed struct {
	uint8_t		sc1;			//0x68
	uint8_t		adr[GDVMS_ADR_SIZE];
	uint8_t		fseq;			//֡���
	uint8_t		iseq;			//֡�����
	uint8_t		sc2;			//0x68
	uint8_t		code : 6,		//������
				abn : 1,		//�쳣��־
				dir : 1;		//���ͷ���
	uint16_t	len;			//���ݳ���
}t_gdvms_header, *p_gdvms_header;


//Internal Functions
//-------------------------------------------------------------------------
//��������
//-------------------------------------------------------------------------
static sys_res gdvms_RmsgAnalyze(void *args)
{
	p_gdvms p = (t_gdvms *)args;
	p_dlrcp pRcp = &p->parent;
	uint8_t *pTemp;
	p_gdvms_header pH;

	chl_RecData(pRcp->chl, pRcp->rbuf, OS_TICK_MS);
	for (; ; buf_Remove(pRcp->rbuf, 1)) {
		for (; ; buf_Remove(pRcp->rbuf, 1)) {
			//���㱨��ͷ����
			if (pRcp->rbuf->len < sizeof(t_gdvms_header))
				return SYS_R_ERR;
			pH = (p_gdvms_header)pRcp->rbuf->p;
			if ((pH->sc1 == 0x68) && (pH->sc2 == 0x68)) {
				if (pH->len > GDVMS_DATA_SIZE)
					continue;
				//֡ͷβѭ������
				if ((pRcp->rbuf->p[2] == 1) && (pRcp->rbuf->p[3] == 0))
					if (pRcp->rbuf->p[6] == 0x16)
						continue;
				break;
			}
		}
		//���㳤��
		if (pRcp->rbuf->len < (sizeof(t_gdvms_header) + pH->len + 2))
			return SYS_R_ERR;
		pTemp = pRcp->rbuf->p + sizeof(t_gdvms_header) + pH->len;
		//CS
		if (cs8(pRcp->rbuf->p, sizeof(t_gdvms_header) + pH->len) != *pTemp++)
			continue;
		//������
		if (*pTemp != 0x16)
			continue;
		if (pH->dir)
			continue;
		//���յ�����
		p->fseq = pH->fseq;
		p->code = pH->code;
		p->abn = pH->abn;
		p->dir = pH->dir;
		buf_Release(p->data);
		buf_Push(p->data, pRcp->rbuf->p + sizeof(t_gdvms_header), pH->len);
		buf_Remove(pRcp->rbuf, sizeof(t_gdvms_header) + pH->len + 2);
		return SYS_R_OK;
	}
}


//-------------------------------------------------------------------------
//����ͷ��ʼ��
//-------------------------------------------------------------------------
static void gdvms_TmsgHeaderInit(p_gdvms p, p_gdvms_header pH)
{

	pH->sc1 = 0x68;
	pH->sc2 = 0x68;
	memcpy(pH->adr, p->adr, GDVMS_ADR_SIZE);
	pH->iseq = 0;
	pH->dir = GDVMS_CDIR_SEND;
}


//-------------------------------------------------------------------------
//��¼
//-------------------------------------------------------------------------
static sys_res gdvms_TmsgLinkcheck (void *p, uint_t nCmd)
{
	sys_res res;
	buf b = {0};

	switch (nCmd) {
	case DLRCP_LINKCHECK_LOGIN:
		nCmd = GDVMS_CCODE_LOGIN;
		buf_Push(b, ((p_gdvms)p)->pwd, 3);
		break;
	case DLRCP_LINKCHECK_LOGOUT:
		nCmd = GDVMS_CCODE_LOGOUT;
		break;
	case DLRCP_LINKCHECK_KEEPALIVE:
		nCmd = GDVMS_CCODE_KEEPALIVE;
		break;
	}
	res = gdvms_TmsgSend(p, nCmd, b, DLRCP_TMSG_REPORT);
	buf_Release(b);
	return res;
}
	
	





//External Functions
//-------------------------------------------------------------------------
//��ʼ��
//-------------------------------------------------------------------------
void gdvms_Init(p_gdvms p)
{

	memset(p, 0, sizeof(t_gdvms));
	p->parent.linkcheck = gdvms_TmsgLinkcheck;
	p->parent.analyze = gdvms_RmsgAnalyze;
}

//-------------------------------------------------------------------------
//���ͱ���
//-------------------------------------------------------------------------
sys_res gdvms_TmsgSend(p_gdvms p, uint_t nCode, buf b, uint_t nType)
{
	t_gdvms_header xH;
	uint_t nCS;

	gdvms_TmsgHeaderInit(p, &xH);
	if (nType == DLRCP_TMSG_REPORT)
		xH.fseq = p->parent.pfc++;
	else
		xH.fseq = p->fseq;
	xH.code = nCode;
	xH.abn = GDVMS_CABN_NORMAL;
	xH.len = b->len;
	nCS = cs8(&xH, sizeof(t_gdvms_header));
	nCS += cs8(b->p, b->len);
	buf_PushData(b, 0x1600 | (nCS & 0xFF), 2);
	return dlrcp_TmsgSend(&p->parent, &xH, sizeof(t_gdvms_header), b->p, b->len);
}


//-------------------------------------------------------------------------
//�����쳣Ӧ��
//-------------------------------------------------------------------------
sys_res gdvms_TmsgError(p_gdvms p, uint_t nCode, uint_t nErr)
{
	t_gdvms_header xH;
	uint8_t aBuf[3];

	gdvms_TmsgHeaderInit(p, &xH);
	xH.fseq = p->fseq;
	xH.code = nCode;
	xH.abn = GDVMS_CABN_ABNORMAL;
	xH.len = 1;
	aBuf[0] = nErr;
	aBuf[1] = cs8(&xH, sizeof(t_gdvms_header)) + nErr;
	aBuf[2] = 0x16;
	return dlrcp_TmsgSend(&p->parent, &xH, sizeof(t_gdvms_header), aBuf, 3);
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
sys_res gdvms_Transmit(p_gdvms p, p_gdvms pD)
{
	sys_res res;
	t_gdvms_header xH;
	uint_t nCS;
	buf b = {0};

	buf_Push(b, p->data->p, p->data->len);
	xH.sc1 = 0x68;
	memcpy(xH.adr, p->adr, GDVMS_ADR_SIZE);
	xH.fseq = p->fseq;
	xH.iseq = 0;
	xH.sc2 = 0x68;
	xH.code = p->code;
	xH.abn = p->abn;
	xH.dir = p->dir;
	xH.len = b->len;
	nCS = cs8(&xH, sizeof(t_gdvms_header));
	nCS += cs8(b->p, b->len);
	buf_PushData(b, 0x1600 | (nCS & 0xFF), 2);
	res = dlrcp_TmsgSend(&pD->parent, &xH, sizeof(t_gdvms_header), b->p, b->len);
	buf_Release(b);
	return res;
}




//-------------------------------------------------------------------------
//��Լ����
//-------------------------------------------------------------------------
sys_res gdvms_Handler(p_gdvms p)
{

	return dlrcp_Handler(&p->parent);
}









#endif


