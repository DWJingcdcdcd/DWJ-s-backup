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
#include "tim.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SEGGER_RTT.h"
#include "hal_spi.h"
#include "hal_uart.h"
#include "hal_key.h"
#include "hal_base.h"
#include "com_lcd_dev.h"
#include "com_tlv5618.h"
#include "com_mds560r.h"
#include "com_delay.h"
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
hal_do_id_t test_led;
hal_do_id_t led1;
hal_do_id_t led2;
hal_do_id_t led3;
hal_do_id_t led4;

hal_key_id_t key_1;     ///< key 1
hal_key_id_t key_2;     ///< key 2
hal_key_id_t key_3;     ///< key 3
hal_key_id_t key_4;     ///< key 4

hal_spi_bus_id_t hal_spi1;
hal_spi_bus_id_t hal_spi2;
hal_spi_dev_id_t lcd_dev;
hal_spi_dev_id_t tlv5618_dev;
uint16_t tlv5618_dev_1;
hal_uart_id_t mds560r_dev;
uint16_t mds560r_dev_1;
uint8_t uart_rx_buf[64];		//
uint8_t uart_tx_buf[64];		//
uint8_t pressure_gauge_message[10] = {0,1,2,3,4,5,6,7,8,9};
extern DMA_HandleTypeDef hdma_usart1_rx;

uint8_t key_1_push = 0;
uint8_t TIM_Flag = 0;
uint8_t n = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
struct _pid {
	float SetPressure;            //定义设定值
	float ActualPressure;        //定义实际值
	float err;                //定义偏差值
	float err_last;            //定义上一个偏差值
	float Kp, Ki, Kd;          //定义比例、积分、微分系数
	float voltage;          //定义电压值（控制执行器的变量）
	float integral;            //定义积分值
} pid;

void PID_init() {
	SEGGER_RTT_printf(0,"PID_init begin \n");
	pid.SetPressure = 0.0;
	pid.ActualPressure = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.voltage = 0.0;
	pid.integral = 0.0;
	pid.Kp = 0.32;
	pid.Ki = 0.0;
	pid.Kd = 0.0;
	SEGGER_RTT_printf(0,"PID_init end \n");
}

float PID_realize(float pressure) {
	pid.SetPressure = pressure;
    pid.ActualPressure = (float)(((uint16_t)pressure_gauge_message[3] << 8) + pressure_gauge_message[4]) / 100;
	pid.err = pid.SetPressure - pid.ActualPressure;
	pid.integral += pid.err;
	pid.voltage = pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
	pid.err_last = pid.err;
	//pid.ActualPressure = (float)(((uint16_t)pressure_gauge_message[3] << 8) + pressure_gauge_message[4]) / 100;
	return pid.ActualPressure;
}

void key_cb(hal_key_id_t key_id, hal_key_msg_type msg_type)
{
    // if key push, turn on beep 30ms
    switch(msg_type){
        case HAL_KEY_MSG_PUSH:
            SEGGER_RTT_printf(0, "Some key have been push!\r\n");
            break;
        default:
            break;
    }
    
    if(key_id == key_1){
        switch(msg_type){
            case HAL_KEY_MSG_PUSH:
                SEGGER_RTT_printf(0, "key_1 push!\r\n"); 
                key_1_push = 1;
                TIM_Flag = 1;
                break;
            case HAL_KEY_MSG_LONG_PUSH:
                SEGGER_RTT_printf(0, "key_1 long push!\r\n");
                break;
            case HAL_KEY_MSG_REP_PUSH:
                SEGGER_RTT_printf(0, "key_1 rep push!\r\n");
                break;
            case HAL_KEY_MSG_UP:
                SEGGER_RTT_printf(0, "key_1 up!\r\n");
                break;
            default:
                break;
        }
    }
    
    if(key_id == key_2){
        switch(msg_type){
            case HAL_KEY_MSG_PUSH:
                SEGGER_RTT_printf(0, "key_2 push!\r\n");
                break;
            case HAL_KEY_MSG_LONG_PUSH:
                SEGGER_RTT_printf(0, "key_2 long push!\r\n");
                break;
            case HAL_KEY_MSG_REP_PUSH:
                SEGGER_RTT_printf(0, "key_2 rep push!\r\n");
                break;
            case HAL_KEY_MSG_UP:
                SEGGER_RTT_printf(0, "key_2 up!\r\n");
                break;
            default:
                break;
        }
    }
    
    if(key_id == key_3){
        switch(msg_type){
            case HAL_KEY_MSG_PUSH:
                SEGGER_RTT_printf(0, "key_3 push!\r\n");
                break;
            case HAL_KEY_MSG_LONG_PUSH:
                SEGGER_RTT_printf(0, "key_3 long push!\r\n");
                break;
            case HAL_KEY_MSG_REP_PUSH:
                SEGGER_RTT_printf(0, "key_3 rep push!\r\n");
                break;
            case HAL_KEY_MSG_UP:
                SEGGER_RTT_printf(0, "key_3 up!\r\n");
                break;
            default:
                break;
        }
    }
    
    if(key_id == key_4){
        switch(msg_type){
            case HAL_KEY_MSG_PUSH:
                SEGGER_RTT_printf(0, "key_4 push!\r\n");
                break;
            case HAL_KEY_MSG_LONG_PUSH:
                SEGGER_RTT_printf(0, "key_4 long push!\r\n");
                break;
            case HAL_KEY_MSG_REP_PUSH:
                SEGGER_RTT_printf(0, "key_4 rep push!\r\n");
                break;
            case HAL_KEY_MSG_UP:
                SEGGER_RTT_printf(0, "key_4 up!\r\n");
                break;
            default:
                break;
        }
    }
    
}

