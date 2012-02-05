#if PCA955X_ENABLE
#include <drivers/pca955x.h>


//Private Defines
#define PCA955X_SLAVEADR		0x70


//Private Macros
#define pca955x_Irq()			sys_GpioRead(&tbl_bspPca955x.tbl[0])


sys_res pca955x_Init(p_dev_i2c p)
{
	sys_res res;

	sys_GpioConf(&tbl_bspPca955x.tbl[0]);

	res = i2c_WriteByte(p, PCA955X_SLAVEADR, 1, 0xFF); //���������Ϊ�ߵ�ƽ
	res = i2c_WriteByte(p, PCA955X_SLAVEADR, 2, 0x00); //�������������ת
	res = i2c_WriteByte(p, PCA955X_SLAVEADR, 3, PCA955X_CFG_IN); //0/1/2/3/4����,5/6/7���	
	return res;
}

int pca955x_GpioRead(p_dev_i2c p, uint_t n)
{
	int nData = 0;

	i2c_WriteAdr(p, PCA955X_SLAVEADR, 0);
	if (i2c_Read(p, PCA955X_SLAVEADR, &nData, 1) != SYS_R_OK)
		pca955x_Init(p);
	return GETBIT(nData, n);
}

void pca955x_GpioSet(p_dev_i2c p, uint_t nPin, uint_t nHL)
{
	sys_res res;
	uint_t nData = 0;

	i2c_WriteAdr(p, PCA955X_SLAVEADR, 1);
	if ((res = i2c_Read(p, PCA955X_SLAVEADR, &nData, 1)) == SYS_R_OK) {
		if (nHL)
			SETBIT(nData, nPin);
		else
			CLRBIT(nData, nPin);
		res = i2c_WriteByte(p, PCA955X_SLAVEADR, 1, nData);
	}
	if (res != SYS_R_OK)
		pca955x_Init(p);
}

#endif

