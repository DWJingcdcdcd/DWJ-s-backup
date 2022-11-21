#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "SEGGER_RTT.h"
#include "hal_spi.h"
#include "com_lcd_dev.h"

hal_do_id_t lcd_rs;
hal_do_id_t lcd_rst;
hal_spi_bus_id_t hal_spi1;
hal_spi_dev_id_t lcd_dev;

void SystemClock_Config(void);


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
  
    hal_do_init(5);
    hal_do_creat(&lcd_rst,GPIOA,GPIO_PIN_3,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&lcd_rs,GPIOA,GPIO_PIN_2,HAL_DO_POLAR_POSITIVE);
    
    hal_spi_bus_init(1);
    hal_spi_bus_creat(&hal_spi1,&hspi1);
    hal_spi_dev_init(hal_spi1,1);   
    hal_spi_dev_creat(hal_spi1,&lcd_dev,GPIOA,GPIO_PIN_2);
    
    com_lcd_creat(hal_spi1,lcd_dev,lcd_rs,lcd_rst);
    com_lcd_init();
    com_lcd_clear_screen();
    com_lcd_disp_str(0, 0, (uint8_t*)"秦时明月汉时关，");
    com_lcd_disp_str(1, 0, (uint8_t*)"万里长征人未还。");
    com_lcd_disp_str(2, 0, (uint8_t*)"但使龙城飞将在，");
    com_lcd_disp_str(3, 0, (uint8_t*)"不教胡马度阴山。");

    while (1)
    {
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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








