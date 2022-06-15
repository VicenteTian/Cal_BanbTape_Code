#include "W25QXX.h"
#include "oled.h"
#include "ff.h"
FATFS fs;
FIL file;
FRESULT res;
uint32_t FLASH_SIZE = 0;				// FLASH�ж����ֽڣ����ݲ�ͬ��FLASHоƬ������õ�����λByte
uint8_t W25QXX_BUFFER[NumByteOfSector]; //����һ�������ܹ����ֽ���(4096�ֽ�)��Ϊ��������

/*��ȡоƬID
�����������
���ز���:
	0XEF13,��ʾоƬ�ͺ�ΪW25Q80
	0XEF14,��ʾоƬ�ͺ�ΪW25Q16
	0XEF15,��ʾоƬ�ͺ�ΪW25Q32
	0XEF16,��ʾоƬ�ͺ�ΪW25Q64
	0XEF17,��ʾоƬ�ͺ�ΪW25Q128
	0XEF18,��ʾоƬ�ͺ�ΪW25Q256
*/
uint16_t W25QXX_ReadID(void)
{
	uint16_t temp = 0;
	uint8_t TX_cmd[6], RX_temp[6];
	TX_cmd[0] = Manufacturer;

	CS_Enable();
	//����ָ��ͽ��ռ������Ƽ�ʹ�ò�ѯģʽ
	HAL_SPI_TransmitReceive(&hspi1, TX_cmd, RX_temp, 6, 10); //���Ͷ�ȡID����
	temp = RX_temp[4] << 8 | RX_temp[5];
	CS_Disable();
	return temp; //����FLASH��ID��
}

/*
��ʼ��FLASH��������ȡFLASH ��ID
����ֵ:
*/
uint8_t W25QXX_Init(void)
{
	uint16_t ReadFlah_Type; //����ʵ�ʶ�ȡ��FLASH�ͺ�
	uint8_t Flag = 0;		//��ǳ�ʼ��flash״̬��0--ʧ�ܣ�1--�ɹ�
	CS_Disable();			// SPI FLASH��ѡ��

	ReadFlah_Type = W25QXX_ReadID(); //��ȡFLASH��ID��ȷ��FLASH�ͺ�
	// printf("FLASH_ID:0x%x\r\n",ReadFlah_Type);	//��ӡflash��оƬID,������
	switch (ReadFlah_Type)
	{
	case 0xEF13: // W25Q80
	{
		FLASH_SIZE = Num80BlockOfChip * NumByteOfBlock; //��*65536 =�ֽ���
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q80\r\n",22,0xff);
		//��ӡflashоƬ��Ϣ
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num80BlockOfChip,Num80BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH ��ʼ���ɹ�
		break;
	}
	case 0xEF14: // W25Q16
	{
		FLASH_SIZE = Num16BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q16\r\n",22,0xff);
		//��ӡflashоƬ��Ϣ
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num16BlockOfChip,Num16BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH ��ʼ���ɹ�
		break;
	}
	case 0xEF15: // W25Q32
	{
		FLASH_SIZE = Num32BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q32\r\n",22,0xff);
		//��ӡflashоƬ��Ϣ
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num32BlockOfChip,Num32BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH ��ʼ���ɹ�
		break;
	}
	case 0xEF16: // W25Q64
	{
		FLASH_SIZE = Num64BlockOfChip * NumByteOfBlock;
		// OLED_ShowString(0,0,(uint8_t*)"W25Q64",16);
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q64\r\n",22,0xff);
		//��ӡflashоƬ��Ϣ
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num64BlockOfChip,Num64BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH ��ʼ���ɹ�
		break;
	}
	case 0xEF17: // W25Q128
	{
		FLASH_SIZE = Num128BlockOfChip * NumByteOfBlock;
		//	HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q128\r\n",23,0xff);
		//��ӡflashоƬ��Ϣ
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num128BlockOfChip,Num128BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH ��ʼ���ɹ�
		break;
	}
	case 0xEF18: // W25Q256
	{
		FLASH_SIZE = Num256BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q256\r\n",23,0xff);
		//��ӡflashоƬ��Ϣ
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num256BlockOfChip,Num256BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH ��ʼ���ɹ�
		break;
	}
	case 0xEF19: // W25Q512
	{
		FLASH_SIZE = Num256BlockOfChip * NumByteOfBlock;
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Flash Mode is W25Q512\r\n",23,0xff);
		//��ӡflashоƬ��Ϣ
		// printf("FLASH_ID:0x%X Block:%d FLASH_SIZE:%dMB total:%d Bytes \r\n",ReadFlah_Type,Num512BlockOfChip,Num512BlockOfChip/16,FLASH_SIZE);
		Flag = 1; // FLASH ��ʼ���ɹ�
		break;
	}
	default:
	{
		// HAL_UART_Transmit(&huart1,(uint8_t *)&"Other FLASH_ID,Flash init error\r\n",33,0xff);
		Flag = 0; // FLASH ��ʼ��ʧ��
		break;
	}
	}
	return Flag;
}

