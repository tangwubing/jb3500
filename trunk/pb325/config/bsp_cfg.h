#ifndef __BSP_CONF_H__
#define __BSP_CONF_H__




//Ӳ���汾
#define VER_HARD						0x0010
//����汾
#define VER_SOFT						0x0056

//Main Control Unit Select
#define ARCH_TYPE						ARCH_T_LM3S5X

//Operation System Select
#define OS_TYPE							OS_T_RTTHREAD

#if OS_TYPE != OS_T_NULL
	//����ϵͳTick(ms)
	#define OS_TICK_MS					10
	//����ϵͳ���ȼ�
	#define OS_PRIO_MAX					32
	//��Ϣ��������
	#define OS_QUEUE_QTY				16
	//��д��ʹ��
	#define OS_RWL_ENABLE				0
#endif

//IO Buffer Select
#define IO_BUF_TYPE						BUF_T_DQUEUE

//����̨ʹ��
#define CONSOLE_ENABLE					0

//�豸ʹ��
#define DEVICE_ENABLE					1

//�ж�ϵͳʹ��
#define IRQ_ENABLE						1

//����ʹ��
#define DEBUG_ENABLE					0
#if DEBUG_ENABLE
	#define MEMORY_DEBUG_ENABLE 		0
	#if MEMORY_DEBUG_ENABLE
		#define rt_malloc 				mem_Malloc
		#define rt_realloc				mem_Realloc
		#define rt_free					mem_Free
	#endif
#endif


//System Header File
#include <header.h>




//�ڲ�8M Oscillatorʹ��
#define MCU_HSI_ENABLE					0

//MCUƵ��
#define MCU_FREQUENCY					MCU_SPEED_HALF

//Internal SRAM End Address
#define MCU_SRAM_BASE_ADR				0x20000000
#define MCU_SRAM_SIZE					0x10000

//Bootloader Size
#define BOOTLOADER_SIZE					0x4000

#if IO_BUF_TYPE == BUF_T_DQUEUE
	#define DQUEUE_QTY					32
#endif


//���Ź�ʹ��
#define WDG_ENABLE						0
#if WDG_ENABLE
	//Ƭ�ڿ��Ź�ʹ��
	#define WDT_INT_ENABLE				1
	//Ƭ�⿴�Ź�ʹ��
	#define WDG_T_EXTERNEL				0
	#if WDG_T_EXTERNEL
		extern tbl_gpio_def tbl_WdgCtrl[];
	#endif
#endif

//GPIOʹ��
#define GPIO_ENABLE						1
#if GPIO_ENABLE
	extern tbl_gpio_def tbl_bspGpio[];
#endif

// 3-8������ʹ��
#define HC138_ENABLE					0
#if HC138_ENABLE
	extern tbl_gpio_def tbl_bspHC138[];
#endif

//HC165ʹ��
#define HC165_ENABLE					1
#if HC165_ENABLE
	extern tbl_gpio_def tbl_bspHC165[];
#endif

//HC595ʹ��
#define HC595_ENABLE                    1
#if HC595_ENABLE
	extern tbl_gpio_def tbl_bspHC595[];
#endif

//��ʽҺ����ʾHT1621ʹ��
#define HT1621_ENABLE					1
#if HT1621_ENABLE
	#include <drivers/ht1621.h>
	extern tbl_gpio_def tbl_bspHT1621[];
#endif

//�û�����
#define KEY_ENABLE						1
#if KEY_ENABLE
	extern tbl_gpio_def tbl_bspKeypad[];
#endif

//Battery enable
#define BATTERY_ENABLE					1
#if BATTERY_ENABLE
	#define BAT_VOL_ENABLE				0
	#define BAT_CTRL_HIGH_EN			1
	extern tbl_gpio_def tbl_bspBattery[];
#endif

//����ɼ�
#define PULSE_COL_ENABLE				1
#if PULSE_COL_ENABLE
	extern tbl_gpio_def tbl_bspPulse[];
#endif

//�ڲ�flashʹ��
#define INTFLASH_ENABLE 				1
#if INTFLASH_ENABLE
	#define INTFLASH_BLK_SIZE			0x400
	#define INTFLASH_BASE_ADR			0
    #define INTFLASH_BLK_START          0
    #define INTFLASH_BLK_END            0
#endif

