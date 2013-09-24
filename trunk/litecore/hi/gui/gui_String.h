#ifndef _GUI_STRING_H_
#define _GUI_STRING_H_

#if GUI_FONT_TYPE == GUI_FONT_STD12
#define ASC_WIDTH  			6  			//�ַ����
#define ASC_HEIGHT  		12			//�ַ��߶�
#define HZ_WIDTH			12 			//���ֿ��
#define HZ_SPAN				1			//���ּ��
#define HZ_HEIGHT			12			//12�㺺�ָ߶�
#define HZ_SIZE				18  		//���ִ�СΪ12X12���󣬺ϼ�18���ֽ�
#elif GUI_FONT_TYPE == GUI_FONT_STD16
#define ASC_WIDTH  			6  			//�ַ����
#define ASC_HEIGHT  		16			//�ַ��߶�
#define HZ_WIDTH			16 			//���ֿ��
#define HZ_SPAN				0			//���ּ��
#define HZ_HEIGHT			16			//16�㺺�ָ߶�
#define HZ_SIZE				32  		//���ִ�СΪ16X16���󣬺ϼ�32���ֽ�
#endif

#define ASC6x8_SIZE			6  			//�ַ��Ĵ�СΪ6X8���󣬺ϼ�6�ֽ�
#define ASC6x12_SIZE		12  		//�ַ��Ĵ�СΪ6X12���󣬺ϼ�12�ֽ�
#define ASC6x16_SIZE		12  		//�ַ��Ĵ�СΪ6X16���󣬺ϼ�12�ֽ�



//External Functions
void gui_DrawString_ASC6x8(int x, int y, const char *pStr, t_color nColor);
void gui_DrawString_ASC6x12(int x, int y, const char *pStr, t_color nColor);
void gui_DrawString_ASC6x16(int x, int y, const char *pStr, t_color nColor);
int gui_DrawString_Mixed(int x, int y, const char *pStr, t_color nColor);
int gui_DrawString_Mixed_Center(int x, int y, const char *pStr, t_color nColor);
int gui_DrawString_Mixed_Right(int y, const char *pStr, t_color nColor);


#endif

