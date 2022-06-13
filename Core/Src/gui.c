/*
 * @Author: Tian Wei
 * @Date: 2022-05-19 14:00:26
 * @Description:
 * @Version: V1.0
 */
#include "gui.h"
#include "key.h"
#include "oled.h"
#include "W25QXX.h"
#include <string.h>
__IO uint8_t is_main_menu = 1;
__IO uint8_t is_show= 0;
Package My_Pack = {"", 0.0, 0.0, 0.0, 0.0, 0, ""};
char c_Pack_ID[4] = {' ', ' ', ' ', '\0'};
char input_buff[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ',', '\0'};
uint8_t Bit_num = 0;
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
		{_S_Width_Input, _S_Thickness_Input, _S_Volume, _Width_Input, _S_Pack_ID, S_Width_Input},
		{_S_Volume, _S_Width_Input, _S_Pack_ID, _Volume, _S_Pack_ID, S_Volume},
		//子菜单
		{_Pack_ID, _Pack_ID, _Pack_ID, _Pack_ID, _Pack_ID, Pack_ID},
		{_Length_Input, _Length_Input, _Length_Input, _Length_Input, _Length_Input, Length_Input},
		{_Thickness_Input, _Thickness_Input, _Thickness_Input, _Thickness_Input, _Thickness_Input, Thickness_Input},
		{_Width_Input, _S_Width_Input, _S_Width_Input, _S_Width_Input, _S_Width_Input, Width_Input},
		{_Volume, _Volume, _Volume, _S_Volume, _S_Pack_ID, Volume_show},
};
/*
函数功能：刷新界面
参数：无
返回值：无
*/
void GUI_Refresh(void)
{
	uint8_t key_val = bsp_GetKey();
	
	if ((key_val != KEY_NONE) && (is_main_menu)&&(((key_val - 1) % 3) == 0)) //只有按键按下且在主菜单模式才刷屏
	{
		beep();	
		
		OLED_ShowNum(2,0, (key_val - 1) / 3, 3, 8);
		is_show=0;
		last_index = func_index; //更新上一界面索引值
		switch (key_val)
		{
		case KEY_13_DOWN:
			WritetoSD("test.txt", "12345\n", 6);
			func_index = table[func_index].previous; //更新索引值 上
			break;
		case KEY_14_DOWN:

			func_index = table[func_index].back; //更新索引值    右
			break;
		case KEY_15_DOWN:
			
			func_index = table[func_index].next; //更新索引值     下
			break;
		case KEY_12_DOWN:
			
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
	if(is_show==0)
	{
	OLED_ShowString(16, 3, "1.", 16);
	/*输入包号*/
	OLED_ShowCHinese(32, 3, 6);
	OLED_ShowCHinese(48, 3, 7);
	OLED_ShowCHinese(64, 3, 0);
	OLED_ShowCHinese(80, 3, 1);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
		is_show=1;
	}
}
void S_Length_Input(uint8_t page_index, uint8_t key_val)
{
		if(is_show==0)
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
	OLED_ShowNum(16, 6, 4, 1, 16);
	OLED_ShowNum(104, 6, 6, 1, 16);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
				is_show=1;
	}
}
void S_Thickness_Input(uint8_t page_index, uint8_t key_val)
{
		if(is_show==0)
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
	OLED_ShowNum(16, 6, 4, 1, 16);
	OLED_ShowNum(104, 6, 6, 1, 16);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
				is_show=1;
	}
}
void S_Width_Input(uint8_t page_index, uint8_t key_val)
{
		if(is_show==0)
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
	OLED_ShowNum(16, 6, 4, 1, 16);
	OLED_ShowNum(104, 6, 6, 1, 16);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
				is_show=1;
	}
}
void S_Volume(uint8_t page_index, uint8_t key_val)
{
		if(is_show==0)
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
	OLED_ShowNum(16, 6, 4, 1, 16);
	OLED_ShowNum(104, 6, 6, 1, 16);
	/*上下翻页*/
	OLED_DrawBMP(60, 0, 68, 1, up);
	OLED_DrawBMP(60, 7, 68, 8, down);
				is_show=1;
	}
}
void Pack_ID(uint8_t page_index, uint8_t key_val)
{
	if (is_main_menu)
	{
		key_val = KEY_NONE;
		bsp_GetKey();
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
		string_input(My_Pack.Pack_File_Name, '\0', 8);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // F1
		{
			if (Bit_num > 0)
			{
				memset(&My_Pack, 0, sizeof(My_Pack));
				is_main_menu = 1;
				char *File_name = (char *)malloc(sizeof(char) * (Bit_num + 5));
				char *title1 = (char *)malloc(sizeof(char) * (Bit_num + 5));
				for (uint8_t i = 0; i < Bit_num; i++)
				{
					File_name[i] = c_Pack_ID[3 - Bit_num + i];
					title1[i] = c_Pack_ID[3 - Bit_num + i];
				}
				char file_end[5] = {'.', 'C', 'S', 'V', '\0'};
				//strcat(File_name, file_end);
				//strcpy(My_Pack.Pack_File_Name, File_name);
				//WritetoSD(File_name, "pack\n", sizeof(c_Pack_ID));
			//	WritetoSD(File_name, c_Pack_ID, sizeof(c_Pack_ID));
				//WritetoSD(File_name, "\nLongth,Thickness,Width\n", sizeof(char) * 25);
				free(File_name);
				OLED_Clear();
				My_Pack.Pice_count = 0; //片数清0
				func_index = _Length_Input;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN)
		{
			//string_input(c_Pack_ID, ' ', 3);
			OLED_ShowString(36, 3, c_Pack_ID, 16);
			Bit_num = 0;
		}
		else if (key_val == KEY_11_DOWN) //#
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _S_Pack_ID;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
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
void Length_Input(uint8_t page_index, uint8_t key_val)
{
	if (is_main_menu)
	{
		is_main_menu = 0;
		OLED_Clear();
		show_frame();
		/*长度：*/
		OLED_ShowCHinese(0, 4, 2);
		OLED_ShowCHinese(16, 4, 5);
		//string_input(input_buff, ' ', 7);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // F1
		{
			My_Pack.length = atof(input_buff);
			if (My_Pack.length > 0)
			{
				is_main_menu = 1;
				OLED_Clear();
				func_index = _Thickness_Input;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // F2
		{
			//string_input(input_buff, ' ', 7);
			OLED_ShowString(40, 4, input_buff, 16);
			Bit_num = 0;
		}
		else if (key_val == KEY_11_DOWN) //#
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _S_Length_Input;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else
		{
			data_input(key_val);
		}
	}
}
void Thickness_Input(uint8_t page_index, uint8_t key_val)
{
	if (is_main_menu)
	{
		is_main_menu = 0;
		OLED_Clear();
		show_frame();
		/*厚度：*/
		OLED_ShowCHinese(0, 4, 4);
		OLED_ShowCHinese(16, 4, 5);
		string_input(input_buff, ' ', 7);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // F1
		{
			My_Pack.thickness = atof(input_buff);
			if (My_Pack.thickness > 0)
			{
				is_main_menu = 1;
				OLED_Clear();
				func_index = _Width_Input;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // F2
		{
			string_input(input_buff, ' ', 7);
			OLED_ShowString(40, 4, input_buff, 16);
			Bit_num = 0;
		}
		else if (key_val == KEY_11_DOWN) //#
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _S_Thickness_Input;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else
		{
			data_input(key_val);
		}
	}
}
void Width_Input(uint8_t page_index, uint8_t key_val)
{
	uint8_t temp[9] = {0};
	if (is_main_menu)
	{
		is_main_menu = 0;
		OLED_Clear();
		show_frame();
		/*宽度：*/
		OLED_ShowCHinese(0, 4, 3);
		OLED_ShowCHinese(16, 4, 5);
		string_input(input_buff, ' ', 7);
		sprintf(temp, "%.2f", My_Pack.length); //保留小数点后3位小数，打印到Data数组中
		OLED_ShowString(0, 1, "L:", 8);
		OLED_ShowString(16, 1, temp, 8);
		string_input(temp, '\0', 9);
		sprintf(temp, "%.2f", My_Pack.thickness);
		OLED_ShowString(0, 2, "H:", 8);
		OLED_ShowString(16, 2, temp, 8);
		string_input(temp, '\0', 9);
		sprintf(temp, "%.2f", My_Pack.width);
		OLED_ShowString(0, 3, "W:", 8);
		OLED_ShowString(16, 3, temp, 8);
		OLED_ShowString(56, 7, "C:", 8);
		OLED_ShowNum(72, 7, My_Pack.Pice_count, 3, 8);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // F1
		{
			My_Pack.width = atof(input_buff);
			if (My_Pack.width > 0)
			{
				My_Pack.Volume += My_Pack.length * My_Pack.thickness * My_Pack.width;

				string_input(temp, '\0', 9);
				temp[7] = ',';
				sprintf(temp, "%.3f", My_Pack.length);
			//	WritetoSD(My_Pack.Pack_File_Name, temp, sizeof(temp));

				string_input(temp, '\0', 9);
				temp[7] = ',';
				sprintf(temp, "%.3f", My_Pack.thickness);
				//WritetoSD(My_Pack.Pack_File_Name, temp, sizeof(temp));

				//WritetoSD(My_Pack.Pack_File_Name, input_buff, sizeof(input_buff));
				//WritetoSD(My_Pack.Pack_File_Name, "\n", sizeof(char) * 2);

				++My_Pack.Pice_count;
				OLED_ShowNum(72, 7, My_Pack.Pice_count, 3, 8);

				string_input(input_buff, '\0', 7);
				sprintf(input_buff, "%.2f", My_Pack.width);
				OLED_ShowString(16, 3, input_buff, 8);

				string_input(input_buff, ' ', 7);
				OLED_ShowString(40, 4, input_buff, 16);
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // F2
		{
			string_input(input_buff, ' ', 7);
			OLED_ShowString(40, 4, input_buff, 16);
			Bit_num = 0;
		}
		else if (key_val == KEY_11_DOWN) //#
		{
			is_main_menu = 1;
			sprintf(My_Pack.All_Volume, "%.3f", My_Pack.Volume); //保留小数点后3位小数，打印到Data数组中
			//WritetoSD(My_Pack.Pack_File_Name, My_Pack.All_Volume, sizeof(My_Pack.All_Volume));
			OLED_Clear();
			func_index = _Volume;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else
		{
			data_input(key_val);
		}
	}
}
void Volume_show(uint8_t page_index, uint8_t key_val)
{
	if (is_main_menu)
	{
		OLED_Clear();
		show_frame();
		/*体积：*/
		OLED_ShowCHinese(0, 4, 10);
		OLED_ShowCHinese(16, 4, 11);
		OLED_ShowCHinese(110, 4, 18);
		OLED_ShowString(40, 4, My_Pack.All_Volume, 16);
		is_main_menu = 0;
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		is_main_menu = 1;
		if (key_val == KEY_13_DOWN)
		{
			OLED_Clear();
			func_index = _S_Volume;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else if (key_val == KEY_12_DOWN)
		{
			OLED_Clear();
			func_index = _Pack_ID;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
	}
}
void string_input(uint8_t string_buff[], uint8_t char_input, uint8_t n)
{
	for (uint8_t i = 0; i < n; i++)
	{
		string_buff[i] = char_input;
	}
}
void show_frame(void)
{
	OLED_ShowString(0, 0, My_Pack.Pack_File_Name, 8);
	OLED_ShowString(32, 4, ":", 16);
	/*确认 返回 */
	OLED_ShowCHinese(0, 6, 12);
	OLED_ShowCHinese(16, 6, 16);
	OLED_ShowCHinese(112, 6, 13);
	OLED_ShowCHinese(96, 6, 17);
}
void data_input(uint8_t key_val)
{
	if (Bit_num > 6)
	{
		Bit_num = 6;
	}
	if (key_val == KEY_10_DOWN)
	{
		input_buff[Bit_num] = '.';
	}
	else
	{
		input_buff[Bit_num] = (key_val - 1) / 3 + '0';
	}

	OLED_ShowString(48, 4, input_buff, 16);
	++Bit_num;
}
