#include "W25QXX.h"
#include "oled.h"
#include "ff.h"
uint32_t FLASH_SIZE = 0;				// FLASH有多少字节，根据不同的FLASH芯片来计算得到，单位Byte
uint8_t W25QXX_BUFFER[NumByteOfSector]; //定义一个扇区总共的字节数(4096字节)作为缓存数组

/*读取芯片ID
传入参数：无
返回参数:
	0XEF13,表示芯片型号为W25Q80
	0XEF14,表示芯片型号为W25Q16
	0XEF15,表示芯片型号为W25Q32
	0XEF16,表示芯片型号为W25Q64
	0XEF17,表示芯片型号为W25Q128
	0XEF18,表示芯片型号为W25Q256
*/
uint16_t W25QXX_ReadID(void)
{
	uint16_t temp = 0;
	uint8_t TX_cmd[6], RX_temp[6];
	TX_cmd[0] = Manufacturer;

	CS_Enable();
	//发送指令和接收简单数据推荐使用查询模式
	HAL_SPI_TransmitReceive(&hspi1, TX_cmd, RX_temp, 6, 10); //发送读取ID命令
	temp = RX_temp[4] << 8 | RX_temp[5];
	CS_Disable();
	return temp; //返回FLASH的ID号
}

/*
初始化FLASH，包括读取FLASH 的ID
返回值:
*/
uint8_t W25QXX_Init(void)
{
	uint16_t ReadFlah_Type; //定义实际读取的FLASH型号
	uint8_t Flag = 0;		//标记初始话flash状态；0--失败，1--成功
	CS_Disable();			// SPI FLASH不选中

	ReadFlah_Type = W25QXX_ReadID(); //读取FLASH的ID，确认FLASH型号
	// printf("FLASH_ID:0x%x\r\n",ReadFlah_Type);	//打印flash的芯片ID,测试用
	switch (ReadFlah_Type)
	{
	case 0xEF13: // W25Q80
	{
		FLASH_SIZE = Num80BlockOfChip * NumByteOfBlock; //块*65536 =字节数
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q80\r\n",22,0xff);
		//打印flash芯片信息
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num80BlockOfChip,Num80BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH 初始化成功
		break;
	}
	case 0xEF14: // W25Q16
	{
		FLASH_SIZE = Num16BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q16\r\n",22,0xff);
		//打印flash芯片信息
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num16BlockOfChip,Num16BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH 初始化成功
		break;
	}
	case 0xEF15: // W25Q32
	{
		FLASH_SIZE = Num32BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q32\r\n",22,0xff);
		//打印flash芯片信息
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num32BlockOfChip,Num32BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH 初始化成功
		break;
	}
	case 0xEF16: // W25Q64
	{
		FLASH_SIZE = Num64BlockOfChip * NumByteOfBlock;
		// OLED_ShowString(0,0,(uint8_t*)"W25Q64",16);
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q64\r\n",22,0xff);
		//打印flash芯片信息
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num64BlockOfChip,Num64BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH 初始化成功
		break;
	}
	case 0xEF17: // W25Q128
	{
		FLASH_SIZE = Num128BlockOfChip * NumByteOfBlock;
		//	HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q128\r\n",23,0xff);
		//打印flash芯片信息
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num128BlockOfChip,Num128BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH 初始化成功
		break;
	}
	case 0xEF18: // W25Q256
	{
		FLASH_SIZE = Num256BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q256\r\n",23,0xff);
		//打印flash芯片信息
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num256BlockOfChip,Num256BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH 初始化成功
		break;
	}
	case 0xEF19: // W25Q512
	{
		FLASH_SIZE = Num256BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q512\r\n",23,0xff);
		//打印flash芯片信息
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num512BlockOfChip,Num512BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH 初始化成功
		break;
	}
	default:
	{
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Other FLASH_ID,Flash init error\r\n",33,0xff);
		Flag = 0; // FLASH 初始化失败
		break;
	}
	}
	return Flag;
}

// W25QXX写使能
//将WEL置位
void W25QXX_Write_Enable(void)
{
	uint8_t cmd = Write_Enable;
	CS_Enable();						   //使能器件
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); //发送写使能
	CS_Disable();						   //取消片选
}

