/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <string.h>
#include "shell_port.h"
#include "multi_button_port.h"


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
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    // IPMI CMD Get Device ID

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, &uart1_rxdata, 1);
  userShellInit();

  user_button_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// Retargets the C library printf function to the USART.
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

// execute as soon as possiable, don't use for, while
void HAL_SYSTICK_Callback(void)
{
    // LED
    static uint32_t led_red_count = 0;
    static uint32_t led_red_cycle = 1000;

    if (led_red_count++ > led_red_cycle)
    {
        led_red_count = 0;
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    }


    // key scan
    static uint32_t key_scan_count = 0;
    static uint32_t key_scan_cycle = 5;     // 5ms, according to readme

    if (key_scan_count++ > key_scan_cycle)
    {
        key_scan_count = 0;
        button_ticks();
    }
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#if 0

    switch (GPIO_Pin)
    {
    case KEY_LEFT_Pin:
        key_right_flag = 1;
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        break;
    case KEY_DOWN_Pin:
        key_down_flag = 1;
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        break;
    case KEY_RIGHT_Pin:
        key_right_flag = 1;
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        break;
    default:
        break;
    }
#endif
}




#define BYTES_PER_LINE 16
#define isprint(x) (((x) >= 32) && ((x) < 127))

void print_buff(const uint8_t *title, const uint8_t *buff, uint32_t len)
{
    uint32_t i = 0;
    uint32_t index = 0; // reduce calc
    uint8_t num_space = 0;
    uint8_t ascii_char_buff[BYTES_PER_LINE + 1];

    ascii_char_buff[BYTES_PER_LINE] = '\0';

    printf("\n%s, buff len is %lu.\n", title, len);

#if 1
    // -----------------------------------------------------------------------------
    // print header
    // -----------------------------------------------------------------------------
    printf("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   0123456789ABCDEF\n");
    printf("------------------------------------------------------------------\n");

#if 0
        // hex: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
        for (i = 0; i < BYTES_PER_LINE; ++i)
        {
            printf("%02X ", i);
        }

        printf("  ");

        // ascii: 0123456789ABCDEF
        for (i = 0; i < BYTES_PER_LINE; ++i)
        {
            printf("%X", i);
        }

        printf("\n");
        // ------
        num_space = BYTES_PER_LINE * 3 + 2 + BYTES_PER_LINE;

        for (i = 0; i < num_space; ++i)
        {
            printf("-");
        }

        printf("\n");
#endif
#endif

    // -----------------------------------------------------------------------------
    // print content
    // -----------------------------------------------------------------------------
    for (i = 0; i < len; ++i)
    {
        index = i % BYTES_PER_LINE;

        if ((index == 0) && (i != 0))
        {
            printf("  %s\n", ascii_char_buff);
            // ascii_char_buff[0] = '\0';        // clear string, no need indeed
        }

        // 2018-09-30
        // testbed error: type mismatch in ternary expression ???
        // ascii_char_buff[index] = isprint(buff[i]) ? buff[i] : '.';

        // '.' = ASCII 0x2E, 46
        if (isprint(buff[i]))
        {
            ascii_char_buff[index] = buff[i];
        }
        else
        {
            ascii_char_buff[index] = '.';
        }

        printf("%02X ", buff[i]);
    }

    // print last line
    // num of bytes in last line, 0 : index-1
    index = len % BYTES_PER_LINE;

    // last line incomplete, fill with spaces
    if (index != 0)
    {
        ascii_char_buff[index] = '\0'; // last + 1 byte
        // think about it...
        num_space = (BYTES_PER_LINE - index) * 3;

        for (i = 0; i < num_space; ++i)
        {
            printf(" ");
        }
    }

    printf("  %s\n", ascii_char_buff);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
