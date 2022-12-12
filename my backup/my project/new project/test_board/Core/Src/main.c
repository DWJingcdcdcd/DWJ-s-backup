/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "SEGGER_RTT.h"
#include "hal_spi.h"
#include "hal_uart.h"
#include "com_lcd_dev.h"
#include "com_tlv5618.h"
#include "com_delay.h"
#include "com_mds560r.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
hal_do_id_t lcd_cs;
hal_do_id_t lcd_rst;
hal_do_id_t mds560r_dir;

hal_spi_bus_id_t hal_spi1;
hal_spi_bus_id_t hal_spi2;
hal_spi_dev_id_t lcd_dev;
hal_spi_dev_id_t tlv5618_dev;
uint16_t tlv5618_dev_1;
hal_uart_id_t mds560r_dev;
uint16_t mds560r_dev_1;
uint8_t uart_rx_buf[64];		//
uint8_t uart_tx_buf[64];		//
extern DMA_HandleTypeDef hdma_usart1_rx;

uint8_t response[9] = {1,2,3,4,5,6,7,8,9};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void uart_cb(uint16_t uart_dev_id, uint8_t *data_p, uint16_t len)
{
    SEGGER_RTT_printf(0, "%d\r\n",data_p[5]);
    HAL_UART_Transmit(&huart2, data_p, 7, 100);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

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
    MX_DMA_Init();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */
    hal_do_init(2);
    hal_do_creat(&lcd_cs,LCD_CS_GPIO_Port,LCD_CS_Pin,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&mds560r_dir,MDS560R_DIR_GPIO_Port,MDS560R_DIR_Pin,HAL_DO_POLAR_POSITIVE);
    
    hal_spi_bus_init(2);
    hal_spi_bus_creat(&hal_spi1,&hspi1);
    hal_spi_dev_init(hal_spi1,1);
    hal_spi_dev_creat(hal_spi1,&lcd_dev,LCD_CS_GPIO_Port,LCD_CS_Pin);
    
    hal_spi_bus_creat(&hal_spi2,&hspi2);      
    hal_spi_dev_init(hal_spi2,1);   
    hal_spi_dev_creat(hal_spi2,&tlv5618_dev,TLV5618_CS_GPIO_Port,TLV5618_CS_Pin);
    
    com_lcd_creat(hal_spi1,lcd_dev,lcd_cs,lcd_rst);
    com_tlv5618_init(1);
    com_tlv5618_creat(&tlv5618_dev_1,hal_spi2,tlv5618_dev);
    
    hal_uart_init(1);
    hal_uart_creat(&mds560r_dev, &huart1, &hdma_usart1_rx, NULL, uart_cb, uart_rx_buf, 64);
    com_mds560r_init(1);
    com_mds560r_creat(&mds560r_dev_1,mds560r_dev,mds560r_dir);
    
    com_lcd_init();
    com_lcd_clear_screen();
    com_lcd_disp_str(0, 0, (uint8_t*)"床前明月光，");
    com_lcd_disp_str(1, 0, (uint8_t*)"疑是地上霜。");
    com_lcd_disp_str(2, 0, (uint8_t*)"举头望明月，");
    com_lcd_disp_str(3, 0, (uint8_t*)"低头思故乡。");
  
    com_tlv5618_set_voltage(tlv5618_dev_1, WRITE_DAC_A, 1.725);
    
    com_mds560r_read_data(mds560r_dev_1);
    /*HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
    com_delay_ms(10);
    hal_uart_trans(mds560r_dev, unlock, 8);
    com_delay_ms(150);
    hal_uart_trans(mds560r_dev, commond, 8);
    com_delay_ms(150);
    hal_uart_trans(mds560r_dev, confirm, 8);
    com_delay_ms(10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);*/
    
    /*HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
    com_delay_ms(10);
    //HAL_UART_Transmit_DMA(&huart1, read, 8);

    HAL_UART_Transmit_DMA(&huart1, unlock, 8);
    com_delay_ms(150);
    HAL_UART_Transmit_DMA(&huart1, commond, 8);
    com_delay_ms(150);
    HAL_UART_Transmit_DMA(&huart1, confirm, 8);
    com_delay_ms(10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);*/
    //if(HAL_UART_Receive_DMA(&huart1, response, 8) == 0){
        //HAL_UART_Transmit(&huart2, response, 8, 100);
      //  ;
   // }
    
    //HAL_UART_Transmit_DMA(&huart2, (uint8_t *) "sunking \r\n", 7);
    //SEGGER_RTT_printf(0,"test uart2 \r\n");
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */
        //HAL_UART_Receive(&huart1, response, 7, 100);
    
        //HAL_UART_Transmit(&huart2, response, 7, 100);
    
        
        
    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
size_t get_sys_time_ms(void)
{
	return HAL_GetTick();
}


