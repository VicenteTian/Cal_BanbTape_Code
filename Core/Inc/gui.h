/*
 * @Author: Tian Wei
 * @Date: 2022-05-19 14:00:38
 * @Description:
 * @Version: V1.0
 */
#ifndef _GUI_H_
#define _GUI_H_
#include "stdint.h"

typedef struct
{
    uint8_t Cur_Index;                           //当前索引项
    uint8_t previous;                            //上一页
    uint8_t next;                                //下一页
    uint8_t enter;                               //确认
    uint8_t back;                                //返回
    void (*current_operation)(uint8_t, uint8_t); //当前索引执行的函数(界面)
} Main_Menu;
//各界面的索引值
typedef struct
{
    uint8_t ceng;             //层数
    float length;             //长度
    float width;              //宽度
    float thickness;          //厚度
    double all_width;         //总宽度
    uint8_t Pice_count;       //总片数
    float short_Pice_count; //短板数
    float short_length;       //短板长度
    double Volume;            //体积
    uint8_t All_Volume[20];   //总体积
} Package;
enum
{
    _S_Pack_ID = 0,     //包序号
    _S_Length_Input,    //输入长度
    _S_Thickness_Input, //输入厚度
    _S_Width_Input,     //输入宽度
    _S_Volume,          //计算结果,体积

    _Pack_ID,         //包序号
    _Length_Input,    //输入长度
    _Thickness_Input, //输入厚度
    _Width_Input,     //输入宽度
    _short_Input,     //短板输入
    _short_num_Input,//短板数量输入
    _Volume,          //计算结果
};

void GUI_Refresh(void);
void S_Pack_ID(uint8_t page_index, uint8_t key_val);
void S_Length_Input(uint8_t page_index, uint8_t key_val);
void S_Thickness_Input(uint8_t page_index, uint8_t key_val);
void S_Width_Input(uint8_t page_index, uint8_t key_val);
void S_Volume(uint8_t page_index, uint8_t key_val);
void Pack_ID(uint8_t page_index, uint8_t key_val);
void Length_Input(uint8_t page_index, uint8_t key_val);
void Thickness_Input(uint8_t page_index, uint8_t key_val);
void Width_Input(uint8_t page_index, uint8_t key_val);
void short_Input(uint8_t page_index, uint8_t key_val);
void short_num_Input(uint8_t page_index, uint8_t key_val);
void Volume_show(uint8_t page_index, uint8_t key_val);
void string_input(uint8_t string_buff[], uint8_t char_input, uint8_t n);

void show_frame(void);
void data_input(uint8_t key_val, uint8_t max_bit_num);
void data_del(void);
#endif
