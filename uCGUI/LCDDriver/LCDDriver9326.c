/*
*********************************************************************************************************
*                                             uC/GUI V3.98
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDDriver9326.C
Purpose     : LCD驱动文件 2011.08.22
----------------------------------------------------------------------   
Version-Date---Author-Explanation                                        
----------------------------------------------------------------------   
0.90.00 030627 JE     a) First release
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>
#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"
#include "includes.h"

/****************************************************************************
*                           ILI9326 LCD IO口定义
* 16位数据   Data_H  P9      Data_L  P8 
* LCD_CS     P56 低电平有效
* LCD_RS     P57 低电平有效
* LCD_RST    P64 
* LCD_WR     P72 低电平有效
* LCD_RD     P73 低电平有效
****************************************************************************/
#define LCD_DATA_H_DIR       P9DIR          // LCD 数据 高8位                
#define LCD_DATA_L_DIR       P8DIR          // LCD 数据 低8位

#define LCD_DATA_H_OUT       P9OUT          // LCD 数据 高8位                
#define LCD_DATA_L_OUT       P8OUT          // LCD 数据 低8位 

#define LCD_DATA_H_IN        P9IN           // LCD 数据 高8位                
#define LCD_DATA_L_IN        P8IN           // LCD 数据 低8位 

#define LCD_CS_PORT          P5             // CS 
#define LCD_CS_BIT           BIT6           // BIT6位 

#define LCD_RS_PORT          P5             // RS 
#define LCD_RS_BIT           BIT7           // BIT7位 

#define LCD_RST_PORT         P6             // RST 
#define LCD_RST_BIT          BIT4           // BIT4位 

#define LCD_WR_PORT          P7             // WR 
#define LCD_WR_BIT           BIT2           // BIT2位 

#define LCD_RD_PORT          P7             // RD 
#define LCD_RD_BIT           BIT3           // BIT3位 

#define LCD_BL_PORT          P11            // BL 
#define LCD_BL_BIT           BIT2           // BIT2位 
/****************************************************************************
* 名    称：void LCD_IO_INT(void)
* 功    能：LCD驱动IO口初始化
* 入口参数：无
* 出口参数：无
* 说    明：16为数据线和其他设备共用
* 调用方法：LCD_IO_INT();
****************************************************************************/
void LCD_IO_INT(void)
{ 
  LCD_DATA_H_DIR = 0xFF;
  LCD_DATA_H_OUT = 0xFF;
  
  LCD_DATA_L_DIR = 0xFF;
  LCD_DATA_L_OUT = 0xFF;
  
  BIT_OUT( LCD_CS );
  SET_BIT( LCD_CS );
  
  BIT_OUT( LCD_RS );
  SET_BIT( LCD_RS );
  
  BIT_OUT( LCD_RST );
  SET_BIT( LCD_RST );
  
  BIT_OUT( LCD_WR );
  SET_BIT( LCD_WR );
  
  BIT_OUT( LCD_RD );
  SET_BIT( LCD_RD );
}
/****************************************************************************
* 名    称：void ILI9326_Delay(unsigned int nCount)
* 功    能：延时
* 入口参数：nCount   延时值
* 出口参数：无
* 说    明：
* 调用方法：ILI9326_Delay(10000);
****************************************************************************/
void ILI9326_Delay(unsigned int nCount)
{
  for(; nCount != 0; nCount--);
}
/****************************************************************************
* 名    称：void ILI9326_WriteIndex(unsigned int idx)
* 功    能：写 ILI9326 控制器寄存器地址
* 入口参数：idx   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
* 调用方法：ILI9326_WriteIndex(0x0000);
****************************************************************************/
void ILI9326_WriteIndex(unsigned int idx)
{
  CLR_BIT( LCD_CS );//LCD 片选
  SET_BIT( LCD_RD ); 
  CLR_BIT( LCD_RS );
  LCD_DATA_H_DIR = 0xFF;
  LCD_DATA_L_DIR = 0xFF;
  LCD_DATA_H_OUT = idx >> 8;
  LCD_DATA_L_OUT = idx;
  CLR_BIT( LCD_WR );
  SET_BIT( LCD_WR );
  SET_BIT( LCD_RS );
  SET_BIT( LCD_CS );//LCD 片选
}