// W25QXX写禁止
//将WEL清零
void W25QXX_Write_Disable(void)
{
	uint8_t cmd = Write_Disable;
	CS_Enable();						   //使能器件
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); //发送写禁止指令
	CS_Disable();						   //取消片选
}

//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
// BIT7  6   5   4   3   2   1   0
// SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// SPR:默认0,状态寄存器保护位,配合WP使用
// TB,BP2,BP1,BP0:FLASH区域写保护设置
// WEL:写使能锁定
// BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
// BIT7  6   5   4   3   2   1   0
// SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
// BIT7      6    5    4   3   2   1   0
// HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
// regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
uint8_t W25QXX_ReadSR(uint8_t regno)
{
	uint8_t TX_cmd[2];
	uint8_t RX_temp[2];
	switch (regno)
	{
	case 1:
	{
		TX_cmd[0] = Read_Status_Register1; //读状态寄存器1指令
		break;
	}
	case 2:
	{
		TX_cmd[0] = Read_Status_Register2; //读状态寄存器2指令
		break;
	}
	case 3:
	{
		TX_cmd[0] = Read_Status_Register3; //读状态寄存器3指令
		break;
	}
	default:
	{
		TX_cmd[0] = Read_Status_Register1;
		break;
	}
	}
	CS_Enable();
	// Poll mode
	//发送指令和接收简单数据推荐使用查询模式
	HAL_SPI_TransmitReceive(&hspi1, TX_cmd, RX_temp, 2, 10); //发送读状态寄存器命令
	CS_Disable();											 //取消FLASH片选
	return RX_temp[1];
}

//写W25QXX状态寄存器
void W25QXX_WriteSR(uint8_t regno, uint8_t sr)
{
	uint8_t TX_cmd[2];
	switch (regno)
	{
	case 1:
	{
		TX_cmd[0] = Write_Status_Register1; //写状态寄存器1指令
		break;
	}
	case 2:
	{
		TX_cmd[0] = Write_Status_Register2; //写状态寄存器2指令
		break;
	}
	case 3:
	{
		TX_cmd[0] = Write_Status_Register3; //写状态寄存器3指令
		break;
	}
	default:
	{
		TX_cmd[0] = Write_Status_Register1;
		break;
	}
	}
	TX_cmd[1] = sr;
	CS_Enable(); //使能器件
	// Poll mode
	//发送指令和接收简单数据推荐使用查询模式
	HAL_SPI_Transmit(&hspi1, TX_cmd, 2, 10); //发送读状态寄存器命令
	CS_Disable();							 //取消片选
}

//等待空闲
void W25QXX_Wait_Busy(void)
{
	while ((W25QXX_ReadSR(1) & 0x01) == 0x01)
		; // 等待BUSY位清空
}

//进入掉电模式
void W25QXX_PowerDown(void)
{
	uint8_t cmd = Power_down; //发送指令设置为掉电

	CS_Enable();						   //使能器件
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); //发送掉电命令
	CS_Disable();						   //取消片选

	HAL_Delay(1); //等待TPD
}

//唤醒
void W25QXX_WAKEUP(void)
{
	uint8_t cmd = Release_Power_down;	   //设置唤醒指令
	CS_Enable();						   //使能器件
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); // send W25X_PowerDown command 0xAB
	CS_Disable();						   //取消片选
	HAL_Delay(1);						   //等待TRES1
}

//擦除一个扇区
// Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:45ms - 400ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
	uint8_t cmd[5];
	cmd[0] = Sector_Erase;
	cmd[1] = (uint8_t)((Dst_Addr * NumByteOfSector) >> 16);
	cmd[2] = (uint8_t)((Dst_Addr * NumByteOfSector) >> 8);
	cmd[3] = (uint8_t)(Dst_Addr * NumByteOfSector);

	W25QXX_Write_Enable();					// SET WEL
	W25QXX_Wait_Busy();						//等待空闲
	CS_Enable();							//使能器件
	HAL_SPI_Transmit(&hspi1, cmd, 4, 4000); //发送擦除扇区指令+地址
	CS_Disable();							//取消片选
	W25QXX_Wait_Busy();						//等待擦除完成
}

