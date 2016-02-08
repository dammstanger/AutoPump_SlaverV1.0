/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��SI4432.c
 * ��	��	��SI4432����ģ�������
 * ע	��	��	ItStatus1 = SI4432_ReadReg(0x03);													//read the Interrupt Status1 register
				ItStatus2 = SI4432_ReadReg(0x04);	���ж�״̬��3�����ܣ�1���ͷ�nIRQ���ţ�
				2����ȡ�����ж�λ������δʹ�ܵ��жϣ���3�������ʹ�ܵ��ж�λ��δʹ�ܵ��޷������
				һ��ɶ�ʹ�á�
 * ʵ��ƽ̨	���Կ�ˮ��V1.0  60S2 11.0592MHz
 * Ӳ������	��
 * �� 	��	��V0.1.160208
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.1.21
 * ���༭	��2016.2.8
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#include "SI4432.h"
#include "stdio.h"
#include "UART_51.h"
#include "globaldefine.h"
#include "Transmit.h"

/****************************�궨��***********************************************/
#define TxGPIOSetting()   SI4432_WriteReg(0x0E, 0x02)
#define RxGPIOSetting()   SI4432_WriteReg(0x0E, 0x01)
#define IdleGPIOSetting() SI4432_WriteReg(0x0E, 0x01)
#define RR		0x00		//������
#define WR		0x80		//д����



/****************************��������*********************************************/
//----------���ű�������-----------------
sbit SI4432_SDN	= 	P5^2;
sbit SI4432_IRQ	= 	P3^2;
sbit SI4432_SEL = 	P5^3;
//---------------------------------------
char 	ItStatus1,ItStatus2;
/****************************��������*********************************************/


void SI4432_Delayms(int n)
{
	uchar a,b;
#ifdef DELAYFUNC
	Delay_ms(n);			//From Systick
#else
	for(;n>0;n--)			//11.0592MHz 1T 1һ����λ1.0007ms
		for(b=129;b>0;b--)
			for(a=20;a>0;a--);
#endif
}

void SI4432_GPIOInit()
{
	RF4432_SDN_0;		//����ģ��
	RF4432_SEL_1;		//Ƭѡ���ߣ��͵�ƽ��Ч
	RF4432_IRQ_1;		//�ж������øߣ��͵�ƽ����
}

void SI4432_WriteReg(char addr,char val)
{
	RF4432_SEL_0;                  
	SPI_ReadWriteByte(addr|WR);      
	SPI_ReadWriteByte(val);          
	RF4432_SEL_1;      
}

char SI4432_ReadReg(char addr)
{
	char retval;
	RF4432_SEL_0;                  
	SPI_ReadWriteByte(addr|RR);      
	retval = SPI_ReadWriteByte(0xff);          
	RF4432_SEL_1;    
	return retval;	
}

char SI4432_Check()
{
	char retval;
	SendString("Check SI4432...\r\n");
	retval = SI4432_ReadReg(0x00);	
	SendString("DEVICE_TYPE: 0x%x.\r\n");
	SendByteASCII(retval);
	SendString("\r\n");
	if(retval!=0x08)
		return 1;
	retval = SI4432_ReadReg(0x01);
	SendString("DEVICE_VERSION: 0x%x.\r\n");	
	SendByteASCII(retval);
	SendString("\r\n");
	retval = SI4432_ReadReg(0x02);	
	if((retval&0x0f)==0)
		SendString("DEVICE_STATUS: at idle mode.\r\n");
	else if((retval&0x0f)==1)
		SendString("DEVICE_STATUS: at RX mode.\r\n");	
	else if((retval&0x0f)==2)
		SendString("DEVICE_STATUS: at TX mode.\r\n");	
	retval = SI4432_ReadReg(0x09);	
	SendString("Crystal Oscillator Load Capacitance: %x\r\n");
	SendByteASCII(retval);
	SendString("\r\n");
	return 0;
}

