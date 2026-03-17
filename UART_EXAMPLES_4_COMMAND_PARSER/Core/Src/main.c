#include "main.h"
#include "UARTDriver.h"
#include "GPIODriver.h"
#include "CommandParser.h"
#include "RingBuffer.h"

#define Control_Char_Start '<'
#define Control_Char_End   '>'


extern uint8_t cmd_ready;
extern RingBuffer_t rx_rb;
uint8_t temp_rx_read_index;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);


int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();


  UARTDriver_init();
  GPIODriver_led_on();


  while (1)
  {

	  /*
	   * Veri geldiği an önce interrutp içinde veri buffer a yazılıyor, sonrasında buradaki is_available() fonksiyonu ile
	   * rx buffer ın boş olup olmadığına bakılıyor, eğer buffer dolu ile true dönüyor ve bu gelen veri direkt read()
	   * ile rx buffer ındaki read indexinin gösterdiği yerdeki indexten okunuyor. Bu okunan değer de direkt olarak geri
	   * gönderiliyordu.
	   */

	  /*
	   * Command parser için gelen bu verilerin önce buffer a yazılması gerekiyor ki zaten bu işlem interupt içinde direkt
	   * gelir gelmez yapılıyor, sonrasında bu gelen verilerin buffer içinden okunması gerekiyor. Gelen verilerin uzunlu
	   * kontrol edilerek null olup olmadığına bakılmalı ,
	   *
	   */

	  if (UARTDriver_data_is_available())
	  {
		  uint8_t b = UARTDriver_read(); // burada veri geldiğinde bu veriyi direkt göndermek yerine komut parser ile işlem yaptırılacak

		  /*if(b == 'A'){				// Gelen veri karakteri ne ise ona göre işlem yaptırma
			  GPIODriver_toggle();
		  }
		  */



		  if(b == Control_Char_Start){
			  ring_buffer_clear(&rx_rb);
			  temp_rx_read_index = rx_rb.read_index;
		  }




		  if(b == Control_Char_End && cmd_ready == 0){
			  cmd_ready = 1;
			  rx_rb.read_index = temp_rx_read_index;
			  CommandParser_RX_CMD_Complete_callback(&rx_rb);
		  }



		  UARTDriver_send_byte(b);
	  }


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
