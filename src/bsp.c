
#define FIXTURE_BSP
#include <stdio.h>
//#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customised.h"
#include "ParticleSensor.h"
#include "bsp.h"

void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}

//unsigned char printbuf[30];
void USART1_IRQHandler(void)
{
	unsigned char rxdata;

	if(USART1->SR & RXNE)
	{
		rxdata = USART1->DR;

		//echo
//		while(!(USART1->SR & TXE));
//		USART1->DR = rxdata;
	}
}

void UartPutChar(unsigned char ch)
{
	while(!(USART1->SR & TXE));
	USART1->DR = ch;
}

u8 Num2Char(u8 num)
{
	if (num > 9)
		return (num-10+'A');
	else
		return (num + '0');
}

void UartPutHexWord(u16 data)
{
	u8 ch;
	u8 temp;
	u16 value = data;

	temp = (value >> 12) & 0x000f;
	ch = Num2Char(temp);
	UartPutChar(ch);

	temp = ((value >> 8) & 0x000f);
	ch = Num2Char(temp);
	UartPutChar(ch);

	temp = ((value >> 4) & 0x000f);
	ch = Num2Char(temp);
	UartPutChar(ch);

	temp = (value & 0x000f);
	ch = Num2Char(temp);
	UartPutChar(ch);

	UartPutChar('\r');

	UartPutChar('\n');
}

void UartPutHexByte(u8 data)
{
	u8 temp;
	u8 ch;

	temp = (data>>4);
	ch = Num2Char(temp);
	UartPutChar(ch);

	temp = (data&0x0f);
	ch = Num2Char(temp);
	UartPutChar(ch);

	UartPutChar('\r');

	UartPutChar('\n');
}

void SendString(unsigned char *buf)
{
	while(*buf)
	{
		UartPutChar(*buf);
		buf++;
	}
}

void SendData(unsigned char *buf, unsigned char length)
{
	while(length)
	{
		UartPutChar(*buf);
		buf++;
		length--;
	}
}

void SysTick_Handler()
{
	static unsigned char LED_On=0;
	/*
	static unsigned int cnt=0;
	cnt++;
	if(cnt == 500)
	{
		cnt = 0;

		LED_On++;
		if(LED_On > 2)
			LED_On = 0;

		if(LED_On == 0)
		{
			LED_On = 0;
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11 | GPIO_PIN_12 ,GPIO_PIN_SET);
		}
		else
		{
			if(LED_On == 1)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11 ,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10 | GPIO_PIN_12 ,GPIO_PIN_SET);
			}
			else
			{
				if(LED_On == 2)
				{
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10 | GPIO_PIN_11 ,GPIO_PIN_SET);
				}
			}
		}
	}*/

	HAL_IncTick();

	if(SendDelayTimer)		SendDelayTimer--;
	if(rh_sample_timer)		rh_sample_timer--;
	if(touch_read_timer)	touch_read_timer--;
	if(backlight_timer)		backlight_timer--;
	if(gui_update_timer)	gui_update_timer--;
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{


	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001)
	{
	/* Enable the Flash prefetch */
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
	__GPIOC_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();

	//LED Init
	GPIO_InitTypeDef LEDs_GPIO;

	LEDs_GPIO.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
	LEDs_GPIO.Pull = GPIO_NOPULL;
	LEDs_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
	LEDs_GPIO.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOD,&LEDs_GPIO);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13,GPIO_PIN_SET);

	//Particle sensors SET signal
	// Sensor 	SET
	// Sensor1  PC3
	// Sensor2	PC2
	// Sensor3	PB9
	// Sensor4	PB8
	// Sensor5	PD3
	LEDs_GPIO.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	LEDs_GPIO.Pull = GPIO_NOPULL;
	LEDs_GPIO.Mode = GPIO_MODE_OUTPUT_OD;
	LEDs_GPIO.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB,&LEDs_GPIO);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9,GPIO_PIN_RESET);

	LEDs_GPIO.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC,&LEDs_GPIO);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2 | GPIO_PIN_3,GPIO_PIN_RESET);

	LEDs_GPIO.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOD,&LEDs_GPIO);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3,GPIO_PIN_RESET);
}