void SI4432_RF_Init(void)
{
	//Set the physical parameters
	//Frequency:       433MHz
	//Modulation type: GFSK
	//CrytalTolerance: Rx,10ppm; Tx, 10ppm
	//Data rate:       9.6Kbps/1.2Kbps
	//AFC:             Enable
	//Frequency deviation: 20KHz

	//set the center frequency to 433 MHz
	SI4432_WriteReg(0x75, 0x53);									//write data to the Frequency Band Select register             
	SI4432_WriteReg(0x76, 0x4B);									//write data to the Nominal Carrier Frequency1 register
	SI4432_WriteReg(0x77, 0x00);  									//write data to the Nominal Carrier Frequency0 register

	//set the desired TX data rate (1.2kbps)
	SI4432_WriteReg(0x6E, 0x09);									//write data to the TXDataRate 1 register
	SI4432_WriteReg(0x6F, 0xD5);									//write data to the TXDataRate 0 register
	SI4432_WriteReg(0x70, 0x2C);									//write data to the Modulation Mode Control 1 register
	SI4432_WriteReg(0x58, 0x80);

	//set the TX power to MAX. dBm
	SI4432_WriteReg(0x6D, 0x1F);									//write data to the TX Power register 

	//set the Tx deviation register (+-20kHz)
	SI4432_WriteReg(0x72, 0x20);									//write data to the Frequency Deviation register 

								/*set the modem parameters according to the exel calculator(parameters: 1.2 kbps, deviation: 20 kHz*/
	SI4432_WriteReg(0x1C, 0x2C);															//write data to the IF Filter Bandwidth register		
	SI4432_WriteReg(0x20, 0x41);															//write data to the Clock Recovery Oversampling Ratio register		
	SI4432_WriteReg(0x21, 0x60);															//write data to the Clock Recovery Offset 2 register		
	SI4432_WriteReg(0x22, 0x27);															//write data to the Clock Recovery Offset 1 register		
	SI4432_WriteReg(0x23, 0x52);															//write data to the Clock Recovery Offset 0 register		
	SI4432_WriteReg(0x24, 0x00);															//write data to the Clock Recovery Timing Loop Gain 1 register		
	SI4432_WriteReg(0x25, 0x04);															//write data to the Clock Recovery Timing Loop Gain 0 register		
	SI4432_WriteReg(0x1D, 0x40);															//write data to the AFC Loop Gearshift Override register		
	SI4432_WriteReg(0x1E, 0x0A);
	SI4432_WriteReg(0x2A, 0x0F);															//write data to the AFC Limiter register		
	SI4432_WriteReg(0x1F, 0x03);
	SI4432_WriteReg(0x69, 0x60);
	
							/*set the packet structure and the modulation type*/
	//set the preamble length to 10bytes if the antenna diversity is used and set to 5bytes if not 
	SI4432_WriteReg(0x34, 0x0C);															//write data to the Preamble Length register
	//set preamble detection threshold to 20bits
	SI4432_WriteReg(0x35, 0x2A); 															//write data to the Preamble Detection Control  register

	//Disable header bytes; set variable packet length (the length of the payload is defined by the
	//received packet length field of the packet); set the synch word to two bytes long
	SI4432_WriteReg(0x33, 0x02);															//write data to the Header Control2 register    
	
	//Set the sync word pattern to 0x2DD4
	SI4432_WriteReg(0x36, 0x2D);															//write data to the Sync Word 3 register
	SI4432_WriteReg(0x37, 0xD4);															//write data to the Sync Word 2 register

	//enable the TX & RX packet handler and CRC-16 (IBM) check
	SI4432_WriteReg(0x30, 0x8D);															//write data to the Data Access Control register
	//Disable the receive header filters
   	SI4432_WriteReg(0x32, 0x00 );															//write data to the Header Control1 register            

	//enable FIFO mode and GFSK modulation
	SI4432_WriteReg(0x71, 0x63);															//write data to the Modulation Mode Control 2 register

	//set  Crystal Oscillator Load Capacitance register
	SI4432_WriteReg(0x09, CRYSTAL_CAPACITANCE);												//write data to the Crystal Oscillator Load Capacitance register
}

