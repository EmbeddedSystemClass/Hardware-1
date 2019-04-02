/*************************************************************************************************************
 * �ļ���:			SIMCOM_AT.c
 * ����:			SIMCOM�ײ�ATָ��ӿ�
 * ����:			cp1300@139.com
 * ����ʱ��:		2015-02-15
 * ����޸�ʱ��:	2018-03-23
 * ��ϸ:			
*************************************************************************************************************/
#include "system.h"
#include "usart.h"
#include "SIMCOM_AT.h"
#include "SIMCOM.h"
#include "string.h"
#include "ucos_ii.h"
 
bool g_SIMC0M_AT_Debug = TRUE;	//�ײ�ATָ�����״̬
 
//���Կ���
#define SIMCOM_DBUG		1
#if SIMCOM_DBUG
	#include "system.h"
	#define SIMCOM_debug(format,...)	{if(g_SIMC0M_AT_Debug){uart_printf(format,##__VA_ARGS__);}}
#else
	#define SIMCOM_debug(format,...)	/\
/
#endif	//SIMCOM_DBUG
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_SendAT(SIMCOM_HANDLE *pHandle, char *pStr)
* ����				:	����һ��ATָ�����ӽ�����\r\n��,����ȴ���Ӧ
* ����				:	pHandle:SIMCOM�����pStr��ָ���ַ���
* ����				:	�ӿڷ���״̬
* ����				:	��
* ����				:	cp1300@139.com
* ʱ��				:	2018-03-23
* ����޸�ʱ�� 		: 	2018-03-23
* ˵��				: 	���ڵײ�ATָ���
*************************************************************************************************************************/
bool SIMCOM_SendAT(SIMCOM_HANDLE *pHandle, char *pStr)
{
	pHandle->pSendData((u8 *)pStr, strlen(pStr));	//����ָ��
	return pHandle->pSendData((u8 *)"\r\n", 2);		//���ͽ�����
}
 
 
 
 
 
/*************************************************************************************************************************
* ����			:	bool SIMCOM_TestAT(SIMCOM_HANDLE *pHandle, u32 retry)
* ����			:	SIMCOM AT ����ͨ�Ų���
* ����			:	pHandle:SIMCOM�����retry:���Դ���
* ����			:	FALSE:ͨ��ʧ��;TRUE:ͨ�ųɹ�
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2013-10-20
* ����޸�ʱ�� : 	2018-03-23
* ˵��			: 	ÿ��100ms��SIMCOMͨ��ģ�鷢��һ��"AT",�ȴ���Ӧ����
*************************************************************************************************************************/
bool SIMCOM_TestAT(SIMCOM_HANDLE *pHandle, u32 retry)
{
	u32 cnt;
	u8 *pRxBuff;
	
	//���ģ�����
	do
	{
		SIMCOM_SendAT(pHandle, "AT");															//����"AT",ͬ��������,���ҵȴ�Ӧ��
		pHandle->pClearRxData();																//���������
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pRxBuff, &cnt, "OK", 10, 150))			//�ȴ���Ӧ,��ʱ150ms
		{
			pHandle->pDelayMS(100);
			return TRUE;
		}
		retry --;
	}while(retry);
	
	pHandle->pDelayMS(100);
	return FALSE;
}
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_WaitSleep(SIMCOM_HANDLE *pHandle, u32 TimeOutMs)
* ����				:	�ȴ�ģ�����,�����»���
* ����				:	pHandle:�����TimeOut:�ȴ���ʱ,ʱ�䵥λms
* ����				:	TRUE:�ɹ�;FALSE:��ʱ
* ����				:	��
* ����				:	cp1300@139.com
* ʱ��				:	2013-10-25
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	���ڵȴ��������,��ֹ���ٲ������ģ�鲻��Ӧ
*************************************************************************************************************************/
bool SIMCOM_WaitSleep(SIMCOM_HANDLE *pHandle, u32 TimeOutMs)
{
	u32 i;
	u32 cnt;
	u8 *pData;
	
	if(TimeOutMs < 100) TimeOutMs = 100;		//����100ms
	pHandle->pSetDTR_Pin(SIMCOM_H_LEVEL);		//�ȴ�ģ����к����SLEEPģʽ
	
	//ѭ���������ֱ�����ʱ������Ϊ������sleepģʽ
	for(i = 0;i < (TimeOutMs/100);i ++)
	{
		pHandle->pDelayMS(100);			//��ʱ100ms
		SIMCOM_SendAT(pHandle, "AT");			//����"AT",ͬ��������,���ҵȴ�Ӧ��
		pHandle->pClearRxData();				//������ռ�����
		if(AT_RETURN_TIME_OUT == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, 100))	//�ȴ���Ӧ,��ʱ100ms
		{
			break;
		}
	}
	pHandle->pSetDTR_Pin(SIMCOM_L_LEVEL);		//����
	
	if(i == (TimeOutMs/100)) 
	{
		SIMCOM_debug("ģ��������ģʽʧ��!\r\n");
		pHandle->pClearRxData();				//������ռ�����
		
		return FALSE;
	}
	pHandle->pDelayMS(100);				//��ʱ100ms
	
	SIMCOM_debug("ģ��������ģʽ�ɹ�!\r\n");
	SIMCOM_TestAT(pHandle, 10);
	pHandle->pClearRxData();				//������ռ�����
	
	return TRUE;
}
 
 
 
 
 
 
 