//�ⲿ��չ����
#define EPI_ENABLE						0
#if EPI_ENABLE
	//���EPIʹ��
	#define EPI_SOFTWARE				0

	//�ⲿNorFlashʹ��
	#define NORFLASH_ENABLE				0
	#if NORFLASH_ENABLE
		#define NORFLASH_BASE_ADR		0x64000000
	#endif
	//�ⲿSRAMʹ��
	#define EXTSRAM_ENABLE 				0
	#if EXTSRAM_ENABLE
		#define EXTSRAM_BASE_ADR		0x68000000
		#define EXTSRAM_SIZE			0x80000
	#endif
	//�ⲿNandFlashʹ��
	#define NANDFLASH_ENABLE			1
	#if NANDFLASH_ENABLE
		//���ECCʹ��
		#define NAND_ECC_SOFT			0
		//NAND��������
		#define NAND_PAGE_DATA			512
		#define NAND_PAGE_SPARE			16
		#define NAND_BLK_PAGE			32
		#define NAND_BLK_QTY			4096
		#define NAND_DATA_WIDTH			8
		//NAND������ַ
		#define NAND_BASE_ADR			0x80000000
	#endif
#endif

//I2Cʹ��
#define I2C_ENABLE						1
#if I2C_ENABLE
	#define BSP_I2C_QTY					1

	#define I2C_SOFTWARE				1
	#if I2C_SOFTWARE
		extern tbl_gpio_def tbl_bspI2cDef[BSP_I2C_QTY];
	#else
		#define I2C_IRQ_ENABLE			0
		extern t_i2c_def tbl_bspI2cDef[BSP_I2C_QTY];
	#endif

	#define PCA955X_ENABLE				0
	#if PCA955X_ENABLE
		#define PCA955X_COMID			0
		#define PCA955X_CFG_IN			0x001F
			extern tbl_gpio_def tbl_bspPca955x[];
	#endif
#endif

//SPIʹ��
#define SPI_ENABLE						1
#if SPI_ENABLE
	#define BSP_SPI_QTY					1
	#define SPI_CE_ENABLE				1
	#define SPI_SEL_ENABLE				0

	#define SPI_SOFTWARE				1
	#if SPI_SOFTWARE
		extern tbl_gpio_def tbl_bspSpiDef[BSP_SPI_QTY];
	#else
		#define SPI_IRQ_ENABLE			0
		extern t_spi_def tbl_bspSpiDef[BSP_SPI_QTY];
	#endif

	//SpiFlashʹ��
	#define SPIFLASH_ENABLE				1
	#if SPIFLASH_ENABLE
		#define BSP_SPIF_QTY			1
		#define SPIF_COMID				0
	#endif

	//SC16IS7Xʹ��
	#define SC16IS7X_ENABLE 			0
	#if SC16IS7X_ENABLE
		#define SC16IS7X_COMID			2
		#define SC16IS7X_SPI_SPEED		400000
		#define SC16IS7X_CRYSTAL		11059200
		extern tbl_gpio_def tbl_bspSc16is7x[];
	#endif

	//ATT7022ʹ��
	#define	ATT7022_ENABLE				0
	#if ATT7022_ENABLE
		#define ATT7022_COMID			0
		#define ATT7022_CSID			6
		#define	ATT7022_CONST_EC		8000
		extern tbl_gpio_def tbl_bspAtt7022[];
	#endif

	//WTV Enable
	#define VOICE_ENABLE				0
	#if VOICE_ENABLE
		#define WTV_SPI_SPEED			200000
		extern p_gpio_def tbl_bspVoice[];
	#endif

	//RC522 Enable
	#define MIFARE_ENABLE				0
	#if MIFARE_ENABLE
		#define RC522_SPI_SPEED			2000000
		extern p_gpio_def tbl_bspMifare[];
	#endif
#endif

