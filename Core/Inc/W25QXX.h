/*
程序是我理解数据手册和学习了大佬写的程序后自己造的轮子，如果觉得好用可以多多移植使用
功能：用于SPI方式读写华邦的W25QXX（W25Q80、W25Q16、W25Q32、W25Q64、W25Q128...）系列FLASH的数据。
开发环境：
	KEIL版本：V5.21a
	CUBEMX版本：V6.2.1
	CUBEMX针对STM32F407VET6的库版本：STM32Cube_FW_F4_V1.26.2
	系统版本：Win10
	FLASH型号：W25Q16
	STM32的SPI口：SPI1（CLK--PB3，MISO--PB4，MOSI--PB5，CS--PB0），如果需要移植，注意这几个IO口。
	W25QXX指令来源：华邦发布于2017年5月9日的数据手册“W25Q16JV”，Revision F版本
	
	其他flash信息：	华邦发布于2015年10月2日的数据手册“W25Q80DV/DL”，Revision H版本
					华邦发布于2017年5月9日的数据手册“W25Q16JV”，Revision F版本
					华邦发布于2017年5月11日的数据手册“W25Q32JV”，Revision F版本
					华邦发布于2017年5月11日的数据手册“W25Q64JV”，Revision H版本
					华邦发布于2016年11月16日的数据手册“W25Q128JV”，Revision C版本
					华邦发布于2017年8月3日的数据手册“W25Q256JV”，Revision G版本
					华邦发布于2019年6月25日的数据手册“W25Q512JV”，Revision B版本
*/

#ifndef __W25QXX_H
#define __W25QXX_H

//说明：这里没有直接引入main.h头文件，是因为我的函数可能和你们的设置不一样，直接使用main.h会很省事，但是可移植性不好。如果你
//移植这个程序的话，可以把下面的几个头文件替换为你的main.h，方便省事。


#include "stdint.h"		//包含这个函数是为了能够使用uint8_t、uint16_t等类似的数据类型定义，不包含自己定义也可以
#include "gpio.h"		//包含这个是为了引入定义的GPIO
#include "spi.h"		//因为是用SPI方式读写FLASH，发指令要用到SPI函数，所以包含上这个头文件

//如果要使用printf函数打印数据，需要在usart.c中添加下面这段代码，在uart.h中包含"stdio.h"头文件，在uart.c里包含这个头文件，在其他地方使用会报警告。
//#include "stdio.h"
//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */

//  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
//  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

//  return ch;
//}



/******************************************************W25QXX操作指令******************************************************/
#define Write_Enable				0x06
#define Volatile_SR_Write_Enable	0x50
#define Write_Disable				0x04
#define Release_Power_down			0xAB
#define Manufacturer				0x90
#define JEDEC_ID					0x9F
#define Read_Unique_ID				0x4B
#define Read_Data					0x03
#define Fast_Read					0x0B
#define Page_Program				0x02
#define Sector_Erase				0x20		//(4KB)
#define Block_Erase					0x52		//(32KB)
#define Block_Erase1				0xD8		//(64KB)
#define Chip_Erase					0xC7		//或0x60
#define Read_Status_Register1		0x05
#define Write_Status_Register1		0x01
#define Read_Status_Register2		0x35
#define Write_Status_Register2		0x31
#define Read_Status_Register3		0x15
#define Write_Status_Register3		0x11
#define Read_SFDP_Register			0x5A
#define Erase_Security_Register		0x44
#define Program_Security_Register	0x42
#define Read_Security_Register		0x48
#define Global_Block_Lock			0x7E
#define Global_Block_Unlock			0x98
#define Read_Block_Lock				0x3D
#define Individual_Block_Lock		0x36
#define Individual_Block_Unlock		0x39
#define Erase_Program_Suspend		0x75
#define Erase_Program_Resume		0x7A
#define Power_down					0xB9
#define Enable_Reset				0x66
#define Reset_Device				0x99
#define Fast_Read_Dual_Output		0x3B
#define Fast_Read_Dual_IO			0xBB
#define Device_ID_Dual_IO			0x92
#define Quad_Input_Page_Program		0x32
#define Fast_Read_Quad_Output		0x6B
#define Device_ID_Quad_IO			0x94
#define Fast_Read_Quad_IO			0xEB
#define Set_Burst_with_Wrap			0x77
/***************************************************************************************************************************/