/*************************************************************************************************************************
* ����				:	SIMCOM_AT_ERROR SIMCOM_GetATResp(SIMCOM_HANDLE *pHandle, u8 **pRxBuff, u32 *pLen, const char *pKeyword, u8 ByteTimeOutMs, u16 TimeOutMs)
* ����				:	��ȡSIMCOM��ATָ����Ӧ
* ����				:	pHandle:���
						pRxBuff:���ջ�����ָ��(���);pLen:���յ������ݴ�С(���),
						pKeyword:�ؼ���,Ϊ�ַ���,����"OK",����ڽ��յ����ַ�������OK�ַ�,�ͷ��سɹ�,����ʧ��(����)
						ByteTimeOutMs:�ֽڳ�ʱʱ��,��λms���255ms
						TimeOutMs:�ȴ���ʱʱ��,��λ����
* ����				:	SIM900_ERROR
* ����				:	��
* ����				:	cp1300@139.com
* ʱ��				:	2018-03-24
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	���������ڽ��ջ������ַ����������'\0'
						�������������������
*************************************************************************************************************************/
SIMCOM_AT_ERROR SIMCOM_GetATResp(SIMCOM_HANDLE *pHandle, u8 **pRxBuff, u32 *pLen, const char *pKeyword, u8 ByteTimeOutMs, u16 TimeOutMs)
{
	int len;
	u16 ReceiveDelay;
 
	if(ByteTimeOutMs < 1) ByteTimeOutMs = 1;
	len = pHandle->pReadData(pRxBuff, ByteTimeOutMs, TimeOutMs, &ReceiveDelay);	//���ûص��ӿڣ���ȡ����
	//�ȴ���ʱ
	if(len == 0)
	{
		return AT_RETURN_TIME_OUT;												//���س�ʱ����
	}
	//���ݽ������
	*pLen = len;																//���ؽ������ݳ���
	if((*pRxBuff)[len-1] != 0)
	{
		(*pRxBuff)[len]	= '\0';													//�����ݽ�β��ӽ����ַ���
	}
	
	SIMCOM_debug("\r\nSIMCOM(%dB)->%s\r\n",len, *pRxBuff);						//��ӡ������Ϣ
	if(strstr((const char*)(*pRxBuff), pKeyword) != NULL) 	//�����ؼ���
	{
		SIMCOM_debug("%s ���سɹ�!\r\n",pKeyword);
	    return AT_RETURN_OK;
	}
	else if(strstr((const char*)(*pRxBuff), "ERROR") != NULL)
	{
		SIMCOM_debug("%s ���ش���!\r\n",pKeyword);
		return AT_RETURN_ERROR;
	}
	else
	{
		SIMCOM_debug("%s ����δ֪!\r\n",pKeyword);
		return AT_RETURN_UNKNOWN;
	}
}
 
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIM900_SetParametersReturnBool(char *pATCom, u8 retry, u16 TimeOutx10MS, const char *pErrorDebug)
* ����				:	����SIM900һ������,����һ��bool״̬
* ����				:	pATCom:AT����;retry:���Դ���;TimeOut:���ʱʱ��,��λ10ms;pErrorDebug:ʧ�ܺ���ʾ�ĵ�����Ϣ
* ����				:	TRUE:ִ�гɹ���,������OK,FALSE:ִ��ʧ����,������ERROR������
* ����				:	SIM900
* ����				:	cp1300@139.com
* ʱ��				:	2014-12-19
* ����޸�ʱ�� 	: 	2014-12-19
* ˵��				: 	���ڼ���������,��ֹ�����ظ�
*************************************************************************************************************************/
bool SIMCOM_SetParametersReturnBool(SIMCOM_HANDLE *pHandle, char *pATCom, u8 retry, u16 TimeOutMs, const char *pErrorDebug)
{
	u32 cnt;
	u8 *pData;
			
	retry += 1;		//���Դ���
	do
	{	
		SIMCOM_SendAT(pHandle,pATCom);			//����AT����
		pHandle->pClearRxData();				//������ռ�����
		if(AT_RETURN_OK == SIMCOM_GetATResp(pHandle, &pData, &cnt, "OK", 10, TimeOutMs))	//�ȴ���Ӧ,��ʱ10s
		{
			pHandle->pClearRxData();			//������ռ�����
			return TRUE;
		}
		
		SIMCOM_Ready(pHandle);					//�ȴ�����
		retry --;
	}while(retry);
	
	if(pErrorDebug!=NULL) 
	{
		uart_printf("%s",pErrorDebug);			//���������Ϣ
	}
	pHandle->pClearRxData();					//������ռ�����
	
	return FALSE;
}
 
 
 
 
/*************************************************************************************************************************
*����        	:	u32 GSM_StringToHex(char *pStr, u8 NumDigits)
*����        	:	��16������ʽ�ַ���ת��Ϊ16����������(���뱣֤�ַ�����ĸ���Ǵ�д)
*����        	:	pStr:�ַ�����ʼָ��
* 					NumDigits:����λ��,16��������λ��
*����        	:	ת���������
*����        	:	��
*����        	:	cp1300@139.com
*ʱ��        	:	2013-04-30
*����޸�ʱ��	:	2013-10-17
*˵��        	:	�����ַ���"A865"ת����Ϊ0xA865,λ��Ϊ4λ
					���뱣֤�ַ�����ĸ���Ǵ�д
*************************************************************************************************************************/
u32 GSM_StringToHex(char *pStr, u8 NumDigits)
{
	u8 temp;
	u32 HEX = 0;
	u8 i;
	
	NumDigits = (NumDigits > 8) ? 8 : NumDigits;	//���֧��8λ16������
	
	for(i = 0;i < NumDigits;i ++)
	{
		HEX <<= 4;
		temp = pStr[i];
		temp = (temp > '9') ? temp - 'A' + 10 : temp - '0';
		HEX |= temp;
	}
	return HEX;
}
 
 
/*************************************************************************************************************************
*����        	:	void GSM_HexToString(u32 HexNum,c har *pStr, u8 NumDigits)
*����        	:	����������ת��Ϊ16������ʽ�ַ���(��ĸΪ��д,����������)
*����        	:	HexNum:16��������
					pStr:�ַ�������ָ��
* 					NumDigits:����λ��,16��������λ��
*����        	:	��
*����        	:	��
*����        	:	cp1300@139.com
*ʱ��        	:	2013-04-30
*����޸�ʱ��	:	2013-04-30
*˵��        	:	�����ַ���0xA865ת����Ϊ"A865",λ��Ϊ4λ
*************************************************************************************************************************/
void GSM_HexToString(u32 HexNum,char *pStr, u8 NumDigits)
{
	u8 temp;
	u8 i;
	
	NumDigits = (NumDigits > 8) ? 8 : NumDigits;	//���֧��8λ16������
	
	for(i = 0;i < NumDigits;i ++)
	{
		temp = 0x0f & (HexNum >> (4 * (NumDigits - 1 - i)));
		temp = (temp > 0x09) ? (temp - 0x0A + 'A') : (temp + '0');
		pStr[i] = temp;
	}
}
 
 
 
 
/*************************************************************************************************************************
*����        	:	u32 GSM_StringToDec(char *pStr, u8 NumDigits)
*����        	:	��10������ʽ�ַ���ת��Ϊ������(���뱣֤��ȫΪ�����ַ�)
*����        	:	pStr:�ַ�����ʼָ��
* 					NumDigits:����λ��,10��������λ��
*����        	:	ת���������
*����        	:	��
*����        	:	cp1300@139.com
*ʱ��        	:	2013-04-30
*����޸�ʱ��	:	2013-04-30
*˵��        	:	�����ַ���"1865"ת����Ϊ1865,λ��Ϊ4λ
					���뱣֤��ȫΪ�����ַ�
*************************************************************************************************************************/
u32 GSM_StringToDec(char *pStr, u8 NumDigits)
{
	u32 temp;
	u32 DEC = 0;
	u8 i;
	u8 j;
	
	NumDigits = (NumDigits > 10) ? 10 : NumDigits;	//���֧��10λ10������
	
	for(i = 0;i < NumDigits;i ++)
	{
		temp = pStr[i] - '0';
		if(temp > 9)			//ֻ�������ַ�Χ
			return 0;
		for(j = 1;j < (NumDigits - i);j ++)
		{
			temp *= 10;
		}
		DEC += temp;
	}
	return DEC;
}

