#include "ott2001a.h"
#include "touch.h"
//#include "ctiic.h"
//#include "usart.h"
//#include "delay.h"
#include "stm32f4xx_hal.h"
#include "bsp.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//4.3����ݴ���������-OTT2001A����	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/11
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//��
//////////////////////////////////////////////////////////////////////////////////	


//��OTT2001Aд��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 OTT2001A_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	/*
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(OTT_CMD_WR);   //����д���� 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
	*/
	u8 sendbuf[10];
	sendbuf[0] = reg>>8;
	sendbuf[1] = reg&0XFF;
	mymemcopy(sendbuf+2,buf,len);
	//wait for ready
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);

	if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)OTT_ADDR, sendbuf, len+2) != HAL_OK)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//��OTT2001A����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void OTT2001A_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	/*
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(OTT_CMD_WR);   //����д���� 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(OTT_CMD_RD);   //���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����    
    */

	u8 sendbuf[2];
	sendbuf[0] = reg>>8;
	sendbuf[1] = reg&0XFF;
	//wait for ready
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)OTT_ADDR, sendbuf, 2);
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
	HAL_I2C_Master_Receive_IT(&I2cHandle, (uint16_t)OTT_ADDR, buf, len);
	while(HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY);
}
//��������/�رղ���
//cmd:1,�򿪴�����;0,�رմ�����
void OTT2001A_SensorControl(u8 cmd)
{
	u8 regval=0X00;
	if(cmd)regval=0X80;
	OTT2001A_WR_Reg(OTT_CTRL_REG,&regval,1); 
} 
//��ʼ��������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
u8 OTT2001A_Init(void)
{
 	u8 regval=0; 
 	/*
	RCC->APB2ENR|=1<<3;		//��ʹ������IO PORTBʱ�� 							 
 	RCC->APB2ENR|=1<<7;		//��ʹ������IO PORTFʱ�� 							 
	GPIOB->CRL&=0XFFFFF0FF;	//PB2 �������
	GPIOB->CRL|=0X00000300;	   
	GPIOB->ODR|=1<<2;	    //PB2 �����
	GPIOF->CRH&=0XFFFFF0FF;	//PF10 ����
	GPIOF->CRH|=0X00000800;	   
	GPIOF->ODR|=1<<10;	   	//PF10 ����	 
	CT_IIC_Init();      	//��ʼ����������I2C����  
	*/
 	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOB_CLK_ENABLE();
	//RESET
	GPIO_InitStruct.Pin       = GPIO_PIN_14;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
	//INT
	GPIO_InitStruct.Pin       = GPIO_PIN_10;
 	GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
 	GPIO_InitStruct.Pull      = GPIO_PULLUP;
 	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

 	OTT_RST_L;				//��λ
//	delay_ms(100);
	HAL_Delay(100);
	OTT_RST_H;				//�ͷŸ�λ
//	delay_ms(100);
 	HAL_Delay(100);
	OTT2001A_SensorControl(1);	//�򿪴����� 
	OTT2001A_RD_Reg(OTT_CTRL_REG,&regval,1);//��ȡ���������мĴ�����ֵ���ж�I2Cͨ���Ƿ�����
//	printf("CTP ID:%x\r\n",regval);
	SendString("CTP ID: ");
	UartPutHexByte(regval);
    if(regval==0x80)return 0;
	return 1;
}

const u16 OTT_TPX_TBL[5]={OTT_TP1_REG,OTT_TP2_REG,OTT_TP3_REG,OTT_TP4_REG,OTT_TP5_REG};
//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 OTT2001A_Scan(u8 x)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 mode;

	OTT2001A_RD_Reg(OTT_GSTID_REG,&mode,1);//��ȡ�������״̬
	if(mode&0X1F)
	{
		tp_dev.sta=(mode&0X1F)|TP_PRES_DOWN|TP_CATH_PRES;
		for(i=0;i<5;i++)
		{
			if(tp_dev.sta&(1<<i))	//������Ч?
			{
				OTT2001A_RD_Reg(OTT_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
				if(tp_dev.touchtype&0X01)//����
				{
					tp_dev.y[i]=(((u16)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
					tp_dev.x[i]=800-((((u16)buf[0]<<8)+buf[1])*OTT_SCAL_X);
				}else
				{
					tp_dev.x[i]=(((u16)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
					tp_dev.y[i]=(((u16)buf[0]<<8)+buf[1])*OTT_SCAL_X;
				}
				//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
			}
		}
		res=1;
		if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//���������ݶ���0,����Դ˴�����
	}

	if((mode&0X1F)==0)//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	 
	} 	

	return res;
}
 































 





























