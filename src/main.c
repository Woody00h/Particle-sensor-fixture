//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customised.h"
#include "ParticleSensor.h"
#include "bsp.h"
#include "TFT-ATK4-3.h"
#include "GUI.h"
#include "LCD_ConfDefaults.h"      /* valid LCD configuration */
#include "Si7020.h"
#include "touch.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define ONE_SECOND_TIMER_RELOAD		1000
#define TEN_SEC_TIMER_RELOAD		10000
#define THREE_SEC_TIMER_RELOAD		3000
#define FIVE_SEC_TIMER_RELOAD		5000
#define HALF_MIN_TIMER_RELOAD		30000
#define ONE_MIN_TIMER_RELOAD		60000

USBD_HandleTypeDef USBD_Device;
unsigned short USB_Send_buf[10];
unsigned char RHSampleStep;
unsigned char ret;
unsigned char touch_pre_state;
extern GUI_BITMAP bm5;
extern GUI_BITMAP bm6;

/*
#if GUIDEMO_LARGE
  #define YSIZE   (LCD_YSIZE - 100)
#else
  #define YSIZE   (LCD_YSIZE - 30)
#endif
*/
#define YSIZE	400
#define DEG2RAD (3.1415926f / 180)

#if LCD_BITSPERPIXEL == 1
  #define COLOR_GRAPH0 GUI_WHITE
  #define COLOR_GRAPH1 GUI_WHITE
#else
  #define COLOR_GRAPH0 GUI_GREEN
  #define COLOR_GRAPH1 GUI_YELLOW
#endif

extern void MainTask(void);

static void _Label(void)
{
  int x, y;
  GUI_SetPenSize(1);
  GUI_ClearRect(0, (LCD_YSIZE - 21) - YSIZE, (LCD_XSIZE - 1), (LCD_YSIZE - 1));
  GUI_DrawRect(30, (LCD_YSIZE - 21) - YSIZE, (LCD_XSIZE - 1), (LCD_YSIZE - 20));
  GUI_SetFont(&GUI_Font6x8);

  for (x = 0; x < (LCD_XSIZE - 20); x += 40)
  {
    int xPos = x + 30;
    //x axis
    GUI_DrawVLine(xPos, (LCD_YSIZE - 20), (LCD_YSIZE - 14));
    GUI_DispDecAt(x / 40, xPos - 2, (LCD_YSIZE - 9), 2);
  }

/*  for (y = 0; y < YSIZE / 2; y += 20)
  {
    int yPos = (LCD_YSIZE - 20) - YSIZE / 2 + y;
    GUI_DrawHLine(yPos, 13, 18);
    if (y)
    {
      GUI_GotoXY(1, yPos - 4);
      GUI_DispSDec(-y / 20, 2);
      yPos = (LCD_YSIZE - 20) - YSIZE / 2 - y;
      GUI_DrawHLine(yPos, 13, 18);
      GUI_GotoXY(1, yPos - 4);
      GUI_DispSDec(y / 20, 2);
    }
    else
    {
      GUI_DispCharAt('0', 7, yPos - 4);
    }
  }*/
  int yPos = LCD_YSIZE - 20;
  u8 i=0;
  for(;yPos>(LCD_YSIZE - 21) - YSIZE;yPos-=20)
  {
	  GUI_DrawHLine(yPos, 19, 24);
	  GUI_GotoXY(1, yPos - 4);
	  GUI_DispDec(i*10, 3);
	  i++;
  }

  GUI_SetFont(&GUI_Font32_ASCII);
  GUI_SetColor(GUI_RED);
  GUI_DispStringHCenterAt("Particle Sensor Fixture",240,100);

  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_SetColor(GUI_LIGHTYELLOW);
  GUI_DispStringAt("Sensor 1:",30,150);
  GUI_DispStringAt("Sensor 2:",30,180);
  GUI_DispStringAt("Sensor 3:",30,210);
  GUI_DispStringAt("Sensor 4:",30,240);
  GUI_DispStringAt("Sensor 5:",30,270);
  GUI_DispStringAt("Temperature:",30,300);
  GUI_DispStringAt("Humidity:",30,330);
}

int main(void)
{
  // At this stage the system clock should have already been configured
  // at high speed.

	unsigned char k,x;
	unsigned short pos;
	HAL_Init();
	MX_GPIO_Init();
	SystemClock_Config();
	USART1_Init();
	USART2_Init();
	USART3_Init();
	USART6_Init();
	UART4_Init();
	UART5_Init();
	SendString("Woody is a genius!\n");

	/* Init Device Library */
	USBD_Init(&USBD_Device, &HID_Desc, 0);
	/* Add Supported Class */
	USBD_RegisterClass(&USBD_Device, USBD_CUSTOMISED_CLASS);
	/* Start Device Process */
	USBD_Start(&USBD_Device);
	SendString("USB config finish!\n");

	//LCD Init
	GUI_Init();
	_Label();

	I2C_Init();
	Si7020Init();

	tp_dev.init();

	//start the sensors in sequence
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3,GPIO_PIN_SET);

	SendDelayTimer = THREE_SEC_TIMER_RELOAD;
	rh_sample_timer = ONE_SECOND_TIMER_RELOAD;
	backlight_timer = HALF_MIN_TIMER_RELOAD;