/******************************************************W25QXX通用设置******************************************************/
/*
说明：W25QXX不管是哪一个型号，区别只是在于块儿的多少，其他的都是一致的。1MB=1024KB，1KB=1024Byte，1Byte=8bit
所有型号均满足（一下数据均来自相应的数据手册）：
	1块儿 = 16扇区
		1块儿 = 16*16*256字节（Byte）= 65536Byte = 64KB（65536Byte/1024=64KB）
	1扇区 = 16页
		1扇区 = 16*256（Byte）= 4096Byte = 4KB
	1页 = 256字节
	只能按扇区、块为单位擦除，或者是全片擦除。写可以1~256字节写，一次最多写256字节
不同型号flash块儿的区别：
	W25Q80：16块，共16*65536 = 1048576 Byte；1048576/1024/1024 = 1MB
	W25Q16：32块，共32*65536 = 2097152 Byte；2097152/1024/1024 = 2MB
	W25Q32:	64块，共64*65536 = 4194304 Byte；4194304/1024/1024 = 4MB
	W25Q64:	128块，共128*65536 = 8388608 Byte；8388608/1024/1024 = 8MB
	W25Q128:256块，共256*65536 = 16777216 Byte；1677216/1024/1024 = 16MB
	W25Q256:512块，共512*65536 = 33554432 Byte；33554432/1024/1024 = 32MB
	W25Q512:1024块，共1024*65536 = 67108864 Byte；67108864/1024/1024 = 64MB
*/

//定义块儿、扇区、页
#define NumByteOfPage		256	//一页有256字节
#define NumPageOfSector		16	//一个扇区有16页
#define NumSectorOfBlock	16	//一块有16个扇区

#define NumByteOfBlock		65536	//一块有65536字节
#define NumByteOfSector		4096	//一个扇区有4096字节

#define Num80BlockOfChip	16	//W25Q80有16块儿
#define Num16BlockOfChip	32	//W25Q16有32块儿
#define Num32BlockOfChip	64	//W25Q32有64块儿
#define Num64BlockOfChip	128	//W25Q64有128块儿
#define Num128BlockOfChip	256	//W25Q128有256块儿
#define Num256BlockOfChip	512	//W25Q256有512块儿
#define Num512BlockOfChip	1024//W25Q256有1024块儿

//定义FLASH芯片ID
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18
#define W25Q512 0XEF19

//设置CS片选脚;0--有效、1--无效；
#define CS_Enable()  	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
#define CS_Disable()  	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

extern uint8_t W25QXX_BUFFER[];	//定义一个扇区总共的字节数(4096字节)作为缓存数组

//定义功能函数
uint16_t	W25QXX_ReadID(void);  	    		//读取FLASH芯片的ID
uint8_t 	W25QXX_Init(void);					//FLASH初始化函数
void W25QXX_Write_Enable(void);  				//写使能 
void W25QXX_Write_Disable(void);				//写保护
uint8_t		W25QXX_ReadSR(uint8_t regno);   	//读取状态寄存器 
void W25QXX_WriteSR(uint8_t regno,uint8_t sr);	//写状态寄存器
void W25QXX_Wait_Busy(void);           			//等待空闲
void W25QXX_PowerDown(void);        			//进入掉电模式
void W25QXX_WAKEUP(void);						//唤醒

void W25QXX_Erase_Sector(uint32_t Dst_Addr);	//扇区擦除
void W25QXX_Erase_Block(uint32_t Dst_Addr); 	//块擦除
void W25QXX_Erase_Chip(void);    	  			//整片擦除

void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr,uint16_t NumByteToRead);			//读取flash数据
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);			//写入flash
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);	//按页写入数据
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);	//无校验写入数据



#endif


