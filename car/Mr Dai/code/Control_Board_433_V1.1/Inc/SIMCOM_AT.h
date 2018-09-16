/*************************************************************************************************************
 * �ļ���:			SIMCOM_AT.h
 * ����:			SIMCOM�ײ�ATָ��ӿ�
 * ����:			cp1300@139.com
 * ����ʱ��:		2015-02-15
 * ����޸�ʱ��:	2018-03-23
 * ��ϸ:			
*************************************************************************************************************/
#ifndef _SIMCOM_AT_H_
#define _SIMCOM_AT_H_
#include "system.h"
#include "SIMCOM.h"
 
extern bool g_SIMC0M_AT_Debug;	//�ײ�ATָ�����״̬
 
 
 
//SIM900���ش���
typedef enum
{
	AT_RETURN_OK			=		0,		//���سɹ�
	AT_RETURN_ERROR			=		1,		//���ش���
	AT_RETURN_UNKNOWN		=		2,		//���ؽ��δ֪
	AT_RETURN_TIME_OUT		=		0xf,	//�ȴ����س�ʱ
}SIMCOM_AT_ERROR;
 
 
 
 
//��ؽӿ�
bool SIMCOM_SendAT(SIMCOM_HANDLE *pHandle, char *pStr);																						//����һ��ATָ�����ӽ�����\r\n��,����ȴ���Ӧ
SIMCOM_AT_ERROR SIMCOM_GetATResp(SIMCOM_HANDLE *pHandle, u8 **pRxBuff, u32 *pLen, const char *pKeyword, u8 ByteTimeOutMs, u16 TimeOutMs);	//��ȡSIMCOM��ATָ����Ӧ
bool SIMCOM_SendAT(SIMCOM_HANDLE *pHandle, char *pStr);																						//����һ��ATָ�����ӽ�����\r\n��,����ȴ���Ӧ
bool SIMCOM_WaitSleep(SIMCOM_HANDLE *pHandle, u32 TimeOutMs);																				//�ȴ�ģ�����,�����»���
bool SIMCOM_TestAT(SIMCOM_HANDLE *pHandle, u32 retry);																						//SIMCOM AT ����ͨ�Ų���
#define SIMCOM_Ready(pHandle)  if(SIMCOM_TestAT(pHandle, 5) == FALSE){SIMCOM_WaitSleep(pHandle, 1000);}										//��SIMCOM����,��ֹ��ס//����ͬ��ʧ��,�ȴ���һ���������
bool SIMCOM_SetParametersReturnBool(SIMCOM_HANDLE *pHandle, char *pATCom, u8 retry, u16 TimeOutMs, const char *pErrorDebug);				//����SIM900һ������,����һ��bool״̬
 
 
 
//ͨ�ù���
u32 GSM_StringToHex(char *pStr, u8 NumDigits);				//��16������ʽ�ַ���ת��Ϊ16����������(���뱣֤�ַ�����ĸ���Ǵ�д)
u32 GSM_StringToDec(char *pStr, u8 NumDigits);				//��10������ʽ�ַ���ת��Ϊ������(���뱣֤��ȫΪ�����ַ�)
void GSM_HexToString(u32 HexNum,char *pStr, u8 NumDigits);	//����������ת��Ϊ16������ʽ�ַ���(��ĸΪ��д,����������)
 
#endif /*SIMCOM_AT*/