//UARTʹ��
#define UART_ENABLE						1
#if UART_ENABLE
	#define BSP_UART_QTY				4
	extern t_uart_def tbl_bspUartDef[BSP_UART_QTY];
	//Timer Uartʹ��
	#define SWUART_ENABLE				1
	#if SWUART_ENABLE
		#define SWUART_QTY				1
	#endif

	//SMARTCARDʹ��
	#define SMARTCARD_ENABLE			0
	#if SMARTCARD_ENABLE
		#define SC_REFLEX_ENABLE		0
	#endif

	//IRDAʹ��
	#define IRDA_ENABLE 				1
	#if IRDA_ENABLE
		#define IRDA_MODE				IRDA_MODE_TIM
	#endif

	//RS485ʹ��
	#define RS485_ENABLE				0
	#if RS485_ENABLE
		#define BSP_RS485_QTY			3
		extern const uint8_t tbl_bspRs485Id[BSP_RS485_QTY];
	#endif

	//VK321Xʹ��
	#define VK321X_ENABLE				0
	#if VK321X_ENABLE
		#define VK321X_CRYSTAL			1743800
		#define VK321X_COMID			3
		#define VK321X_INT_ID			13
		extern tbl_gpio_def tbl_bspVk321x[];
	#endif

	//TDK6515ʹ��
	#define	TDK6515_ENABLE				1
	#if TDK6515_ENABLE
		#define TDK6515_COMID			1
		extern tbl_gpio_def tbl_bspTdk6515[];
	#endif

	//MODEMʹ��
	#define MODEM_ENABLE				1
	#if MODEM_ENABLE
		#define BSP_MODEM_QTY			1
		#define MODEM_PWR_ENABLE		1
		#define MODEM_RST_ENABLE		1
		#define MODEM_CTS_ENABLE		0
		#define MODEM_RTS_ENABLE		0
		#define MODEM_DTR_ENABLE		0
		#define MODEM_DCD_ENABLE		0

		#include <drivers/modem.h>
		extern t_modem_def * const tbl_bspModem[BSP_MODEM_QTY];
	#endif
#endif

//RTCʹ��
#define RTC_ENABLE						1
#if RTC_ENABLE
	#define RTC_TYPE					RTC_T_R202X
	#define RTC_COMID					0
#endif

//Flashʹ��
#define FLASH_ENABLE					1

//BKPʹ��
#define BKP_ENABLE						0
#if BKP_ENABLE
	#define BKP_TYPE					BKP_T_EEPROM
	#define BKP_COMID					0
	#define BKP_DEVID					0xA0
#endif

//SFSʹ��
#define SFS_ENABLE						1

//USBʹ��
#define USB_ENABLE						1
#if USB_ENABLE
	#define USB_CONTROLL_ENABLE			0
#endif


//TCPIPʹ��
#define TCPPS_ENABLE					1
#if TCPPS_ENABLE
	//Э��ջ����
	#define TCPPS_TYPE					TCPPS_T_KEILTCP

	//���ݴ���������ȼ�
	#define TCPPS_THREAD_PRIORITY		((256 - 224) / 8)
	#define TCPPS_THREAD_MBOXSIZE		8
	#define TCPPS_THREAD_STACKSIZE		1024

	//���ݴ�����(Unit: OS_Tick)
	#define TCPPS_DEBUG_ENABLE			0
	#define TCPPS_STATS_ENABLE			0

	#define TCPPS_MEMP_MALLOC			1
	#define TCPPS_PBUF_NUM				8

	//EtherNet Interface����
	#define TCPPS_ETH_ENABLE			0
	#if TCPPS_ETH_ENABLE
		#define TCPPS_ETH_PAD_SIZE		2
		/* ethernet if thread options */
		#define TCPPS_ETH_PRIORITY		((256 - 208) / 8)
		#define TCPPS_ETH_MBOX_SIZE		4
		#define TCPPS_ETH_STACKSIZE		512
	#endif

	//PPP Interface����
	#define TCPPS_PPP_ENABLE			1
	#if TCPPS_PPP_ENABLE
		#define TCPPS_PPP_PRIORITY		((256 - 216) / 8)
		#define TCPPS_PPP_STACKSIZE		512
	#endif

	#define TCPPS_SLIP_ENABLE			0
	#define TCPPS_LOOPIF_ENABLE			0

	#define TCPPS_IGMP_ENABLE			0

	#define TCPPS_ICMP_ENABLE			1

	#define TCPPS_SNMP_ENABLE			0

	#define TCPPS_RAW_PCB_NUM			1

	#define TCPPS_TCP_ENABLE			1
	#if TCPPS_TCP_ENABLE
		#define TCPPS_TCP_PCB_NUM		8
		#define TCPPS_TCP_SEG_NUM		16
	#endif

	#define TCPPS_UDP_ENABLE			1
	#if TCPPS_UDP_ENABLE
		#define TCPPS_UDP_PCB_NUM		4

		#define TCPPS_DHCP_ENABLE		0

		#define TCPPS_DNS_ENABLE		1
		#define TCPPS_DNS_TABLE_NUM		4
		#define TCPPS_DNS_NAME_SIZE		256
	#endif

	#define NETIO_SERVER_ENABLE			0

	#define FTP_SERVER_ENABLE			0

#endif

