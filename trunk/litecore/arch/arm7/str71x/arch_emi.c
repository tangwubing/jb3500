
#if EPI_ENABLE
void str7_EmiInit()
{

	//EMI��������
	GPIO_Config(GPIO2, BITMASK(0), GPIO_AF_PP);			//CS0
	GPIO_Config(GPIO2, BITMASK(1), GPIO_AF_PP);			//CS1
	GPIO_Config(GPIO2, BITMASK(2), GPIO_AF_PP);			//CS2
	GPIO_Config(GPIO2, BITMASK(3), GPIO_AF_PP);			//CS0
	GPIO_Config(GPIO2, BITMASK(4), GPIO_AF_PP);			//A20
	GPIO_Config(GPIO2, BITMASK(5), GPIO_AF_PP);			//A21
	GPIO_Config(GPIO2, BITMASK(6), GPIO_AF_PP);			//A22
	GPIO_Config(GPIO2, BITMASK(7), GPIO_AF_PP);			//A23
}
#endif

