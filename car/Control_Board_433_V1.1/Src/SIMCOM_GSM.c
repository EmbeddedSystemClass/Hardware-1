/*************************************************************************************************************
 * �ļ���:			SIMCOM_GSM.c
 * ����:			SIMCOM GSM��ؽӿ�
 * ����:			cp1300@139.com
 * ����ʱ��:		2015-02-15
 * ����޸�ʱ��:	2018-03-23
 * ��ϸ:			
*************************************************************************************************************/
#include "system.h"
#include "usart.h"
#include "SIMCOM_GSM.h"
#include "SIMCOM_AT.h"
#include "string.h"
#include "SIMCOM.h"
#include <stdlib.h>
 
bool g_SIMC0M_GSM_Debug = TRUE;	//�ײ�ATָ�����״̬
 
//���Կ���
#define SIMCOM_GSM_DBUG		1
#if SIMCOM_GSM_DBUG
	#include "system.h"
	#define SIMCOM_GSM_debug(format,...)	{if(g_SIMC0M_GSM_Debug){uart_printf(format,##__VA_ARGS__);}}
#else
	#define SIMCOM_GSM_debug(format,...)	/\
/
#endif	//SIMCOM_GSM_DBUG
 
 
 
 
/*************************************************************************************************************************
* ����			:	bool SIMCOM_NetworkConfig(SIMCOM_HANDLE *pHandle, SIMCOM_MODE_TYPE ModeType, SIMCOM_SIM_SELECT SIM_Select)
* ����			:	SIMCOM��������
* ����			:	pHandle�������ModeType��ͨ��ģ���ͺţ�SIM_Select:SIM��ѡ��
* ����			:	TRUE:�ɹ�,FALSE:ʧ��	
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2018-01-17
* ����޸�ʱ��	: 	2018-03-24
 * ��ϸ			:
*************************************************************************************************************************/ 
bool SIMCOM_NetworkConfig(SIMCOM_HANDLE *pHandle, SIMCOM_MODE_TYPE ModeType, NETWORK_CONFIG_TYPE *pConfig)	
{
	char buff[16];
	
	pConfig->ModeType = ModeType;					//��¼ͨ��ģ���ͺ�
	if(ModeType == SIMCOM_SIM7000C)					//SIM7000C��Ҫѡ����ģʽ
	{	
		switch(pConfig->NB_EnableMode)
		{
			case 0: //GSMģʽ
			{
				uart_printf("[DTU]����GSM����ģʽ!\r\n");
				if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CNMP=13", SIMCOM_DEFAULT_RETRY, 200, "\r\n����SIM7000C GSMģʽʧ��!\r\n") == FALSE) return FALSE; //GSMģʽ
			}break;
			case 1://NBģʽ
			{
				uart_printf("[DTU]����NBIOT����ģʽ!\r\n");
				if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CNMP=38", SIMCOM_DEFAULT_RETRY, 200, "\r\n����SIM7000C LTE NBģʽʧ��!\r\n") == FALSE) return FALSE; //LTE only��ʹ��NB-IOT����ʱCNMP��Ҫ����Ϊ38��
				//CAT NBģʽ����
				switch(pConfig->ModeType)
				{
					case CAT_M_MODE: //CATģʽ
					{
						sprintf(buff,"AT+CMNB=%d",1); //catģʽ
					}break;
					default: 
					{
						sprintf(buff,"AT+CMNB=%d",2); //NBIOTģʽ
					}break;
				}
				if(SIMCOM_SetParametersReturnBool(pHandle, buff, SIMCOM_DEFAULT_RETRY, 200, "\r\n����SIM7000C CAT NBģʽʧ��!\r\n") == FALSE) return FALSE; //1: CAT-M 2: NB-IOT
				//��������
				if(pConfig->isNB_ScarEnable) //��������
				{
					sprintf(buff,"AT+NBSC=%d",1); 
				}
				else
				{
					sprintf(buff,"AT+NBSC=%d",0); 
				}
				if(SIMCOM_SetParametersReturnBool(pHandle, buff, SIMCOM_DEFAULT_RETRY, 200, "\r\n����SIM7000C NB ����ģʽʧ��!\r\n") == FALSE) return FALSE; 
			}break;
			default:return TRUE;					//���ԣ���������
		}
	}
	return TRUE;
}
 
 
 
 
/*************************************************************************************************************************
* ����				:	SIM_CARD_STATUS SIMCOM_GetCPIN(SIMCOM_HANDLE *pHandle)
* ����				:	��ȡSIM��״̬
* ����				:	��
* ����				:	FALSE:ʧ��;TRUE:�ɹ�
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2014-06-26
* ����޸�ʱ�� 		: 	2014-06-26
* ˵��				: 	2017-09-05 �� ����SIM��״̬Ϊ3��״̬
*************************************************************************************************************************/
SIM_CARD_STATUS SIMCOM_GetCPIN(SIMCOM_HANDLE *pHandle)
{
	u32 cnt;
	char *p;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	int status;
	u8 *pData;
	
	do
	{	
		//+CPIN: READY
		SIMCOM_SendAT(pHandle, "AT+CPIN?");											
		pHandle->pClearRxData();												//������ռ�����
		status = SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 20, 200);		//�ȴ���Ӧ,��ʱ200MS
		if(AT_RETURN_OK == status)												//����OK
		{
			p = strstr((const char*)pData, "+CPIN: READY");						//�����ַ�"+CPIN: READY"
			if(p != NULL) 														//�����ɹ�
			{
				return SIM_READY;												//SIM������
			}
			break;
		}
		else if(AT_RETURN_ERROR == status)										//����ERROR
		{
			p = strstr((const char*)pData, "ERROR");							//������δ׼��������־
			if(p != NULL) 														//�����ɹ�
			{
				return SIM_NOT_READY;											//SIM��δ����
			}
			break;
		}
		
		SIMCOM_Ready(pHandle);													//�ȴ�����
		pHandle->pDelayMS(1000);												//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return SIM_UNKNOWN;															//SIM��δ֪
}
 
 
 
