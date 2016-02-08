
/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��DS18B20.h
 * ��	��	��DS18B20����	Ӧ����STC12C5A60S2	11.0592MHz
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V1.0  60S2 11.0592MHz
 * Ӳ������	��
 * �� 	��	��V0.1.160128
 * ������ϵ	��PoolAuto
 * ��汾	����
 * ����ʱ��	��2016.1.28
 * ���༭	��2016.1.28
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

#ifndef _DS18B20_H__
#define _DS18B20_H__
/****************************����ͷ�ļ�*******************************************/
#include "globaldefine.h"
#include "UART_51.h"

/****************************�궨��***********************************************/
//�ഫ����ģʽ
#define MULTI_SENSOR					0

//REGISTER COMMANDS
#define REGISTER_9_BITS  0x1F
#define REGISTER_10_BITS 0x3F
#define REGISTER_11_BITS 0x5F
#define REGISTER_12_BITS 0x7F
//ROM COMMANDS
#define ROM_Search_Cmd   0xF0				 //���� ROM
#define ROM_Read_Cmd     0x33				 //��ROM
#define ROM_Match_Cmd    0x55				 //���� ROM	���
#define ROM_Skip_Cmd     0xCC				 //���� ROM
#define ROM_AlarmSearch_Cmd 0xEC			 //�澯��������
//DS18b20 FUNCTION COMMANDS
#define Convert_T         0x44
#define Write_Scratchpad  0x4E
#define Read_Scratchpad   0xBE
#define Copy_Scratchpad   0x48
#define Recall_EEPROM     0x88
#define Read_PowerSupply  0x84

/****************************��������*********************************************/
extern uchar RomID[2][8];

/****************************��������*********************************************/

/****************************��������*********************************************/
void delay_18B20(uint i);
void DS18B20_Init();
uchar ReadOneChar();
void WriteOneChar(uchar dat);
uint DS18B20_ReadTemperature(uchar NO);
char DS18B20_SearchRomID();
void DS18B20_Read_RomID(char *id);

void SendTemp(uint temp);
void Send_RomID();
void SendROMID(unsigned char n);


#endif

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