/****************************************************************************
* 名    称：void ILI9326_WriteData(unsigned int dat)
* 功    能：写 ILI9326 寄存器数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，调用前需先写寄存器地址，内部函数
* 调用方法：ILI9326_WriteData(0x1030)
****************************************************************************/
void ILI9326_WriteData(unsigned int dat)
{
  CLR_BIT( LCD_CS );//LCD 片选
  SET_BIT( LCD_RS );
  SET_BIT( LCD_RD );
  LCD_DATA_H_DIR = 0xFF;
  LCD_DATA_L_DIR = 0xFF;
  LCD_DATA_H_OUT = dat >> 8;
  LCD_DATA_L_OUT = dat;
  CLR_BIT( LCD_WR );  
  SET_BIT( LCD_WR );
  SET_BIT( LCD_CS );//LCD 片选 
}
/****************************************************************************
* 名    称：unsigned int ILI9326_ReadData(void)
* 功    能：读取控制器数据
* 入口参数：无
* 出口参数：返回读取到的数据
* 说    明：内部函数
* 调用方法：i=ILI9326_ReadData();
****************************************************************************/
unsigned int ILI9326_ReadData(void)
{
  unsigned int val=0;
  CLR_BIT( LCD_CS );//LCD 片选
  SET_BIT( LCD_RS );
  SET_BIT( LCD_WR );
  CLR_BIT( LCD_RD );
  SET_BIT( LCD_RD );
  LCD_DATA_H_DIR = 0x00;
  LCD_DATA_L_DIR = 0x00;
  val=LCD_DATA_H_OUT; 
  val=val<<8;
  val=val|LCD_DATA_L_OUT;
  SET_BIT( LCD_CS );//LCD 片选
  return val;
}
/****************************************************************************
* 名    称：unsigned int ILI9326_ReadRegister(unsigned int index)
* 功    能：读取指定地址寄存器的值
* 入口参数：index    寄存器地址
* 出口参数：寄存器值
* 说    明：内部函数
* 调用方法：i=ILI9326_ReadRegister(0x0022);
****************************************************************************/
unsigned int ILI9326_ReadRegister(unsigned int index)
{
  CLR_BIT( LCD_CS );//LCD 片选
  CLR_BIT( LCD_RS );
  SET_BIT( LCD_RD );
  CLR_BIT( LCD_WR );
  SET_BIT( LCD_WR );
  LCD_DATA_H_DIR = 0xFF;
  LCD_DATA_L_DIR = 0xFF;
  LCD_DATA_H_OUT = index >> 8;
  LCD_DATA_L_OUT = index;
  SET_BIT( LCD_RS );
  return (ILI9326_ReadData());
}
/****************************************************************************
* 名    称：void ILI9326_BackLight(unsigned char status)
* 功    能：开、关液晶背光
* 入口参数：status  1:背光开  0:背光关
* 出口参数：无
* 说    明：
* 调用方法：ILI9326_BackLight(1);
****************************************************************************/
void ILI9326_BackLight(unsigned char status)
{
  if ( status )
  {
    BIT_OUT( LCD_BL );
    SET_BIT( LCD_BL );
  }
  else
  {
    BIT_OUT( LCD_BL );
    CLR_BIT( LCD_BL );
  }
}
/****************************************************************************
* 名    称：void ILI9326_Reset()
* 功    能：复位 ILI9326 控制器
* 入口参数：无
* 出口参数：无
* 说    明：复位控制器，内部函数
* 调用方法：ILI9326_Reset()
****************************************************************************/
void ILI9326_Reset(void)
{ 
  CLR_BIT( LCD_RST );
  ILI9326_Delay(65000);
  SET_BIT( LCD_RST );
  ILI9326_Delay(65000);
}
/****************************************************************************
* 名    称：void ILI9326_WriteRegister(unsigned int index,unsigned int dat)
* 功    能：写指定地址寄存器的值
* 入口参数：index    寄存器地址
*           dat      寄存器值
* 出口参数：无
* 说    明：内部函数
* 调用方法：ILI9326_WriteRegister(0x0000,0x0001);
****************************************************************************/
void LCD_CtrlWrite_ILI9326(unsigned int index,unsigned int dat)
{
  ILI9326_WriteIndex(index);
  ILI9326_WriteData(dat);
}
void LCD_WRITE_CMD(unsigned int x)
{
  ILI9326_WriteIndex(x);
}