// W25QXXдʹ��
//��WEL��λ
void W25QXX_Write_Enable(void)
{
	uint8_t cmd = Write_Enable;
	CS_Enable();						   //ʹ������
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); //����дʹ��
	CS_Disable();						   //ȡ��Ƭѡ
}

// W25QXXд��ֹ
//��WEL����
void W25QXX_Write_Disable(void)
{
	uint8_t cmd = Write_Disable;
	CS_Enable();						   //ʹ������
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); //����д��ָֹ��
	CS_Disable();						   //ȡ��Ƭѡ
}

//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
// BIT7  6   5   4   3   2   1   0
// SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
// TB,BP2,BP1,BP0:FLASH����д��������
// WEL:дʹ������
// BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
// BIT7  6   5   4   3   2   1   0
// SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
// BIT7      6    5    4   3   2   1   0
// HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
// regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
uint8_t W25QXX_ReadSR(uint8_t regno)
{
	uint8_t TX_cmd[2];
	uint8_t RX_temp[2];
	switch (regno)
	{
	case 1:
	{
		TX_cmd[0] = Read_Status_Register1; //��״̬�Ĵ���1ָ��
		break;
	}
	case 2:
	{
		TX_cmd[0] = Read_Status_Register2; //��״̬�Ĵ���2ָ��
		break;
	}
	case 3:
	{
		TX_cmd[0] = Read_Status_Register3; //��״̬�Ĵ���3ָ��
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
	//����ָ��ͽ��ռ������Ƽ�ʹ�ò�ѯģʽ
	HAL_SPI_TransmitReceive(&hspi1, TX_cmd, RX_temp, 2, 10); //���Ͷ�״̬�Ĵ�������
	CS_Disable();											 //ȡ��FLASHƬѡ
	return RX_temp[1];
}

//дW25QXX״̬�Ĵ���
void W25QXX_WriteSR(uint8_t regno, uint8_t sr)
{
	uint8_t TX_cmd[2];
	switch (regno)
	{
	case 1:
	{
		TX_cmd[0] = Write_Status_Register1; //д״̬�Ĵ���1ָ��
		break;
	}
	case 2:
	{
		TX_cmd[0] = Write_Status_Register2; //д״̬�Ĵ���2ָ��
		break;
	}
	case 3:
	{
		TX_cmd[0] = Write_Status_Register3; //д״̬�Ĵ���3ָ��
		break;
	}
	default:
	{
		TX_cmd[0] = Write_Status_Register1;
		break;
	}
	}
	TX_cmd[1] = sr;
	CS_Enable(); //ʹ������
	// Poll mode
	//����ָ��ͽ��ռ������Ƽ�ʹ�ò�ѯģʽ
	HAL_SPI_Transmit(&hspi1, TX_cmd, 2, 10); //���Ͷ�״̬�Ĵ�������
	CS_Disable();							 //ȡ��Ƭѡ
}

//�ȴ�����
void W25QXX_Wait_Busy(void)
{
	while ((W25QXX_ReadSR(1) & 0x01) == 0x01)
		; // �ȴ�BUSYλ���
}

//�������ģʽ
void W25QXX_PowerDown(void)
{
	uint8_t cmd = Power_down; //����ָ������Ϊ����

	CS_Enable();						   //ʹ������
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); //���͵�������
	CS_Disable();						   //ȡ��Ƭѡ

	HAL_Delay(1); //�ȴ�TPD
}

//����
void W25QXX_WAKEUP(void)
{
	uint8_t cmd = Release_Power_down;	   //���û���ָ��
	CS_Enable();						   //ʹ������
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 10); // send W25X_PowerDown command 0xAB
	CS_Disable();						   //ȡ��Ƭѡ
	HAL_Delay(1);						   //�ȴ�TRES1
}