/*************************************************************************************************************************
* ����				:	SIMCOM_NETSTATUS SIM900_GetGSMNetworkStatus(SIMCOM_HANDLE *pHandle)
* ����				:	��ȡGSM����ע��״̬
* ����				:	pHandle:���
* ����				:	SIMCOM_NETSTATUS
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2013-10-29
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	������ע���,���ܱ��ܾ�,������ܾ�,��ȡ����ע��״̬����ʾ
						ע��ɹ���,����ͨ������AT ����ȥ��ѯ,�ᷢ������ע��ʧ��
*************************************************************************************************************************/
SIMCOM_NETSTATUS SIM900_GetGSMNetworkStatus(SIMCOM_HANDLE *pHandle)
{
	u32 cnt;
	char *p;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	
	do
	{	
		//+CREG: 0,1
		SIMCOM_SendAT(pHandle, "AT+CREG?");											//����"AT+CREG?",��ȡ����ע��״̬
		pHandle->pClearRxData();													//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 20, 200))	//�ȴ���Ӧ,��ʱ200MS
		{
			p = strstr((const char*)pData, "+CREG:");								//�����ַ�"+CREG:"
			if(p != NULL) 															//�����ɹ�
			{
				SIMCOM_TestAT(pHandle, 1);
				return (SIMCOM_NETSTATUS)GSM_StringToDec(&p[9], 1);
			}
			break;
		}
		
		SIMCOM_Ready(pHandle);														//�ȴ�����
		pHandle->pDelayMS(1000);													//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	SIMCOM_TestAT(pHandle, 2);
	return SIMCOM_NET_ERROR;
}
 
 
 
 
/*************************************************************************************************************************
* ����				:	SIMCOM_NETSTATUS SIMCOM_GetDataNetworkStatus(SIMCOM_HANDLE *pHandle)
* ����				:	��ȡ��������ע��״̬
* ����				:	pHandle:���
* ����				:	SIMCOM_NETSTATUS
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2013-10-29
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	���ڻ�ȡNB���������GPRS��������ע��״̬
*************************************************************************************************************************/
SIMCOM_NETSTATUS SIMCOM_GetDataNetworkStatus(SIMCOM_HANDLE *pHandle)
{
	u32 cnt;
	char *p;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	
	do
	{	
		//+CGREG: 0,1
		SIMCOM_SendAT(pHandle, "AT+CGREG?");										//����"AT+CGREG?",��ȡ����ע��״̬
		pHandle->pClearRxData();													//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 20, 200))	//�ȴ���Ӧ,��ʱ200MS
		{
			p = strstr((const char*)pData, "+CGREG:");								//�����ַ�"+CGREG:"
			if(p != NULL) 															//�����ɹ�
			{
				SIMCOM_TestAT(pHandle, 1);
				return (SIMCOM_NETSTATUS)GSM_StringToDec(&p[10], 1);
			}
			break;
		}
		
		SIMCOM_Ready(pHandle);														//�ȴ�����
		pHandle->pDelayMS(1000);													//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	SIMCOM_TestAT(pHandle, 2);
	return SIMCOM_NET_ERROR;
}
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIM900_SetGPRS_PackDatatSize(SIMCOM_HANDLE *pHandle)
* ����				:	����SIM900/SIM800 GPRS�������ݻ�����
* ����				:	pHandle�����
* ����				:	FALSE:ʧ��;TRUE:�ɹ�
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2014-09-11
* ����޸�ʱ�� 		: 	2014-09-11
* ˵��				: 	����������ݰ�1460B����
*************************************************************************************************************************/
bool SIM900_SetGPRS_PackDatatSize(SIMCOM_HANDLE *pHandle)
{
	char buff[36];
 
	//�ȿ���͸��ģʽ��������
	SIMCOM_SetParametersReturnBool(pHandle, "AT+CIPMODE=1", SIMCOM_DEFAULT_RETRY, 2000, "����͸��ģʽʧ��!\r\n");	//����͸��ģʽ
	
	//����GPRS�������ݰ���С
	//AT+CIPCCFG=3,2,1024,1 //����͸������ //3-�ش�����Ϊ3��,2-�ȴ���������ʱ��Ϊ //2*200ms,1024-���ݻ�����Ϊ1024���ֽ� //1-֧��ת���˳�͸��
	sprintf(buff,"AT+CIPCCFG=3,2,%d,1",1460);
	return SIMCOM_SetParametersReturnBool(pHandle, buff, SIMCOM_DEFAULT_RETRY, 200, "GPRS�������ݻ���������ʧ��!\r\n");	//����	
}
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_ModuleInit(SIMCOM_HANDLE *pHandle)
* ����				:	��ʼ��SIMCOMģ��������ã�������ʧ�ܣ�
* ����				:	pHandle:���
* ����				:	FALSE:��ʼ��ʧ��;TRUE:��ʼ���ɹ�
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2013-10-16
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	�������ϵ磬����ȡģ���ͺţ����ݲ�ͬ���ͺ�ģ��ֱ���г�ʼ��
*************************************************************************************************************************/
bool SIMCOM_ModuleInit(SIMCOM_HANDLE *pHandle)
{
	u8 retry = 5;							//���Դ���
	
	pHandle->pSetDTR_Pin(SIMCOM_L_LEVEL);	//DTR=0,�˳��͹���ģʽ
	//���ģ�����,����֤ͨ������
	SIMCOM_Ready(pHandle);
	SIMCOM_TestAT(pHandle, 20);
	
	switch(pHandle->SimcomModeType)			//��ͬ��оƬ���ڲ�һ���ĳ�ʼ��
	{
		case SIMCOM_SIM2000:				//SIM2000��Ҫ�ȹر�URC,�������ʾCall Ready
		{
			SIMCOM_SetParametersReturnBool(pHandle, "AT+CIURC=0", SIMCOM_DEFAULT_RETRY, 110, "\r\n�ر�Call Ready��ʾʧ��!\r\n");
		}break;
		default:break;
	}
	//���ùرջ���
	if(SIMCOM_SetParametersReturnBool(pHandle, "ATE 0", SIMCOM_DEFAULT_RETRY, 110, "\r\n�ر�AT����ģʽʧ��!\r\n") == FALSE)
	{
		return FALSE;
	}
	//���ö���Ϣ��ʽΪPDU��ʽ
	if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CMGF=0", SIMCOM_DEFAULT_RETRY, 110, "\r\n���ö���Ϣ��ʽΪPDU��ʽʧ��!\r\n") == FALSE)
	{
		uart_printf("\r\n����DCD����ģʽʧ��!\r\n");
		return FALSE;
	}
	//����DCD����ģʽ��DCD��ֻ�������ز�����ʱΪON��
	if(SIMCOM_SetParametersReturnBool(pHandle, "AT&C1", SIMCOM_DEFAULT_RETRY, 110, "\r\n����DCD����ģʽʧ��!\r\n") == FALSE)
	{
		uart_printf("\r\n����DCD����ģʽʧ��!\r\n");
		//return FALSE;
	}
	//���� DTR ����ģʽ,DTR ��ON��OFF��TA�ڱ��ֵ�ǰ����ͨ����ͬʱ���л�������ģʽ
	if(SIMCOM_SetParametersReturnBool(pHandle, "AT&D1", SIMCOM_DEFAULT_RETRY, 110, "\r\n����DTR����ģʽʧ��!\r\n") == FALSE)
	{
		uart_printf("\r\n����DTR����ģʽʧ��!\r\n");
		//return FALSE;
	}
	
//	//ʹ��RI������ʾ
//	if(SIM900_SetParametersReturnBool("AT+CFGRI=1", SIMCOM_DEFAULT_RETRY, 11, "\r\n����RI������ʾʧ��!\r\n") == FALSE)
//	{
//		return FALSE;
//	}
	
	//����ģ��sleepģʽʹ��//����"AT+CSCLK",����SLEEPģʽ;0:�ر�;1:�ֶ�;2:�Զ�����5S�Ӻ�����
	if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CSCLK=1", SIMCOM_DEFAULT_RETRY, 110, "\r\n����SLEEPʧ��!\r\n") == FALSE)
	{
		return FALSE;
	}
	
 
 
	//��鿨�Ƿ����
	retry = 8;					//���Դ���
	do
	{
		if(SIMCOM_GetCPIN(pHandle)==SIM_READY)
		{
			uart_printf("\r\nSIM��׼������!\r\n");
			break;
		}
		else
		{
			uart_printf("\r\nSIM��δ׼������!\r\n");
		}
		
		SIMCOM_Ready(pHandle);						//�ȴ�����
		pHandle->pDelayMS(1000);					//ʧ����ʱ1�������
		retry --;
	}while(retry);
	if(retry == 0)
	{
		uart_printf("\r\nSIM��δ׼������!\r\n");
		pHandle->pClearRxData();					//������ռ�����
		return FALSE;
	}
	
//	//�ϵ�ɾ�����ж���
//	retry = SIMCOM_DEFAULT_RETRY;						//���Դ���
//	do
//	{
//		if(SIM900_DelMultiSMS(DelSMS) == TRUE)//ɾ������
//		{
//			//uart_printf("�ϵ�ɾ�����ųɹ�!\r\n");
//			break;
//		}
//		SIM900_Ready();	//�ȴ�����
//		retry --;
//	}while(retry);
//	if(retry == 0)
//	{
//		uart_printf("�ϵ�ɾ������ʧ��!\r\n");
//		SIM900_ClearRxCnt();				//���������
//		return FALSE;
//	}
	
	//2016-09-20�����õȴ���Ϣ�ϱ���ʱʱ��Ϊ1���ӣ���Ϊ������Ŀ�����ֳ�ʱ���
	switch(pHandle->SimcomModeType)	//��ͬ��оƬ���ڲ�һ���ĳ�ʼ��
	{
		case SIMCOM_SIM800:			//SIM800��Ҫ�ȴ�����ʱ�䳤һЩ
		{
			retry = 65;
		}break;
		default:retry=35;break;
	}
	
	//�ر�����Ϣ�Զ��ϱ�
	while(retry)
	{
		if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CNMI=0", SIMCOM_DEFAULT_RETRY, 110, "\r\n�ر�����Ϣ�Զ��ϱ�ʧ��!\r\n") == FALSE)
		{
			//return FALSE;
		}
		else break;
		pHandle->pDelayMS(1000);				//��ʱ1��
		retry --;
	}
	if(retry == 0) return FALSE;
 
	
	switch(pHandle->SimcomModeType)	//��ͬ��оƬ���ڲ�һ���ĳ�ʼ��
	{
		case LYNQ_L700: break;
		case SIMCOM_SIM7600:
		{
			//����TCP�շ����
			retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
			while(retry)
			{
				//�������Դ���Ϊ3�Σ����ҷ�����ʱΪ120ms
				if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CIPCCFG=3,100,,1,0,,1000", SIMCOM_DEFAULT_RETRY, 110, "\r\n����TCP/IPʧ��!\r\n") == FALSE)
				{
					//return FALSE;
				}
				else break;
				pHandle->pDelayMS(1000);				//��ʱ1��
				retry --;
			}
			if(retry == 0)
			{
				uart_printf("\r\n����TCP�ط������Լ�������ʱʧ��!\r\n");
				pHandle->pClearRxData();				//������ռ�����
				return FALSE;
			}
			//���ò��õȵ�������Ӧ
			retry = SIMCOM_DEFAULT_RETRY;			//���Դ���
			while(retry)
			{
				//�������Դ���Ϊ3�Σ����ҷ�����ʱΪ120ms
				if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CIPSENDMODE=0", SIMCOM_DEFAULT_RETRY, 110, "\r\n���õȴ�����ACK����ʧ��!\r\n") == FALSE)
				{
					//return FALSE;
				}
				else break;
				pHandle->pDelayMS(1000);				//��ʱ1��
				retry --;
			}
			if(retry == 0)
			{
				uart_printf("\r\n���ò��õȴ�����ACKʧ��!\r\n");
				pHandle->pClearRxData();				//������ռ�����
			}
			
			//��ʾ�������ݳ���
			retry = SIMCOM_DEFAULT_RETRY;			//���Դ���
			while(retry)
			{
				//�������Դ���Ϊ3�Σ����ҷ�����ʱΪ120ms
				if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CIPHEAD=1", SIMCOM_DEFAULT_RETRY, 110, "\r\n��ʾ�������ݳ�������ʧ��!\r\n") == FALSE)
				{
					//return FALSE;
				}
				else break;
				pHandle->pDelayMS(1000);				//��ʱ1��
				retry --;
			}
			if(retry == 0)
			{
				uart_printf("\r\n������ʾ�������ݳ���ʧ��!\r\n");
				pHandle->pClearRxData();				//������ռ�����
				return FALSE;
			}
			
			//����ʾ��������IPͷ
			retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
			while(retry)
			{
				//�������Դ���Ϊ3�Σ����ҷ�����ʱΪ120ms
				if(SIMCOM_SetParametersReturnBool(pHandle, "AT+CIPSRIP=0", SIMCOM_DEFAULT_RETRY, 110, "\r\n����ʾ��������IPͷ����ʧ��!\r\n") == FALSE)
				{
					//return FALSE;
				}
				else break;
				pHandle->pDelayMS(1000);				//��ʱ1��
				retry --;
			}
			if(retry == 0)
			{
				uart_printf("\r\n����ʾ��������IPͷʧ��!\r\n");
				pHandle->pClearRxData();				//������ռ�����
				return FALSE;
			}
			
		}break;
		default:	//2Gģ�����Ҫ�������õ�	
		{
			//����GPRS�������ݻ�������С
			retry = SIMCOM_DEFAULT_RETRY;			//���Դ���
			do
			{
				if(SIM900_SetGPRS_PackDatatSize(pHandle) == TRUE)
				{
					break;
				}
				retry --;
			}while(retry);
			if(retry == 0)
			{
				uart_printf("\r\n����GPRS�����Сʧ��!\r\n");
				pHandle->pClearRxData();				//������ռ�����
				return FALSE;
			}
		}break;
	}
	
 
	pHandle->s_isInitStatus = TRUE;					//ģ��ɹ���ʼ��
	pHandle->pClearRxData();						//������ռ�����
	
	return TRUE;
}
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_GetModuleInfo(SIMCOM_HANDLE *pHandle, SIMCOM_INFO *pInfo)
* ����				:	��ȡģ��������Ϣ
* ����				:	pHandle:�����pInfo:��Ϣ�ṹ��ָ��
* ����				:	FALSE:ʧ��;TRUE:�ɹ�
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2014-07-29
* ����޸�ʱ�� 		: 	2014-10-08
* ˵��				: 	SIMCOM_INFO_SIZE:������󳤶�
						SIMCOM_VER_SIZE:����汾��������
						2014-10-08���ڸ���ģ��������������AT+GMI�󷵻���AT+GMI�����»�ȡʧ�ܣ�������ַ�����AT+�����»�ȡ�����Ա��������
						2016-12-07:�޸Ļ�ȡģ���ͺ�ָ��ΪAT+CGMM,���ڼ���SIM7600
*************************************************************************************************************************/
bool SIMCOM_GetModuleInfo(SIMCOM_HANDLE *pHandle, SIMCOM_INFO *pInfo)
{
	u32 i,cnt;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	char *p;
	u8 *pData;
	
	//��ջ�����
	pInfo->Manu[0] = 0;
	pInfo->Model[0] = 0;
	pInfo->Ver[0] = 0;
	pInfo->IMEI[0] = 0;
	
	retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	//��ȡ��������Ϣ
	do
	{
		SIMCOM_TestAT(pHandle, 10);
		SIMCOM_SendAT(pHandle, "AT+GMI");												//�������������
		
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 200))		//�ȴ���Ӧ,��ʱ200MS
		{
			//uart_printf("%s\r\n",pData);
			if(strstr((const char*)pData, "AT+") == NULL) 								//�����ؼ���
			{
				for(i = 0;i < (SIMCOM_INFO_SIZE-1);i ++)
				{
					if((pData[2+i] == '\r') || (pData[2+i] == '\n') || (pData[2+i] == '\0')) break;
					pInfo->Manu[i] = pData[2+i];
				}
				pInfo->Manu[i] = 0;
				break;
			}
		}	
		SIMCOM_Ready(pHandle);															//�ȴ�����
		pHandle->pDelayMS(1000);														//ʧ����ʱ1�������
		retry --;
	}while(retry);
	if(retry == 0) return FALSE;
	
	retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	//��ȡ�ͺ�
	do
	{
		SIMCOM_SendAT(pHandle, "AT+CGMM");	
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 200))		//�ȴ���Ӧ,��ʱ200MS
		{
			for(i = 0;i < (SIMCOM_INFO_SIZE-1);i ++)
			{
				if((pData[2+i] == '\r') || (pData[2+i] == '\n') || (pData[2+i] == '\0')) break;
				pInfo->Model[i] = pData[2+i];
			}
			pInfo->Model[i] = 0;
			break;
		}
		SIMCOM_Ready(pHandle);															//�ȴ�����
		pHandle->pDelayMS(1000);														//ʧ����ʱ1�������
		retry --;
	}while(retry);
	if(retry == 0) return FALSE;
	
	retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	//��ȡ����汾
	do
	{
		SIMCOM_SendAT(pHandle, "AT+GMR");
		pHandle->pClearRxData();														//������ռ�����	
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 200))		//�ȴ���Ӧ,��ʱ200MS
		{
			p = strstr((char *)pData, "+GMR: ");
			if(p != NULL) 
			{
				p+= strlen("+GMR: ");	//SIM7600ǰ����� +GMR:  ����������
				for(i = 0;i < (SIMCOM_VER_SIZE-1);i ++)
				{
					if((p[i] == '\r') || (p[i] == '\n') || (p[i] == '\0')) break;
					pInfo->Ver[i] = p[i];
				}
				pInfo->Ver[i] = 0;
			}
			else 
			{
				for(i = 0;i < (SIMCOM_VER_SIZE-1);i ++)
				{
					if((pData[2+i] == '\r') || (pData[2+i] == '\n') || (pData[2+i] == '\0')) break;
					pInfo->Ver[i] = pData[2+i];
				}
				pInfo->Ver[i] = 0;
			}
			
			break;
		}
		SIMCOM_Ready(pHandle);															//�ȴ�����
		pHandle->pDelayMS(1000);														//ʧ����ʱ1�������
		retry --;
	}while(retry);
	if(retry == 0) return FALSE;
	
	retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	//��ȡ���к�
	do
	{
		SIMCOM_SendAT(pHandle, "AT+GSN");	
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 200))		//�ȴ���Ӧ,��ʱ200MS
		{
			for(i = 0;i < (SIMCOM_INFO_SIZE-1);i ++)
			{
				if((pData[2+i] == '\r') || (pData[2+i] == '\n') || (pData[2+i] == '\0')) break;
				pInfo->IMEI[i] = pData[2+i];
			}
			pInfo->IMEI[i] = 0;
			break;
		}
		SIMCOM_Ready(pHandle);															//�ȴ�����
		pHandle->pDelayMS(1000);														//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	
	return TRUE;									
}
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_COPS(SIMCOM_HANDLE *pHandle, char pCOPS_Buff[SIMCOM_INFO_SIZE])
* ����				:	��ȡ��Ӫ������
* ����				:	pHandle:�����pCOPS_Buff:��Ӫ������
* ����				:	FALSE:ʧ��;TRUE:�ɹ�
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2014-07-29
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	SIMCOM_INFO_SIZE ������󳤶�
*************************************************************************************************************************/
bool SIMCOM_COPS(SIMCOM_HANDLE *pHandle, char pCOPS_Buff[SIMCOM_INFO_SIZE])
{
	u32 i,cnt;
	u8 retry = 5;						//���Դ���
	char *p;
	u8 *pData;
	
	//��ջ�����
	pCOPS_Buff[0] = 0;
	
	switch(pHandle->SimcomModeType)		//��ͬ��оƬ���ڲ�һ���ĳ�ʼ��
	{
		case SIMCOM_SIM2000:			//SIM2000��Ҫ��ζ�ȡ,�ȴ���ʱ��Ƚϳ�
		{
			retry = 28;
		}break;
		default:break;
	}
	
	//��ȡ��Ӫ��
	do
	{
		SIMCOM_SendAT(pHandle, "AT+COPS?");											//��ʾģ�鵱ǰע���������Ӫ��
		pHandle->pClearRxData();													//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 20, 150))	//�ȴ���Ӧ,��ʱ300MS
		{
			p = strstr((const char*)pData, "\"");
			if(p != NULL)
			{
				p ++;
				for(i = 0;i < (SIMCOM_INFO_SIZE-1);i ++)
				{
					if((p[i] == '\r') || (p[i] == '\n') || (p[i] == '\0') || (p[i] == '\"')) break;
					pCOPS_Buff[i] = p[i];
				}
				pCOPS_Buff[i] = 0;
				return TRUE;
			}
		}
		
		SIMCOM_Ready(pHandle);													//�ȴ�����
		pHandle->pDelayMS(1000);											//��ʱ1��
		retry --;
	}while(retry);
	
	
	return FALSE;																//��ʱ//����	
}
 
 
 
