#ifndef __BSP_MW_RF_35_H__
#define __BSP_MW_RF_35_H__

//ͨѶ�ַ�����
#define MW_STX          0x02
#define MW_ETX          0x03
#define MW_DLE          0x10
#define MW_NAK          0x15

//�����������ֶ���
#define MW_COMMAND_LED             0x35     //led��ʾ
#define MW_COMMAND_BUZZER          0x36     //������
#define MW_COMMAND_R_EE            0x37     //��EEPROM
#define MW_COMMAND_W_EE            0x38     //дEEPROM
#define MW_COMMAND_S_TM            0x39     //����ʱ��
#define MW_COMMAND_R_TM            0x3A     //��ȡʱ��
#define MW_COMMAND_S_CK            0x4D     //����У��
#define MW_COMMAND_R_RF            0x4E     //��λRFģ��
#define MW_COMMAND_R_ED            0x4F     //���������汾
#define MW_COMMAND_W_RD            0x54     //д����������
#define MW_COMMAND_R_RD            0x55     //��������ֵ
#define MW_COMMAND_S_CD            0x60     //ѡ������

//�����Ͷ���
#define MW_TYPEA                   0x00     //14433_TYPEA
#define MW_TYPEB                   0x01     //14433_TYPEA
#define MW_TYPEO                   0x02     //15693

//mf��׼������
#define MF_COMMAND_REQUEST         0x2F     //�����߲�����Χ�ڵĿ�Ƭ��������ͼ���ź�
#define MF_COMMAND_REQUEST2        0x41     //�����߲�����Χ�ڵĿ�Ƭ��������ͼ���ź�
#define MF_COMMAND_ANITI           0x42     //����ͻ���������ؿ�Ƭϵ�к�
#define MF_COMMAND_GET_CAP         0x43     //�Ӷ������ѡȡһ���������кŵĿ������ؿ�������
#define MF_COMMAND_ATTEST          0x44     //��֤
#define MF_COMMAND_HALT            0x45     //����Ƭ������ͣ״̬
#define MF_COMMAND_READ            0x46     //��һ�����ݿ�
#define MF_COMMAND_WRITE           0x47     //дһ�����ݿ�
#define MF_COMMAND_INCR            0x48     //��ֵ�����Ŀ������ֵ�������洢�ڿ����ڲ��洢����
#define MF_COMMAND_DECR            0x49     //��ֵ�����Ŀ���м�ֵ�������洢�ڿ����ڲ��洢����
#define MF_COMMAND_R_SEND_M        0x4A     //��ĳ������ݴ��뿨���ڲ��Ĵ�����
#define MF_COMMAND_M_SEND_R        0x4B     //���ڲ��Ĵ��������ݴ��͵�ĳһ����
#define MF_COMMAND_LOAD_PASSWORD   0x4C     //װ������

#define MF_COMMAND_RESET           0x4E     //reset ??

#define MF_COMMAND_OTHER           0x56     //������֤����
//MF pro������
#define MF_COMMAND_CPU_RESET       0x5A     //��λ�ǽӴ�ʽCPU��
#define MF_COMMAND_CPU_APDU        0x5B     //����APDU�����CPU��
#define MF_COMMAND_CPU_END         0x5C     //����CPU������
//SAM������
#define MH_SAM_RESET               0x3C     //��λSAM��
#define MH_SAM_COMMAND             0x3D     //����SAM����


//�������������ݷ���֡
typedef struct
{
	uint8_t  cTxSeq;		//�������
	uint8_t  cCommand;	//������
	uint8_t  cLength;    //֡����
	buf buf;	//��������
	uint8_t	cBcc;       //У��
}StructMWTxData,*pStructMWTxData;

//�������������ݽ���֡
typedef struct
{
	uint8_t  cRxSeq;		//�������
	uint8_t  cStatus;	//������
	uint8_t  cLength;    //֡����
	buf buf;		//��������
}StructMWRxData,*pStructMWRxData;

