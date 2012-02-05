#ifndef __GUI_BASIC_H__
#define __GUI_BASIC_H__


#ifdef __cplusplus
extern "C" {
#endif





//Public Defines
#define COLOR_WHITE  			0
#define COLOR_BLACK  			0xFFFFFFFF


typedef struct {
	int x;
	int y;
}POINT;

typedef struct {
	int nLeft;
	int nTop;
	int nHeight;
	int nWidth;
}RECT;

//External Functions
#define gui_Refresh()			lcd_Refresh()
#define gui_Redraw()			lcd_Redraw()
#define gui_BglOn()				lcd_Bgl(1)
#define gui_BglOff()			lcd_Bgl(0)
#define gui_DrawPoint(x, y, c)	lcd_DrawPoint(x, y, c)
#define gui_GetPoint(x, y)		lcd_GetPoint(x, y)

void gui_Init(void);
void gui_DrawLine(int x0, int y0, int x1, int y1, t_color color);				//����
void gui_DrawHLine(int x0, int y0, int x1, t_color color);
void gui_DrawRLine(int x0, int y0, int y1, t_color color);
void gui_DrawRect_Hol(int x1, int y1, int x2, int y2, t_color color);			//�����ľ���
void gui_DrawRect_Fill(int x1, int y1, int x2, int y2, t_color color);			//��ʵ�ľ���
void gui_DrawIcon_Mono(int x1, int y1, int x2, int y2, const uint8_t *p);
void gui_DrawBMP_Mono(uint8_t *p);

#ifdef __cplusplus
}
#endif

#endif

