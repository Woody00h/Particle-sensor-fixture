#define SI7020
#include "Si7020.h"
#include "stm32f4xx_hal.h"
#include "bsp.h"

#define QUARTER_SEC_TIMER_RELOAD 250

//
//Measure Relative Humidity, Hold Master Mode
//
u8 Si7020SendCommand(u8 cmd)
{
	HAL_StatusTypeDef ret;
	u8 sendbuf[] = {cmd};

	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);

	if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)SI7020_ADDR, sendbuf, 1) != HAL_OK)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	//return(ISendByte(SI7020_ADDR,cmd));
}

//
//read the result of the RH measurement.
//
u8 Si7020Read_RH_NHM(u8 * buffer)
{
	HAL_StatusTypeDef ret;

	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);

	if(HAL_I2C_Master_Receive_IT(&I2cHandle,(uint16_t)SI7020_ADDR, buffer, 3) != HAL_OK)
	{
		return 0;
	}
	else
	{
		//wait read finish
		while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
		return 1;
	}
}

//
//read the temperature result of the pre RH measurement
//
u8 Si7020Read_Temp_after_RHM(u8 * buffer)
{
	u8 ret;
	ret = Si7020SendCommand(RD_TEMP_PRE_RHM);
	
	if (!ret)
	{
		SendString("RD CMD FAIL\n");
		return 0;
	}
	
	//wait for ready
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);

	if(HAL_I2C_Master_Receive_IT(&I2cHandle,(uint16_t)SI7020_ADDR, buffer, 2) != HAL_OK)
	{
		SendString("RD BUF FAIL\n");
		return 0;
	}
	else
	{
		//wait for read finish
		while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
		return 1;
	}
}

//
//transfrom the raw data to the temperature value
//175.72*Temp_Code/65536-46.85
//
s8 Si7020CalcTemp(u16 data)
{
	s32 temp;
	s8 result;
	
//	RTS_H;
//	M8C_DisableGInt;
	
	temp = data;
	temp = temp*175;
	temp += (data>>2)*3;
	temp >>= 16;
	temp -= 47;
	if (temp < -9)
		temp = -9;
	
	result = temp;
	
//	M8C_EnableGInt;
//	RTS_L;
	return result;
}

//
//transfrom the raw data to the RH value
//125*Temp_Code/65536-6
//
u8 Si7020CalcRH(u16 data)
{
	u32 temp;
	u8 result;
	
//	RTS_H;
//	M8C_DisableGInt;
	
	temp = data;
	temp = 125*temp;
	temp >>= 16;
	if (temp >= 6)
	{
		if (temp >= 106)
		{
			temp = 100;
		}
		else
		{
			temp -= 6;
		}
	}
	else 
	{
		temp = 0;
	}
	
	result = temp;
//	M8C_EnableGInt;
//	RTS_L;
	return result;
}


void Si7020Init()
{
	u8 my_data[2];
	my_data[0] = WR_REG1;
	my_data[1] = 0;
//	ISendBytes(SI7020_ADDR,my_data,2);
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)SI7020_ADDR, my_data, 2);
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	data_length = 24;
}

void LeftRotate()
{
	u8 k;
	for(k=0;k<3;k++)
	{
		RecBuf[k] <<= 1;
		if(k < 2)
		{
			RecBuf[k] |= (RecBuf[k+1]&0x80)?1:0;
		}
	}
	data_length --;
}

u8 CRC8Check()
{
	while(1)
	{
		if(RecBuf[0]&0x80)
		{
			RecBuf[0] ^= poly_h;
			RecBuf[1] ^= poly_l;
			LeftRotate();
		}
		else
		{
			LeftRotate();
		}

		if(data_length <= 8)
			break;
	}
	
	data_length = 24;
	
	if(!RecBuf[0])
	{
		return 1;	//check pass
	}
	else
	{
		return 0; //check fail
	}
}

