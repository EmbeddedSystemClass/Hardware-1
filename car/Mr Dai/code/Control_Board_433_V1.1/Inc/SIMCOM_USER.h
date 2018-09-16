/*************************************************************************************************************
 * �ļ���:			SIMCOM_USER.h
 * ����:			SIMCOM�û��㺯��
 * ����:			cp1300@139.com
 * ����ʱ��:		2015-02-15
 * ����޸�ʱ��:	2018-03-23
 * ��ϸ:			
*************************************************************************************************************/
#ifndef _SIMCOM_SUER_H_
#define _SIMCOM_SUER_H_
#include "system.h"
#include "simcom.h"
 
//SIMCOM ��ʼ������
typedef enum
{
	SIMCOM_INIT_OK				=	0,	//��ʼ���ɹ�
	
	SIMCOM_POWER_UP_ERROR		=	1,	//�ϵ����
	SIMCOM_REG_ERROR			=	2,	//ע�������ʱ��
	SIMCOM_INIT_ERROR			=	3,	//��ʼ�����ô���
	SIMCOM_SIM_NOT_REALYE		=	4,	//SIM��δ���������ϵ�ʧ��
	SIMCOM_NULL_ERROR			=	255	//״̬��Ч
}SIMCOM_USER_ERROR;
 
 
//API
//��ʼ��SIMCOM����ӿ�
bool SIMCOM_Init(SIMCOM_HANDLE *pHandle,
		bool (* pSendData)(u8 *pDataBuff, u16 DataLen),											//�������ݽӿڣ��������ʧ�ܣ�����FALSE,�ɹ�����TRUE;
		int (* pReadData)(u8 **pDataBuff, u8 ByteTimeOutMs, u16 TimeOutMs, u16 *pReceiveDelay),	//�������ݽӿڣ��������ݳ��ȣ����ʧ�ܷ���<=0,�ɹ����������ݳ���
		void (*pClearRxData)(void),																//������ջ�������������������������ݻ���������
		void (*pSetDTR_Pin)(u8 Level),															//DTR���ŵ�ƽ����-���ڿ���sleepģʽ�����˳�͸��ģʽ
		void (*pSetPWRKEY_Pin)(u8 Level),														//PWRKEY�������ŵ�ƽ����-���ڿ���
		u8 (*pGetSTATUS_Pin)(void),																//��ȡSTATUS���ŵ�ƽ-����ָʾģ���ϵ�״̬
		u8 (*pGetDCD_Pin)(void),																//��ȡDCD���ŵ�ƽ-�ߵ�ƽATָ��ģʽ���͵�ƽΪ͸��ģʽ
		void (*pDelayMS)(u32 ms),																//ϵͳ��ʱ����
		void (*pIWDG_Feed)(void)																//���ϵͳ���Ź�(����Ϊ��)
	);
		
SIMCOM_USER_ERROR SIMCOM_RegisNetwork(SIMCOM_HANDLE *pHandle, u16 Retry, u16 NetworkDelay,const char **pModeInof);//SIMCOMģ���ϵ��ʼ����ע������
bool SIMCOM_PhoneMessageNumberInitialize(SIMCOM_HANDLE *pHandle, u8 retry);						//SIMCOM ��ʼ����ȡ�������ĺ����Լ���������,�ź�ǿ��,CIMI���������ھ��pHandle�У�
 
 
 
#endif /*_SIMCOM_SUER_H_*/

