/*************************************************************************************************************
 * �ļ���:			SIMCOM_GSM.h
 * ����:			SIMCOM GSM��ؽӿ�
 * ����:			cp1300@139.com
 * ����ʱ��:		2015-02-15
 * ����޸�ʱ��:	2018-03-23
 * ��ϸ:			
*************************************************************************************************************/
#ifndef _SIMCOM_GSM_H_
#define _SIMCOM_GSM_H_
#include "system.h"
#include "simcom.h"
 
bool SIMCOM_NetworkConfig(SIMCOM_HANDLE *pHandle, SIMCOM_MODE_TYPE ModeType, NETWORK_CONFIG_TYPE *pConfig);		//SIMCOM��������
SIM_CARD_STATUS SIMCOM_GetCPIN(SIMCOM_HANDLE *pHandle);															//��ȡSIM��״̬
SIMCOM_NETSTATUS SIM900_GetGSMNetworkStatus(SIMCOM_HANDLE *pHandle);											//��ȡGSM����ע��״̬
SIMCOM_NETSTATUS SIMCOM_GetDataNetworkStatus(SIMCOM_HANDLE *pHandle);											//��ȡ��������ע��״̬
bool SIMCOM_ModuleInit(SIMCOM_HANDLE *pHandle);																	//��ʼ��SIMCOMģ��������ã�������ʧ�ܣ�
bool SIMCOM_GetModuleInfo(SIMCOM_HANDLE *pHandle, SIMCOM_INFO *pInfo);											//��ȡģ��������Ϣ
bool SIMCOM_COPS(SIMCOM_HANDLE *pHandle, char pCOPS_Buff[SIMCOM_INFO_SIZE]);									//��ȡ��Ӫ������
SIMCOM_NETMODE_TYPE SIM7XXX_GetNetworkMode(SIMCOM_HANDLE *pHandle);												//��ȡSIM7XXXϵ��ģ��������ʽ
bool SIMCOM_HardwarePowerUP(SIMCOM_HANDLE *pHandle, bool isTest);												//SIMCOMģ��Ӳ������
bool SIMCOM_HardwarePowerDOWN(SIMCOM_HANDLE *pHandle, bool isTest);												//SIMCOMģ��Ӳ���ػ�
SIMCOM_MODE_TYPE SIMCOM_GetMode(SIMCOM_HANDLE *pHandle);														//��ȡSIMCOMģ����ͺ�
int SIMCOM_GetSignal(SIMCOM_HANDLE *pHandle);																	//��ȡ�ź�ǿ��
bool SIMCOM_GetBookNumber(SIMCOM_HANDLE *pHandle, u8 index, char pPhoneNumber[16]);								//�ӵ绰����ȡһ���绰����(��������SIM7000)
bool SIMCOM_GetPhoneNumber(SIMCOM_HANDLE *pHandle, char pPhoneNumber[16]);										//��ȡ��������(��ȥ��ǰ���86�����Ƴ���15λ,��������SIM7000��SIM2000�Լ����ſ�)
bool SIMCOM_GetServeNumber(SIMCOM_HANDLE *pHandle,  char pPhoneNumber[16]);										//��ȡ���ŷ������ĺ���(��ȥ��ǰ���86�����Ƴ���15λ,��������SIM7000��SIM2000�Լ����ſ�)
bool SIMCOM_GetCIMI(SIMCOM_HANDLE *pHandle, char pCIMI[16]);													//��ȡSIM��CIMI����(SIM��Ψһid���������)
bool SIM7000C_GetNB_APN(SIMCOM_HANDLE *pHandle, char pAPN[17]);													//��ȡSIM7000C NBIOT �����
 
#endif /*_SIMCOM_GSM_H_*/

