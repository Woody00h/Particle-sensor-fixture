//*****************************************************************************
//
// kitronix320x240x16_ssd2119_8bit.h - Prototypes for the Kitronix K350QVG-V1-F
//                                     display driver with an SSD2119
//                                     controller.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 6075 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************

#ifndef __KITRONIX320X240X16_SSD2119_8BIT_H__
#define __KITRONIX320X240X16_SSD2119_8BIT_H__


//
// Read start bit.  This is ORed with LCD_COMMAND_PORT or LCD_DATA_PORT to
// initiate a read request from the LCD controller.
//
#define LCD_READ_START      0x00000004


//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void Kitronix320x240x16_SSD2119Init(void);
extern const tDisplay g_sKitronix320x240x16_SSD2119;
extern void Kitronix320x240x16_SSD2119SetLCDControl(unsigned char ucMask,
                                                    unsigned char ucVal);

#endif // __KITRONIX320X240X16_SSD2119_H__
