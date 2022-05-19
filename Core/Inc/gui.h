/*
 * @Author: Tian Wei
 * @Date: 2022-05-19 14:00:38
 * @Description: 
 * @Version: V1.0
 */
#ifndef _GUI_H_
#define	_GUI_H_
#include "stdint.h"

typedef struct
{
	uint8_t Cur_Index;//当前索引项
	uint8_t previous;//上一页
	uint8_t next;//下一页
	uint8_t enter;//确认
	uint8_t back;//返回
	void (*current_operation)(uint8_t,uint8_t);//当前索引执行的函数(界面)
}Main_Menu;
//各界面的索引值
enum
{
	_Main_UI=0,
	_Pack_ID,//包序号
	_Length_Input,//输入长度
	_Thickness_Input,//输入厚度
	_Width_Input,//输入宽度
	_Wifi_Child,//计算结果
};

void GUI_Refresh(void);
void Main_UI(uint8_t page_index,uint8_t key_val);
#endif
