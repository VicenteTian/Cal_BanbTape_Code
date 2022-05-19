/*
 * @Author: Tian Wei
 * @Date: 2022-05-19 14:00:26
 * @Description:
 * @Version: V1.0
 */
#include "gui.h"
#include "key.h"
#include "oled.h"
const unsigned char Enter[] =
	{0xFF, 0x01, 0x41, 0xC1, 0x81, 0x01, 0x01, 0x01, 0x01, 0x81, 0xC1, 0x61, 0x39, 0x19, 0x01, 0xFF,
	 0xFF, 0x80, 0x80, 0x80, 0x81, 0x83, 0x86, 0x86, 0x83, 0x83, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF};
const unsigned char back[] =
	{0xFF, 0x01, 0x01, 0x31, 0x79, 0xFD, 0x31, 0x31, 0x31, 0x31, 0x31, 0xF1, 0xF1, 0x01, 0x01, 0xFF,
	 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8F, 0x8F, 0x80, 0x80, 0xFF};
const unsigned char up[] =
	{0x08, 0x0C, 0x0E, 0x0F, 0x0F, 0x0E, 0x0C, 0x08};
const unsigned char down[] =
	{0x01, 0x03, 0x07, 0x0F, 0x0F, 0x07, 0x03, 0x01};
static uint8_t func_index = _Main_UI;					 //当前页面索引值
static uint8_t last_index = _Main_UI;					 //上一个界面索引值
static void (*current_operation_func)(uint8_t, uint8_t); //定义一个函数指针
														 // 索引表
Main_Menu table[20] =
	{
		// Cur_Index,previous,next,enter,back,(*current_operation)(u8,u8)
		{_Main_UI, _Main_UI, _Main_UI, _Pack_ID, _Pack_ID, Main_UI},
};
/*
函数功能：刷新界面
参数：无
返回值：无
*/
void GUI_Refresh(void)
{
	uint8_t key_val = bsp_GetKey();
	if (key_val != 0) //只有按键按下才刷屏
	{
		last_index = func_index; //更新上一界面索引值
		switch (key_val)
		{
		case KEY_2_DOWN:
			func_index = table[func_index].previous; //更新索引值 上
			break;
		case KEY_6_DOWN:
			func_index = table[func_index].enter; //更新索引值    右
			break;
		case KEY_8_DOWN:
			func_index = table[func_index].next; //更新索引值     下
			break;
		case KEY_4_DOWN:
			func_index = table[func_index].back; //更新索引值     左
			break;
		}
	}
	current_operation_func = table[func_index].current_operation;
	(*current_operation_func)(func_index, key_val); //执行当前索引对应的函数
}
/*
函数功能：显示主界面
参数：u8 page_index,u8 key_val
返回值：无
*/
void Main_UI(uint8_t page_index, uint8_t key_val)
{
	/*输入包号*/
	OLED_ShowCHinese(32, 3, 6);
	OLED_ShowCHinese(48, 3, 7);
	OLED_ShowCHinese(64, 3, 0);
	OLED_ShowCHinese(80, 3, 1);
	/*返回 确认*/
	OLED_ShowCHinese(0, 6, 12);
	OLED_ShowCHinese(112, 6, 13);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
}
