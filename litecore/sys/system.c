
//Private Defines
#define SYS_TIMER_ENABLE			0


//Private Variables


//In application
extern void app_Entry(void);
extern void app_Daemon(uint_t nCnt);


#if OS_TYPE
void sys_IOHandle(void *args)
{
	static uint_t nCnt = 0;
	uint_t i;

	nCnt += 1;
#if IRQ_HALF_ENABLE
	//底半中断处理
	if (irq_Wait() == SYS_R_OK)
		irq_BottomHandler();
#endif
	if ((nCnt % (100 / OS_TICK_MS)) == 0) {
#if OS_QUEUE_QTY
		os_que_Idle();
#endif
#if KEY_ENABLE
		//按键扫描
		i = key_Read();
		if (i)
			os_que_Send(QUE_EVT_KEYBOARD, NULL, &i, sizeof(uint_t), 1000);
#endif
#if PULSE_COL_ENABLE
		//脉冲采集
		i = pulse_Read();
		if (i)
			os_que_Send(QUE_EVT_PULSE, NULL, &i, sizeof(uint_t), 200);
#endif
#if UART_ENABLE
		//串口维护
		uart_Maintain();
#endif
	}
#if USB_ENABLE
	usb_HostHandler();
#endif
#if TCPPS_ENABLE
	net_Handler();
#endif
}

#if SYS_TIMER_ENABLE
struct rt_timer timer_IOHandle;
#else
os_thd_declare(SysIo, 1024);
void tsk_SysIo(void *args)
{

	for (; ; ) {
		sys_IOHandle(NULL);
		os_thd_Slp1Tick();
	}
}
#endif
#endif


#if OS_TYPE
void sys_Maintain()
{
	uint_t nCnt;

	for (nCnt = 0; ; nCnt++) {
		os_thd_Sleep(1000);
#if MODEM_ENABLE
		modem_Run();
#endif
		if ((nCnt & 0x03) == 0) {
#if DEBUG_MEMORY_ENABLE
			list_memdebug(0, 0);
#endif
#if BAT_VOL_ENABLE
			bat_VolGet();
#endif
		}
		if ((nCnt & 0x1F) == 0) {
#if RTC_ENABLE
			rtc_Maintain();
#endif
#if REGISTER_ENABLE
			reg_Maintain();
#endif
#if FLASH_ENABLE
			flash_Flush(60);
#endif
		}
#if UART_ENABLE
		if ((nCnt & 0x0FFF) == 0)
			uart_Reopen();
#endif
	}
}


static struct rt_timer timer_daemon;
void sys_Daemon(void *args)
{
	static uint_t nCnt = 0;

	if ((nCnt & 0x03) == 0)
		wdg_Reload(1);
	if ((nCnt % 10) == 0) {
#if RTC_ENABLE
		rtc_OsTick();
#endif
#if BATTERY_ENABLE
		bat_Maintain();
#endif
	}

	app_Daemon(nCnt);

	nCnt += 1;
}
#endif


void sys_Init()
{

	//创建系统守护定时(1S)
#if OS_TYPE
	rt_timer_init(&timer_daemon, "daemon", sys_Daemon, NULL, 100 / OS_TICK_MS, RT_TIMER_FLAG_PERIODIC);
	rt_timer_start(&timer_daemon);
#endif

#if OS_TYPE
	buf_Init();
#endif

#if OS_QUEUE_QTY
	os_que_Init();
#endif

#if IRQ_ENABLE
	irq_Init();
#endif

#if BATTERY_ENABLE
	bat_On();
#endif

//-------------------------------------------------------------------------
//GPIO Functions
//-------------------------------------------------------------------------
#if PULSE_COL_ENABLE
	pulse_Init();
#endif
#if HT1621_ENABLE
	ht1621_Init();
#endif

//-------------------------------------------------------------------------
//External Parallel Interface Functions
//-------------------------------------------------------------------------
#if NANDFLASH_ENABLE
	nand_Init();
#endif

//-------------------------------------------------------------------------
//I2C Interface Functions
//-------------------------------------------------------------------------
#if PCA955X_ENABLE
	pca955x_Init();
#endif

//-------------------------------------------------------------------------
//SPI Interface Functions
//-------------------------------------------------------------------------
#if SC16IS7X_ENABLE
	sc16is7x_Init();
#endif
#if ATT7022_ENABLE
	att7022_Init();
#endif
#if NRSEC3000_ENABLE
	nrsec3000_Init();
#endif
#if VOICE_ENABLE
	wtv_Init();
#endif
#if MIFARE_ENABLE
	mf_InitGpio();
#endif

//-------------------------------------------------------------------------
//UART Interface Functions
//-------------------------------------------------------------------------
#if VK321X_ENABLE
	vk321x_Init();
#endif
#if TDK6515_ENABLE
	tdk6515_Init();
#endif
#if MODEM_ENABLE
	modem_Init();
#endif

//-------------------------------------------------------------------------
//Real Time Clock Functions
//-------------------------------------------------------------------------
#if RTC_ENABLE
	rtc_Init();
#endif

//-------------------------------------------------------------------------
//Backup Functions
//-------------------------------------------------------------------------
#if BKP_ENABLE
	bkp_Init();
#endif

//-------------------------------------------------------------------------
//Flash System Functions
//-------------------------------------------------------------------------
#if INTFLASH_ENABLE
	intf_Init();
#endif
#if NORFLASH_ENABLE
	norf_Init();
#endif
#if SPIFLASH_ENABLE
	spif_Init();
#endif
#if FLASH_ENABLE
	flash_Init();
#endif


//-------------------------------------------------------------------------
//File System Functions
//-------------------------------------------------------------------------
#if FS_ENABLE
	fs_init();
#endif

//-------------------------------------------------------------------------
//USB Interface Functions
//-------------------------------------------------------------------------
#if USB_ENABLE
	usb_Init();
#endif


//-------------------------------------------------------------------------
//Register Subsystem Functions
//-------------------------------------------------------------------------
#if REGISTER_ENABLE
	reg_Init();
#endif



//-------------------------------------------------------------------------
//Network Subsystem Functions
//-------------------------------------------------------------------------
#if TCPPS_ENABLE
	net_Init();
#endif

//-------------------------------------------------------------------------
//In Application Programming Functions
//-------------------------------------------------------------------------
#if GDFTS_ENABLE
	gdfts_Init();
#endif

#if GUI_ENABLE
	gui_Init();
#endif

#if OS_TYPE
	//创建系统IO处理线程
#if SYS_TIMER_ENABLE
	rt_timer_init(&timer_IOHandle, "sysio", sys_IOHandle, NULL, 1, RT_TIMER_FLAG_PERIODIC);
	rt_timer_start(&timer_IOHandle);
#else
	os_thd_Create(SysIo, 240);
#endif

	//创建应用层线程
	app_Entry();

	//系统维护线程
	sys_Maintain();
#endif
}


