/*
 * ParticleSensor.c
 *
 *  Created on: Sep 25, 2015
 *      Author: wchen
 */

#define PMS3003
#include "stm32f4xx_hal.h"
#include "ParticleSensor.h"
/*
 *NOTE:This code is for the Particle sensor PMS3003.
 *     Do not apply it on other particle sensor.
 */

#define RECV_TIMEOUT_RELOAD 9
#define RXNE 	0x20
#define TXE		0x80

PMSFrameUnion Sensor1Union;
PMSFrameUnion Sensor2Union;
PMSFrameUnion Sensor3Union;
PMSFrameUnion Sensor4Union;
PMSFrameUnion Sensor5Union;

ParticleSensor Sensor1 = {0, &Sensor1Union, 0, 0, 0, 0,};
ParticleSensor Sensor2 = {0, &Sensor2Union, 0, 0, 0, 0,};
ParticleSensor Sensor3 = {0, &Sensor3Union, 0, 0, 0, 0,};
ParticleSensor Sensor4 = {0, &Sensor4Union, 0, 0, 0, 0,};
ParticleSensor Sensor5 = {0, &Sensor5Union, 0, 0, 0, 0,};

ParticleSensor * Sensors[5] = {&Sensor1, &Sensor2, &Sensor3, &Sensor4, &Sensor5};

void Sensor_Recv_Handler(ParticleSensor *sensor, u8 rxdata)
{
	if(sensor->HeadFlag == 0)
	{
		//head receive
		if(sensor->DataPtr < 2)
		{
			sensor->PMSUnion->PMRxBuf[sensor->DataPtr] = rxdata;
			sensor->DataPtr ++;
		}
		else
		{
#ifdef LITTLE_ENDIAN
			sensor->PMSUnion->MyPMFrame.Head >>= 8;
#else
			sensor->PMSUnion->MyPMFrame.Head <<= 8;
#endif
			sensor->PMSUnion->PMRxBuf[1] = rxdata;
			sensor->DataPtr = 2;
		}

		//head check
		if( sensor->DataPtr == 2 && sensor->PMSUnion->MyPMFrame.Head == FRAME_HEAD)
		{
			sensor->HeadFlag = 1;
		}
	}
	else
	{
		sensor->PMSUnion->PMRxBuf[sensor->DataPtr] = rxdata;
		if(sensor->DataPtr >= (BUF_LENGTH-1))
		{
			sensor->HeadFlag = 0;
			sensor->DataPtr = 0;
			sensor->PMSUnion->MyPMFrame.Length = 0;
			sensor->PMSUnion->MyPMFrame.Head = 0;
		}
		else
		{
			sensor->DataPtr ++;
		}

		if(sensor->DataPtr >= 6 && \
		   sensor->DataPtr == (WORD_SWAP(sensor->PMSUnion->MyPMFrame.Length) + 4))
		{
			sensor->PMSFrameFlag = 1;
			sensor->HeadFlag = 0;
			sensor->DataPtr = 0;
		}
	}
}
//sensor 1
void USART2_IRQHandler()
{
	u8 rxdata;

	if(!(USART2->SR & RXNE))
		return;

	rxdata = USART2->DR;

	Sensor_Recv_Handler(&Sensor1, rxdata);
}

//sensor 5
void USART3_IRQHandler()
{
	u8 rxdata;

	if(!(USART3->SR & RXNE))
		return;

	rxdata = USART3->DR;

	Sensor_Recv_Handler(&Sensor5, rxdata);
}

//sensor 3
void UART4_IRQHandler()
{
	u8 rxdata;

	if(!(UART4->SR & RXNE))
		return;

	rxdata = UART4->DR;

	Sensor_Recv_Handler(&Sensor3, rxdata);
/*
	if(Sensor3.HeadFlag == 0)
	{
		//head receive
		if(Sensor3.DataPtr < 2)
		{
			(Sensor3.PMSUnion)->PMRxBuf[Sensor3.DataPtr] = rxdata;
			Sensor3.DataPtr ++;
		}
		else
		{
#ifdef LITTLE_ENDIAN
			(Sensor3.PMSUnion)->MyPMFrame.Head >>= 8;
#else
			(Sensor3.PMSUnion)->MyPMFrame.Head <<= 8;
#endif
			(Sensor3.PMSUnion)->PMRxBuf[1] = rxdata;
			Sensor3.DataPtr = 2;
		}

		//head check
		if( Sensor3.DataPtr == 2 && (Sensor3.PMSUnion)->MyPMFrame.Head == FRAME_HEAD)
		{
			Sensor3.HeadFlag = 1;
		}
	}
	else
	{
		(Sensor3.PMSUnion)->PMRxBuf[Sensor3.DataPtr] = rxdata;
		if(Sensor3.DataPtr >= (BUF_LENGTH-1))
		{
			Sensor3.HeadFlag = 0;
			Sensor3.DataPtr = 0;
			(Sensor3.PMSUnion)->MyPMFrame.Length = 0;
			(Sensor3.PMSUnion)->MyPMFrame.Head = 0;
		}
		else
		{
			Sensor3.DataPtr ++;
		}

		if(Sensor3.DataPtr >= 6 && Sensor3.DataPtr == (WORD_SWAP((Sensor3.PMSUnion)->MyPMFrame.Length) + 4))
		{
			Sensor3.PMSFrameFlag = 1;
			Sensor3.HeadFlag = 0;
			Sensor3.DataPtr = 0;
		}
	}*/
}

//sensor 2
void UART5_IRQHandler()
{
	u8 rxdata;

	if(!(UART5->SR & RXNE))
		return;

	rxdata = UART5->DR;

	Sensor_Recv_Handler(&Sensor2, rxdata);
}

//sensor 4
void USART6_IRQHandler()
{
	u8 rxdata;

    if(!(USART6->SR & RXNE))
    	return;

    rxdata = USART6->DR;

    Sensor_Recv_Handler(&Sensor4, rxdata);
}

/*
*check sum of the frame.
*return 1-pass
*return 0-fail
*/
u8 FrameCheck(ParticleSensor *sensor)
{
	u16 checksum = 0;
	u8 i = 0;
	u16 len = WORD_SWAP(sensor->PMSUnion->MyPMFrame.Length);

	for (i = 0; i < (len+2); i++)
	{
		checksum += sensor->PMSUnion->PMRxBuf[i];
	}

	sensor->PMSUnion->MyPMFrame.Length = 0;
	sensor->PMSUnion->MyPMFrame.Head = 0;

	if (checksum == WORD_SWAP(sensor->PMSUnion->MyPMFrame.checksum))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

u16 WordSwap(u16 data)
{
	return (data<<8)|(data>>8);
}