/********************************************************************************
 DESCRIPTION:
 LCM display window setting.
 PARAMETERS:
 xs: Horizontal Address Start Position
 xe: Horizontal Address End Position
 ys: Vertical Address Start Position
 ye: Vertical Address End Position
 RETURNS:
 REMARKS:
*********************************************************************************/
void LCD_set_Window_ILI9326(unsigned int xs,unsigned int xe,unsigned int ys,unsigned int ye)
{
  LCD_CtrlWrite_ILI9326(0x0210,xs);
  LCD_CtrlWrite_ILI9326(0x0211,xe);
  LCD_CtrlWrite_ILI9326(0x0212,ys);
  LCD_CtrlWrite_ILI9326(0x0213,ye);  
  LCD_CtrlWrite_ILI9326(0x0200,xs);
  LCD_CtrlWrite_ILI9326(0x0201,ys);  
  LCD_WRITE_CMD(0x0202); 
}

/****************************************************************************
* 名    称：void LCD_SetPoint(unsigned int x,unsigned int y,unsigned int point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：LCD_SetPoint(10,10,25);
****************************************************************************/
void LCD_SetPoint(int x,int y,int point)
{ 
  LCD_CtrlWrite_ILI9326(0x0200, y);
  LCD_CtrlWrite_ILI9326(0x0201, x);
  LCD_CtrlWrite_ILI9326(0x0202, point);
}
/****************************************************************************
* 名    称：unsigned int LCD_GetPoint(int x,int y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=LCD_GetPoint(10,10)
****************************************************************************/
unsigned int LCD_GetPoint(int x,int y)
{ 
  LCD_CtrlWrite_ILI9326(0x0200, y);
  LCD_CtrlWrite_ILI9326(0x0201, x);
  LCD_WRITE_CMD(0x0202);
  return (ILI9326_ReadData());
}
/****************************************************************************
* 名    称：void Draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1,unsigned int color)
* 功    能：在指定座标画直线
* 入口参数：x0     A点行座标
*           y0     A点列座标
*           x1     B点行座标
*           y1     B点列座标
*           color  线颜色
* 出口参数：无
* 说    明：
* 调用方法：Draw_line(0,0,240,320,0x0000);
****************************************************************************/
void Draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1,unsigned int color)
{
  unsigned int x,y;
  unsigned int dx;// = abs(x1 - x0);
  unsigned int dy;// = abs(y1 - y0);
  if(y0==y1)
  {
    if(x0<=x1)
        x=x0;
    else
    {
        x=x1;
        x1=x0;
    }
    while(x <= x1)
    {
        LCD_SetPoint(x,y0,color);
        x++;
    }
    return;
  }
  else if(y0>y1)
    dy=y0-y1;
  else
    dy=y1-y0;
  if(x0==x1)
  {
    if(y0<=y1)
      y=y0;
    else
    {
        y=y1;
        y1=y0;
    }
    while(y <= y1)
    {
        LCD_SetPoint(x0,y,color);
        y++;
    }
    return;
  }
  else if(x0 > x1)
  {
    dx=x0-x1;
    x = x1;
    x1 = x0;
    y = y1;
    y1 = y0;
  }
  else
  {
    dx=x1-x0;
    x = x0;
    y = y0;
  }
  if(dx == dy)
  {
    while(x <= x1)
    {
      x++;
      if(y>y1)
        y--;
      else
        y++;
      LCD_SetPoint(x,y,color);
    }
  }
  else
  {
  LCD_SetPoint(x, y, color);
  if(y < y1)
  {
          if(dx > dy)
          {
            int p = dy * 2 - dx;
            int twoDy = 2 * dy;
            int twoDyMinusDx = 2 * (dy - dx);
            while(x < x1)
            {
              x++;
              if(p < 0)
                 p += twoDy;
              else
              {
                 y++;
                 p += twoDyMinusDx;
              }
            LCD_SetPoint(x, y,color);
            }
         }
         else
         {
            int p = dx * 2 - dy;
            int twoDx = 2 * dx;
            int twoDxMinusDy = 2 * (dx - dy);
            while(y < y1)
            {
              y++;
              if(p < 0)
                 p += twoDx;
              else
              {
                x++;
                p+= twoDxMinusDy;
              }
              LCD_SetPoint(x, y, color);
            }
           }
          }
          else
          {
                  if(dx > dy)
                  {
                    int p = dy * 2 - dx;
                    int twoDy = 2 * dy;
                    int twoDyMinusDx = 2 * (dy - dx);
                    while(x < x1)
                    {
                      x++;
                      if(p < 0)
                      {
                        p += twoDy;
                      }
                      else
                      {
                        y--;
                        p += twoDyMinusDx;
                      }
                      LCD_SetPoint(x, y,color);
                    }
                  }
                  else
                  {
                    int p = dx * 2 - dy;
                    int twoDx = 2 * dx;
                    int twoDxMinusDy = 2 * (dx - dy);
                    while(y1 < y)
                    {
                      y--;
                      if(p < 0)
                         p += twoDx;
                      else
                      {
                         x++;
                         p+= twoDxMinusDy;
                      }
                      LCD_SetPoint(x, y,color);
                  }
             }
        }
   }
}
void Fill_Color(unsigned int Data)
{       
  unsigned int n,i;
  LCD_WRITE_CMD(0x0202);
  BIT_OUT(LCD_RS);
  SET_BIT(LCD_RS);
  BIT_OUT(LCD_CS);
  CLR_BIT(LCD_CS);
  LCD_DATA_H_DIR = 0xFF;
  LCD_DATA_L_DIR = 0xFF;
  LCD_DATA_H_OUT = Data >> 8;
  LCD_DATA_L_OUT = Data;
  BIT_OUT(LCD_WR);
  for(i=0;i<400;i++)
    for(n=0;n<240;n++)
    {
      CLR_BIT( LCD_WR );
      SET_BIT( LCD_WR );
    }
  BIT_OUT(LCD_RS);
  SET_BIT(LCD_RS);  
}
void ILI9326_init(void)
{
  LCD_IO_INT();
  ILI9326_Reset();
  LCD_CtrlWrite_ILI9326(0x0000, 0x0000);  
  LCD_CtrlWrite_ILI9326(0x0400, 0x6200); 
  LCD_CtrlWrite_ILI9326(0x0008, 0x0808); 
    
  LCD_CtrlWrite_ILI9326(0x0300, 0x0C00); 
  LCD_CtrlWrite_ILI9326(0x0301, 0x5A0B); 
  LCD_CtrlWrite_ILI9326(0x0302, 0x0906); 
  LCD_CtrlWrite_ILI9326(0x0303, 0x1017); 
  LCD_CtrlWrite_ILI9326(0x0304, 0x2300); 
  LCD_CtrlWrite_ILI9326(0x0305, 0x1700); 
  LCD_CtrlWrite_ILI9326(0x0306, 0x6309);
  LCD_CtrlWrite_ILI9326(0x0307, 0x0C09);
  LCD_CtrlWrite_ILI9326(0x0308, 0x100C);
  LCD_CtrlWrite_ILI9326(0x0309, 0x2232);
  
  LCD_CtrlWrite_ILI9326(0x0010, 0x0016);
  LCD_CtrlWrite_ILI9326(0x0011, 0x0101);
  LCD_CtrlWrite_ILI9326(0x0012, 0x0000);
  LCD_CtrlWrite_ILI9326(0x0013, 0x0001);
  
  LCD_CtrlWrite_ILI9326(0x0100, 0x0330);
  LCD_CtrlWrite_ILI9326(0x0101, 0x0237);
  LCD_CtrlWrite_ILI9326(0x0103, 0x0D00);
  LCD_CtrlWrite_ILI9326(0x0280, 0x6100);
  LCD_CtrlWrite_ILI9326(0x0102, 0xC1B0); 
  
  LCD_CtrlWrite_ILI9326(0x0001, 0x0080);
  LCD_CtrlWrite_ILI9326(0x0002, 0x0100);
  LCD_CtrlWrite_ILI9326(0x0003, 0x1030);
  LCD_CtrlWrite_ILI9326(0x0009, 0x0001);
  LCD_CtrlWrite_ILI9326(0x000C, 0x0000);
  LCD_CtrlWrite_ILI9326(0x0090, 0x8000);
  LCD_CtrlWrite_ILI9326(0x000F, 0x0000);
  
  LCD_CtrlWrite_ILI9326(0x0210, 0x0000);
  LCD_CtrlWrite_ILI9326(0x0211, 0x00EF);
  LCD_CtrlWrite_ILI9326(0x0212, 0x0000);
  LCD_CtrlWrite_ILI9326(0x0213, 0x018F);
  LCD_CtrlWrite_ILI9326(0x0500, 0x0000);
  LCD_CtrlWrite_ILI9326(0x0501, 0x0000);
  LCD_CtrlWrite_ILI9326(0x0502, 0x005F);
  LCD_CtrlWrite_ILI9326(0x0401, 0x0001);
  LCD_CtrlWrite_ILI9326(0x0404, 0x0000);
  
  LCD_CtrlWrite_ILI9326(0x0007, 0x0100);
  LCD_CtrlWrite_ILI9326(0x0200, 0x0000);
  LCD_CtrlWrite_ILI9326(0x0201, 0x0000);
  
  LCD_WRITE_CMD(0x0202);
}
/*********************************************************************
*
*       Exported functions  导出的函数
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_L0_SetPixelIndex
**********************************************************************/
void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) 
{
    LCD_SetPoint(x,y,PixelIndex);
}

