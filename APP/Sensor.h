/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------文件信息----------------------------------------------------------
 * 文件名	：Sensor.h
 * 描	述	：传感器数据处理
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
#ifndef __SENSOR_H_
#define __SENSOR_H_

#include "globaldefine.h"



#define PRS_RDY			0x80				//压力采集完成
#define TMP_RDY			0x40				//温度采集完成

#define SW_ON			0					//以电平定义开关量的通断
#define SW_OFF			1
extern char g_sensor_sta1;

void PressDatHandle(uchar dat_h,uchar dat_l);
void TemperDatHandle();

#endif



/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/