void SI4432_Init()
{
	SendString("SI4432 IO Init......\r\n");
	SI4432_GPIOInit();
	SendString("IO Init completed.\r\n");
	SI4432_Delayms(20);
	while(SI4432_Check())
	{
		SendString("SI4432 is not found......\r\n");
		SI4432_Delayms(500);
	}
	/*set the GPIO's according the testcard type*/
	SI4432_WriteReg(0x0B, 0xCA);						//GPIO0 Configuration	Direct Digital Output
   	SI4432_WriteReg(0x0C, 0xCA);						//GPIO1 Configuration	Direct Digital Output
   	SI4432_WriteReg(0x0D, 0xCA);						//GPIO2 Configuration	Direct Digital Output
	SI4432_Delayms(10);
	SendString("SI4432 GPIOInit complete,config device......\r\n");
	
	//��ȡ�ж�״̬�Ĵ�������жϱ�־�Ա��ͷ�NIRQ����
	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1
	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2
	
	//SW reset�������λ���ģ������оƬ   
    SI4432_WriteReg(0x07, 0x80);				//write 0x80 to the Operating & Function Control1 register
	SI4432_Delayms(20);
	
	//�ȴ���λ��ɣ�����λ��ɺ����жϷ������ͻ�Ҳ����������ֱ����ʱ����20ms�����õȴ��ж�����
	//�ȴ�����20ms��λ��ɣ���ʱ�������ж�״̬�Ĵ����ͷ��ж�����
	//while ( NIRQ == 1);  
	//read interrupt status registers to clear the interrupt flags and release NIRQ pin
	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1
	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2
	
	SendString("Config RF......\r\n");
	//���ݲ�ͬ����Ƶ������ʼ����Ƶģ�飻
    SI4432_RF_Init();	
	
	//Ҫ����жϣ���Ҫ���ж�
	SI4432_WriteReg(0x05, 0xff);			
	SI4432_WriteReg(0x06, 0xff);			
	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1	
	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2	
	
	SI4432_WriteReg(0x05, 0x07);				//��������������������ͺͽ����жϼ�CRCУ����󶼿�����������
	SI4432_WriteReg(0x06, 0x00);				//���������������ڽ����ж�ʱ���йص��жϱ�־���ܱ���ʱ�������Ӱ������ж����͵��жϣ�������
	SendString("SI4432Init complete.\r\n");
}


//------------------------------------------------------------------------------------------------
// Function Name :RFSetRxMode
//
// Return Value : None
// Parameters   : None
// Notes        : ����ģ�鹤���ڽ���״̬�£���һ��׼���õ�ʱ�򣬵��øú���������ģ�鹤���ڽ���ģʽ��
//
//-----------------------------------------------------------------------------------------------
void SI4432_SetRxMode(void)
{  
	//read interrupt status registers to release all pending interrupts
	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1	
	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2	

	//Set the Rx switcher������GPIO�ڿ���ģ���ڲ������߿��ء�
	RxGPIOSetting();
	
	/*enable receiver chain*/
	SI4432_WriteReg(0x07, 0x05);
	
	SendString("SI4432_Set RxMode......\r\n");
}

//------------------------------------------------------------------------------------------------
// Function Name :SI4432_SetTxMode
//
// Return Value : None
// Parameters   : None
// Notes        : ����ģ�鹤���ڽ���״̬�£���һ��׼���õ�ʱ�򣬵��øú���������ģ�鹤���ڷ���״̬ģʽ��
//
//-----------------------------------------------------------------------------------------------
void SI4432_SetTxMode(void)
{
	
	//Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1	
	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2	

	//Set the Rx switcher������GPIO�ڿ���ģ���ڲ������߿��ء�
	TxGPIOSetting();
	
	/*enable transmitter*/
	//The radio forms the packet and send it automatically.
	SI4432_WriteReg(0x07, 0x09);						//0x09

	SendString("SI4432_Set TxMode......\r\n");
}

//------------------------------------------------------------------------------------------------
// Function Name :SI4432_SetIdleMode
//
// Return Value : None
// Parameters   : None
// Notes        : ����ģ�鹤���ڿ���״̬�¡���ģ�鲻��Ҫ��������Ҳ����Ҫ�������ݵ�ʱ����Ե��øú����Ա�ʡ�磻
//
//-----------------------------------------------------------------------------------------------
void SI4432_SetIdleMode(void)
{

	//Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1	
	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2	

	IdleGPIOSetting();																//����ʱΪ���մ�

	//The radio will work in idle mode
	SI4432_WriteReg(0x07, 0x01);													//����Ϊready ģʽ��ʹ�ܸ����л������ͺͽ���

	SendString("SI4432_Set IdleMode......\r\n");
}