/*********************************************************************
*
*       LCD_L0_GetPixelIndex
**********************************************************************/
unsigned int LCD_L0_GetPixelIndex(int x, int y)
{
  return LCD_GetPoint(x,y);
}

/*********************************************************************
*
*       LCD_L0_XorPixel
**********************************************************************/
void LCD_L0_XorPixel(int x, int y) 
{
  LCD_PIXELINDEX Index = LCD_GetPoint(x,y);
  LCD_SetPoint(x,y,LCD_NUM_COLORS-1-Index);
}

/*********************************************************************
*
*       LCD_L0_DrawHLine
**********************************************************************/
void LCD_L0_DrawHLine(int x0, int y,  int x1)
{
  Draw_line(x0,y,x1,y,LCD_COLORINDEX);
}

/*********************************************************************
*
*       LCD_L0_DrawVLine
**********************************************************************/
void LCD_L0_DrawVLine(int x, int y0,  int y1) 
{
  Draw_line(x,y0,x,y1,LCD_COLORINDEX);
}

/*********************************************************************
*
*       LCD_L0_FillRect
**********************************************************************/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1)
{
#if !LCD_SWAP_XY
  for (; y0 <= y1; y0++) 
  {
    LCD_L0_DrawHLine(x0,y0, x1);
  }
#else
  for (; x0 <= x1; x0++) 
  {
    LCD_L0_DrawVLine(x0,y0, y1);
  }
#endif
}

void DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans)
{
  LCD_PIXELINDEX pixels;
  LCD_PIXELINDEX Index0 = *(pTrans+0);
  LCD_PIXELINDEX Index1 = *(pTrans+1);
// Jump to right entry point
  pixels = *p;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR)) {
  case 0:
    #if defined (SETNEXTPIXEL)   /* Optimization ! */
      x+=Diff;
      LCD_SetCursor(x,y);
    #endif
    switch (Diff&7) 
    {
      case 0:   
        goto WriteBit0;
      case 1:   
        goto WriteBit1;
      case 2:
        goto WriteBit2;
      case 3:
        goto WriteBit3;
      case 4:
        goto WriteBit4;
      case 5:   
        goto WriteBit5;
      case 6:   
        goto WriteBit6;
      case 7:   
        goto WriteBit7;
    }
    break;
  case LCD_DRAWMODE_TRANS:
    switch (Diff&7) 
    {
      case 0:
        goto WriteTBit0;
      case 1:
        goto WriteTBit1;
      case 2:
        goto WriteTBit2;
      case 3:
        goto WriteTBit3;
      case 4:
        goto WriteTBit4;
      case 5:   
        goto WriteTBit5;
      case 6:   
        goto WriteTBit6;
      case 7:   
        goto WriteTBit7;
    }
    break;
  case LCD_DRAWMODE_XOR:
    switch (Diff&7) 
    {
      case 0:   
        goto WriteXBit0;
      case 1:   
        goto WriteXBit1;
      case 2:
        goto WriteXBit2;
      case 3:
        goto WriteXBit3;
      case 4:
        goto WriteXBit4;
      case 5:   
        goto WriteXBit5;
      case 6:   
        goto WriteXBit6;
      case 7:   
        goto WriteXBit7;
    }
  }
