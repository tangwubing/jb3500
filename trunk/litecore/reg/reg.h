#ifndef __REG_H__
#define __REG_H__


#ifdef __cplusplus
extern "C" {
#endif


//Register���ݽṹ
typedef const struct {
	uint8_t		id;			//�Ĵ������
	uint8_t		type;		//�Ĵ�������
	uint16_t	size;		//�Ĵ�������
}t_register, *p_register;

typedef const struct {
	uint8_t		id;			//�Ĵ�������
	uint8_t		qty;		//�Ĵ���Ԫ������
	uint16_t	type : 4,	//�Ĵ���������
				size : 12;	//�Ĵ������С
	p_register	tblreg;	//�Ĵ���Ԫ���ַ
}t_reg_grptbl, *p_reg_grptbl;




//�Ĵ����������Ͷ���
#define REG_TYPE_SINT 				0x01		//�з��� HEX
#define REG_TYPE_UINT 				0x02		//�޷��� HEX
#define REG_TYPE_SBCD 				0x03		//�з��� BCD
#define REG_TYPE_UBCD 				0x04		//�޷��� BCD
#define REG_TYPE_SDEC				0x05		//�з���fixpoint
#define REG_TYPE_UDEC				0x06		//�޷���fixpoint
#define REG_TYPE_FLOAT				0x07		//float

#define REG_TYPE_STRING 			0x10		//�ַ���,ĩβ��0
#define REG_TYPE_OTHER 				0x11		//��������
#define REG_TYPE_BIN 				0x12		//2��������,������ֵת��
#define REG_TYPE_TIME 				0x13		//ʱ��
#define REG_TYPE_IP					0x14		//IP��ַ���˿�
#define REG_TYPE_PASSWORD			0x15		//����
#define REG_TYPE_BOOL				0x80		//��Ч/��Ч
#define REG_TYPE_METER				0x81		//�������
#define REG_TYPE_TNLINE				0x82		//��������߷�ʽ
#define REG_TYPE_COMMODE			0x83		//����ͨѶģ�鹤����ʽ 
#define REG_TYPE_UARTPARITY			0x84		//UARTУ��
#define REG_TYPE_UARTMODE			0x85		//UARTģʽ
#define REG_TYPE_UARTSTOP			0x86		//UARTֹͣλ
#define REG_TYPE_RESET				0x87		//�ն˸�λ��־

//�Ĵ�����������
#define REG_O_COPY					0x00
#define REG_O_ADD					0x01
#define REG_O_SUB					0x02
#define REG_O_MUL					0x03
#define REG_O_DIV					0x04
#define REG_O_OR					0x05
#define REG_O_AND					0x06
#define REG_O_SETBIT				0x07
#define REG_O_CLRBIT				0x08



//��Ϣ��Ŷ���
#define REG_DA_ALL					0xFFFF

//AFN����
#define GW3761_AFN_CONFIRM			0x00
#define GW3761_AFN_RESET			0x01
#define GW3761_AFN_LINKCHECK		0x02
#define GW3761_AFN_CMD_RELAY		0x03
#define GW3761_AFN_PARA_SET			0x04
#define GW3761_AFN_CMD_CTRL			0x05
#define GW3761_AFN_AUTHORITY		0x06
#define GW3761_AFN_CASCADE_QUERY	0x08
#define GW3761_AFN_CONFIG_GET		0x09
#define GW3761_AFN_PARA_GET			0x0A
#define GW3761_AFN_DATA_TASK		0x0B
#define GW3761_AFN_DATA_L1			0x0C
#define GW3761_AFN_DATA_L2			0x0D
#define GW3761_AFN_DATA_L3			0x0E
#define GW3761_AFN_FILE_TRANS		0x0F
#define GW3761_AFN_DATA_TRANS		0x10





//External Functions
int reg_Get(uint_t nDa, uint_t nID, void *pData);
uint32_t reg_GetValue(uint_t nDa, uint_t nID);
int reg_GetBit(uint_t nDa, uint_t nID, uint_t nBit);

int reg_Set(uint_t nDa, uint_t nID, const void *pData, uint_t nSave);
void reg_SetValue(uint_t nDa, uint_t nID, uint32_t nData, uint_t nSave);

int reg_GetRegSize(uint_t nID);
int reg_GetRegType(uint_t nID);

int reg_ArithmeticCopy(uint_t nDestDa, uint_t nDestReg, uint_t nSrcDa, uint_t nSrcReg, uint_t nType, uint32_t nParam, uint_t nSave);
int reg_OperateAdd(uint_t nDa, uint_t nReg, uint32_t nParam, uint_t nSave);


sys_res reg_GroupSave(uint_t nDa, uint_t nGrp);
sys_res reg_GroupCopy(uint_t nDestDa, uint_t nDestGrp, uint_t nSrcDa, uint_t nSrcGrp, uint_t nSave);

void reg_DefaultReg(uint_t nDa, uint_t nReg, uint_t nSave);
void reg_DefaultGroup(uint_t nDa, uint_t nGrp, uint_t nSave);

sys_res reg_Init(void);
sys_res reg_Maintain(void);


#ifdef __cplusplus
}
#endif

#endif

