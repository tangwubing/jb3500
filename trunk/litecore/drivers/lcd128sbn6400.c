#if GUI_LCD_TYPE == GUI_LCD_T_128_SBN6400


//Private Defines
#define LCD_DELAY					50
#define LCD_LEFT_SCR				BITMASK(3)
#define LCD_RIGHT_SCR				BITMASK(4)

//Register Defines
#define	LCD_DISPON					0x3F	//Dispaly on
#define	LCD_DISPOFF					0x3E	//Display off
#define	LCD_DISPFIRST				0xC0	//Set Start Line:0
#define	LCD_SETX					0x40	//Set XPage
#define	LCD_SETY					0xB8	//Set Y Address

//Private Variables
static uint_t lcd_nBusy = 0;

static uint8_t gui_aBuf[LCD_Y_MAX / 8][LCD_X_MAX];
static uint8_t gui_aOld[LCD_Y_MAX / 8][LCD_X_MAX];

//Private Macros
#define lcd_Rst(x)			sys_GpioSet(gpio_node(tbl_bspLcdCtrl, 0), (x))
#define lcd_Bgc(x)			sys_GpioSet(gpio_node(tbl_bspLcdCtrl, 1), (x))


//Private Functions

//��LCDд����,nScr����������
static void lcd_WriteCmd(int nScr, int nData)
{

	__raw_writeb(nData, GUI_LCD_ADR_CMD | nScr);
	sys_Delay(LCD_DELAY);
}

//��LCDд����,nScr����������
static void lcd_WriteData(int nScr, int nData)
{

	__raw_writeb(nData, GUI_LCD_ADR_DATA | nScr);
	sys_Delay(LCD_DELAY);
}

//����X��������
static void lcd_SetXaddr(int nScr, int x)
{

	lcd_WriteCmd(nScr, x | LCD_SETX);
	sys_Delay(LCD_DELAY * 4);
}

//����Y��������
static void lcd_SetYaddr(int nScr, int y)
{

	lcd_WriteCmd(nScr, y | LCD_SETY);
	sys_Delay(LCD_DELAY * 4);
}

//LCD��ʼ��
void lcd_Init()
{

	//Control GPIO Initialize
	sys_GpioConf(&tbl_bspLcdCtrl[0]);
	sys_GpioConf(&tbl_bspLcdCtrl[1]);
	//Reset
	lcd_Rst(0);
	sys_Delay(10000);
	lcd_Rst(1);
	sys_Delay(5000);
	//Initialize
	lcd_WriteCmd(LCD_LEFT_SCR, LCD_DISPON);
	lcd_WriteCmd(LCD_RIGHT_SCR, LCD_DISPON);
	lcd_WriteCmd(LCD_LEFT_SCR, LCD_DISPFIRST);		//����������0
	lcd_WriteCmd(LCD_RIGHT_SCR, LCD_DISPFIRST); 	//����������0
}

//�������
void lcd_Bgl(int nOnOff)
{

	lcd_Bgc(nOnOff);
}

//����
void lcd_DrawPoint(int x, int y, t_color color)
{

	x &= LCD_X_MAX - 1;
	y &= LCD_Y_MAX - 1;
	color ?	SETBIT(gui_aBuf[y / 8][x], y & 7) :
			CLRBIT(gui_aBuf[y / 8][x], y & 7);
}

//����
void lcd_ClearAll(t_color color)
{

	if (color == COLOR_WHITE)
		memset(gui_aBuf, 0, sizeof(gui_aBuf));
	else
		memset(gui_aBuf, 0xFF, sizeof(gui_aBuf));
}

//��д��
void lcd_Refresh()
{
	uint_t i, j, k;

	if (lcd_nBusy)
		return;
	for (i = 0; i < (LCD_Y_MAX / 8); i++) {
		lcd_SetYaddr(LCD_LEFT_SCR, i);
		for (j = 0, k = LCD_X_MAX; j < (LCD_X_MAX >> 1); j++) {
			if (gui_aOld[i][j] != gui_aBuf[i][j]) {
				gui_aOld[i][j] = gui_aBuf[i][j];
				if (k != j) {
					k = j;
					lcd_SetXaddr(LCD_LEFT_SCR, j);
				}
				if (gui_aBuf[i][j] == 0x3F)
					gui_aBuf[i][j] = 0x3F;
				lcd_WriteData(LCD_LEFT_SCR, gui_aBuf[i][j]);
				k += 1;
			}
		}
		lcd_SetYaddr(LCD_RIGHT_SCR, i);
		for (j = (LCD_X_MAX >> 1), k = LCD_X_MAX; j < LCD_X_MAX; j++) {
			if (gui_aOld[i][j] != gui_aBuf[i][j]) {
				gui_aOld[i][j] = gui_aBuf[i][j];
				if (k != j) {
					k = j;
					lcd_SetXaddr(LCD_RIGHT_SCR, j);
				}
				if (gui_aBuf[i][j] == 0x3F)
					gui_aBuf[i][j] = 0x3F;
				lcd_WriteData(LCD_RIGHT_SCR, gui_aBuf[i][j]);
				k += 1;
			}
		}
	}
}

//�ػ�
void lcd_Redraw()
{
	uint_t i;
	uint8_t *pLeft, *pRight;

	lcd_nBusy = 1;
	lcd_WriteCmd(LCD_LEFT_SCR, LCD_DISPON);
	lcd_WriteCmd(LCD_RIGHT_SCR, LCD_DISPON);
	lcd_WriteCmd(LCD_LEFT_SCR, LCD_DISPFIRST);		//����������0
	lcd_WriteCmd(LCD_RIGHT_SCR, LCD_DISPFIRST); 	//����������0
	for (i = 0; i < (LCD_Y_MAX / 8); i++) {
		lcd_SetXaddr(LCD_LEFT_SCR, 0);
		lcd_SetXaddr(LCD_RIGHT_SCR, 0);
		lcd_SetYaddr(LCD_LEFT_SCR, i);
		lcd_SetYaddr(LCD_RIGHT_SCR, i);
		pLeft = &gui_aBuf[i][0];
		pRight = &gui_aBuf[i][LCD_X_MAX / 2];
		while (pRight < &gui_aBuf[i][LCD_X_MAX]) {
			lcd_WriteData(LCD_LEFT_SCR, *pLeft++);
			lcd_WriteData(LCD_RIGHT_SCR, *pRight++);
		}
	}
	lcd_nBusy = 0;
}


#endif
