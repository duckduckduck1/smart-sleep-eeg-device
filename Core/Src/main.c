
#include "main.h"
#include <stdbool.h>
#include <stdio.h>
void SystemClock_Config(void);


uint16_t uartFalg = 0;
uint8_t rx_buf [6] = { 0, };



void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) // Обработчик UART 
{
	
}


int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  __enable_irq();
  MX_TIM3_Init();
	HAL_TIM_Base_Start(&htim2);
	
	HAL_UART_Receive_IT(&huart1 ,rx_buf , 1 );
	enum States { Max_reset, Max_init, Max_read, test };
	enum States state = Max_reset;
	
	struct CNFG_GEN_init cnfg_gen = {0, };
	struct CNFG_GEN_init cnfg_gen_read = {0,};
	
	cnfg_gen.RBIASN = 0x01;
	cnfg_gen.RBIASP = 0x01;
	cnfg_gen.RBIASV = 0x01;
	cnfg_gen.EN_RBIAS = 0x01;
	cnfg_gen.DCLOFF_VTH = 0x00;
	cnfg_gen.DCLOFF_IMAG = 0; // RBIASV
	cnfg_gen.DCLOFF_IPOL = 0;
	cnfg_gen.EN_DCLOFF = 0x00;
	cnfg_gen.EN_ECG = 1;
	cnfg_gen.FMSTR = 0x01;
	cnfg_gen.EN_ULP_LON = 0;
	////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////
		// CNFG_ECG_init
	struct CNFG_ECG_init cnfg_ecg = {0, };
	struct CNFG_ECG_init cnfg_ecg_read = {0,};
	
	cnfg_ecg.DLPF = 0x01;
	cnfg_ecg.DHPF = 1;
	cnfg_ecg.GAIN = 0x11;
	cnfg_ecg.RATE = 0x01;
	
	///////////////////////////////////////////////////////
	
	struct MNGR_DYN_init mngr_dyn = {0, };
	struct MNGR_DYN_init mngr_dyn_read = {0, };
	mngr_dyn.FAST_TH = 0x3F;
	mngr_dyn.FAST = 0;
	NSS_up;
	uint8_t eTag = 0;
	
	uint32_t ecg = 0;
	
	struct CNFG_EMUX_init cnfg_emux = {0, };
	struct CNFG_EMUX_init cnfg_emux_read = {0,};
	
	cnfg_emux.CALN_SEL = 0x00;
	cnfg_emux.CALP_SEL = 0x00;
	cnfg_emux.OPENN = 0;
	cnfg_emux.OPENP = 0;
	cnfg_emux.POL = 0;
	struct CNFG_CAL_init cnfg_cal = { 0,};
	struct CNFG_CAL_init cnfg_cal_read = {0,};
	cnfg_cal.EN_VCAL = 0;
	cnfg_cal.VMAG = 0;
	cnfg_cal.VMODE = 0;
	cnfg_cal.FIFTY = 1;
	cnfg_cal.FCAL = 4;
	cnfg_cal.THIGH_L = 0x07;
	cnfg_cal.THIGH_H = 0xFF;
	struct RO_STATUS statusFIFO = {0,};
	uint8_t spi_rx[3] = {0, };
	uint8_t spi_status[3] = { 0, };
	uint8_t spi_mng_int[4] = { 0, 0x09, 0x00, 0x04 };
	uint32_t eeg_buf[200] = { 0, };
	uint8_t eeg_buf_8bit[36] = {0, };
	uint16_t count = 0;
	uint8_t check = 0;
	
	uint8_t testDMA[33] = { 1, };
	uint8_t up[33] = { 0, };
	for (uint16_t i = 0; i < 33; i++)
	{
		up[i] = 1;
	}
	uint8_t down[33] = { 0, };
	uint16_t countSeq = 0;
	FCLK_on;


  while (1)
	  
  {

	  switch (state)
	  {
	  case test:
		  if (countSeq < 2)
		  {
			  HAL_UART_Transmit(&huart1, down, 33, 100);
			  countSeq++;
		  }
		  else
		  {
			  HAL_UART_Transmit(&huart1, up, 33, 100);
			  countSeq = 0;
		  }
		  
		  HAL_Delay(10);
			
		  break;
	  case Max_reset:
					
		  MAX_SYNCH();
		  MAX_FIFO_RESET();
		  state = Max_init;
		  break;
		  state = Max_init;
	  case Max_init:						
		  MAX_CNFG_CAL_init(cnfg_cal);
		  MAX_CNFG_EMUX_init(cnfg_emux);
		  MAX_CNFG_GEN_init(cnfg_gen);
		  MAX_CNFG_ECG_init(cnfg_ecg);
		  MAX_MNGR_DYN_init(mngr_dyn);
		  uint8_t JDY_init[] = { "AT+NAMEEEG\r\n" };
		  HAL_UART_Transmit(&huart1, JDY_init, 10, 100);

		  cnfg_cal_read = MAX_CNFG_CAL_read();
		  cnfg_emux_read = MAX_CNFG_EMUX_init_read();
		  MAX_WREG(MNGR_INT, spi_mng_int);
		  MAX_RREG(MNGR_INT, spi_rx);
		  FCLK_on;
		  state = Max_read;
		  break;
	  case Max_read:	
		  MAX_RREG(STATUS, spi_status);
		  if ((spi_status[0] &= 0x80) == 0x80)
		  {
			  

			
			  MAX_RREG(STATUS, spi_status);
			  if (count < 12)
			  {
				  check = 1;
				  
				  for (uint16_t i = 0; i < 2; i++) // 2 слова в буфере 
				  {
					  MAX_WriteBuff_8bit(eeg_buf_8bit, count + i);
				  }
				  
				  count += 2;
				  

			  }
			  else
			  {		
				  check = 0;
				  count = 0;
				  HAL_UART_Transmit(&huart1, eeg_buf_8bit, 36, 100);
				  
				  //clearBuff(eeg_buf_8bit, 3);
					  

					  				  
			  }
		  }
		  else
		  {
			  check = 3;
		  }
		  break;
		  }
		 
	  }
  }



void ClearBuff_32bit(uint32_t *buf, uint16_t size)
{
	for (uint16_t i = 0; i < size; i++)
	{
		buf[i] = 0;
	}
}


//Функция принимает младший байт из пакета для проверки Etag
// Valid- valid - 0x00 
//fast mode - time valid / data unvalid - 0x01
//LastValid- valid with delay - 0x02
//LastFast- time valid / data unvalid - 0x03
//Empty - unvalid - 0x06
// OverFlow - unvalid - 0x07
												
												
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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




void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

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
#endif 