//擦除一个块
// Dst_Addr:块地址 根据实际容量设置
//擦除一个块的最少时间:150ms - 2000ms
void W25QXX_Erase_Block(uint32_t Dst_Addr)
{
	uint8_t cmd[5];
	cmd[0] = Block_Erase1;
	cmd[1] = (uint8_t)((Dst_Addr * NumByteOfBlock) >> 16);
	cmd[2] = (uint8_t)((Dst_Addr * NumByteOfBlock) >> 8);
	cmd[3] = (uint8_t)(Dst_Addr * NumByteOfBlock);

	W25QXX_Write_Enable(); // SET WEL
	W25QXX_Wait_Busy();

	CS_Enable();						  //使能器件
	HAL_SPI_Transmit(&hspi1, cmd, 4, 20); //发送扇区擦除指令

	CS_Disable();		//取消片选
	W25QXX_Wait_Busy(); //等待擦除完成
}

void W25QXX_Erase_Chip(void)
{
	uint8_t cmd = Chip_Erase;
	W25QXX_Write_Enable();					 //写使能
	W25QXX_Wait_Busy();						 //等待空闲
	CS_Enable();							 //使能器件
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 1000); //发送片擦除命令
	CS_Disable();							 //取消片选
											 // W25QXX_Wait_Busy();   				//等待芯片擦除结束
}

// SPI方式读取FLASH
//在指定地址开始读取指定长度的数据
// pBuffer:	数据存储区
// ReadAddr:	开始读取的地址(24bit)
// NumByteToRead:	要读取的字节数(最大65535)
void W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	uint8_t cmd[5], cmd2[1];
	cmd[0] = Read_Data;
	cmd[1] = (uint8_t)((ReadAddr) >> 16);
	cmd[2] = (uint8_t)((ReadAddr) >> 8);
	cmd[3] = (uint8_t)(ReadAddr);

	CS_Enable();																	   //使能器件
	HAL_SPI_Transmit(&hspi1, cmd, 4, 20);											   //发送读取数据指令+读取指令的地址
	HAL_SPI_TransmitReceive(&hspi1, cmd2, pBuffer, NumByteToRead, 10 * NumByteToRead); //发送读取数据指令，cmd2不够的数据会自动补0发送
	CS_Disable();
}

// SPI方式写FLASH
//从指定地址开始写入指定长度的数据
//该函数带擦除操作!
// pBuffer:		数据存储区
// WriteAddr:	开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大65535)
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;

	secpos = WriteAddr / NumByteOfSector; //扇区地址
	secoff = WriteAddr % NumByteOfSector; //在扇区内的偏移
	secremain = NumByteOfSector - secoff; //扇区剩余空间大小
	// printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用

	if (NumByteToWrite <= secremain) //当需要写入的字节小于剩余空间数
	{
		secremain = NumByteToWrite; //设置剩余空间为需要写入的字节数
	}
	while (1)
	{
		W25QXX_Read(W25QXX_BUF, secpos * NumByteOfSector, NumByteOfSector); //读出整个扇区的内容，

		for (i = 0; i < secremain; i++) //校验数据
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
			{
				break; //需要擦除
			}
		}

		if (i < secremain) //需要擦除
		{
			W25QXX_Erase_Sector(secpos); //擦除这个扇区

			for (i = 0; i < secremain; i++) //复制
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}

			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * NumByteOfSector, NumByteOfSector); //写入整个扇区
		}
		else
		{
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		}

		if (NumByteToWrite == secremain)
		{
			// printf("Write Finished!!\r\n");
			break; //写入结束了
		}
		else //写入未结束
		{
			secpos++;					 //扇区地址增1
			secoff = 0;					 //偏移位置为0
			pBuffer += secremain;		 //指针偏移
			WriteAddr += secremain;		 //写地址偏移
			NumByteToWrite -= secremain; //字节数递减

			if (NumByteToWrite > NumByteOfSector)
			{
				secremain = NumByteOfSector; //下一个扇区还是写不完
			}
			else
			{
				secremain = NumByteToWrite; //下一个扇区可以写完了
			}
		}
	}
}

// SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void W25QXX_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t cmd[5];
	cmd[0] = Page_Program;
	cmd[1] = (uint8_t)((WriteAddr) >> 16);
	cmd[2] = (uint8_t)((WriteAddr) >> 8);
	cmd[3] = (uint8_t)(WriteAddr);

	W25QXX_Write_Enable(); // SET WEL
	W25QXX_Wait_Busy();

	CS_Enable();											 //使能器件
	HAL_SPI_Transmit(&hspi1, cmd, 4, 20);					 //发送写页命令
	HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, 4000); //发送要写入的数据指令+地址

	CS_Disable();		//取消片选
	W25QXX_Wait_Busy(); //等待写入结束
}

//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大65535)
// CHECK OK
void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t pageremain;
	uint16_t NumByteToWriteNow;
	pageremain = NumByteOfPage - WriteAddr % NumByteOfPage; //单页剩余的字节数
	NumByteToWriteNow = NumByteToWrite;
	if (NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWriteNow; //不大于256个字节
	}
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);

		if (NumByteToWriteNow == pageremain)
		{
			// printf("Write_No_CHECK Finished!!\r\n");	//测试用
			break; //写入结束了
		}
		else // NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;
			NumByteToWriteNow -= pageremain; //减去已经写入了的字节数

			if (NumByteToWriteNow > NumByteOfPage)
			{
				pageremain = NumByteOfPage; //一次可以写入256个字节
			}
			else
			{
				pageremain = NumByteToWriteNow; //不够256个字节了
			}
		}
	};
}
/*   APP               */
FATFS fs;
FIL file;
FRESULT res;
void mount_disk(void)
{
	res = f_mount(&fs, "0:", 1); //挂载
	if (res == FR_NO_FILESYSTEM) //没有文件系统，格式化
	{
		OLED_Clear();
		OLED_ShowString(0, 0, "No file Sys ", 16);
		res = f_mkfs("", 0, 0); //格式化
		if (res == FR_OK)
		{
			// printf("格式化成功! \r\n");
			res = f_mount(NULL, "0:", 1); //格式化后先取消挂载
			res = f_mount(&fs, "0:", 1);  //重新挂载
			if (res == FR_OK)
			{
				// OLED_ShowString(0, 0, "flash mount ok ", 16);
			}
		}
		else
		{
			OLED_ShowString(0, 0, "SD format fail ", 16);
		}
	}
	else if (res == FR_OK)
	{
		// OLED_ShowString(0, 0, "flash mount ok ", 16);
		//  printf("挂载成功! \r\n");
	}
	else
	{
		OLED_ShowString(0, 0, "SD mount fail ", 16);
	}
}
void WritetoSD(char file_name[], uint8_t write_buff[], uint8_t bufSize)
{
	uint8_t Bw = 0;
	res = f_open(&file, file_name, FA_OPEN_ALWAYS | FA_WRITE);
	if ((res & FR_DENIED) == FR_DENIED)
	{
		OLED_ShowString(0, 0, "flash full ", 16);
		// printf("卡存储已满，写入失败!\r\n");
	}
	f_lseek(&file, f_size(&file)); //确保写词写入不会覆盖之前的数据
	if (res == FR_OK)
	{
		// printf("打开成功/创建文件成功！ \r\n");
		res = f_write(&file, write_buff, bufSize, &Bw); //写数据到SD卡
		if (res == FR_OK)
		{
			 OLED_ShowString(0, 0, "Write ok ", 16);
			//  printf("文件写入成功！ \r\n");
		}
		else
		{
			OLED_ShowString(0, 0, "write fail", 16);
			// printf("文件写入失败！ \r\n");
		}
	}
	else
	{
		OLED_ShowString(0, 0, "file open fail ", 16);
		// printf("打开文件失败!\r\n");
	}
	f_close(&file); //关闭文件
	// f_mount(NULL, "0:", 1); //取消挂载
}