/*************************************************************************************************************************
* ����				:	SIMCOM_NETMODE_TYPE SIM7XXX_GetNetworkMode(SIMCOM_HANDLE *pHandle)
* ����				:	��ȡSIM7XXXϵ��ģ��������ʽ
* ����				:	pHandle:���
* ����				:	SIMCOM_NETMODE_TYPE
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2016-12-29
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	���ڴ�SIM7600ģ��������ʽ
						����������ע��ɹ�����л�ȡ����������
						+CNSMOD: 0,15
						����SIM7000ϵ�л�ȡ����ʽ
*************************************************************************************************************************/
SIMCOM_NETMODE_TYPE SIM7XXX_GetNetworkMode(SIMCOM_HANDLE *pHandle)
{
	u32 cnt;
	u8 retry = 3;
	char *p;
	int temp;
	u8 *pData;
	
	//��ȡ�ͺ�
	do
	{
		SIMCOM_SendAT(pHandle, "AT+CNSMOD?");	
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 200))		//�ȴ���Ӧ,��ʱ200MS
		{
			p = strstr((char *)pData, "+CNSMOD: 0,");
			if(p==NULL) p = strstr((char *)pData, "+CNSMOD: 1,");
			p += strlen("+CNSMOD: 0,");
			temp = atoi(p);
			if(temp > 16) continue; 
			else return (SIMCOM_NETMODE_TYPE)temp;
		}
		
 
		SIMCOM_Ready(pHandle);													//�ȴ�����
		pHandle->pDelayMS(1000);												//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return SIMCOM_NETMODE_NULL;
}
 
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_HardwarePowerUP(SIMCOM_HANDLE *pHandle, bool isTest)
* ����				:	SIMCOMģ��Ӳ������
* ����				:	pHandle�������isTest:�Ƿ��⿪���Ƿ�ɹ�,ͨ��STATUS�ű�Ϊ�ߵ�ƽ���Լ���Ƿ񿪻��ɹ�
* ����				:	�����Ƿ�ɹ�
* ����				:	��
* ����				:	cp1300@139.com
* ʱ��				:	2013-10-29
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	����SIM900ģ�鿪��,����PWR
						2016-12-07:�޸Ķ�̬����Ƿ��ϵ�ɹ�������SIM7600CE����
