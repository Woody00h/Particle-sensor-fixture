/*
 * ParticleSensor.h
 *
 *  Created on: Sep 25, 2015
 *      Author: wchen
 */

#ifndef PARTICLESENSOR_H_
#define PARTICLESENSOR_H_

#define LITTLE_ENDIAN

typedef unsigned    char    u8;
typedef signed      char    s8;
typedef unsigned    short   u16;
typedef signed      short   s16;
typedef unsigned    int     u32;
typedef signed      int     s32;

#ifdef LITTLE_ENDIAN
#define FRAME_HEAD	0x4d42
#else
#define FRAME_HEAD	0x424d
#endif

#define BUF_LENGTH	40
//particle sensor frame definition
typedef struct
{
	u16 Head;
	u16 Length;
	u16 PM1_0_US;
	u16 PM2_5_US;
	u16 PM10_US;
	u16 PM1_0_CN;
	u16 PM2_5_CN;
	u16 PM10_CN;
	u16 Reserve1;
	u16 Reserve2;
	u16 Reserve3;
	u16 checksum;
}PMFrameField;

typedef union
{
	u8 PMRxBuf[BUF_LENGTH];
	PMFrameField MyPMFrame;
}PMSFrameUnion;

typedef struct
{
	u16 data_pm2_5;
	PMSFrameUnion * PMSUnion;
	u8 HeadFlag;
	u8 HeadPointer;
	u8 DataPtr;
	u8 PMSFrameFlag;
}ParticleSensor;

#ifdef LITTLE_ENDIAN
#define WORD_SWAP(a)	WordSwap(a)
#else
#define WORD_SWAP(a)	a
#endif

#ifdef PMS3003
#define PMS3003_EXTERN
#else
#define PMS3003_EXTERN extern
extern ParticleSensor Sensor1;
extern ParticleSensor Sensor2;
extern ParticleSensor Sensor3;
extern ParticleSensor Sensor4;
extern ParticleSensor Sensor5;
extern ParticleSensor * Sensors[];
#endif //PMS3003

/*
PMS3003_EXTERN u8 HeadFlag;
PMS3003_EXTERN u8 HeadPointer;
PMS3003_EXTERN u8 DataPtr;
PMS3003_EXTERN u8 PMSFrameFlag;
PMS3003_EXTERN PMSFrameUnion MyPMSUnion;

PMS3003_EXTERN u16 data_pm2_5;
PMS3003_EXTERN u16 data_pm1_0;
PMS3003_EXTERN u8 RecTimeoutTimer;
*/
PMS3003_EXTERN u8 FrameCheck(ParticleSensor *sensor);
PMS3003_EXTERN u16 WordSwap(u16 data);

#endif /* PARTICLESENSOR_H_ */
