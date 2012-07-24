#ifndef __GD5100_H__
#define __GD5100_H__


//Public Defines

//通讯权限
#define GD5100_PWD_LOW					0x01
#define GD5100_PWD_HIGH					0x02
#define GD5100_PWD_ALL					(GD5100_PWD_LOW | GD5100_PWD_HIGH)


//错误编码
#define GD5100_ERR_NO_ERROR				0x00	//正确,无错误
#define GD5100_ERR_RELAY_FALE			0x01	//中继命令没有返回
#define GD5100_ERR_INVALID				0x02	//设置内容非法
#define GD5100_ERR_NO_PERMIT			0x03	//密码权限不足
#define GD5100_ERR_NO_DATA				0x04	//无此项数据
#define GD5100_ERR_TIME_INVALID			0x05	//命令时间失效
#define GD5100_ERR_NO_ADDR				0x11	//目标地址不存在
#define GD5100_ERR_SEND_FAIL			0x12	//发送失败
#define GD5100_ERR_SMS_LONG				0x13	//短消息帧太长

#define GD5100_DATA_INVALID				0xFFFFFFFF



//Public Typedefs

//接收报文结构
typedef struct {
 	uint16_t	rtua;			//地市区县码
 	uint16_t	terid;			//终端地址
 	uint16_t	ste : 6,		//状态
 				fseq : 7,		//帧序号
 				iseq : 3;		//帧内序号
 	uint8_t		code : 6,		//控制码
 				abn : 1,		//异常标志
 				dir : 1;		//传送方向
 	buf			data;			//接收数据
}gd5100_rmsg[1];


//发送报文结构
typedef struct {
	uint8_t	ste : 4,
			retry : 4;
	uint8_t	tmo;
	buf		data;
}t_gd5100_tmsg, *gd5100_tmsg;


//规约结构
typedef struct {
	t_dlrcp		parent;
	uint8_t		pwd[3];
	uint16_t	group;
 	uint16_t	rtua;
 	uint16_t	terid;
	gd5100_rmsg	rmsg;
}t_gd5100, *p_gd5100;






//External Functions
void gd5100_Init(p_gd5100 p);
sys_res gd5100_TmsgSend(p_gd5100 p, uint_t nCode, buf b, uint_t nType);
sys_res gd5100_TmsgError(p_gd5100 p, uint_t nCode, uint_t nErr);
sys_res gd5100_Handler(p_gd5100 p);

void gd5100_Response(p_gd5100 p);



#endif

