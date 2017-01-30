/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Sensor.c
 * 描	述	：对传感器数据进行处理的应用
 *            
 * 实验平台	：自控水泵V1.0  60S2 11.0592MHz
 * 硬件连接	：
 * 版 	本	：V0.1.160214
 * 从属关系	：PoolAuto
 * 库版本	：无
 * 创建时间	：2016.1.28
 * 最后编辑	：2016.2.14
 **-------------------------------------------------------------------------------

 * 作	者	：Damm Stanger
 * 邮	箱	：dammstanger@qq.com
**********************************************************************************************/

/****************************包含头文件*******************************************/
#include "globaldefine.h"
#include "ADC_51.h"
#include "UART_51.h"
#include "Transmit.h"
#include "Sensor.h"
#include "DS18B20.h"
/****************************宏定义***********************************************/

/****************************变量定义*********************************************/
PAG_DATA sensor_data={0xff,0xff,0xff,0xff,0xff,0xff,0,0};

char g_sensor_sta1 = 0;							//传感器状态字1

/********************************************************************************
 * 函数名：TemperDatHandle()
 * 描述  ：温度数据处理
 * 输入  ：-tmp 传感器温度值放大100倍
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void TemperDatHandle()
{
	uint dat;
	dat = DS18B20_ReadTemperature(1);
	sensor_data.temp1_h = (uchar)(dat>>8);
	sensor_data.temp1_l = (uchar)dat;	
//	SendString("temp1:\r\n");									//调试信息时候用
//	SendTemp(dat);
//	SendString("\r\n");
	dat = DS18B20_ReadTemperature(2);
	sensor_data.temp2_h = (uchar)(dat>>8);
	sensor_data.temp2_l = (uchar)dat;	
//	SendString("temp2:\r\n");									//调试信息时候用
//	SendTemp(dat);
//	SendString("\r\n");
	g_sensor_sta1 |= TMP_RDY;									//温度采集完成置位
}	

/********************************************************************************
 * 函数名：PressDatHandle()
 * 描述  ：压力数据处理
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/
void PressDatHandle(uchar dat_h,uchar dat_l)
{
	sensor_data.press_h = dat_h;
	sensor_data.press_l = dat_l;	
	SendString("ADC data:\r\n");			
	SendByteASCII(sensor_data.press_h);
	SendByteASCII(sensor_data.press_l);
	SendString("\r\n");	
	g_sensor_sta1 |= PRS_RDY;				//压力采集完成置位
}	


/********************************************************************************
 * 函数名：SPI_ISR()
 * 描述  ：SPI中断服务函数
 * 输入  ：-
 * 返回  ：-
 * 调用  ：-
 ********************************************************************************/

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/
