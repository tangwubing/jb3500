#ifndef __KEYPAD_H__
#define __KEYPAD_H__



#ifdef __cplusplus
extern "C" {
#endif



//Public Defines
#define KEY_T_DIR			0	//ֱ�ӽ���
#define KEY_T_ARR			1	//����ɨ��







//External Functions
void key_Init(void);
uint_t key_Read(void);


#ifdef __cplusplus
}
#endif

#endif

