/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��main.c
 * ��	��	��Һλ�Զ�������Ŀ����
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V1.0  60S2 11.0592MHz
 * Ӳ������	��
 * �� 	��	��V0.0.160214
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2015.7.11
 * ���༭	��2016.2.14
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "globaldefine.h"
#include "stdio.h"
#include "UART_51.h"
#include "SI4432.h"
#include "Transmit.h"
#include "DS18B20.h"
#include "ADC_51.h"
#include "sensor.h"
/****************************�궨��***********************************************/
#define P2_5	0x20
#define P2_7	0x80

#define POSSW	P0			//Һλ���زɼ���P0�ڵ�ǰ6λ

/****************************��������*********************************************/

/****************************��������*********************************************/
sbit START 	= P2^7;			
sbit STOP	= P2^5;
sbit ALARM	= P4^0;
char temp;
char dat[] = "abc";
/****************************��������*********************************************/

void delay200ms(void)   //��� -0.000000000028us
{
    unsigned char a,b,c;
    for(c=178;c>0;c--)
        for(b=230;b>0;b--)
            for(a=12;a>0;a--);
}

void delay1s(void)   ////11.0592 STC1T ��� -0.000000000099us
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
	P2M0 = 0;//P2_5|P2_7;				//P2.1 P2.2ǿ����
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
	SI4432_SetRxMode();	//����ģʽ
	ADC_Init();
	delay1s();
	SendString("ROMID Search...\r\n");
	SendROMID(DS18B20_SearchRomID());
	SendString("\r\n");					//������Ϣʱ����
	//-----------------------------------------------------
	EA = 1;								//ע�⣺�����ʼ�����ٿ��жϣ�

	while(1)
	{	
		if(Trans_RevPakFin)
		{	
			Trans_RevPakFin = 0;
			//Һλ�ɼ�����
			ADC_STARTCOV(ADC_CH0,ADC_SPEED_540T);
			while(!(g_sensor_sta1&PRS_RDY));					//�ȴ�ѹ���ɼ����
			//�¶Ȳɼ�����
			TemperDatHandle();
			//Һλ���زɼ�	
			sensor_data.possw = POSSW;
			//���
			if(1==Pak_Handle())
			{
				g_sensor_sta1 = 0;								//������д�������־λ
				LED2 = 0;
				SendString("valid cmd received.\r\n");					
				delay200ms();
				LED2 = 1;
			}
		}
//		delay1s();
//		SendTemp(DS18B20_ReadTemperature(1));
//		delay1s();
//		SendTemp(DS18B20_ReadTemperature(2));
//		dat = P0;
//		SendByteASCII(dat);
//		SendString("\r\n");
	}//end of while
}//end of main



void EXTI0_ISR() interrupt 0 
{	//��Ӧ��IE0�Զ����
	SI4432_ISR();
	SendString("ISR completed.\r\n");					//������Ϣʱ����	
}

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

