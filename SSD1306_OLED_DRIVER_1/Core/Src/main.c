#include "main.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"

#include "SSD1306_CUSTOM_Driver.h"



I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;



void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);


const unsigned char wifi_icon[] = {
    0x07, 0xE0, 0x1F, 0xF8, 0x38, 0x1C, 0x70, 0x0E, 0x63, 0xC6, 0x07, 0xE0, 0x0C, 0x30, 0x00, 0x00,
    0x01, 0x80, 0x03, 0xC0, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00
};




int main(void)
{


  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();




  ssd1306_Init();
  ssd1306_Fill(Black);


  // Sabit bir başlık yazalım (Bu yazı hiç değişmeyecek)
  	ssd1306_SetCursor(20, 1);
  	ssd1306_WriteString("SENSOR DEGERI", Font_7x10, White);
  	ssd1306_Line(0, 13, 127, 13, White); // Başlığın altına çizgi çek
  	ssd1306_UpdateScreen(); // Tek seferlik çizimleri ekrana gönder

  	//ssd1306_DrawCircle(30, 35, 15, White);
  	// İçi Dolu Daire (Merkez: x=90, y=30, Yarıçap: r=15)
  	//ssd1306_FillCircle(90, 35, 15, White);


  	ssd1306_SetContrast(0);		// Tüm ekranın ışığını ayarlar
  	ssd1306_UpdateScreen();
	HAL_Delay(100);


  	for(int i = 0; i<26;i++){
  		ssd1306_SetContrast(i*10);
  		ssd1306_InvertRectangle(20, 0, 111, 9);
  		ssd1306_UpdateScreen();
  		HAL_Delay(100);
  	}




  	SSD1306_WriteCenteredString("MEHMET DORA", Font_11x18, White);
  	ssd1306_UpdateScreen();


  	//HAL_Delay(1000);


  	//ssd1306_Fill(Black);
  	//SSD1306_WriteCenteredStringInRegion("STM32F411RE", Font_7x10, Black, 30, 20, 120, 60);
  	//ssd1306_UpdateScreen();


  	// Kaydırma konumunu tutacak değişken (static olduğu için hafızada kalır)
	static uint16_t my_scroll_pos = 0;
	char* uzun_metin = "HANG GLIDING TRAINING - Take to the skies!";





	DrawImage(20,20,16,16,wifi_icon, White, Font_7x10);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);


	DrawLiveGraph();
	HAL_Delay(1000);



	ssd1306_Fill(Black);
	// Normal Yazı
	ssd1306_SetCursor(10, 30);
	ssd1306_WriteString("STM32 Normal", Font_7x10, White);
	// Kalın (Bold) Yazı
	ssd1306_SetCursor(10, 50);
	ssd1306_WriteStringBold(&SSD1306,"STM32 BOLD", Font_7x10, White);
	ssd1306_UpdateScreen();

	HAL_Delay(1000);


  while (1)
  {

	  /*
	  for(int i = 1; i<61; i++){
			ssd1306_FillRectangle((20 + i), 25, (40+i), 45, White);
			ssd1306_UpdateScreen();
			//HAL_Delay(10);
			ssd1306_FillRectangle(20, 25, 100, 45, Black);
	  }
	  */



      // x: 10 ile 117, y: 25 ile 45 arasındaki bölgeye kayan yazıyı çizelim
      // Bu bölgenin dışına taşan yazılar kırpılacaktır.
      SSD1306_WriteScrollingStringInRegion(uzun_metin, Font_11x18, White, 10, 24, 117, 60, &my_scroll_pos,3);
      // Bir çerçeve çizerek bölgeyi görselleştirelim (isteğe bağlı)
      ssd1306_DrawRectangle(8, 21, 119, 63, White);
      ssd1306_UpdateScreen();

      // Kaydırma hızını bu gecikme belirler (örn: 30ms)
      //HAL_Delay(30);
      ssd1306_Fill(Black);



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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