void sys_Start()
{
	uint_t i;
	p_gpio_def p;
#if I2C_ENABLE
	p_dev_i2c pI2c;
#endif
#if SPI_ENABLE
	p_dev_spi pSpi;
#endif
#if UART_ENABLE
	p_dev_uart pUart;
#endif
	
	arch_Init();

#if IRQ_ENABLE
	irq_VectorInit();
#endif

#if GPIO_ENABLE
	for (p = tbl_bspGpio[0]; p < tbl_bspGpio[1]; p++) {
		sys_GpioConf(p);
	}
#endif

#if HC138_ENABLE
	for (p = tbl_bspHC138[0]; p < tbl_bspHC138[1]; p++) {
		sys_GpioConf(p);
	}
#endif

#if HC165_ENABLE
	for (p = tbl_bspHC165[0]; p < tbl_bspHC165[1]; p++) {
		sys_GpioConf(p);
	}
#endif

#if HC595_ENABLE
	for (p = tbl_bspHC595[0]; p < tbl_bspHC595[1]; p++) {
		sys_GpioConf(p);
	}
#endif

#if BATTERY_ENABLE
	for (p = tbl_bspBattery[0]; p < tbl_bspBattery[1]; p++) {
		sys_GpioConf(p);
	}
#if BAT_VOL_ENABLE
	arch_AdcInit();
#endif
#endif

#if WDG_ENABLE
	wdg_Init();
#endif

#if BKP_ENABLE
	bkp_Init();
#endif

#if KEY_ENABLE
	key_Init();
#endif

#if IO_BUF_TYPE == BUF_T_DQUEUE
	dque_Init(dqueue);
#endif

#if EPI_SOFTWARE
	for (p = tbl_bspEpiData[0]; p < tbl_bspEpiData[1]; p++) {
		sys_GpioConf(p);
	}
#endif

#if I2C_ENABLE
	bzero(dev_I2c, sizeof(dev_I2c));
	for (pI2c = dev_I2c, i = 0; pI2c < ARR_ENDADR(dev_I2c); pI2c++, i++) {
		pI2c->parent->id = i;
		pI2c->parent->type = DEV_T_I2C;
#if I2C_SOFTWARE
		i2cbus_Init(pI2c);
#else
		pI2c->def = &tbl_bspI2cDef[i];
		arch_I2cInit(pI2c);
#endif
	}
#endif

#if SPI_ENABLE
	bzero(dev_Spi, sizeof(dev_Spi));
	for (pSpi = dev_Spi, i = 0; pSpi < ARR_ENDADR(dev_Spi); pSpi++, i++) {
		pSpi->parent->id = i;
		pSpi->parent->type = DEV_T_SPI;
		pSpi->csid = SPI_CSID_INVALID;
#if SPI_SOFTWARE
		spibus_Init(pSpi);
#else
		pSpi->def = &tbl_bspSpiDef[i];
		arch_SpiInit(pSpi);
#endif
	}
#endif

#if UART_ENABLE
	bzero(dev_Uart, sizeof(dev_Uart));
	for (pUart = dev_Uart, i = 0; pUart < ARR_ENDADR(dev_Uart); pUart++, i++) {
		pUart->parent->id = i;
		pUart->parent->type = DEV_T_UART;
		pUart->def = &tbl_bspUartDef[i];
		uart_Init(pUart);
	}
#endif

#if OS_TYPE
	os_Start();
#else
	sys_Init();
#endif
}

void sys_Reset()
{

#if REGISTER_ENABLE
	reg_Maintain();
#endif
#if FLASH_ENABLE
	flash_Flush(0);
#endif
	arch_Reset();
}

void sys_Delay(volatile uint_t n)
{

	while (n--);
}