const static unsigned char mf_trans_pass[16][6]={
      { 0xbd,0xde,0x6f,0x37,0x83,0x83 },
      { 0x14,0x8a,0xc5,0xe2,0x28,0x28 },
      { 0x7d,0x3e,0x9f,0x4f,0x95,0x95 },
      { 0xad,0xd6,0x6b,0x35,0xc8,0xc8 },
      { 0xdf,0xef,0x77,0xbb,0xe4,0xe4 },
      { 0x09,0x84,0x42,0x21,0xbc,0xbc },
      { 0x5f,0xaf,0xd7,0xeb,0xa5,0xa5 },
      { 0x29,0x14,0x8a,0xc5,0x9f,0x9f },
      { 0xfa,0xfd,0xfe,0x7f,0xff,0xff },
      { 0x73,0x39,0x9c,0xce,0xbe,0xbe },
      { 0xfc,0x7e,0xbf,0xdf,0xbf,0xbf },
      { 0xcf,0xe7,0x73,0x39,0x51,0x51 },
      { 0xf7,0xfb,0x7d,0x3e,0x5a,0x5a },
      { 0xf2,0x79,0x3c,0x1e,0x8d,0x8d },
      { 0xcf,0xe7,0x73,0x39,0x45,0x45 },
      { 0xb7,0xdb,0x6d,0xb6,0x7d,0x7d }
   };


uint8_t mw_35lt_beep(p_dev_uart rs485_3_dev,uint8_t *cTxS,uint16_t beep_time);
void send_35lt_start_byte(p_dev_uart rs485_3_dev);
uint8_t mw_35lt_set_time(p_dev_uart rs485_3_dev,uint8_t *cTxS);
uint8_t mw_sam_reset( p_dev_uart rs485_3_dev, uint8_t *cTxS ,uint8_t baud);
uint16_t mw_sam_get_random( p_dev_uart rs485_3_dev, uint8_t *cTxS ,uint8_t len,uint8_t * data);
uint8_t mw_35lt_requst_card( p_dev_uart rs485_3_dev, uint8_t *cTxS, uint8_t command );
uint8_t mw_35lt_anti_card( p_dev_uart rs485_3_dev, uint8_t *cTxS, uint32_t *cardnumber);
uint8_t mw_35lt_select_card( p_dev_uart rs485_3_dev, uint8_t *cTxS, uint32_t cardnumber);
uint8_t mw_35lt_reset_cpu( p_dev_uart rs485_3_dev, uint8_t *cTxS);
uint8_t mw_35lt_end_cpu( p_dev_uart rs485_3_dev, uint8_t *cTxS);

p_dev_uart init_rs485_3(void);
uint8_t mw_35lt_requst_cpu( p_dev_uart rs485_3_dev, uint8_t *cTxS, uint8_t command ,uint32_t * cardnumber);

uint8_t mw_35lt_reset( p_dev_uart rs485_3_dev, uint8_t *cTxS);
uint16_t mw_cpu_select_bin( p_dev_uart rs485_3_dev, uint8_t *cTxS ,uint8_t p1,uint8_t p2,uint8_t * pdata, uint8_t len);
uint16_t mw_cpu_read_bin( p_dev_uart rs485_3_dev, uint8_t *cTxS ,uint8_t p1,uint8_t p2, buf pdata, uint8_t len);
uint16_t mw_cpu_get_ramd( p_dev_uart rs485_3_dev, uint8_t *cTxS, void *data);
uint16_t mw_cpu_update_bin( p_dev_uart rs485_3_dev, uint8_t *cTxS ,uint8_t p1,uint8_t p2,buf pdata, uint8_t len);

uint8_t mw_35lt_init_cpu(p_dev_uart rs485_3_dev,uint8_t *cTxS,uint32_t *cardnumber);
uint8_t mw_35lt_check_card(p_dev_uart rs485_3_dev,uint8_t *cTxseq,uint32_t *cardnumber);

uint8_t mw_35lt_cpu_ext_auth(p_dev_uart rs485_3_dev,uint8_t *cTxseq,uint32_t *cardnumber);
uint8_t mw_35lt_int_auth(p_dev_uart rs485_3_dev, uint8_t *cTxseq,uint32_t *cardnumber);


#endif
