/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "bmp.h"
#include "key.h"
#include "SDdriver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void WritetoSD(BYTE write_buff[],uint8_t bufSize);
char SD_FileName[] = "tw.csv";
uint8_t WriteBuffer[] = "12,13,14\n";
uint32_t c=0;
//uint8_t test_sd =0;	//���ڲ��Ը�ʽ��
uint8_t write_cnt =0;	//дSD������




void WritetoSD(BYTE write_buff[],uint8_t bufSize)
{
	FATFS fs;
	FIL file;
	uint8_t res=0;
	UINT Bw;	
	
	res = SD_init();		//SD����ʼ��
	
	if(res == 1)
	{
		OLED_ShowNum(0, 2,1, 4, 16);
	}
	else
	{
		OLED_ShowNum(0, 2,2, 4, 16);
c=		SD_GetSectorCount();
	}
	
	res=f_mount(&fs,"0:",1);		//����
	
//	if(test_sd == 0)		//���ڲ��Ը�ʽ��
	if(res == FR_NO_FILESYSTEM)		//û���ļ�ϵͳ����ʽ��
	{
//		test_sd =1;				//���ڲ��Ը�ʽ��
		OLED_ShowNum(0, 2,3, 4, 16);	
		res = f_mkfs("", 0, 0);		//��ʽ��sd��
		if(res == FR_OK)
		{
			OLED_ShowNum(0, 2,4, 4, 16);	
			res = f_mount(NULL,"0:",1); 		//��ʽ������ȡ������
			res = f_mount(&fs,"0:",1);			//���¹���	
			if(res == FR_OK)
			{
				OLED_ShowNum(0, 2,51, 4, 16);
			}	
		}
		else
		{
			OLED_ShowNum(0, 2,6, 4, 16);	
		}
	}
	else if(res == FR_OK)
	{
		OLED_ShowNum(0, 2,7, 4, 16);	
	}
	else
	{
		OLED_ShowNum(0, 2,8, 4, 16);
	}	
	
	res = f_open(&file,SD_FileName,FA_OPEN_ALWAYS |FA_WRITE);   //���򿨻򴴽�ԨSD_FileName������Ϊ������ļ���fileָ��ָ����ļ�
	if((res & FR_DENIED) == FR_DENIED)
	{
		OLED_ShowNum(0, 2,9, 4, 16);		
	}
	
	f_lseek(&file, f_size(&file));//ȷ��д��д�벻�Ḳ��֮ǰ������
	if(res == FR_OK)
	{
		OLED_ShowNum(0, 2,10, 4, 16);
		res = f_write(&file,write_buff,bufSize,&Bw);		//д���ݵ�SD��
		if(res == FR_OK)
		{
			OLED_ShowNum(0, 2,11, 4, 16);		
		}
		else
		{
			OLED_ShowNum(0, 2,12, 4, 16);
		}		
	}
	else
	{
		OLED_ShowNum(0, 2,13, 4, 16);
	}	
	
	f_close(&file);						//�ر��ļ�		
	f_mount(NULL,"0:",1);		 //ȡ������
	
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint16_t ADC_ConvertedValue=0;
	uint8_t power_ConvertedValue=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
 // MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
	HAL_ADCEx_Calibration_Start(&hadc1);
  /* ����ADת����ʹ��DMA������ж� */
  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue,sizeof(ADC_ConvertedValue)); 
  bsp_InitKeyVar();
	beep();
  OLED_Init();
  OLED_Clear();
  OLED_DrawBMP(0, 0, 128, 8, BMP1);
	WritetoSD(WriteBuffer,sizeof(WriteBuffer));	
	MX_USB_DEVICE_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    bsp_KeyScan();
    key_handler();
		//OLED_ShowNum(0, 4,ADC_ConvertedValue, 4, 16);
		//power_ConvertedValue=(ADC_ConvertedValue-3500)/4.14;
		//OLED_ShowNum(0, 4,power_ConvertedValue, 2, 16);
		//HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_ConvertedValue,sizeof(ADC_ConvertedValue)); 
		//HAL_Delay(20);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
