#ifndef _SI7020_
#define _SI7020_

typedef unsigned    char    u8;
typedef signed      char    s8;
typedef unsigned    short   u16;
typedef signed      short   s16;
typedef unsigned    int     u32;
typedef signed      int     s32;
    
//command table
#define MRH_HMM				0xE5	//Measure Relative Humidity, Hold Master Mode
#define MRH_NHMM			0xF5	//Measure Relative Humidity, No Hold Master Mode
#define MT_HMM				0xE3	//Measure Temperature, Hold Master Mode
#define MT_NHMM				0xF3	//Measure Temperature, No Hold Master Mode
#define RD_TEMP_PRE_RHM		0xE0	//Read Temperature Value from Previous RH Measurement
#define SENSOR_RESET		0xFE	//Reset
#define WR_REG1				0xE6	//Write RH/T User Register 1
#define RD_REG1				0xE7	//Read RH/T User Register 1
#define WR_HEAT_CTL_REG		0x51	//Write Heater Control Register
#define RD_HEAT_CTL_REG		0x11	//Read Heater Control Register
#define RD_ID_BYTE_1		0xFA0F	//Read Electronic ID 1st Byte
#define RD_ID_BYTE_2		0xFC09	//Read Electronic ID 2nd Byte
#define RD_FW_REV			0x84B8	//Read Firmware Revision

#define SI7020_ADDR			0x80

/*typedef union
{
    u16 HumidityData;
    u16 TemperatureData;
    u8  RecBuf[5];
}Si7020Union;*/

#ifdef SI7020
#define SI7020_EXTERN 
u8 poly_h=0x98;//10011000b;
u8 poly_l=0x80;//1b;
u8 data_length;
#else 
#define SI7020_EXTERN extern 
#endif 


SI7020_EXTERN u8 Si7020SendCommand(u8 cmd);
SI7020_EXTERN u8 Si7020Read_RH_NHM(u8 * buffer);
SI7020_EXTERN u8 Si7020Read_Temp_after_RHM(u8 * buffer);
SI7020_EXTERN s8 Si7020CalcTemp(u16 data);
SI7020_EXTERN u8 Si7020CalcRH(u16 data);
SI7020_EXTERN u8 CRC8Check();
SI7020_EXTERN void LeftRotate();
SI7020_EXTERN void Si7020Init();

SI7020_EXTERN u8 Humidity;
SI7020_EXTERN s8 Temperature;
SI7020_EXTERN u8 RecBuf[5];
//SI7020_EXTERN Si7020Union MySi7020Union;
SI7020_EXTERN u16  Si7020Data;

#endif