// to PC ternimal
void USART1_Init()
{
	UART_HandleTypeDef USART1_Handle;
	__GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef USART1_GPIO;

	USART1_GPIO.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	USART1_GPIO.Alternate = GPIO_AF7_USART1;
	USART1_GPIO.Speed = GPIO_SPEED_FAST;
	USART1_GPIO.Mode = GPIO_MODE_AF_PP;
	USART1_GPIO.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA,&USART1_GPIO);

	__USART1_CLK_ENABLE();
	USART1_Handle.Instance = USART1;
	USART1_Handle.Init.BaudRate = 115200;
	USART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	USART1_Handle.Init.StopBits = UART_STOPBITS_1;
	USART1_Handle.Init.Parity = UART_PARITY_NONE;
	USART1_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART1_Handle.Init.Mode = UART_MODE_TX_RX;
	USART1_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&USART1_Handle);
	USART1->CR1 = 0x202C;

	HAL_NVIC_SetPriority(USART1_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

//sensor 3
void UART4_Init()
{
	UART_HandleTypeDef UART4_Handle;
	GPIO_InitTypeDef UART4_GPIO;

	__GPIOC_CLK_ENABLE();

	UART4_GPIO.Pin = GPIO_PIN_11 | GPIO_PIN_10;
	UART4_GPIO.Alternate = GPIO_AF8_UART4;
	UART4_GPIO.Speed = GPIO_SPEED_FAST;
	UART4_GPIO.Mode = GPIO_MODE_AF_PP;
	UART4_GPIO.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC,&UART4_GPIO);

	__UART4_CLK_ENABLE();
	UART4_Handle.Instance = UART4;
	UART4_Handle.Init.BaudRate = 9600;
	UART4_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART4_Handle.Init.StopBits = UART_STOPBITS_1;
	UART4_Handle.Init.Parity = UART_PARITY_NONE;
	UART4_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART4_Handle.Init.Mode = UART_MODE_TX_RX;
	UART4_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&UART4_Handle);
	UART4->CR1 = 0x202C;

	HAL_NVIC_SetPriority(UART4_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);
}

//sensor 4
void USART6_Init()
{
	UART_HandleTypeDef USART6_Handle;
	GPIO_InitTypeDef USART6_GPIO;

	__GPIOC_CLK_ENABLE();

	USART6_GPIO.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	USART6_GPIO.Alternate = GPIO_AF8_USART6;
	USART6_GPIO.Speed = GPIO_SPEED_FAST;
	USART6_GPIO.Mode = GPIO_MODE_AF_PP;
	USART6_GPIO.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC,&USART6_GPIO);

	__USART6_CLK_ENABLE();
	USART6_Handle.Instance = USART6;
	USART6_Handle.Init.BaudRate = 9600;
	USART6_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	USART6_Handle.Init.StopBits = UART_STOPBITS_1;
	USART6_Handle.Init.Parity = UART_PARITY_NONE;
	USART6_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART6_Handle.Init.Mode = UART_MODE_TX_RX;
	USART6_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&USART6_Handle);
	USART6->CR1 = 0x202C;

	HAL_NVIC_SetPriority(USART6_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
}

