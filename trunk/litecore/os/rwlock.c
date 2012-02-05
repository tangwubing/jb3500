#include <string.h>



void os_rwl_Init(os_rwl rwl, const char *name)
{

	bzero(rwl, sizeof(os_rwl));
	rt_sem_init(&rwl->sem, name, 1, RT_IPC_FLAG_FIFO);
}

//----------------------------------------------------------------------------
//Function Name  : os_rwl_Trylock
//Description    :  ��ͼ������д��
//Input            : -rwl:ָ��Ҫ���Ķ�д�Ĵ���
//                   -nTmo:��ʱʱ��
//Output         : None
//Return          :-SYS_R_OK  :�ɹ�
//                  -SYS_R_TMO:��ʱ
//----------------------------------------------------------------------------
sys_res os_rwl_Trylock(os_rwl rwl, int nTmo)
{
	sys_res res = SYS_R_TMO;

	nTmo /= OS_TICK_MS;
	do {
		if (rwl->rcnt == 0) {
			if (rt_sem_take(&rwl->sem, nTmo) == RT_EOK) {
				rwl->ste = RWL_S_LOCK;
				res = SYS_R_OK;
			} else {
				rwl->ste = RWL_S_IDLE;
				res = SYS_R_TMO;
			}
			break;
		} else
			os_thd_Slp1Tick();
	} while (nTmo--);
	return res;
}


//----------------------------------------------------------------------------
//Function Name  : os_rwl_Unlock
//Description    :   ������д�����ͷŻ�����
//Input            : -rwl:ָ��Ҫ�����Ķ�д��
//                   -nTmo:��ʱʱ��
//Output         : None
//Return          :-SYS_R_OK  :�ɹ�
//                  -SYS_R_TMO:��ʱ
//----------------------------------------------------------------------------
void os_rwl_Unlock(os_rwl rwl)
{

	if (rwl->ste == RWL_S_LOCK) {
		rt_sem_release(&rwl->sem);
		rwl->ste = RWL_S_IDLE;
	}
}


//----------------------------------------------------------------------------
//Function Name  : os_rwl_Wait
//Description    :   �ȴ�����os_rwl_Unlock()�ͷŶ�д��
//Input            : -rwl:ָ��Ҫ�ȵĶ�д��
//                   -nTmo:��ʱʱ��
//Output         : None
//Return          :-SYS_R_OK  :�ɹ�
//                  -SYS_R_TMO:��ʱ
//----------------------------------------------------------------------------
sys_res os_rwl_Wait(os_rwl rwl, int nTmo)
{

	nTmo /= OS_TICK_MS;
	do {
		if (rwl->ste == RWL_S_IDLE) {
			rwl->rcnt += 1;
			return SYS_R_OK;
		}
		os_thd_Slp1Tick();
	} while (nTmo--);
	return SYS_R_TMO;
}


//----------------------------------------------------------------------------
//Function Name  : os_rwl_Release
//Description    :  �ͷŶ�д��
//Input            : -rwl:ָ���ͷŵĶ�д�Ĵ���
//Output         : None
//Return          :None
//----------------------------------------------------------------------------
void os_rwl_Release(os_rwl rwl)
{

	if (rwl->rcnt)
		rwl->rcnt -= 1;
}


