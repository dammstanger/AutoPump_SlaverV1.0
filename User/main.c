/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：main.c
 * 描	述	：液位自动控制项目程序
 *            
 * 实验平台	：自控水泵V1.0  60S2 11.0592MHz
 * 硬件连接	：
 * 版 	本	：V0.0.150711
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2015.7.11
 * 最后编辑	：2016.1.26
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
/****************************宏定义***********************************************/
#define P2_5	0x20
#define P2_7	0x80

/****************************变量声明*********************************************/

/****************************变量定义*********************************************/
sbit START 	= P2^7;
sbit STOP	= P2^5;
sbit ALARM	= P4^0;
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

void JiDianQ_Init()
{;
	P2M0 = 0;//P2_5|P2_7;				//P2.1 P2.2强推挽
	P2M1 = 0;
}



void main()
{
	delay1s();
	AUXR = AUXR|0x40;  	// T1, 1T Mode

//	IE2 |= ESPI;
	EXTI0_Init();                         
	UART_Init();
//	JiDianQ_Init();
	SPI_Init(MASTER);
	delay1s();
	SI4432_Init();
	SI4432_SetRxMode();	//接收模式
	ADC_Init();
//	delay1s();
//	SendString("ROMID Search...\r\n");
//	SendROMID(DS18B20_SearchRomID());
//	SendString("\r\n");					//调试信息时候用
	//-----------------------------------------------------
	EA = 1;								//注意：外设初始化完再开中断！

	while(1)
	{	
		if(Trans_RevPakFin)
		{	
			Trans_RevPakFin = 0;
			//液位采集计算
			ADC_STARTCOV(ADC_CH0,ADC_SPEED_540T);
			while(!(g_sensor_sta1&0x80));
			g_sensor_sta1 &= ~0x80;							//清除标志位
			//温度采集计算
			TemperDatHandle(DS18B20_ReadTemperature(1));
			//打包
			if(1==Pak_Handle())
			{
				g_sensor_sta1 = 0;
				LED1 = 0;
				SendString("valid data received.\r\n");					//调试信息时候用
				delay200ms();
				LED1 = 1;
			}
		}
	}//end of while
}//end of main



void EXTI0_ISR() interrupt 0 
{	//响应后IE0自动清除
	SI4432_ISR();
	SendString("ISR completed.\r\n");					//调试信息时候用	
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

