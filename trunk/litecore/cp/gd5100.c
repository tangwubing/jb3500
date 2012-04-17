#if RTC_ENABLE


//Private Defines
//��������򳤶�
#define GD5100_DATA_SIZE				4096

//���ͷ�����
#define GD5100_CDIR_RECV				0x00	//��վ����
#define GD5100_CDIR_SEND				0x01	//�ն˷���

//�쳣��־����
#define GD5100_CABN_NORMAL				0x00	//ȷ��֡
#define GD5100_CABN_ABNORMAL			0x01	//��֡

//�����붨��
#define GD5100_CCODE_LOGIN				0x21	//��¼
#define GD5100_CCODE_LOGOUT				0x22	//��¼�˳�
#define GD5100_CCODE_KEEPALIVE			0x24	//����



//Private Typedef
typedef __packed struct {
 	uint8_t		sc1;			//0x68
 	uint16_t	rtua;			//����������
 	uint16_t	terid;			//�ն˵�ַ
 	uint16_t	msta : 6,		//��վ���
 				fseq : 7,		//֡���
 				iseq : 3;		//֡�����
 	uint8_t		sc2;			//0x68
 	uint8_t		code : 6,		//������
 				abn : 1,		//�쳣��־
 				dir : 1;		//���ͷ���
 	uint16_t	len;			//���ݳ���
}t_gd5100_header, *p_gd5100_header;



//Internal Functions
//-------------------------------------------------------------------------
//��������
//-------------------------------------------------------------------------
static sys_res gd5100_RmsgAnalyze(void *args)
{
	t_gd5100 *p = (t_gd5100 *)args;
	uint8_t *pTemp;
	p_gd5100_header pH;

	chl_RecData(p->parent.chl, p->parent.rbuf, OS_TICK_MS);
	for (; ; buf_Remove(p->parent.rbuf, 1)) {
		for (; ; buf_Remove(p->parent.rbuf, 1)) {
			//���㱨��ͷ����
			if (p->parent.rbuf->len < sizeof(t_gd5100_header))
				return SYS_R_ERR;
			pH = (p_gd5100_header)p->parent.rbuf->p;
			if ((pH->sc1 == 0x68) && (pH->sc2 == 0x68)) {
				if (pH->len > GD5100_DATA_SIZE)
					continue;
				//֡ͷβѭ������
				if ((p->parent.rbuf->p[2] == 1) && (p->parent.rbuf->p[3] == 0))
					if (p->parent.rbuf->p[6] == 0x16)
						continue;
				break;
			}
		}
		//���㳤��
		if (p->parent.rbuf->len < (sizeof(t_gd5100_header) + pH->len + 2))
			return SYS_R_ERR;
		pTemp = p->parent.rbuf->p + sizeof(t_gd5100_header) + pH->len;
		//CS
		if (cs8(p->parent.rbuf->p, sizeof(t_gd5100_header) + pH->len) != *pTemp++)
			continue;
		//������
		if (*pTemp != 0x16)
			continue;
		//���յ�����
		p->parent.msta = pH->msta;
		p->rmsg->rtua = pH->rtua;
		p->rmsg->terid = pH->terid;
		p->rmsg->fseq = pH->fseq;
		p->rmsg->iseq = pH->iseq;
		p->rmsg->code = pH->code;
		p->rmsg->abn = pH->abn;
		p->rmsg->dir = pH->dir;
		buf_Release(p->rmsg->data);
		buf_Push(p->rmsg->data, p->parent.rbuf->p + sizeof(t_gd5100_header), pH->len);
		buf_Remove(p->parent.rbuf, sizeof(t_gd5100_header) + pH->len + 2);
		return SYS_R_OK;
	}
}


//-------------------------------------------------------------------------
//����ͷ��ʼ��
//-------------------------------------------------------------------------
static void gd5100_TmsgHeaderInit(p_gd5100 p, p_gd5100_header pH)
{

	pH->sc1 = 0x68;
	pH->sc2 = 0x68;
	pH->rtua = p->rtua;
	pH->terid = p->terid;
	pH->iseq = 0;
	pH->dir = GD5100_CDIR_SEND;
}


//-------------------------------------------------------------------------
//��¼
//-------------------------------------------------------------------------
static sys_res gd5100_TmsgLinkcheck (void *p, uint_t nCmd)
{
	sys_res res;
	buf b = {0};

	switch (nCmd) {
	case DLRCP_LINKCHECK_LOGIN:
		nCmd = GD5100_CCODE_LOGIN;
		break;
	case DLRCP_LINKCHECK_LOGOUT:
		nCmd = GD5100_CCODE_LOGOUT;
		break;
	case DLRCP_LINKCHECK_KEEPALIVE:
		nCmd = GD5100_CCODE_KEEPALIVE;
		break;
	}
	buf_Push(b, ((p_gd5100)p)->pwd, 3);
	res = gd5100_TmsgSend(p, nCmd, b, DLRCP_TMSG_REPORT);
	buf_Release(b);
	return res;
}
	
	





//External Functions
//-------------------------------------------------------------------------
//��ʼ��
//-------------------------------------------------------------------------
void gd5100_Init(p_gd5100 p)
{

	memset(p, 0, sizeof(t_gw3761));
	p->group = 0;
	p->parent.linkcheck = gd5100_TmsgLinkcheck;
	p->parent.analyze = gd5100_RmsgAnalyze;
}

//-------------------------------------------------------------------------
//���ͱ���
//-------------------------------------------------------------------------
sys_res gd5100_TmsgSend(p_gd5100 p, uint_t nCode, buf b, uint_t nType)
{
	t_gd5100_header xH;
	uint_t nCS;

	gd5100_TmsgHeaderInit(p, &xH);
	if (nType == DLRCP_TMSG_REPORT) {
		xH.msta = 0;
		xH.fseq = p->parent.pfc++;
	} else {
		xH.msta = p->parent.msta;
		xH.fseq = p->rmsg->fseq;
	}
	xH.code = nCode;
	xH.abn = GD5100_CABN_NORMAL;
	xH.len = b->len;
	nCS = cs8(&xH, sizeof(t_gd5100_header));
	nCS = (nCS + cs8(b->p, b->len)) & 0xFF;
	buf_PushData(b, 0x1600 | nCS, 2);
	return dlrcp_TmsgSend(&p->parent, &xH, sizeof(t_gd5100_header), b->p, b->len);
}


//-------------------------------------------------------------------------
//�����쳣Ӧ��
//-------------------------------------------------------------------------
sys_res gd5100_TmsgError(p_gd5100 p, uint_t nCode, uint_t nErr)
{
	t_gd5100_header xH;
	uint8_t nCS, aBuf[3];

	gd5100_TmsgHeaderInit(p, &xH);
	xH.msta = p->parent.msta;
	xH.fseq = p->rmsg->fseq;
	xH.code = nCode;
	xH.abn = GD5100_CABN_ABNORMAL;
	xH.len = 1;
	nCS = cs8(&xH, sizeof(t_gd5100_header));
	nCS += nErr;
	aBuf[0] = nErr;
	aBuf[1] = nCS;
	aBuf[2] = 0x16;
	return dlrcp_TmsgSend(&p->parent, &xH, sizeof(t_gd5100_header), aBuf, 3);
}



//-------------------------------------------------------------------------
//��Լ����
//-------------------------------------------------------------------------
sys_res gd5100_Handler(p_gd5100 p)
{

	return dlrcp_Handler(&p->parent);
}









#endif

