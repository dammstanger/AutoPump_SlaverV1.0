/******************** (C) COPYRIGHT 2016 DammStanger *****************************************
**--------------�ļ���Ϣ----------------------------------------------------------
 * �ļ���	��IIC_soft_51.c
 * ��	��	��ģ��IIC
 *            
 * ʵ��ƽ̨	���Կ�ˮ��V2.0
 * Ӳ������	��
 * �� 	��	��V0.0.160129
 * ������ϵ	��AutoPumpV2 
 * ��汾	����
 * ����ʱ��	��2016.1.29
 * ���༭	��2016.1.29
 **-------------------------------------------------------------------------------

 * ��	��	��Damm Stanger
 * ��	��	��dammstanger@qq.com
**********************************************************************************************/

#ifndef _IIC_SOFT_51_H__
#define _IIC_SOFT_51_H__
/****************************����ͷ�ļ�*******************************************/
#include "globaldefine.h"
/****************************�궨��***********************************************/

/****************************���Ͷ���*********************************************/

/****************************��������*********************************************/

/****************************��������*********************************************/
void IIC_GPIO_Config(void);
bool IIC_WriteReg(uchar slaveAddr,uchar REG_Address,uchar REG_data);
uchar IIC_ReadReg(uchar slaveAddr, uchar readAddr);
uchar IIC_MultRead(uchar slaveAddr, uchar* pBuffer, uchar readAddr, uchar NumByteToRead);
uchar IIC_MultWrite(uchar slaveAddr, uchar* pBuffer, uchar wrtieAddr, uchar NumByteToWrite);





#endif

/******************* (C) COPYRIGHT 2016 DammStanger *****END OF FILE************/

