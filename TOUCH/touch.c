#include "touch.h" 
//#include "stdlib.h"
//#include "math.h"
#include "gt9147.h"
#include "ott2001a.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A/GT9147等） 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/11
//版本：V2.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//修改说明
//V2.0 20140311
//增加对电容触摸屏的支持(需要添加:ctiic.c和ott2001a.c两个文件)
//V2.1 20141028
//电容触摸屏增加GT9147的支持
//////////////////////////////////////////////////////////////////////////////////

_m_tp_dev tp_dev=
{
	TP_Init,
	0,
	0,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};

//触摸屏初始化  		    
//返回值:0,没有进行校准
//       1,进行过校准
u8 TP_Init(void)
{	
	if(GT9147_Init()==0)	//是GT9147
	{
		tp_dev.scan=GT9147_Scan;	//扫描函数指向GT9147触摸屏扫描
		SendString("It's GT9147!\n");
	}else
	{
		OTT2001A_Init();
		tp_dev.scan=OTT2001A_Scan;	//扫描函数指向OTT2001A触摸屏扫描
		SendString("It's OTT2001A!\n");
	}
	tp_dev.touchtype|=0X80;	//电容屏
	tp_dev.touchtype|=lcddev.dir&0X01;//横屏还是竖屏
	return 0;
}