//EtherNet Interfaceʹ��
#if TCPPS_ETH_ENABLE
	//LM3S9X
	#define LM3S_ETH_ENABLE				1
	//LPC176X
	#define LPC176X_ETH_ENABLE 			0
	//DM90000
	#define DM9000_ENABLE				0
	#if DM9000_ENABLE
		//������ַ
		#define DM9000_BASE_ADR 		0x60000000
		#define DM9000_ADR_CMD			DM9000_BASE_ADR
		#define DM9000_ADR_DATA 		(DM9000_BASE_ADR | BITMASK(24))
		//����λ��ѡ��
		#define DM9000_BUS_WIDTH		16
		//�ж�ʹ��
		#define DM9000_INT_ENABLE		1
		#define DM9000_INT_ID			7
		//���ƽŶ���
		extern p_gpio_def tbl_bspDm9000[];
	#endif
#endif

//ͨѶͨ��ʹ��
#define CHANNEL_ENABLE					1

//ͨѶ��Լʹ��
#define DLT645_ENABLE					1
#define GW3762_ENABLE					0

//RPCʹ��
#define RPC_ENABLE						0

//RTPʹ��
#define RTP_ENABLE						0

//File Systemʹ��
#define FS_ENABLE						1
#if FS_ENABLE
	//Rom FileSystemʹ��
	#define ROMFS_ENABLE				0
	//Net FileSystemʹ��
	#define NFS_ENABLE					0
	//Nand FileSystemʹ��
	#define NANDFS_ENABLE				0
	#if NANDFS_ENABLE
		#define FS_NAND_PATH			"/nf0"
	#endif
	//SpiFlash FileSystemʹ��
	#define SPIFS_ENABLE				0
	#if SPIFS_ENABLE
		#define FS_SPIF_PATH			"/sf0"
		#define SPIF_FS_BASE_BLOCK		256
	#endif
	//Usb Mscʹ��
	#define USBMSC_ENABLE				1
	#if USBMSC_ENABLE
		#define FS_USBMSC_PATH			"/"
	#endif
#endif

//Register Systemʹ��
#define REGISTER_ENABLE 				0
#if REGISTER_ENABLE
	#define REGISTER_LOADER_QTY 		16

	#define REGISTER_SIZE_TN			8
	#define REGISTER_SIZE_METER 		8
	#define REGISTER_SIZE_GROUP 		2
	#define REGISTER_SIZE_PULSE 		2
	#define REGISTER_SIZE_ANA			8
	#define REGISTER_SIZE_TASK			1
#endif

#define DLRCP_ENABLE					1
#if DLRCP_ENABLE
	#define GD5100_ENABLE				1

	#define GW3761_ENABLE				1
	#define GW3761_TYPE					GW3761_T_GWJC2009
	#define GW3761_IDCHECK_ENABLE		0
	#define GW3761_ESAM_ENABLE			0
	#if GW3761_ESAM_ENABLE
		#define GW3761_ESAM_UARTID		6
	#endif
#endif

//GDFTSʹ��
#define GDFTS_ENABLE					0
#if GDFTS_ENABLE
	#include <fs/sfs/sfs.h>

	#define GDFTS_ID_SELF				1
	extern t_flash_dev upd_DataDev;
	extern sfs_dev upd_SfsDev;
#endif

//ͼ�ν���
#define GUI_ENABLE						0
#if GUI_ENABLE
	//LCD�ͺ�ѡ��
	#define GUI_LCD_TYPE				GUI_LCD_T_160_UC1698
	#define GUI_COLOR_SIZE				2
	//������ַ
	#define GUI_LCD_BASE_ADR			0x6C000000
	#define GUI_LCD_ADR_CMD 			GUI_LCD_BASE_ADR
	#define GUI_LCD_ADR_DATA			(GUI_LCD_BASE_ADR | BITMASK(23))
	//�ֿ�����
	#define GUI_FONT_TYPE				GUI_FONT_STD12
	//�ֿ�洢
	#define GUI_FONT_CARR_TYPE			GUI_FONT_CARR_T_NAND
	//�ֿ�ֱ�ӷ��ʵ�ַ
	#define GUI_FONT_BASE				0x08040000
	//�ֿ��ļ�
	#define GUI_FONT_FILENAME			"/font/chs12st"
	//�ֿ�ƫ�Ƶ�ַ
	#define GUI_FONT_OFFSET 			4064
	//���ƽŶ���
	extern p_gpio_def tbl_bspLcdCtrl[];
#endif


#endif