//------------------------------------------------------------------------------------------------
// Function Name :SI4432_FIFO_SendData
//
// Return Value : None
// Parameters   : uint8 length: the length of the content which will be sent out. This value should
//                not more than 64bytes because the FIFO is 64bytes. If it is bigger than 64, the
//                content should be sent out for more times according to the FIFO empty interrupt.
//                uint8 *payload: point to the content.
// Notes        : In FIFO mode, send out one packet data.
//                ���øú������԰�����д��ģ���FIFO�в������ݷ��ͳ�ȥ�����ݳ��Ȳ��ܴ���64���ֽڣ�
//
//-----------------------------------------------------------------------------------------------
void SI4432_FIFO_SendData(char length, char *payload)
{
	char i;
	
//	SI4432_SetIdleMode();
 																			
	//set the length of the payload.	
	SI4432_WriteReg(0x3E, length);

	//fill the payload into the transmit FIFO
	for(i = 0; i < length; i++)
	{
		SI4432_WriteReg(0x7F, payload[i]);	
	}

	SI4432_SetTxMode();
	SendString("SI4432_Send FIFO data....\r\n");

	/*��ѯ��ʽ*/
//	while(RF4432_IRQ == 1);				//��ѯ�ж��������жϷ�����ɣ�������ɱ������������жϵ��鷳
//	SendString("Has been Sended.\r\n");	//������Ϣʱ����
	//��ȡ״̬�Ĵ���0x03��0x04���Ա��ͷ��ж�
//	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1	
//	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2	

}

/********************************************************************************
 * ��������SI4432_ISR()
 * ����  ��SPI�жϷ�����
 * ����  ��-
 * ����  ��-
 * ����  ��-
 ********************************************************************************/
void SI4432_ISR(void)
{
	uchar i,Length_revdat;
	i = SI4432_ReadReg(0x07);					//��ȡ�豸ģʽ
	SendString("mode in ISR:\r\n");
	SendByteASCII(i);
	SendString("\r\n");

	//����־λ������
	ItStatus1 = SI4432_ReadReg(0x03);			//��ȡ�жϱ�־�Ĵ���1	
	ItStatus2 = SI4432_ReadReg(0x04);			//��ȡ�жϱ�־�Ĵ���2	
	SendString("Interrupt occued\r\n");
	SendByteASCII(ItStatus1);
	SendByteASCII(ItStatus2);
	SendString("\r\n");
	if(ItStatus1&0x02)							//���յ���Ч����
	{
		SendString(" rev Interrupt\r\n");
		
		Length_revdat = SI4432_ReadReg(0x4B);	//��ȡ��Ч���ݳ���
		if(PAG_REVSIZE==Length_revdat)
		{
			for(i=0;i<Length_revdat;i++)
			{
				Revbuf[i] = SI4432_ReadReg(0x7F);
			}
		}
		//reset the RX FIFO
		SI4432_WriteReg(0x08, 0x02);			//write 0x02 to the Operating Function Control 2 register
		SI4432_WriteReg(0x08, 0x00);	
		//�������ݺ�ģ���Զ�����readyģʽ����Ҫ�л��ؽ���ģʽ���ܳ����������ݣ���������ж�Ҳ�������
		SI4432_SetRxMode();	
		Trans_RevPakFin = 1;					//��ǽ��յ�����
	}
	if(ItStatus1&0x04)							//���ݷ������
	{
		SendString(" send Interrupt\r\n");
		
		//�л��ؽ���ģʽ
		SI4432_SetRxMode();			
	}
}


void debug()
{
	char i;
	i = SI4432_ReadReg(0x07);												//read the Interrupt Status1 register		
	SendString("mode register:\r\n");
	SendByteASCII(i);
	SendString("\r\n");
}
/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

