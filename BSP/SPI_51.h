
/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��SPI_51.h
 * ��	��	��
 *                    
 * ʵ��ƽ̨	���Կ�ˮ��V1.0  60S2 11.0592MHz
 * Ӳ������	��
 * �� 	��	��
 * ��汾		��
 * ����ʱ��	��2016.1.21
 * ���༭	��2016.1.21
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/
#ifndef __SPI_51_H
#define __SPI_51_H
/****************************����ͷ�ļ�*******************************************/
#include "globaldefine.h"

//SPIģʽ
#define MASTER			1
#define SLAVER			0

//SPSTAT״̬�Ĵ���
#define SPIF				0x80
#define WCOL				0x40

//SPCTL�Ĵ���
#define SSIG				0x80
#define SPEN				0x40
#define DORD				0x20
#define MSTR				0x10
#define CPOL				0x08
#define CPHA				0x04
//EI2
#define ESPI				0x02

// SPI�����ٶ����� Ҳ��������Ԥ��Ƶ�Ĵ���
#define SPI_SPEED_4   		0				//2.1MHz		11.0592MHz 
#define SPI_SPEED_16 		0x01			//700kHz
#define SPI_SPEED_64   		0x02			//175kHz
#define SPI_SPEED_128  		0x03			//86.5kHz

void SPI_Init(bit mode);			 	//��ʼ��SPI��
void SPI_SetSpeed(bit SpeedSet); 		//����SPI�ٶ�   
char SPI_ReadWriteByte(char dat);		//SPI���߶�дһ���ֽ�
		 
#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