void uart_cb(uint16_t uart_dev_id, uint8_t *data_p, uint16_t len)
{
    //SEGGER_RTT_printf(0, "%d\r\n",data_p[5]);
    for(int a = 0; a < 7; a ++){
        pressure_gauge_message[a] = data_p[a];
    }
    //HAL_UART_Transmit(&huart2, pressure_gauge_message, 7, 100);
    
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    uint16_t i = 0;
    
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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim1);
    PID_init();
    hal_base_init();
       
    hal_do_init(7);
    hal_do_creat(&lcd_cs,LCD_CS_GPIO_Port,LCD_CS_Pin,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&mds560r_dir,MDS560R_DIR_GPIO_Port,MDS560R_DIR_Pin,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&test_led,TEST_LED_GPIO_Port,TEST_LED_Pin,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&led1,LED1_GPIO_Port,LED1_Pin,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&led2,LED2_GPIO_Port,LED2_Pin,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&led3,LED3_GPIO_Port,LED3_Pin,HAL_DO_POLAR_POSITIVE);
    hal_do_creat(&led4,LED4_GPIO_Port,LED4_Pin,HAL_DO_POLAR_POSITIVE);
    
    hal_key_init(4);
    hal_key_creat(&key_1,KEY_1_GPIO_Port,KEY_1_Pin,HAL_KEY_POLAR_POSITIVE,&key_cb);
    hal_key_creat(&key_2,KEY_2_GPIO_Port,KEY_2_Pin,HAL_KEY_POLAR_POSITIVE,&key_cb);
    hal_key_creat(&key_3,KEY_3_GPIO_Port,KEY_3_Pin,HAL_KEY_POLAR_POSITIVE,&key_cb);
    hal_key_creat(&key_4,KEY_4_GPIO_Port,KEY_4_Pin,HAL_KEY_POLAR_POSITIVE,&key_cb);
    
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
  
    //com_tlv5618_set_voltage(tlv5618_dev_1, WRITE_DAC_A, 0.6);
    
    com_mds560r_read_data(mds560r_dev_1);
    
    //hal_do_output_high(led1);
    //hal_do_output_high(led3);
    
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        hal_key_poll();
        hal_base_timer_timeout();
        i++;
        if(i == 10000){
            hal_do_output_high(test_led);
        }
        if(i >= 20000){
            hal_do_output_low(test_led);
            i = 0;
        }             
    /* USER CODE END WHILE */

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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(TIM_Flag == 1){
        n++;
        if(n >= 15){
            n = 0;
            uint16_t j = 0;
            uint8_t k = 0;
            com_mds560r_read_data(mds560r_dev_1);
            while((pressure_gauge_message[0] != 1) || (pressure_gauge_message[1] != 3)){
                j ++;
                if(j >= 1000){
                    break;
                }                
            }
            if(j >= 1000){
                SEGGER_RTT_printf(0, "pressure gauge error!\r\n");
                for(k = 0; k < 7; k ++){
                    SEGGER_RTT_printf(0, "%d\r\n",pressure_gauge_message[k]);
                    pressure_gauge_message[k] = 0;
                }
            }
            else{
                PID_realize(2);
                com_tlv5618_set_voltage(tlv5618_dev_1, WRITE_DAC_A, pid.voltage);
                
                SEGGER_RTT_printf(0, "Pressure:%f\r\n", pid.ActualPressure);
                SEGGER_RTT_printf(0, "Vol:%f\r\n", pid.voltage);
                 

                for(k = 0; k < 7; k ++){
                    pressure_gauge_message[k] = k;
                }

            }
        }     
    }
}
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