//	GUI_CURSOR_Show();
//	GUI_DrawBitmap(&bm5,0,0);
//	GUI_DrawBitmap(&bm6,200,400);

	// Infinite loop
	while (1)
    {
       // Add your code here.
		asm("WFI");
		/******************************************************************************************/
		/***************************Check every sensor*********************************************/
		/******************************************************************************************/
		pos = 150;
		for(k=0; k<5; k++)
		{
			if(Sensors[k]->PMSFrameFlag)
			{
				Sensors[k]->PMSFrameFlag = 0;
				if(FrameCheck(Sensors[k]))
				{
					Sensors[k]->data_pm2_5 = WORD_SWAP(Sensors[k]->PMSUnion->MyPMFrame.PM2_5_US);

					USB_Send_buf[k*2] = 1;

					if(USB_Send_buf[k*2+1])
					{
						USB_Send_buf[k*2+1] = ((Sensors[k]->data_pm2_5>>2)*3) + (USB_Send_buf[k*2+1]>>2);
					}
					else
					{
						USB_Send_buf[k*2+1] = Sensors[k]->data_pm2_5;
					}
				}

				GUI_DispDecAt(Sensors[k]->data_pm2_5,200,pos,3);
			}
			pos += 30;
		}

		/******************************************************************************************/
		/***************************Send the PM2.5 data to PC *************************************/
		/******************************************************************************************/
		if(!SendDelayTimer)
		{
			SendDelayTimer = FIVE_SEC_TIMER_RELOAD;
//			SendDelayTimer = ONE_MIN_TIMER_RELOAD;
			USBD_HID_SendReport(&USBD_Device, (uint8_t *)USB_Send_buf,20);
//			SendData((uint8_t *)USB_Send_buf,20);
			memset((uint8_t *)USB_Send_buf,0,20);
		}

		/******************************************************************************************/
		/***************************Measure the RH and Temperature*********************************/
		/******************************************************************************************/
		if(!rh_sample_timer)
		{
			rh_sample_timer = ONE_SECOND_TIMER_RELOAD;
			if (!RHSampleStep)
			{
				ret = Si7020SendCommand(MRH_NHMM); // send the command(Measure RH, No Hold Master Mode)
				if(ret)
				{
					RHSampleStep = 1;
				}
				else
				{
//					SendString("Command fail\n");
				}
			}
			else
			{
				//read RH
				ret = Si7020Read_RH_NHM(RecBuf);
				if(!ret)
				{
//					SendString("Read RH fail\n");
				}
				/*UartPutHexByte(RecBuf[0]);
				UartPutHexByte(RecBuf[1]);
				UartPutHexByte(RecBuf[2]);*/
				Si7020Data = RecBuf[0];
				Si7020Data <<= 8;
				Si7020Data += RecBuf[1];

//				UartPutHexWord(Si7020Data);
				//CRC Check
				if (CRC8Check())
				{
					Humidity = Si7020CalcRH(Si7020Data);
					/*print some info*/
//					SendString("Humidity: ");
//					UartPutHexByte(Humidity);
					GUI_DispDecAt(Humidity,200,330,2);
				}
				else
				{
//					SendString("CRC FAIL!\n");
				}

				//read temperature
				ret = Si7020Read_Temp_after_RHM(RecBuf);
				if(!ret)
				{
					SendString("Read Temp fail\n");
				}
				/*UartPutHexByte(RecBuf[0]);
				UartPutHexByte(RecBuf[1]);
				UartPutHexByte(RecBuf[2]);*/
				Si7020Data = RecBuf[0];
				Si7020Data <<= 8;
				Si7020Data += RecBuf[1];

				Temperature = Si7020CalcTemp(Si7020Data);
				/*print some info*/
//				SendString("Temperature: ");
//                UartPutHexWord(Si7020Data);
//                UartPutHexByte(Temperature);

                RHSampleStep = 0;
                GUI_GotoXY(200,300);
                GUI_DispSDec(Temperature,3);
			}
		}
		/******************************************************************************************/
		/***************************Read the CAP TOUCH Position************************************/
		/******************************************************************************************/
		if(!touch_read_timer)
		{
			touch_read_timer = ONE_SECOND_TIMER_RELOAD;
			tp_dev.scan(0);
			for(k=0;k<CT_MAX_TOUCH;k++)
			{
				if(tp_dev.sta & (1<<k))
				{
//					UartPutChar('X');UartPutChar('0'+k);UartPutChar(':');UartPutHexWord(tp_dev.x[k]);
//					UartPutChar('Y');UartPutChar('0'+k);UartPutChar(':');UartPutHexWord(tp_dev.y[k]);
					BACK_LIGHT_ON;
					backlight_timer = HALF_MIN_TIMER_RELOAD;
				}
			}
		}
/*		if(!gui_update_timer)
		{
			gui_update_timer = 50;
			GUI_TOUCH_Exec();
			GUI_Exec();
		}*/
		if(!backlight_timer)
		{
			BACK_LIGHT_OFF;
		}

		/*if(tp_dev.sta != touch_pre_state)
		{
			if(!(tp_dev.sta & 0x80) && (touch_pre_state & 0x80))
			{
				GUI_DrawBitmap(pictures[x],0,0);
				x++;
				if(x > 1)
					x = 0;
			}
			touch_pre_state = tp_dev.sta;
		}*/
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