//sensor 1
void USART2_Init()
{
	UART_HandleTypeDef USART2_Handle;
	GPIO_InitTypeDef USART2_GPIO;

	__GPIOD_CLK_ENABLE();

	USART2_GPIO.Pin = GPIO_PIN_6 | GPIO_PIN_5;
	USART2_GPIO.Alternate = GPIO_AF7_USART2;
	USART2_GPIO.Speed = GPIO_SPEED_FAST;
	USART2_GPIO.Mode = GPIO_MODE_AF_PP;
	USART2_GPIO.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOD,&USART2_GPIO);

	__USART2_CLK_ENABLE();
	USART2_Handle.Instance = USART2;
	USART2_Handle.Init.BaudRate = 9600;
	USART2_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	USART2_Handle.Init.StopBits = UART_STOPBITS_1;
	USART2_Handle.Init.Parity = UART_PARITY_NONE;
	USART2_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART2_Handle.Init.Mode = UART_MODE_TX_RX;
	USART2_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&USART2_Handle);
	USART2->CR1 = 0x202C;

	HAL_NVIC_SetPriority(USART2_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

//sensor 5
void USART3_Init()
{
	UART_HandleTypeDef USART3_Handle;
	GPIO_InitTypeDef USART3_GPIO;
	//PD8,PD9
	__GPIOD_CLK_ENABLE();

	USART3_GPIO.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	USART3_GPIO.Alternate = GPIO_AF7_USART3;
	USART3_GPIO.Speed = GPIO_SPEED_FAST;
	USART3_GPIO.Mode = GPIO_MODE_AF_PP;
	USART3_GPIO.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOD,&USART3_GPIO);

	__USART3_CLK_ENABLE();
	USART3_Handle.Instance = USART3;
	USART3_Handle.Init.BaudRate = 9600;
	USART3_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	USART3_Handle.Init.StopBits = UART_STOPBITS_1;
	USART3_Handle.Init.Parity = UART_PARITY_NONE;
	USART3_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART3_Handle.Init.Mode = UART_MODE_TX_RX;
	USART3_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&USART3_Handle);
	USART3->CR1 = 0x202C;

	HAL_NVIC_SetPriority(USART3_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

//sensor 2
void UART5_Init()
{
	UART_HandleTypeDef UART5_Handle;
	GPIO_InitTypeDef UART5_GPIO;

	//PD2
	__GPIOD_CLK_ENABLE();
	UART5_GPIO.Pin = GPIO_PIN_2;
	UART5_GPIO.Alternate = GPIO_AF8_UART5;
	UART5_GPIO.Speed = GPIO_SPEED_FAST;
	UART5_GPIO.Mode = GPIO_MODE_AF_PP;
	UART5_GPIO.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD,&UART5_GPIO);

	//PC12
	__GPIOC_CLK_ENABLE();
	UART5_GPIO.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOC,&UART5_GPIO);

	__UART5_CLK_ENABLE();
	UART5_Handle.Instance = UART5;
	UART5_Handle.Init.BaudRate = 9600;
	UART5_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART5_Handle.Init.StopBits = UART_STOPBITS_1;
	UART5_Handle.Init.Parity = UART_PARITY_NONE;
	UART5_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART5_Handle.Init.Mode = UART_MODE_TX_RX;
	UART5_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&UART5_Handle);
	UART5->CR1 = 0x202C;

	HAL_NVIC_SetPriority(UART5_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(UART5_IRQn);
}

void I2C_Init()
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable GPIO Clocks #################################################*/
  /* Enable GPIO TX/RX clock PB6 PB7 */
  __GPIOB_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* I2C GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL; //extern pull up
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*##-3- Enable I2C peripherals Clock #######################################*/
  /* Enable I2C1 clock */
  __I2C1_CLK_ENABLE();

  I2cHandle.Instance             = I2C1;

	I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	I2cHandle.Init.ClockSpeed      = 100000;
	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	I2cHandle.Init.OwnAddress1     = 0x55;
	I2cHandle.Init.OwnAddress2     = 0;

	if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
	{
	  /* Initialization Error */
//	  Error_Handler();
	}

  /*##-4- Configure the NVIC for I2C #########################################*/
  /* NVIC for I2C1 */
  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
}

/**
  * @brief  This function handles I2C event interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C data transmission
  */
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&I2cHandle);
}

/**
  * @brief  This function handles I2C error interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C error
  */
void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&I2cHandle);
}

void mymemcopy(u8 * dst, u8 * src, u8 size)
{
	while(size)
	{
		*dst = *src;
		src++;
		dst++;
		size--;
	}
}

u8 mystrcmp(unsigned char * str1,unsigned char * str2)
{
	do
	{
		if(*str1 != *str2)
			return 1;
		str2++;
	}
	while(*str1++);

	return 0;
}
