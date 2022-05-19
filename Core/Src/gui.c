/*
 * @Author: Tian Wei
 * @Date: 2022-05-19 14:00:26
 * @Description:
 * @Version: V1.0
 */
#include "gui.h"
#include "key.h"
#include "oled.h"
#include "SDdriver.h"
#include <string.h>
__IO uint8_t is_main_menu = 1;
char c_Pack_ID[4] = {'0', '0', '0', '\0'};
char file_end[5] = {'.', 'c', 's', 'v', '\0'};
uint8_t Bit_num = 0;
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
static uint8_t func_index = _S_Pack_ID;					 //当前页面索引值
static uint8_t last_index = _S_Pack_ID;					 //上一个界面索引值
static void (*current_operation_func)(uint8_t, uint8_t); //定义一个函数指针
														 // 索引表
Main_Menu table[20] =
	{
		// Cur_Index,        previous,       next,            enter,          back,   (*current_operation)(u8,u8)
		{_S_Pack_ID, _S_Volume, _S_Length_Input, _Pack_ID, _S_Pack_ID, S_Pack_ID},
		{_S_Length_Input, _S_Pack_ID, _S_Thickness_Input, _Length_Input, _S_Pack_ID, S_Length_Input},
		{_S_Thickness_Input, _S_Length_Input, _S_Width_Input, _Thickness_Input, _S_Pack_ID, S_Thickness_Input},
		{_S_Width_Input, _S_Thickness_Input, _S_Volume, _S_Width_Input, _S_Pack_ID, S_Width_Input},
		{_S_Volume, _S_Width_Input, _S_Pack_ID, _S_Volume, _S_Pack_ID, S_Volume},
		//子菜单
		{_Pack_ID, _Pack_ID, _Pack_ID, _Pack_ID, _Pack_ID, Pack_ID},
};
/*
函数功能：刷新界面
参数：无
返回值：无
*/
void GUI_Refresh(void)
{
	uint8_t key_val = bsp_GetKey();
	if ((key_val != KEY_NONE) && (is_main_menu)) //只有按键按下且在主菜单模式才刷屏
	{
		last_index = func_index; //更新上一界面索引值
		switch (key_val)
		{
		case KEY_2_DOWN:
			beep();
			func_index = table[func_index].previous; //更新索引值 上
			break;
		case KEY_6_DOWN:
			beep();
			func_index = table[func_index].back; //更新索引值    右
			break;
		case KEY_8_DOWN:
			beep();
			func_index = table[func_index].next; //更新索引值     下
			break;
		case KEY_4_DOWN:
			beep();
			func_index = table[func_index].enter; //更新索引值     左
			break;
		}
	}
	current_operation_func = table[func_index].current_operation;
	(*current_operation_func)(func_index, key_val); //执行当前索引对应的函数
}
/*
函数功能：显示包输入
参数：u8 page_index,u8 key_val
返回值：无
*/
void S_Pack_ID(uint8_t page_index, uint8_t key_val)
{
	OLED_ShowString(16, 3, "1.", 16);
	/*输入包号*/
	OLED_ShowCHinese(32, 3, 6);
	OLED_ShowCHinese(48, 3, 7);
	OLED_ShowCHinese(64, 3, 0);
	OLED_ShowCHinese(80, 3, 1);
	/*确认 返回 */
	OLED_ShowCHinese(0, 6, 12);
	OLED_ShowCHinese(112, 6, 13);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
}
void S_Length_Input(uint8_t page_index, uint8_t key_val)
{
	OLED_ShowString(16, 3, "2.", 16);
	/*输入长度*/
	OLED_ShowCHinese(32, 3, 6);
	OLED_ShowCHinese(48, 3, 7);
	OLED_ShowCHinese(64, 3, 2);
	OLED_ShowCHinese(80, 3, 5);
	/*确认 返回 */
	OLED_ShowCHinese(0, 6, 12);
	OLED_ShowCHinese(112, 6, 13);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
}
void S_Thickness_Input(uint8_t page_index, uint8_t key_val)
{
	OLED_ShowString(16, 3, "3.", 16);
	/*输入厚度*/
	OLED_ShowCHinese(32, 3, 6);
	OLED_ShowCHinese(48, 3, 7);
	OLED_ShowCHinese(64, 3, 4);
	OLED_ShowCHinese(80, 3, 5);
	/*确认 返回 */
	OLED_ShowCHinese(0, 6, 12);
	OLED_ShowCHinese(112, 6, 13);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
}
void S_Width_Input(uint8_t page_index, uint8_t key_val)
{
	OLED_ShowString(16, 3, "4.", 16);
	/*输入宽度*/
	OLED_ShowCHinese(32, 3, 6);
	OLED_ShowCHinese(48, 3, 7);
	OLED_ShowCHinese(64, 3, 3);
	OLED_ShowCHinese(80, 3, 5);
	/*确认 返回 */
	OLED_ShowCHinese(0, 6, 12);
	OLED_ShowCHinese(112, 6, 13);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
}
void S_Volume(uint8_t page_index, uint8_t key_val)
{
	OLED_ShowString(16, 3, "5.", 16);
	/*计算体积*/
	OLED_ShowCHinese(32, 3, 14);
	OLED_ShowCHinese(48, 3, 15);
	OLED_ShowCHinese(64, 3, 10);
	OLED_ShowCHinese(80, 3, 11);
	/*确认 返回 */
	OLED_ShowCHinese(0, 6, 12);
	OLED_ShowCHinese(112, 6, 13);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
}
void Pack_ID(uint8_t page_index, uint8_t key_val)
{
	if (is_main_menu)
	{
		OLED_Clear();
		is_main_menu = 0;
		/*包号：0~999*/
		OLED_ShowCHinese(0, 0, 0);
		OLED_ShowCHinese(16, 0, 1);
		OLED_ShowString(32, 0, ": 0-999", 16);
		/*确认 返回 */
		OLED_ShowCHinese(0, 6, 12);
		OLED_ShowCHinese(16, 6, 16);
		OLED_ShowCHinese(112, 6, 13);
		OLED_ShowCHinese(96, 6, 17);
		OLED_ShowString(36, 4, "---", 16);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // F1
		{
			is_main_menu = 1;
			for (uint8_t i = 0; i < Bit_num; i++)
			{
				/* code */
			}
			char *File_name = (char *)malloc(sizeof(char) * (Bit_num + 5));
			char *title1 = (char *)malloc(sizeof(char) * (Bit_num+5));
			for (uint8_t i = 0; i < Bit_num; i++)
			{
				File_name[i] = c_Pack_ID[3 - Bit_num + i];
				title1[i] = c_Pack_ID[3 - Bit_num + i];
			}
			strcat(File_name, file_end);
			WritetoSD(File_name, title1, sizeof(title1));
			WritetoSD(File_name, "\nWidth,Longth,Thickness\n", sizeof(char)*25);
			OLED_Clear();
			func_index = page_index - 5;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, key_val); //执行当前索引对应的函数
			Bit_num = 0;
		}
		else if (key_val == KEY_13_DOWN)
		{
			c_Pack_ID[0] = ' ';
			c_Pack_ID[1] = ' ';
			c_Pack_ID[2] = ' ';
			OLED_ShowString(36, 3, c_Pack_ID, 16);
			Bit_num = 0;
		}
		else
		{
			if (Bit_num > 2)
			{
				Bit_num = 2;
			}
			if (Bit_num == 0)
			{
				c_Pack_ID[2] = (key_val - 1) / 3 + '0';
			}
			else if (Bit_num == 1)
			{
				c_Pack_ID[1] = c_Pack_ID[2];
				c_Pack_ID[2] = (key_val - 1) / 3 + '0';
			}
			else
			{
				c_Pack_ID[0] = c_Pack_ID[1];
				c_Pack_ID[1] = c_Pack_ID[2];
				c_Pack_ID[2] = (key_val - 1) / 3 + '0';
			}
			OLED_ShowString(36, 3, c_Pack_ID, 16);
			++Bit_num;
		}
	}
}
