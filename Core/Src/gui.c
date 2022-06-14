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
__IO uint8_t is_show = 0;
Package My_Pack = {0, 0, 0, 0, 0, 0, 0, 0, 0, " "};
char c_Pack_ID[10] = {0};
char file_name[9] = {0};
__IO uint8_t file_name_bit = 0;
char input_buff[10] = {0};
uint8_t Bit_num = 0;
const unsigned char up[] =
	{0x08, 0x0C, 0x0E, 0x0F, 0x0F, 0x0E, 0x0C, 0x08};
const unsigned char down[] =
	{0x01, 0x03, 0x07, 0x0F, 0x0F, 0x07, 0x03, 0x01};
static uint8_t func_index = _S_Pack_ID;					 //当前页面索引值
static uint8_t last_index = _S_Pack_ID;					 //上一个界面索引值
static void (*current_operation_func)(uint8_t, uint8_t); //定义一个函数指针
														 // 索引表
Main_Menu table[12] =
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
		{_short_Input, _short_Input, _short_Input, _short_Input, _short_Input, short_Input},
		{_short_num_Input, _short_num_Input, _short_num_Input, _short_num_Input, _short_num_Input, short_num_Input},
		{_Volume, _Volume, _Volume, _S_Volume, _Volume, Volume_show},
};
/*
函数功能：刷新界面
参数：无
返回值：无
*/
void GUI_Refresh(void)
{
	uint8_t key_val = bsp_GetKey();

	if ((key_val != KEY_NONE) && (is_main_menu) && (((key_val - 1) % 3) == 0)) //只有按键按下且在主菜单模式才刷屏
	{
		beep();
		is_show = 0;
		last_index = func_index; //更新上一界面索引值
		switch (key_val)
		{
		case KEY_13_DOWN:
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
	if (is_show == 0)
	{
		OLED_ShowString(16, 3, "1.", 16);
		/*包号*/
		OLED_ShowCHinese(64, 3, 0);
		OLED_ShowCHinese(80, 3, 1);
		/*上下翻页*/
		OLED_DrawBMP(60, 0, 68, 1, up);
		OLED_DrawBMP(60, 7, 68, 8, down);
		is_show = 1;
	}
}
void S_Length_Input(uint8_t page_index, uint8_t key_val)
{
	if (is_show == 0)
	{
		OLED_ShowString(16, 3, "2.", 16);
		/*长度*/
		OLED_ShowCHinese(64, 3, 2);
		OLED_ShowCHinese(80, 3, 5);
		/*上下翻页*/
		OLED_DrawBMP(60, 0, 68, 1, up);
		OLED_DrawBMP(60, 7, 68, 8, down);
		is_show = 1;
	}
}
void S_Thickness_Input(uint8_t page_index, uint8_t key_val)
{
	if (is_show == 0)
	{
		OLED_ShowString(16, 3, "3.", 16);
		/*厚度*/
		OLED_ShowCHinese(64, 3, 4);
		OLED_ShowCHinese(80, 3, 5);
		/*上下翻页*/
		OLED_DrawBMP(60, 0, 68, 1, up);
		OLED_DrawBMP(60, 7, 68, 8, down);
		is_show = 1;
	}
}
void S_Width_Input(uint8_t page_index, uint8_t key_val)
{
	if (is_show == 0)
	{
		OLED_ShowString(16, 3, "4.", 16);
		/*宽度*/
		OLED_ShowCHinese(64, 3, 3);
		OLED_ShowCHinese(80, 3, 5);
		/*上下翻页*/
		OLED_DrawBMP(60, 0, 68, 1, up);
		OLED_DrawBMP(60, 7, 68, 8, down);
		is_show = 1;
	}
}
void S_Volume(uint8_t page_index, uint8_t key_val)
{
	if (is_show == 0)
	{
		OLED_ShowString(16, 3, "5.", 16);
		/*体积*/
		OLED_ShowCHinese(64, 3, 6);
		OLED_ShowCHinese(80, 3, 7);
		/*上下翻页*/
		OLED_DrawBMP(60, 0, 68, 1, up);
		OLED_DrawBMP(60, 7, 68, 8, down);
		is_show = 1;
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
		OLED_ShowCHinese(0, 4, 0);
		OLED_ShowCHinese(16, 4, 1);
		string_input(c_Pack_ID, '\0', 10);
		string_input(input_buff, '\0', 10);
		file_name_bit = 0;
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // F1确认
		{
			if (Bit_num > 0)
			{
				memset(&My_Pack, 0, sizeof(My_Pack));
				string_input(file_name, '\0', sizeof(file_name));
				uint8_t i = 0;
				while ((input_buff[i] != '\0') && (input_buff[i] != ' '))
				{
					c_Pack_ID[i] = input_buff[i];
					if (i < 4)
					{
						file_name[i] = input_buff[i];
						++file_name_bit;
					}
					++i;
				}
				// file_name[file_name_bit] = '-';
				//++file_name_bit;
				is_main_menu = 1;
				OLED_Clear();
				func_index = _Length_Input;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
			else
			{
				is_main_menu = 1;
				OLED_Clear();
				func_index = _S_Pack_ID;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
			}
		}
		else if (key_val == KEY_13_DOWN) //清除
		{
			data_del();
		}
		else if (key_val == KEY_14_DOWN) //返回
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _S_Pack_ID;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else //普通数字输入
		{
			data_input(key_val, 9);
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
		OLED_ShowString(112, 5, "ft", 8);
		OLED_ShowString(0, 0, file_name, 8);
		string_input(input_buff, '\0', 10);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) //确认
		{
			My_Pack.length = atof(input_buff);
			if (My_Pack.length > 0)
			{
				/* 				uint8_t i = 0;
								while ((input_buff[i] != '\0') && (input_buff[i] != '.') && (input_buff[i] != ' '))
								{
									file_name[i + file_name_bit] = input_buff[i];
									++i;
								}
								file_name_bit += i;
								file_name[file_name_bit] = '-';
								++file_name_bit; */
				is_main_menu = 1;
				OLED_Clear();
				func_index = _Thickness_Input;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // 清除
		{
			data_del();
		}
		else if (key_val == KEY_14_DOWN) //返回
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _S_Length_Input;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else
		{
			data_input(key_val, 6);
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
		OLED_ShowString(96, 5, "inch", 8);
		OLED_ShowString(0, 0, file_name, 8);
		string_input(input_buff, '\0', 10);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // 确认
		{
			My_Pack.thickness = atof(input_buff);
			if (My_Pack.thickness > 0)
			{
				/*				uint8_t i = 0;
								while ((input_buff[i] != '\0') && (input_buff[i] != '.') && (input_buff[i] != ' '))
								{
									file_name[i + file_name_bit] = input_buff[i];
									++i;
								}
								file_name_bit += i; */
				is_main_menu = 1;
				file_name[file_name_bit] = '.';
				++file_name_bit;
				file_name[file_name_bit] = 'C';
				++file_name_bit;
				file_name[file_name_bit] = 'S';
				++file_name_bit;
				file_name[file_name_bit] = 'V';
				OLED_ShowString(0, 0, file_name, 8);
				WritetoSD(file_name, "包号,长度,厚度\n", 21);
				WritetoSD(file_name, c_Pack_ID, 10);
				WritetoSD(file_name, ",", 2);
				func_index = _Width_Input;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // 清除
		{
			data_del();
		}
		else if (key_val == KEY_14_DOWN) //返回
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _S_Thickness_Input;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else
		{
			data_input(key_val, 6);
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
		OLED_ShowString(0, 0, file_name, 8);
		/*宽度：*/
		OLED_ShowCHinese(0, 4, 3);
		OLED_ShowCHinese(16, 4, 5);
		string_input(input_buff, '\0', sizeof(input_buff));
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
		OLED_ShowString(10, 7, "C:", 8);
		OLED_ShowNum(26, 7, My_Pack.ceng, 3, 8);
		OLED_ShowString(96, 7, "P:", 8);
		OLED_ShowNum(112, 7, My_Pack.Pice_count, 3, 8);
		OLED_ShowString(96, 3, "inch", 8);
		string_input(temp, '\0', 9);
		temp[7] = ',';
		sprintf(temp, "%.3f", My_Pack.length);
		WritetoSD(file_name, "temp", 5);
		string_input(temp, '\0', 9);
		temp[7] = '\n';
		sprintf(temp, "%.3f", My_Pack.thickness);
		WritetoSD(file_name, temp, sizeof(temp));
		WritetoSD(file_name, "宽度\n", 8);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // F1
		{
			My_Pack.width = atof(input_buff);
			if (My_Pack.width > 0)
			{
				My_Pack.all_width += My_Pack.width;
				++My_Pack.Pice_count;
				OLED_ShowNum(112, 7, My_Pack.Pice_count, 3, 8);

				WritetoSD(file_name, input_buff, sizeof(input_buff));
				WritetoSD(file_name, ",", 2);

				string_input(input_buff, '\0', 9);
				sprintf(input_buff, "%.2f", My_Pack.width);
				OLED_ShowString(16, 3, input_buff, 8);

				string_input(input_buff, ' ', 7);
				OLED_ShowString(48, 4, input_buff, 16);
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // 清除
		{
			data_del();
		}
		else if (key_val == KEY_15_DOWN) //回车
		{
			My_Pack.ceng++;
			OLED_ShowNum(26, 7, My_Pack.ceng, 3, 8);
			WritetoSD(file_name, "\n", 2);
		}
		else if (key_val == KEY_11_DOWN) //=
		{
			is_main_menu = 1;
			WritetoSD(file_name, "\n总片数,", 12);
			string_input(temp, '\n', 9);
			// sprintf(temp, "%d", My_Pack.Pice_count);
			WritetoSD(file_name, temp, sizeof(temp));
			WritetoSD(file_name, ",总宽度,", 12);
			string_input(temp, '\n', 9);
			sprintf(temp, "%.3f", My_Pack.all_width); //保留小数点后3位小数，打印到数组中
			WritetoSD(file_name, temp, sizeof(temp));
			// sprintf(My_Pack.All_Volume, "%.3f", My_Pack.Volume); //保留小数点后3位小数，打印到Data数组中
			//  WritetoSD(My_Pack.Pack_File_Name, My_Pack.All_Volume, sizeof(My_Pack.All_Volume));
			OLED_Clear();
			func_index = _short_Input;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
			Bit_num = 0;
		}
		else
		{
			data_input(key_val, 6);
		}
	}
}
void short_Input(uint8_t page_index, uint8_t key_val)
{
	if (is_main_menu)
	{
		is_main_menu = 0;
		OLED_Clear();
		/*短板：*/
		// OLED_ShowString(2, 0, file_name, 8);
		WritetoSD(file_name, "\n短板长度,短板数量\n", 28);
		string_input(input_buff, '\0', 10);
		OLED_ShowString(0, 0, "short length:", 8);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // 确认
		{
			My_Pack.short_length = atof(input_buff);
			if (My_Pack.short_length > 0)
			{
				is_main_menu = 1;
				//WritetoSD(file_name, input_buff, sizeof(input_buff));
				OLED_Clear();
				func_index = _short_num_Input;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // 清除
		{
			data_del();
		}
		else if (key_val == KEY_14_DOWN) //返回
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _Width_Input;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else
		{
			data_input(key_val, 6);
		}
	}
}
void short_num_Input(uint8_t page_index, uint8_t key_val)
{
	if (is_main_menu)
	{
		is_main_menu = 0;
		OLED_Clear();
		/*短板：*/
		// OLED_ShowString(2, 0, file_name, 8);
		string_input(input_buff, '\0', 10);
		// WritetoSD(file_name, ",", 2);
		OLED_ShowString(0, 0, "short num:", 8);
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		if (key_val == KEY_12_DOWN) // 
		{
			OLED_ShowString(0, 4, "in   424:", 8);
			My_Pack.short_Pice_count = atof(input_buff);
			if (My_Pack.short_Pice_count > 0)
			{
				is_main_menu = 1;
				WritetoSD(file_name, input_buff, sizeof(input_buff));
				WritetoSD(file_name, "\n总体积,", 12);
				OLED_Clear();
				func_index = _Volume;
				current_operation_func = table[func_index].current_operation;
				(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
				Bit_num = 0;
			}
		}
		else if (key_val == KEY_13_DOWN) // 清除
		{
			data_del();
		}
		else if (key_val == KEY_14_DOWN) //返回
		{
			is_main_menu = 1;
			OLED_Clear();
			func_index = _short_Input;
			current_operation_func = table[func_index].current_operation;
			(*current_operation_func)(func_index, KEY_NONE); //执行当前索引对应的函数
		}
		else
		{
			data_input(key_val, 6);
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
		OLED_ShowCHinese(0, 4, 6);
		OLED_ShowCHinese(16, 4, 7);
		OLED_ShowString(40, 4, My_Pack.All_Volume, 16);
		is_main_menu = 0;
	}
	if ((key_val != KEY_NONE) && (((key_val - 1) % 3) == 0))
	{
		beep();
		is_main_menu = 1;
		if (key_val == KEY_14_DOWN)
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
	// OLED_ShowString(0, 0, My_Pack.Pack_File_Name, 8);
	OLED_ShowString(32, 4, ":", 16);
}
void data_input(uint8_t key_val, uint8_t max_bit_num)
{
	if (Bit_num >= max_bit_num)
	{
		Bit_num = max_bit_num - 1;
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
void data_del(void)
{
	if (Bit_num == 0)
	{
		Bit_num = 1;
	}
	Bit_num--;
	input_buff[Bit_num] = ' ';
	OLED_ShowString(48, 4, input_buff, 16);
}
