#if INTFLASH_ENABLE


//Private Typedefs
typedef unsigned char  	BOOLEAN;        //  ��������                  
typedef unsigned char  	INT8U;          //  �޷���8λ���ͱ���         
typedef signed   char  	INT8S;          //  �з���8λ���ͱ���         
typedef unsigned short 	INT16U;         //  �޷���16λ���ͱ���        
typedef signed   short 	INT16S;         //  �з���16λ���ͱ���        
typedef unsigned long  	INT32U;         //  �޷���32λ���ͱ���        
typedef signed   long  	INT32S;         //  �з���32λ���ͱ���        
typedef float          	FP32;           //  �����ȸ�������32λ���ȣ�   
typedef double         	FP64;           //  ˫���ȸ�������64λ���ȣ�   



/* 
 *  ����CCLKֵ��С����λΪKHz 
 */
#define  IAP_FCCLK            (MCU_CLOCK / 1000)
#define  IAP_ENTER_ADR        0x1FFF1FF1                                  /* IAP��ڵ�ַ����              */

/* 
 *  ����IAP������
 */                                     
#define     IAP_Prepare                                50               /* ѡ������                     */
                                                                        /* ����ʼ�����š����������š�   */                    
#define     IAP_RAMTOFLASH                             51               /* �������� FLASHĿ���ַ       */
                                                                        /* RAMԴ��ַ    ����д���ֽ���  */
                                                                        /* ϵͳʱ��Ƶ�ʡ�               */
#define     IAP_ERASESECTOR                            52               /* ��������    ����ʼ������     */
                                                                        /* ���������š�ϵͳʱ��Ƶ�ʡ�   */
#define     IAP_BLANKCHK                               53               /* �������    ����ʼ�����š�   */
                                                                        /* ���������š�                 */
#define     IAP_READPARTID                             54               /* ������ID    ���ޡ�           */
#define     IAP_BOOTCODEID                             55               /* ��Boot�汾�š��ޡ�           */
#define     IAP_COMPARE                                56               /* �Ƚ�����    ��Flash��ʼ��ַ  */
                                                                        /* RAM��ʼ��ַ����Ҫ�Ƚϵ�      */
                                                                        /* �ֽ�����                     */

/*
 *  ���庯��ָ��  
 */
typedef INT32U (*PIAP_Entry)(INT32U param_tab[], INT32U result_tab[]);

PIAP_Entry IAP_Entry = (PIAP_Entry)IAP_ENTER_ADR;

INT32U paramin[8];                                                     /* IAP��ڲ���������            */
INT32U paramout[8];                                                    /* IAP���ڲ���������            */

/*********************************************************************************************************
** Function name:       sectorPrepare
** Descriptions:        IAP��������ѡ���������50
** input parameters:    sec1:           ��ʼ����
**                      sec2:           ��ֹ����
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ     
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
static INT32U sectorPrepare(INT8U sec1, INT8U sec2)
{  
    paramin[0] = IAP_Prepare;                                           /* ����������                   */
    paramin[1] = sec1;                                                  /* ���ò���                     */
    paramin[2] = sec2;                            
    IAP_Entry(paramin, paramout);                                    /* ����IAP�������              */
   
    return (paramout[0]);                                               /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       ramCopy
** Descriptions:        ����RAM�����ݵ�FLASH���������51
** input parameters:    dst:            Ŀ���ַ����FLASH��ʼ��ַ����512�ֽ�Ϊ�ֽ�
**                      src:            Դ��ַ����RAM��ַ����ַ�����ֶ���
**                      no:             �����ֽڸ�����Ϊ512/1024/4096/8192
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ     
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
static INT32U ramToFlash(INT32U dst, INT32U src, INT32U no)
{  
    paramin[0] = IAP_RAMTOFLASH;                                        /* ����������                   */
    paramin[1] = dst;                                                   /* ���ò���                     */
    paramin[2] = src;
    paramin[3] = no;
    paramin[4] = IAP_FCCLK;
    IAP_Entry(paramin, paramout);                                    /* ����IAP�������              */
    
    return (paramout[0]);                                               /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       sectorErase
** Descriptions:        �����������������52
** input parameters:    sec1            ��ʼ����
**                      sec2            ��ֹ����92
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ                     
*********************************************************************************************************/
static INT32U sectorErase(INT8U sec1, INT8U sec2)
{  
    paramin[0] = IAP_ERASESECTOR;                                       /* ����������                   */
    paramin[1] = sec1;                                                  /* ���ò���                     */
    paramin[2] = sec2;
    paramin[3] = IAP_FCCLK;
    IAP_Entry(paramin, paramout);                                    /* ����IAP�������              */
   
    return (paramout[0]);                                               /* ����״̬��                   */
}


void arch_IntfInit()
{

}

sys_res arch_IntfErase(adr_t adr)
{
	uint_t nBlk;

	nBlk = adr / INTFLASH_BLK_SIZE;
	os_interrupt_Disable();
#if WDG_ENABLE
	wdg_Reload(0);
#endif
	sectorPrepare(nBlk, nBlk);
#if WDG_ENABLE
	wdg_Reload(0);
#endif
	sectorErase(nBlk, nBlk);
#if WDG_ENABLE
	wdg_Reload(0);
#endif
	os_interrupt_Enable();
	return SYS_R_OK;
}

sys_res arch_IntfProgram(adr_t adr, const void *pData, uint_t nLen)
{
	uint_t nBlk;

	nBlk = adr / INTFLASH_BLK_SIZE;
	os_interrupt_Disable();
#if WDG_ENABLE
	wdg_Reload(0);
#endif
	sectorPrepare(nBlk, nBlk);
#if WDG_ENABLE
	wdg_Reload(0);
#endif
	ramToFlash(adr, (INT32U)pData, nLen);
#if WDG_ENABLE
	wdg_Reload(0);
#endif
	os_interrupt_Enable();
	return SYS_R_OK;
}


#endif