*************************************************************************************************************************/
bool SIMCOM_HardwarePowerUP(SIMCOM_HANDLE *pHandle, bool isTest)
{	
	u8 i,j;
	
	pHandle->pSetDTR_Pin(SIMCOM_L_LEVEL);	//DTR=0,�˳��͹���ģʽ
	pHandle->s_isInitStatus = FALSE;		//ģ��û�г�ʼ��,��Ҫ���³�ʼ��
	if(isTest)								//��Ҫ����Ƿ񿪻��ɹ�
	{
		if(pHandle->pGetSTATUS_Pin() == SIMCOM_H_LEVEL)				//�������Ѿ��Ǹߵ�ƽ��
		{
			return TRUE;
		}			
		for(i = 0;i < 2;i ++)
		{
			pHandle->pSetPWRKEY_Pin(SIMCOM_L_LEVEL);	//����1200ms����
			pHandle->pDelayMS(1200);
			pHandle->pSetPWRKEY_Pin(SIMCOM_H_LEVEL);	//�ָ��ߵ�ƽ
			for(j = 0;j < 6;j ++)
			{
				pHandle->pDelayMS(1000);
				if(pHandle->pGetSTATUS_Pin() == SIMCOM_H_LEVEL)				//�������Ѿ��Ǹߵ�ƽ��
				{
					return TRUE;
				}
			}
		}
		
		return FALSE;
	}
	else	//�������Ƿ񿪻��ɹ�
	{
		pHandle->pSetPWRKEY_Pin(SIMCOM_L_LEVEL);	//����1200ms����
		pHandle->pDelayMS(1200);
		pHandle->pSetPWRKEY_Pin(SIMCOM_H_LEVEL);	//�ָ��ߵ�ƽ
		pHandle->pDelayMS(3000);;					//��ʱ3S�ȴ��������	
		
		return TRUE;
	}
}
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_HardwarePowerDOWN(SIMCOM_HANDLE *pHandle, bool isTest)
* ����				:	SIMCOMģ��Ӳ���ػ�
* ����				:	pHandle�������isTest:�Ƿ��⿪���Ƿ�ɹ�,ͨ��STATUS�ű�Ϊ�ߵ�ƽ���Լ���Ƿ�ػ��ɹ�
* ����				:	�ػ��Ƿ�ɹ�
* ����				:	��
* ����				:	cp1300@139.com
* ʱ��				:	2013-10-29
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	����SIM900ģ��ػ���,����PWR����1SС��5S
						2016-12-07:�Ż��ػ�������SIM7600
						һ��Ҫ�Ȼ�ȡģ���ͺţ���ͬģ��ػ�ʱ�䲻һ��
