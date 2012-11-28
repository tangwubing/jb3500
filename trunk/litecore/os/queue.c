#include <string.h>


//Private Defines
#define OS_QUE_S_IDLE			0
#define OS_QUE_S_ALLOC			1
#define OS_QUE_S_BUSY			2


//Private Variables
static t_os_que queue[OS_QUEUE_QTY];

//------------------------------------------------------------------
//Function Name  :os_que_Alloc
//Description    :   �ҳ� ���е���Ϣ���пռ䣬������  ���ַ
//Input            : None
//Output         :  None
//Return          : -que:���е���Ϣ���пռ�ĵ�ַ
//                   -NULL: û�ҵ�
//------------------------------------------------------------------
static os_que os_que_Alloc()
{
	os_que p;

	for (p = queue; p < ARR_ENDADR(queue); p++)
		if (p->ste == OS_QUE_S_IDLE) {
			p->ste = OS_QUE_S_ALLOC;
			return p;
		}
	return NULL;
}


//----------------------------------------------------------------------------
//Function Name  :os_que_Release
//Description    :   �ͷ�p���ڴ�ռ�
//Input            : p:�Ӻ���os_que_Find()���صĿ��� �Ķ��пռ�ĵ�ַ
//Output         :  None
//Return          : None
//----------------------------------------------------------------------------
void os_que_Release(os_que p)
{

	if (p->ste != OS_QUE_S_IDLE) {
		if (p->evt & QUE_EVT_BUF_MASK)
			buf_Release(p->data->b);
		bzero(p, sizeof(t_os_que));
	}
}

//----------------------------------------------------------------------------
//Function Name  :os_que_Init
//Description    :   ��ʼ����Ϣ���пռ�
//Input            : None
//Output         :  None
//Return          : None
//----------------------------------------------------------------------------
void os_que_Init()
{

	bzero(queue, sizeof(queue));
}


//----------------------------------------------------------------------------
//Function Name  :os_que_Send
//Description    :    ���ض��Ķ��пռ䷢������
//Input            : -nEvt:�ض����¼�
//                   -*pDev:�ض����豸
//                   -*pData:�����͵�����
//                   -nLen:���ݳ���
//                   - nTmo:��ʱʱ��
//Output         :  
//Return          : -SYS_R_FULL:��Ϣ���пռ�����
//                   -SYS_R_TMO:��ʱ
//----------------------------------------------------------------------------
sys_res os_que_Send(uint_t nEvt, void *pDev, void *pData, uint_t nLen, int nTmo)
{
	os_que p;

	for (nTmo /= OS_TICK_MS; ; nTmo--) {
		if ((p = os_que_Alloc()) != NULL) {
			p->evt = nEvt;
			p->dev = pDev;
			p->tmo = nTmo;
			if (p->evt & QUE_EVT_BUF_MASK)
				buf_Push(p->data->b, pData, nLen);
			else
				p->data->val = *(uint_t *)pData;
			return SYS_R_OK;
		}
		if (nTmo == 0)
			break;
		os_thd_Slp1Tick();
	}
	return SYS_R_TMO;
}

//----------------------------------------------------------------------------
//Function Name  :os_que_IsrSend
//Description    :    �����ж����ض��Ķ��пռ䷢������
//Input            : -nEvt:�ض����¼�
//                   -*pDev:�ض����豸
//                   -*pData:�����͵�����
//                   -nLen:���ݳ���      
//Output         :  
//Return          : -SYS_R_FULL:��Ϣ���пռ�����
//                   -SYS_R_TMO:��ʱ
//                   -OS_R_OK:�ɹ�
//----------------------------------------------------------------------------
sys_res os_que_IsrSend(uint_t nEvt, void *pDev, void *pData, uint_t nLen, int nTmo)
{
	os_que p;

	if ((p = os_que_Alloc()) != NULL) {
		p->evt = nEvt;
		p->dev = pDev;
		p->tmo = nTmo / OS_TICK_MS;
		if (p->evt & QUE_EVT_BUF_MASK)
			buf_Push(p->data->b, pData, nLen);
		else
			p->data->val = *(uint_t *)pData;
		return SYS_R_OK;
	}
	return SYS_R_TMO;
}


//----------------------------------------------------------------------------
//Function Name  :os_que_Wait
//Description    :    �ȴ��ض��¼����ض��豸
//Input            : -nEvt:�ض����¼�
//                   -*pDev:�ض����豸
//                   - nTmo:��ʱʱ��
//Output         :  None
//Return          : -p:�ض����¼����ض����豸��ָ���ַ
//                   -NULL:��ʱʱ�䵽���ȴ�ʧ��
//----------------------------------------------------------------------------
os_que os_que_Wait(uint_t nEvt, void *pDev, int nTmo)
{
	os_que p;

	for (nTmo /= OS_TICK_MS; ; nTmo--) {
		for (p = queue; p < ARR_ENDADR(queue); p++) {
			if (p->ste == OS_QUE_S_ALLOC) {
				if ((p->evt == nEvt) && (p->dev == pDev)) {
					p->ste = OS_QUE_S_BUSY;
					return p;
				}
			}
		}
		if (nTmo == 0)
			break;
		os_thd_Slp1Tick();
	}
	return NULL;
}

//----------------------------------------------------------------------------
//Function Name  :os_que_Idle
//Description    :   ʹ��Ϣ���пռ��Ϊ����״̬
//Input            : None
//Output         :  None
//Return          : None
//----------------------------------------------------------------------------
void os_que_Idle()
{
	os_que p;

	for (p = queue; p < ARR_ENDADR(queue); p++) {
		if (p->ste == OS_QUE_S_ALLOC) {
			if (--p->tmo == 0)
				os_que_Release(p);
		}
	}
}

