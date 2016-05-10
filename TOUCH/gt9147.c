#include "stdio.h"
#include "gt9147.h"
#include "touch.h"
#include "stm32f4xx_hal.h"
#include "bsp.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//4.3寸电容触摸屏-GT9147 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/10/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
const u8 GT9147_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
};  
//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
u8 GT9147_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
	buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//计算校验和
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(u8*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
	return 0;
} 
//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	/*
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件
    return ret;
    */

	u8 sendbuf[10];
	sendbuf[0] = reg>>8;
	sendbuf[1] = reg&0XFF;
	mymemcopy(sendbuf+2,buf,len);
	//wait for ready
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);

	if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)GT9147_ADDR, sendbuf, len+2) != HAL_OK)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	/*
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件
    */

	u8 sendbuf[2];
	sendbuf[0] = reg>>8;
	sendbuf[1] = reg&0XFF;
	//wait for ready
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)GT9147_ADDR, sendbuf, 2);
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	HAL_I2C_Master_Receive_IT(&I2cHandle, (uint16_t)GT9147_ADDR, buf, len);
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
} 
//初始化GT9147触摸屏
//返回值:0,初始化成功;1,初始化失败 
u8 GT9147_Init(void)
{
	u8 temp[5]; 
	/*
	RCC->APB2ENR|=1<<3;		//先使能外设IO PORTB时钟 							 
 	RCC->APB2ENR|=1<<7;		//先使能外设IO PORTF时钟 							 
	GPIOB->CRL&=0XFFFFF0FF;	//PB2 推挽输出
	GPIOB->CRL|=0X00000300;	   
	GPIOB->ODR|=1<<2;	    //PB2 输出高
	GPIOF->CRH&=0XFFFFF0FF;	//PF10 推挽输出
	GPIOF->CRH|=0X00000300;	   
	GPIOF->ODR|=1<<10;	   	//PF10 输出1
	CT_IIC_Init();      	//初始化电容屏的I2C总线
	*/
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin       = GPIO_PIN_14 | GPIO_PIN_10;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10 | GPIO_PIN_14,GPIO_PIN_SET);

	GT_RST_L;				//复位
//	delay_ms(10);
	HAL_Delay(10);
 	GT_RST_H;				//释放复位
//	delay_ms(10);
 	HAL_Delay(10);
 	/*
	GPIOF->CRH&=0XFFFFF0FF;	//清除原来设置
	GPIOF->CRH|=0X00000800;	//PF10 下拉输入	 
	GPIOF->ODR&=~(1<<10);	//PF10 下拉
	*/
 	GPIO_InitStruct.Pin       = GPIO_PIN_10;
 	GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
 	GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
 	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

// 	delay_ms(100);
 	HAL_Delay(100);
	GT9147_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
	temp[4]=0;
//	printf("CTP ID:%s\r\n",temp);	//打印ID
	SendString(temp);
	if(mystrcmp((char*)temp,"9147")==0)//ID==9147
	{
		temp[0]=0X02;			
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT9147
 		GT9147_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
		if(temp[0]<0X60)//默认版本比较低,需要更新flash配置
		{
//			printf("Default Ver:%d\r\n",temp[0]);
			SendString("Default Ver:");
			UartPutHexByte(temp[0]);
			GT9147_Send_Cfg(1);//更新并保存配置
		}
		delay_ms(10);
		temp[0]=0X00;	 
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);	//结束复位   	
		return 0;
	} 
	return 1;
}
const u16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 GT9147_Scan(u8 x)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 mode;

	GT9147_RD_Reg(GT_GSTID_REG,&mode,1);//读取触摸点的状态

	if((mode&0XF)&&((mode&0XF)<6))
	{
		temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义
		tp_dev.sta = ~temp;
		for(i=0;i<5;i++)
		{
			if(tp_dev.sta&(1<<i))	//触摸有效?
			{
				GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
				if(tp_dev.touchtype&0X01)//横屏
				{
					tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
					tp_dev.x[i]=800-(((u16)buf[3]<<8)+buf[2]);
				}else
				{
					tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
					tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
				}
				//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				if(tp_dev.x[i] > 480 || tp_dev.y[i] > 800)
				{
					tp_dev.sta &= ~(1<<i);
				}
				else
				{
					res = 1;
//					tp_dev.sta |= 1<<i;
					tp_dev.sta |= TP_PRES_DOWN|TP_CATH_PRES;
				}
			}
		}
//		res=1;
		if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据
	}

	if(mode&0X80&&((mode&0XF)<6))
	{
		temp=0;
		GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志
	}

	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	return res;
}
 



























