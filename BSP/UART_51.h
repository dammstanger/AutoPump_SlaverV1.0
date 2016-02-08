
/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��UART_51.h
 * ��	��	��
 *                    
 * ʵ��ƽ̨	���Կ�ˮ��V1.0  60S2 11.0592MHz
 * Ӳ������	��
 * �� 	��	��
 * ��汾	��
 * ����ʱ��	��2016.1.22
 * ���༭	��2016.1.24
 **-------------------------------------------------------------------------------
 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

/****************************����ͷ�ļ�*******************************************/
#ifndef __UART_51_H
#define __UART_51_H

void UART_Init(void);
void SendOneByte(unsigned char dat);
void SendString(unsigned char *str);
void SendByteASCII(unsigned char dat);
void SendByteNum(unsigned char dat);

void UART1_SendStr(unsigned char len,unsigned char *str);


#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