//����һ������
// Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:45ms - 400ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
	uint8_t cmd[5];
	cmd[0] = Sector_Erase;
	cmd[1] = (uint8_t)((Dst_Addr * NumByteOfSector) >> 16);
	cmd[2] = (uint8_t)((Dst_Addr * NumByteOfSector) >> 8);
	cmd[3] = (uint8_t)(Dst_Addr * NumByteOfSector);

	W25QXX_Write_Enable();					// SET WEL
	W25QXX_Wait_Busy();						//�ȴ�����
	CS_Enable();							//ʹ������
	HAL_SPI_Transmit(&hspi1, cmd, 4, 4000); //���Ͳ�������ָ��+��ַ
	CS_Disable();							//ȡ��Ƭѡ
	W25QXX_Wait_Busy();						//�ȴ��������
}

//����һ����
// Dst_Addr:���ַ ����ʵ����������
//����һ���������ʱ��:150ms - 2000ms
void W25QXX_Erase_Block(uint32_t Dst_Addr)
{
	uint8_t cmd[5];
	cmd[0] = Block_Erase1;
	cmd[1] = (uint8_t)((Dst_Addr * NumByteOfBlock) >> 16);
	cmd[2] = (uint8_t)((Dst_Addr * NumByteOfBlock) >> 8);
	cmd[3] = (uint8_t)(Dst_Addr * NumByteOfBlock);

	W25QXX_Write_Enable(); // SET WEL
	W25QXX_Wait_Busy();

	CS_Enable();						  //ʹ������
	HAL_SPI_Transmit(&hspi1, cmd, 4, 20); //������������ָ��

	CS_Disable();		//ȡ��Ƭѡ
	W25QXX_Wait_Busy(); //�ȴ��������
}

void W25QXX_Erase_Chip(void)
{
	uint8_t cmd = Chip_Erase;
	W25QXX_Write_Enable();					 //дʹ��
	W25QXX_Wait_Busy();						 //�ȴ�����
	CS_Enable();							 //ʹ������
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 1000); //����Ƭ��������
	CS_Disable();							 //ȡ��Ƭѡ
											 // W25QXX_Wait_Busy();   				//�ȴ�оƬ��������
}

// SPI��ʽ��ȡFLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
// pBuffer:	���ݴ洢��
// ReadAddr:	��ʼ��ȡ�ĵ�ַ(24bit)
// NumByteToRead:	Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	uint8_t cmd[5], cmd2[1];
	cmd[0] = Read_Data;
	cmd[1] = (uint8_t)((ReadAddr) >> 16);
	cmd[2] = (uint8_t)((ReadAddr) >> 8);
	cmd[3] = (uint8_t)(ReadAddr);

	CS_Enable();																	   //ʹ������
	HAL_SPI_Transmit(&hspi1, cmd, 4, 20);											   //���Ͷ�ȡ����ָ��+��ȡָ��ĵ�ַ
	HAL_SPI_TransmitReceive(&hspi1, cmd2, pBuffer, NumByteToRead, 10 * NumByteToRead); //���Ͷ�ȡ����ָ�cmd2���������ݻ��Զ���0����
	CS_Disable();
}

// SPI��ʽдFLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
// pBuffer:		���ݴ洢��
// WriteAddr:	��ʼд��ĵ�ַ(24bit)
// NumByteToWrite:Ҫд����ֽ���(���65535)
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;

	secpos = WriteAddr / NumByteOfSector; //������ַ
	secoff = WriteAddr % NumByteOfSector; //�������ڵ�ƫ��
	secremain = NumByteOfSector - secoff; //����ʣ��ռ��С
	// printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������

	if (NumByteToWrite <= secremain) //����Ҫд����ֽ�С��ʣ��ռ���
	{
		secremain = NumByteToWrite; //����ʣ��ռ�Ϊ��Ҫд����ֽ���
	}
	while (1)
	{
		W25QXX_Read(W25QXX_BUF, secpos * NumByteOfSector, NumByteOfSector); //�����������������ݣ�

		for (i = 0; i < secremain; i++) //У������
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
			{
				break; //��Ҫ����
			}
		}

		if (i < secremain) //��Ҫ����
		{
			W25QXX_Erase_Sector(secpos); //�����������

			for (i = 0; i < secremain; i++) //����
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}

			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * NumByteOfSector, NumByteOfSector); //д����������
		}
		else
		{
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
		}

		if (NumByteToWrite == secremain)
		{
			// printf("Write Finished!!\r\n");
			break; //д�������
		}
		else //д��δ����
		{
			secpos++;					 //������ַ��1
			secoff = 0;					 //ƫ��λ��Ϊ0
			pBuffer += secremain;		 //ָ��ƫ��
			WriteAddr += secremain;		 //д��ַƫ��
			NumByteToWrite -= secremain; //�ֽ����ݼ�

			if (NumByteToWrite > NumByteOfSector)
			{
				secremain = NumByteOfSector; //��һ����������д����
			}
			else
			{
				secremain = NumByteToWrite; //��һ����������д����
			}
		}
	}
}

// SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
// pBuffer:���ݴ洢��
// WriteAddr:��ʼд��ĵ�ַ(24bit)
// NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void W25QXX_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t cmd[5];
	cmd[0] = Page_Program;
	cmd[1] = (uint8_t)((WriteAddr) >> 16);
	cmd[2] = (uint8_t)((WriteAddr) >> 8);
	cmd[3] = (uint8_t)(WriteAddr);

	W25QXX_Write_Enable(); // SET WEL
	W25QXX_Wait_Busy();

	CS_Enable();											 //ʹ������
	HAL_SPI_Transmit(&hspi1, cmd, 4, 20);					 //����дҳ����
	HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, 4000); //����Ҫд�������ָ��+��ַ

	CS_Disable();		//ȡ��Ƭѡ
	W25QXX_Wait_Busy(); //�ȴ�д�����
}

//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
// pBuffer:���ݴ洢��
// WriteAddr:��ʼд��ĵ�ַ(24bit)
// NumByteToWrite:Ҫд����ֽ���(���65535)
// CHECK OK
void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t pageremain;
	uint16_t NumByteToWriteNow;
	pageremain = NumByteOfPage - WriteAddr % NumByteOfPage; //��ҳʣ����ֽ���
	NumByteToWriteNow = NumByteToWrite;
	if (NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWriteNow; //������256���ֽ�
	}
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);

		if (NumByteToWriteNow == pageremain)
		{
			// printf("Write_No_CHECK Finished!!\r\n");	//������
			break; //д�������
		}
		else // NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;
			NumByteToWriteNow -= pageremain; //��ȥ�Ѿ�д���˵��ֽ���

			if (NumByteToWriteNow > NumByteOfPage)
			{
				pageremain = NumByteOfPage; //һ�ο���д��256���ֽ�
			}
			else
			{
				pageremain = NumByteToWriteNow; //����256���ֽ���
			}
		}
	};
}
/*   APP               */

void mount_disk(void)
{
	res = f_mount(&fs, "0:", 1); //����
	if (res == FR_NO_FILESYSTEM) //û���ļ�ϵͳ����ʽ��
	{
		OLED_Clear();
		OLED_ShowString(0, 0, "No file Sys ", 16);
		res = f_mkfs("", 0, 0); //��ʽ��
		if (res == FR_OK)
		{
			// printf("��ʽ���ɹ�! \r\n");
			res = f_mount(NULL, "0:", 1); //��ʽ������ȡ������
			res = f_mount(&fs, "0:", 1);  //���¹���
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
		//  printf("���سɹ�! \r\n");
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
		// printf("���洢������д��ʧ��!\r\n");
	}
	f_lseek(&file, f_size(&file)); //ȷ��д��д�벻�Ḳ��֮ǰ������
	if (res == FR_OK)
	{
		// printf("�򿪳ɹ�/�����ļ��ɹ��� \r\n");
		res = f_write(&file, write_buff, bufSize, &Bw); //д���ݵ�SD��
		if (res == FR_OK)
		{
			// OLED_ShowString(0, 0, "Write ok ", 8);
			//  printf("�ļ�д��ɹ��� \r\n");
		}
		else
		{
			OLED_ShowString(0, 0, "write fail", 16);

			// printf("�ļ�д��ʧ�ܣ� \r\n");
		}
	}
	else
	{
		OLED_ShowString(0, 0, "file open fail ", 16);
		// printf("���ļ�ʧ��!\r\n");
	}
	f_close(&file); //�ر��ļ�
					// f_mount(NULL, "0:", 1); //ȡ������
}