*************************************************************************************************************************/
bool SIMCOM_HardwarePowerDOWN(SIMCOM_HANDLE *pHandle, bool isTest)
{
	u8 i,j;
	
	pHandle->s_isInitStatus = FALSE;		//ģ��û�г�ʼ��,��Ҫ���³�ʼ��
	if(isTest)								//��Ҫ����Ƿ񿪻��ɹ�
	{
		if(pHandle->pGetSTATUS_Pin() == SIMCOM_L_LEVEL)				//�������Ѿ��ǵ͵�ƽ��
		{
			return TRUE;
		}
		for(i = 0;i < 2;i ++)
		{
			pHandle->pSetPWRKEY_Pin(SIMCOM_L_LEVEL);				//����1200ms�ػ�
			switch(pHandle->SimcomModeType)
			{
				case SIMCOM_SIM7600:
				{
					pHandle->pDelayMS(3000);						//SIM7600�ػ�����2.5S
				}break;
				default:
				{
					pHandle->pDelayMS(1000);
				}break;
			}
				
			pHandle->pDelayMS(200);
			pHandle->pSetPWRKEY_Pin(SIMCOM_H_LEVEL);				//�ָ��ߵ�ƽ
			for(j = 0;j < 5;j ++)
			{
				pHandle->pDelayMS(3000);							//��ʱ3S�ȴ��ػ����	
				if(pHandle->pGetSTATUS_Pin() == SIMCOM_L_LEVEL)		//�������Ѿ��ǵ͵�ƽ��
				{
					return TRUE;
				}
			}
		}
		
		return FALSE;
	}
	else	//�������Ƿ񿪻��ɹ�
	{
		pHandle->pSetPWRKEY_Pin(SIMCOM_L_LEVEL);	//����1200ms�ػ�
		pHandle->pDelayMS(1200);
		pHandle->pSetPWRKEY_Pin(SIMCOM_H_LEVEL);	//�ָ��ߵ�ƽ
		pHandle->pDelayMS(3000);;					//��ʱ3S�ȴ��ػ����	
		
		return TRUE;
	}
}
 
 
/*************************************************************************************************************************
* ����				:	SIMCOM_MODE_TYPE SIMCOM_GetMode(SIMCOM_HANDLE *pHandle)
* ����				:	��ȡSIMCOMģ����ͺ�
* ����				:	pHandle:���
* ����				:	�ͺ�,��SIMCOM_MODE_TYPE
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2015-09-12
* ����޸�ʱ�� 		: 	2015-09-12
* ˵��				: 	����ʶ���ͺ�,��Ӧ��ʼ��
						2016-12-07:�޸�ָ��ΪAT+CGMM������SIM7600��(��ָ��:AT+GOI)
*************************************************************************************************************************/
SIMCOM_MODE_TYPE SIMCOM_GetMode(SIMCOM_HANDLE *pHandle)
{
	u32 cnt;
	u8 retry = SIMCOM_DEFAULT_RETRY+1;
	u8 *pData;
	
	//��ȡ�ͺ�
	do
	{
		SIMCOM_SendAT(pHandle, "AT+CGMM");	
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 200))		//�ȴ���Ӧ,��ʱ200MS
		{
			if(strstr((char *)pData, "SIM900") != NULL)  return SIMCOM_SIM900;
			else if(strstr((char *)pData, "SIM800") != NULL)  return SIMCOM_SIM800;
			else if(strstr((char *)pData, "SIM2000") != NULL)  return SIMCOM_SIM2000;
			else if(strstr((char *)pData, "SIM7600") != NULL)  return SIMCOM_SIM7600;
			else if(strstr((char *)pData, "SIMCOM_SIM868") != NULL)  return SIMCOM_SIM868;
			else if(strstr((char *)pData, "SIMCOM_SIM7000C") != NULL)  return SIMCOM_SIM7000C;
			else if(strstr((char *)pData, "LYNQ_L700") != NULL)  return LYNQ_L700;			
			else 
			{
				uart_printf("δ֪ͨ��ģ�飺%s\r\n",pData);
				return SIMCOM_INVALID;
			}
		}
		
 
		SIMCOM_Ready(pHandle);													//�ȴ�����
		pHandle->pDelayMS(1000);												//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return SIMCOM_INVALID;
}
 
 
/*************************************************************************************************************************
* ����			:	bool SIMCOM_GetServeNumber(PHONE_NUMBER *pServeNumber)
* ����			:	��ȡ���ŷ������ĺ���(��ȥ��ǰ���86�����Ƴ���15λ,��������SIM7000��SIM2000�Լ����ſ�)
* ����			:	pServeNumber:�绰����洢������ָ��
* ����			:	FALSE:ͨ��ʧ��;TRUE:ͨ�ųɹ�
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2013-10-20
* ����޸�ʱ�� 	: 	2013-10-20
* ˵��			: 	��ȡSIM���ڲ��Ķ��ŷ������ĺ���,һ���ڰ���SIM����ʱ���Ѿ�����������.
					���û��Ԥ�ö������ĺ�����Ҫʹ���ֻ���������
					2014-07-12:ֻҪ����OK����Ϊ�ɹ�,��Ϊ�п���û�����ö������ĺ���
					2016-01-26:�Զ�ѡ���Ƿ�����+86
*************************************************************************************************************************/
bool SIMCOM_GetServeNumber(SIMCOM_HANDLE *pHandle,  char pPhoneNumber[16])
{
	u8 i,n;
	u32 cnt;
	char *p,*p1;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	
	do
	{	
		//+CSCA: "+8613800270500",145
		SIMCOM_SendAT(pHandle, "AT+CSCA?");												//����"AT+CSCA",��ȡ���ŷ������ĺ���
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 30, 600))		//�ȴ���Ӧ,��ʱ600MS
		{
			p = strstr((const char*)pData, "+CSCA:");									//�����ַ�"+CSCA:"
			if(p != NULL) 																//�����ɹ�
			{
				p = strstr(p+1, "+");	//����"+"
				if(p != NULL)
				{
					p1 = strstr(p+1, "\"");	//����"\""
					if(p1 != NULL)			
					{
						if(p[0] == '+')			//�����+��ͷ���������Ӻ�
						{
							p+=3;				//����+86
						}						
						else if(p[0]=='8' && p[1] == '6') //����86
						{
							p+=2;
						}
						n = p1 - p;			//����绰���볤��
						if(n > 15) n = 15;	//���ƺ��볤��Ϊ15�ֽ�
						for(i = 0;i < n;i ++)
						{
							pPhoneNumber[i] = p[i];	//���Ƶ绰����
						}
						pPhoneNumber[i] = '\0';		//��ӽ�����
 
						SIMCOM_GSM_debug("�������ĺ���:%s\r\n",pPhoneNumber);
						
						return TRUE;
					}
				}
			}
			else
			{
				pPhoneNumber[0] = '\0';
				SIMCOM_GSM_debug("�������ĺ���:Ϊ��,û������\r\n");
				
				return TRUE;
			}
		}
		SIMCOM_Ready(pHandle);					//�ȴ�����
		pHandle->pDelayMS(1000);				//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return FALSE;
}
 
 
/*************************************************************************************************************************
* ����			:	bool SIMCOM_GetPhoneNumber(SIMCOM_HANDLE *pHandle, char pPhoneNumber[16])
* ����			:	��ȡ��������(��ȥ��ǰ���86�����Ƴ���15λ,��������SIM7000��SIM2000�Լ����ſ�)
* ����			:	pHandle:�����pPhoneNumber�����뻺����
* ����			:	FALSE:ͨ��ʧ��;TRUE:ͨ�ųɹ�
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2013-10-20
* ����޸�ʱ�� 	: 	2018-03-24
* ˵��			: 	ͨ����Ԥ�汾�����뵽SIM��,Ҳ����û�У���������SIM7000��SIM2000C,�������ڵ��ſ���
					2014-07-12:ֻҪ����OK����Ϊ�ɹ�,��Ϊ�п���û�����õ绰����
					2016-01-26:�޸��ֽڳ�ʱ������ĳЩ������ֳ�ʱ��û���յ�OK
								�Զ�ѡ���Ƿ�����+86
*************************************************************************************************************************/
bool SIMCOM_GetPhoneNumber(SIMCOM_HANDLE *pHandle, char pPhoneNumber[16])
{
	u8 n;
	u8 i;
	u32 cnt;
	char *p,*p1;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	
 
	do
	{	
		//+CNUM: "","15871750634",129,7,4
		SIMCOM_SendAT(pHandle, "AT+CNUM");												//����"AT++CNUM",��ȡ����
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 30, 600))		//�ȴ���Ӧ,��ʱ600MS
		{
			p = strstr((const char*)pData, "+CNUM:");									//�����ַ�"+CNUM:"		
			if(p != NULL) 																//�����ɹ�
			{
				p = strstr(p+1, "\",\"");		//����","	//��ʼ
				if(p != NULL)
				{
					p1 = strstr(p+3, "\",");	//����",//����
					if(p1 != NULL)			
					{
						p+=3;	//����","
						if(p[0] == '+')			//�����+��ͷ���������Ӻ�
						{
							p+=3;				//����+86
						}						
						else if(p[0]=='8' && p[1] == '6') //����86
						{
							p+=2;
						}
						
						n = p1 - p;			//����绰���볤��
						if(n > 15) n = 15;	//���ƺ��볤��Ϊ15�ֽ�
						for(i = 0;i < n;i ++)
						{
							pPhoneNumber[i] = p[i];	//���Ƶ绰����
						}
						pPhoneNumber[i] = '\0';		//��ӽ�����
						SIMCOM_GSM_debug("��������:%s\r\n",pPhoneNumber);
						
						return TRUE;
					}
				}
			}
			else
			{
				pPhoneNumber[0] = '\0';
				
				SIMCOM_GSM_debug("��������:Ϊ��,û������\r\n");
				return TRUE;
			}
		}
		
		SIMCOM_Ready(pHandle);				//�ȴ�����
		pHandle->pDelayMS(1000);			//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return FALSE;
}
 
 
/*************************************************************************************************************************
* ����			:	bool SIMCOM_GetBookNumber(SIMCOM_HANDLE *pHandle, u8 index, char pPhoneNumber[16])
* ����			:	�ӵ绰����ȡһ���绰����(��������SIM7000)
* ����			:	pHandle:�����index:�绰��������,1-255;CenterPhone:�绰����洢������ָ��
* ����			:	FALSE:ͨ��ʧ��;TRUE:ͨ�ųɹ�
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2013-10-20
* ����޸�ʱ�� 	: 	2016-06-15
* ˵��			: 	���ڴӵ绰����ȡһ���绰���룬��������ſ�SIM2000Cģ��洢�������뵽��һ������
*************************************************************************************************************************/
bool SIMCOM_GetBookNumber(SIMCOM_HANDLE *pHandle, u8 index, char pPhoneNumber[16])
{
	u8 i,n;
	u32 cnt;
	char *p,*p1;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	char buff[32];
	
	if(index < 1) return FALSE;				//������1��ʼ
	do
	{	
		//+CPBR: 1,"152778787878",129,"Phone"
		sprintf(buff,"AT+CPBR=%d",index);
		SIMCOM_SendAT(pHandle, buff);													//����"AT+CPBR=1",��ȡ����
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 30, 600))		//�ȴ���Ӧ,��ʱ600MS
		{
			p = strstr((const char*)pData, "+CPBR:");					//�����ַ�"+CPBR:"		
			if(p != NULL) 													//�����ɹ�
			{
				p = strstr(p+1, ",\"");	//����,"	//��ʼ
				if(p != NULL)
				{
					p1 = strstr(p+2, "\",");	//����",//����
					if(p1 != NULL)			
					{
						p+=2;	//����,"
						if(p[0] == '+')	//�����+��ͷ���������Ӻ�
						{
							p+=3;	//����+86
						}
						else if(p[0]=='8' && p[1] == '6') //����86
						{
							p+=2;
						}
						
						n = p1 - p;			//����绰���볤��
						if(n > 15) n = 15;	//���ƺ��볤��Ϊ15�ֽ�
						for(i = 0;i < n;i ++)
						{
							pPhoneNumber[i] = p[i];	//���Ƶ绰����
						}
						pPhoneNumber[i] = '\0';		//��ӽ�����
						
						SIMCOM_GSM_debug("����:%s\r\n",pPhoneNumber);
						return TRUE;
					}
				}
			}
			else
			{
				pPhoneNumber[0] = '\0';
				SIMCOM_GSM_debug("����:Ϊ��\r\n");
				
				return TRUE;
			}
		}
		SIMCOM_Ready(pHandle);					//�ȴ�����
		pHandle->pDelayMS(1000);				//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return FALSE;
}
 
 
/*************************************************************************************************************************
* ����				:	int SIMCOM_GetSignal(SIMCOM_HANDLE *pHandle)
* ����				:	��ȡ�ź�ǿ��
* ����				:	pHandle:���
* ����				:	<0:��ȡʧ��;0-31:�ź�ǿ��;
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2013-10-21
* ����޸�ʱ�� 		: 	2013-10-21
* ˵��				: 	��
*************************************************************************************************************************/
int SIMCOM_GetSignal(SIMCOM_HANDLE *pHandle)
{
	u8 temp;
	u32 cnt;
	char *p;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	
	do
	{	
		//+CSQ: 27,0
		//+CSQ: 5,0
		//+CSQ: 16,99
		SIMCOM_SendAT(pHandle,"AT+CSQ");												//����"AT++CSQ",��ȡ����
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 20, 400))		//�ȴ���Ӧ,��ʱ400MS
		{
			p = strstr((const char*)pData, "+CSQ:");									//�����ַ�"+CSQ:"
			if(p != NULL) 																//�����ɹ�
			{
				if(p[7] != ',' && p[8] != ',') p[8] = '\0';
				temp = atoi(&p[6]);
				
				return temp;
			}
			break;
		}
		SIMCOM_Ready(pHandle);					//�ȴ�����
		pHandle->pDelayMS(1000);				//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return -1;
}
 
 
 
 
 
