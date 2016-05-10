/*
 * bsp.h
 *
 *  Created on: Oct 9, 2015
 *      Author: wchen
 */

#ifndef BSP_H_
#define BSP_H_

#define RXNE 	0x20
#define TXE		0x80

#ifdef FIXTURE_BSP
#define BSP_EXTERN
#else
#define BSP_EXTERN extern
#endif

void UartPutChar(unsigned char ch);
u8 Num2Char(u8 num);
void UartPutHexWord(u16 data);
void UartPutHexByte(u8 data);
void SendString(unsigned char *buf);
void SendData(unsigned char *buf, unsigned char length);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void UART4_Init();
void UART5_Init();
void USART1_Init();
void USART2_Init();
void USART3_Init();
void USART6_Init();
void I2C_Init();
void mymemcopy(u8 * dst, u8 * src, u8 size);
u8 mystrcmp(unsigned char * str1,unsigned char * str2);

extern PCD_HandleTypeDef hpcd;

BSP_EXTERN unsigned short SendDelayTimer;
BSP_EXTERN unsigned short IICTimeOutTimer;
BSP_EXTERN unsigned short rh_sample_timer;
BSP_EXTERN unsigned short touch_read_timer;
BSP_EXTERN unsigned short backlight_timer;
BSP_EXTERN unsigned short gui_update_timer;
/* I2C handler declaration */
BSP_EXTERN I2C_HandleTypeDef I2cHandle;
#endif /* BSP_H_ */
