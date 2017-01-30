/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：main.c
 * 描	述	：液位自动控制项目程序
 *            
 * 实验平台	：自控水泵V1.0  60S2 11.0592MHz
 * 硬件连接	：
 * 版 	本	：V0.0.160214
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2015.7.11
 * 最后编辑	：2016.2.14
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "globaldefine.h"
#include "stdio.h"
#include "UART_51.h"
#include "SI4432.h"
#include "Transmit.h"
#include "DS18B20.h"
#include "ADC_51.h"
#include "sensor.h"
#include "string.h"
/****************************宏定义***********************************************/
#define P2_5	0x20
#define P2_7	0x80

#define POSSW	P0			//液位开关采集，P0口的前6位
/****************************变量声明*********************************************/

/****************************变量定义*********************************************/

sbit FLOW 	= P3^3;			

char temp;
char dat[] = "abc";
/****************************函数声明*********************************************/

void delay200ms(void)   //误差 -0.000000000028us
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=230;b>0;b--)
            for(a=12;a>0;a--);
}

void delay1s(void)   ////11.0592 STC1T 误差 -0.000000000099us
{
    unsigned char a,b,c;
    for(c=217;c>0;c--)
        for(b=171;b>0;b--)
            for(a=73;a>0;a--);
}

void EXTI0_Init()
{
	IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
  EX0 = 1;                        //enable INT0 interrupt
}


void main()
{
	delay1s();
	AUXR = AUXR|0x40;  	// T1, 1T Mode

//	IE2 |= ESPI;
	EXTI0_Init();                         //4432的中断设置
	UART_Init();													//波特率9600
	SPI_Init(MASTER);
	delay1s();
	SI4432_Init();
	SI4432_SetRxMode();	//接收模式
	ADC_Init(AFPORT_P1_4);
	delay1s();
#if MULTI_SENSOR
	SendString("ROMID Search...\r\n");
	SendROMID(DS18B20_SearchRomID());
	SendString("\r\n");					//调试信息时候用
#endif
	
	//-----------------------------------------------------
	EA = 1;								//注意：外设初始化完再开中断！

	while(1)
	{	
		if(Trans_RevPakFin)
		{	
			Trans_RevPakFin = 0;
			//液位采集计算
			ADC_STARTCOV(ADC_CH4,ADC_SPEED_540T);
			while(!(g_sensor_sta1&PRS_RDY));					//等待压力采集完成
			
			//温度采集计算
//		TemperDatHandle();
			//液位开关采集	
//		sensor_data.possw = POSSW;
			//流量开关采集	
//			sensor_data.flow = FLOW;
			//打包
			if(1==Pak_Handle())
			{
				g_sensor_sta1 = 0;								//清除所有传感器标志位
				LED2 = 0;
				SendString("valid cmd received.\r\n");					
				delay200ms();
				LED2 = 1;
			}
		}
		//********************code for test**************************************
//		DATA_Cmd_ACK();
//		LED2 = 0;
//		SendString("valid cmd received.\r\n");					
//		delay200ms();
//		LED2 = 1;
//		ADC_STARTCOV(ADC_CH4,ADC_SPEED_540T);
//		while(!(g_sensor_sta1&PRS_RDY));					//等待压力采集完成
//		sprintf("level: %d:\r\n",);
//		SendString("level: %s:\r\n");	
//		sensor_data.flow = FLOW;
//		SendString("flow data:\r\n");					
//		SendByteASCII(sensor_data.flow);
//		SendString("\r\n");	
//		delay1s();
//		delay1s();
		//********************end of code for test**************************************
	}//end of while
}//end of main



void EXTI0_ISR() interrupt 0 
{	//响应后IE0自动清除
	SI4432_ISR();
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