/*************************************************************************************************************************
* ����			:	bool SIMCOM_GetCIMI(SIMCOM_HANDLE *pHandle, char pCIMI[16])
* ����			:	��ȡSIM��CIMI����(SIM��Ψһid���������)
* ����			:	pHandle�������pCIMI:CIMI����������15�ֽ�
* ����			:	FALSE:ͨ��ʧ��;TRUE:ͨ�ųɹ�
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2017-11-16
* ����޸�ʱ�� 	: 	2018-03-24
* ˵��			: 	���ڻ�ȡ��ΨһCIMI��ţ���ֹĳЩ���޷���ȡ�������룬����뿨��һһ��Ӧ
*************************************************************************************************************************/
bool SIMCOM_GetCIMI(SIMCOM_HANDLE *pHandle, char pCIMI[16])
{
	u32 cnt;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	
	do
	{	
		//460041037206894
		SIMCOM_SendAT(pHandle, "AT+CIMI");												//����"AT+CIMI"
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 20, 200))		//�ȴ���Ӧ,��ʱ200MS
		{
			if(pData[0]!='\r' || pData[1]!='\n') continue;
			memcpy(pCIMI, &pData[2], 15);	//����ǰ��\r\n
			pCIMI[15] = 0;		//��ӽ�����
			
			SIMCOM_GSM_debug("��ȡCIMI�ɹ���%s\r\n", pCIMI);
			return TRUE;
		}
		
		SIMCOM_Ready(pHandle);															//�ȴ�����
		pHandle->pDelayMS(1000);														//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return FALSE;
}
 
 
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIM7000C_GetNB_APN(SIMCOM_HANDLE *pHandle, char pAPN[17])
* ����				:	��ȡSIM7000C NBIOT �����
* ����				:	pHandle�������pAPN������㻺����
* ����				:	TRUE:�ɹ���FALSE:ʧ��
* ����				:	�ײ�
* ����				:	cp1300@139.com
* ʱ��				:	2018-01-16
* ����޸�ʱ�� 		: 	2018-01-16
* ˵��				: 	������NBIOTģʽ����ʹ��
*************************************************************************************************************************/
bool SIM7000C_GetNB_APN(SIMCOM_HANDLE *pHandle, char pAPN[17])
{
	u32 cnt;
	char *p;
	u8 retry = SIMCOM_DEFAULT_RETRY;		//���Դ���
	u8 *pData;
	u8 i;
 
	do
	{	
		//+CGNAPN: 1,"ctnb"
		SIMCOM_SendAT(pHandle, "AT+CGNAPN");											//����ATָ��
		pHandle->pClearRxData();														//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 2000))		//�ȴ���Ӧ,��ʱ2000MS
		{
			p = strstr((const char*)pData, "+CGNAPN: 1,\"");							//�����ַ�+CGNAPN: 1,"
			if(p != NULL) 																//�����ɹ�
			{
				p += strlen("+CGNAPN: 1,\"");
				for(i = 0;i < 17;i ++)
				{
					if(p[i] == '\"')											//��������λ ��
					{
						pAPN[i] = 0;
						if(i == 0) break;										//̫����
						return TRUE;
					}
					pAPN[i] = p[i];
				}
			}
		}
		
		SIMCOM_Ready(pHandle);													//�ȴ�����
		pHandle->pDelayMS(1000);												//ʧ����ʱ1�������
		retry --;
	}while(retry);
	
	return FALSE;
}