/*
        Write with transparency
*/
  WriteTBit0:
    if (pixels&(1<<7)) LCD_SetPoint(x+0, y, Index1);
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(1<<6)) LCD_SetPoint(x+1, y, Index1);
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(1<<5)) LCD_SetPoint(x+2, y, Index1);
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(1<<4)) LCD_SetPoint(x+3, y, Index1);
    if (!--xsize)
      return;
  WriteTBit4:
    if (pixels&(1<<3)) LCD_SetPoint(x+4, y, Index1);
    if (!--xsize)
      return;
  WriteTBit5:
    if (pixels&(1<<2)) LCD_SetPoint(x+5, y, Index1);
    if (!--xsize)
      return;
  WriteTBit6:
    if (pixels&(1<<1)) LCD_SetPoint(x+6, y, Index1);
    if (!--xsize)
      return;
  WriteTBit7:
    if (pixels&(1<<0)) LCD_SetPoint(x+7, y, Index1);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteTBit0;

/*
        Write without transparency
*/

  WriteBit0:
    LCD_SetPoint(x+0, y, (pixels&(1<<7)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit1:
    LCD_SetPoint(x+1, y, (pixels&(1<<6)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit2:
    LCD_SetPoint(x+2, y, (pixels&(1<<5)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit3:
    LCD_SetPoint(x+3, y, (pixels&(1<<4)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit4:
    LCD_SetPoint(x+4, y, (pixels&(1<<3)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit5:
    LCD_SetPoint(x+5, y, (pixels&(1<<2)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit6:
    LCD_SetPoint(x+6, y, (pixels&(1<<1)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit7:
    LCD_SetPoint(x+7, y, (pixels&(1<<0)) ? Index1 : Index0);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteBit0;
/*
        Write XOR mode
*/
  WriteXBit0:
    if (pixels&(1<<7)) LCD_L0_XorPixel(x+0, y);
    if (!--xsize)
      return;
  WriteXBit1:
    if (pixels&(1<<6)) LCD_L0_XorPixel(x+1, y);
    if (!--xsize)
      return;
  WriteXBit2:
    if (pixels&(1<<5)) LCD_L0_XorPixel(x+2, y);
    if (!--xsize)
      return;
  WriteXBit3:
    if (pixels&(1<<4)) LCD_L0_XorPixel(x+3, y);
    if (!--xsize)
      return;
  WriteXBit4:
    if (pixels&(1<<3)) LCD_L0_XorPixel(x+4, y);
    if (!--xsize)
      return;
  WriteXBit5:
    if (pixels&(1<<2)) LCD_L0_XorPixel(x+5, y);
    if (!--xsize)
      return;
  WriteXBit6:
    if (pixels&(1<<1)) LCD_L0_XorPixel(x+6, y);
    if (!--xsize)
      return;
  WriteXBit7:
    if (pixels&(1<<0)) LCD_L0_XorPixel(x+7, y);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteXBit0;
}
static void  DrawBitLine2BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans)
{
  LCD_PIXELINDEX pixels;
// Jump to right entry point
  pixels = *p;
  if (pTrans) 
  {
    /*with palette*/
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3) 
    {
    case 0:
      goto WriteTBit0;
    case 1:
      goto WriteTBit1;
    case 2:
      goto WriteTBit2;
    default:
      goto WriteTBit3;
    } 
    else switch (Diff&3)
    {
    case 0:
      goto WriteBit0;
    case 1:
      goto WriteBit1;
    case 2:
      goto WriteBit2;
    default:
      goto WriteBit3;
    }
  /*Write without transparency*/
  WriteBit0:
    LCD_SetPoint(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteBit1:
    LCD_SetPoint(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteBit2:
    LCD_SetPoint(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteBit3:
    LCD_SetPoint(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteBit0;
  /*Write with transparency*/
  WriteTBit0:
    if (pixels&(3<<6))
      LCD_SetPoint(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(3<<4))
      LCD_SetPoint(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(3<<2))
      LCD_SetPoint(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(3<<0))
      LCD_SetPoint(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteTBit0;
  } 
  else 
  { 
    /*without palette */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) 
      switch (Diff&3) 
      {
      case 0:
        goto WriteDDPTBit0;
      case 1:
        goto WriteDDPTBit1;
      case 2:
        goto WriteDDPTBit2;
      default:
        goto WriteDDPTBit3;
      } 
    else 
      switch (Diff&3)
      {
      case 0:
        goto WriteDDPBit0;
      case 1:
        goto WriteDDPBit1;
      case 2:
        goto WriteDDPBit2;
      default:
        goto WriteDDPBit3;
      }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    LCD_SetPoint(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPBit1:
    LCD_SetPoint(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPBit2:
    LCD_SetPoint(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPBit3:
    LCD_SetPoint(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels&(3<<6)) LCD_SetPoint(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&(3<<4)) LCD_SetPoint(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPTBit2:
    if (pixels&(3<<2)) LCD_SetPoint(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPTBit3:
    if (pixels&(3<<0)) LCD_SetPoint(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPTBit0;
  }
}
static void  DrawBitLine4BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans)
{
  LCD_PIXELINDEX pixels;
  pixels = *p;
  if (pTrans)
  {
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)
    {
      if ((Diff&1) ==0) goto WriteTBit0;
        goto WriteTBit1;
    }
    else
    {
      if ((Diff&1) ==0) goto WriteBit0;
        goto WriteBit1;
    }
  WriteBit0:
    LCD_SetPoint(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteBit1:
    LCD_SetPoint(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteBit0;
  /*
          Write with transparency
  */
  WriteTBit0:
    if (pixels>>4) LCD_SetPoint(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&0xf) LCD_SetPoint(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteTBit0;
  }
  else 
  {
    /*
      without palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) 
    {
      if ((Diff&1) ==0) goto WriteDDPTBit0;
      goto WriteDDPTBit1;
    } 
    else 
    {
      if ((Diff&1) ==0) goto WriteDDPBit0;
      goto WriteDDPBit1;
    }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    LCD_SetPoint(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPBit1:
    LCD_SetPoint(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels>>4) LCD_SetPoint(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&0xf) LCD_SetPoint(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPTBit0;
  }
}
void DrawBitLine8BPP(int x, int y, U8 const*p, int xsize, const LCD_PIXELINDEX*pTrans) 
{
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) 
  {
    if (pTrans) 
    {
      for (;xsize > 0; xsize--,x++,p++) 
      {
        pixel = *p;
        LCD_SetPoint(x, y, *(pTrans+pixel));
      }
    } 
    else 
    {
      for (;xsize > 0; xsize--,x++,p++) 
      {
        LCD_SetPoint(x, y, *p);
      }
    }
  } 
  else 
  {   /* Handle transparent bitmap */
    if (pTrans)
    {
      for (; xsize > 0; xsize--, x++, p++) 
      {
        pixel = *p;
        if (pixel) 
        {
          LCD_SetPoint(x+0, y, *(pTrans+pixel));
        }
      }
    } 
    else 
    {
      for (; xsize > 0; xsize--, x++, p++) 
      {
        pixel = *p;
        if (pixel) 
        {
          LCD_SetPoint(x+0, y, pixel);
        }
      }
    }
  }
}
void DrawBitLine16BPP(int x, int y, U16 const*p, int xsize)
{
  LCD_PIXELINDEX Index;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0)
  {
    for (;xsize > 0; xsize--,x++,p++)
    {
      LCD_SetPoint(x, y, *p);
    }
  }
  else
  {   /* Handle transparent bitmap */
    for (; xsize > 0; xsize--, x++, p++)
    {
      Index = *p;
      if (Index)
      {
        LCD_SetPoint(x+0, y, Index);
      }
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawBitmap
**********************************************************************/
void LCD_L0_DrawBitmap(int x0, 
                       int y0,
                       int xsize, 
                       int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, 
                       int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
  int i;
  switch (BitsPerPixel)
  {
  case 1:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 2:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 4:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 8:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 16:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine16BPP(x0, i+y0, (U16*)pData, xsize);
      pData += BytesPerLine;
    }
    break;
  }
}

/*********************************************************************
*
*       LCD_L0_SetOrg
**********************************************************************/
void LCD_L0_SetOrg(int x, int y) 
{
  GUI_USE_PARA(x);
  GUI_USE_PARA(y);
}

/*********************************************************************
*
*       LCD_On / LCD_Off
**********************************************************************/
void LCD_On (void) 
{
  ILI9326_BackLight(1);
}
void LCD_Off(void)
{
  ILI9326_BackLight(0); 
}

/*********************************************************************
*
*       LCD_L0_Init
**********************************************************************/
int LCD_L0_Init(void) 
{
  ILI9326_init();
  return 0;
}

/*********************************************************************
*
*       LCD_L0_SetLUTEntry
**********************************************************************/
void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR Color) 
{
  GUI_USE_PARA(Pos);
  GUI_USE_PARA(Color);
}

/*********************************************************************
*
*       LCD_L0_GetDevFunc
**********************************************************************/
void * LCD_L0_GetDevFunc(int Index) 
{
  GUI_USE_PARA(Index);
  return NULL;
}
