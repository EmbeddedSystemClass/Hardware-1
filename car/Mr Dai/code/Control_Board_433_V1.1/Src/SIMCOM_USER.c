/*************************************************************************************************************
 * �ļ���:			SIMCOM_USER.c
 * ����:			SIMCOM�û��㺯��
 * ����:			cp1300@139.com
 * ����ʱ��:		2015-02-15
 * ����޸�ʱ��:	2018-03-23
 * ��ϸ:			
*************************************************************************************************************/
#include "system.h"
#include "usart.h"
#include "string.h"
#include "ucos_ii.h"
#include "SIMCOM_USER.h"
#include "SIMCOM_GSM.h"
#include "SIMCOM_GPRS.h"
#include "SIMCOM_AT.h"
#include "SIMCOM.h"
 
bool g_SIMC0M_USER_Debug = TRUE;	//Ӧ�ò�ָ�����״̬
 
//���Կ���
#define SIMCOM_USER_DBUG		1
#if SIMCOM_USER_DBUG
	#include "system.h"
	#define SIMCOM_USER_debug(format,...)	{if(g_SIMC0M_USER_Debug){uart_printf(format,##__VA_ARGS__);}}
#else
	#define SIMCOM_USER_debug(format,...)	/\
/
#endif	//SIMCOM_USER_DBUG
	
const char  *const SIMCOM_NETWORK_NAME[18] = {"δע��","GSM","GPRS","EGPRS (EDGE)","WCDMA","HSDPA only(WCDMA)","HSUPA only(WCDMA)","HSPA (HSDPA and HSUPA, WCDMA)",
"LTE","TDS-CDMA","TDS-HSDPA only","TDS-HSUPA only","TDS- HSPA (HSDPA and HSUPA)","CDMA","EVDO","HYBRID (CDMA and EVDO)","1XLTE(CDMA and LTE)","δ֪,����"};
 
 
 
/*************************************************************************************************************************
* ����				:	bool SIMCOM_Init(SIMCOM_HANDLE *pHandle,
							bool (* pSendData)(u8 *pDataBuff, u16 DataLen),											
							int (* pReadData)(u8 **pDataBuff, u8 ByteTimeOutMs, u16 TimeOutMs, u16 *pReceiveDelay),	
							void (*pClearRxData)(void),																
							void (*pSetDTR_Pin)(u8 Level),															
							void (*pSetPWRKEY_Pin)(u8 Level),														
							u8 (*pGetSTATUS_Pin)(void),	
							u8 (*pGetDCD_Pin)(void),
							void (*pDelayMS)(u32 ms),																
							void (*pIWDG_Feed)(void)
* ����				:	��ʼ��SIMCOM����ӿ�
* ����				:	pSl651_Handle:�����
						pSendCallBack�����ͻص�������pDataBuff���������ݻ�������DataLen���������ݳ��ȣ�
						pReadCallBack���������ݻص���������ȴ�ֱ�����ݱ�д�뵽���ջ�������pDataBuff:�������ݻ�������ByteTimeOut���ȴ����ֽڳ�ʱʱ�䣬��λms,TimeOut:���ݰ���ʱʱ�䣬��λms��
						pClearRxData:������ջ�������������������������ݻ���������
						pSetDTR_Pin:DTR���ŵ�ƽ����-���ڿ���sleepģʽ�����˳�͸��ģʽ
						pSetPWRKEY_Pin:PWRKEY�������ŵ�ƽ����-���ڿ���
						pGetSTATUS_Pin:��ȡSTATUS���ŵ�ƽ-����ָʾģ���ϵ�״̬
						pGetDCD_Pin:��ȡDCD���ŵ�ƽ-�ߵ�ƽATָ��ģʽ���͵�ƽΪ͸��ģʽ
						pDelayMS:ϵͳ��ʱ����
						pIWDG_Feed:���ϵͳ���Ź�(����Ϊ��)
* ����				:	��
* ����				:	TRUE:�ɹ���FALSE:ʧ��
* ����				:	cp1300@139.com
* ʱ��				:	2018-03-24
* ����޸�ʱ�� 		: 	2018-03-24
* ˵��				: 	��pIWDG_Feed�ӿڿ���Ϊ�գ�����ӿھ�����Ϊ�գ������������
*************************************************************************************************************************/
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
	)
{
	if(pHandle == NULL) 
	{
		DEBUG("��Ч�ľ����\r\n");
		return FALSE;
	}
	//�������
	pHandle->SimcomModeType = SIMCOM_INVALID;							//ģ���ͺ�
	pHandle->TelecomCarr[0] = 0;										//��Ӫ������
	memset(&pHandle->SIMCOM_Info, 0, sizeof(SIMCOM_INFO));				//SIMCOMͨ��ģ�������Ϣ�ṹ��
	memset(&pHandle->NetworkConfig, 0, sizeof(NETWORK_CONFIG_TYPE));	//����ģʽ����
	pHandle->NetworkMode = SIMCOM_NETMODE_NULL;							//��ǰ������ʽ
	
	
	//�ײ�ͨ�Žӿ�
	pHandle->pSendData = pSendData;										//�������ݽӿڣ��������ʧ�ܣ�����FALSE,�ɹ�����TRUE;
	pHandle->pReadData = pReadData;										//�������ݽӿڣ��������ݳ��ȣ����ʧ�ܷ���<=0,�ɹ����������ݳ���
	pHandle->pClearRxData = pClearRxData;								//������ջ�������������������������ݻ���������
	pHandle->pSetDTR_Pin = pSetDTR_Pin;									//DTR���ŵ�ƽ����-���ڿ���sleepģʽ�����˳�͸��ģʽ
	pHandle->pSetPWRKEY_Pin = pSetPWRKEY_Pin;							//PWRKEY�������ŵ�ƽ����-���ڿ���
	pHandle->pGetSTATUS_Pin = pGetSTATUS_Pin;							//��ȡSTATUS���ŵ�ƽ-����ָʾģ���ϵ�״̬
	pHandle->pGetDCD_Pin = pGetDCD_Pin;									//��ȡDCD���ŵ�ƽ-�ߵ�ƽATָ��ģʽ���͵�ƽΪ͸��ģʽ
	//ϵͳ�ӿ�
	pHandle->pDelayMS = pDelayMS;										//ϵͳ��ʱ����
	pHandle->pIWDG_Feed = pIWDG_Feed;									//���ϵͳ���Ź�(����Ϊ��)
	//�ڲ�״̬����
	pHandle->s_isInitStatus = FALSE;									//���ڼ�¼ģ���ʼ��״̬����λ���ϵ���Ϊ��Ч
	//����Ƿ��нӿ�Ϊ��
	if(pHandle->pSendData==NULL || pHandle->pReadData==NULL || pHandle->pClearRxData==NULL || pHandle->pSetDTR_Pin==NULL || pHandle->pSetPWRKEY_Pin==NULL || 
		pHandle->pGetSTATUS_Pin==NULL || pHandle->pGetDCD_Pin==NULL || pHandle->pDelayMS==NULL)
	{
		DEBUG("�����лص��ӿ�Ϊ�գ�\r\n");
		return FALSE;
	}
	
	return TRUE;
}
 
 
 
/*************************************************************************************************************************
* ����			:	void SIMCOM_PrintfModel(SIMCOM_HANDLE *pHandle, SIMCOM_MODE_TYPE ModeType, const char **pModeInof)
* ����			:	��ʾ����ӡģ���ͺ�
* ����			:	pHandle�������ModeType��ģ���ͺţ�pModeInof:����ģ���ͺ���Ϣ(����Ҫ����Ϊ��)
* ����			:	��
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2017-10-17
* ����޸�ʱ��	: 	2018-03-24
* ˵��			: 	
*************************************************************************************************************************/ 
void SIMCOM_PrintfModel(SIMCOM_HANDLE *pHandle, SIMCOM_MODE_TYPE ModeType, const char **pModeInof)
{
	switch(ModeType)
	{
		case SIMCOM_SIM900 :	//Ĭ��ΪSIM900
		{
			if(pModeInof!=NULL) *pModeInof = "SIM900ϵ��";//ͨ��ģ���ͺ�	
			uart_printf("[DTU]ͨ��ģ��ΪSIM900\r\n");	
		}break;
		case SIMCOM_SIM800 :	//SIM800
		{
			if(pModeInof!=NULL) *pModeInof = "SIM800ϵ��";			//ͨ��ģ���ͺ�
			uart_printf("[DTU]ͨ��ģ��ΪSIM800\r\n");
		}break;
		case SIMCOM_SIM2000	:	//SIM2000
		{
			
			if(pModeInof!=NULL) *pModeInof = "SIM2000ϵ��";			//ͨ��ģ���ͺ�
			uart_printf("[DTU]ͨ��ģ��ΪSIM2000\r\n");
		}break;
		case SIMCOM_SIM7600	:	//SIM7600
		{
			if(pModeInof!=NULL) *pModeInof = "SIM7600ϵ��";			//ͨ��ģ���ͺ�
			uart_printf("[DTU]ͨ��ģ��ΪSIM7600\r\n");
		}break;
		case SIMCOM_SIM868:		//SIM868
		{
			if(pModeInof!=NULL) *pModeInof = "SIM868ϵ��";			//ͨ��ģ���ͺ�
			uart_printf("[DTU]ͨ��ģ��ΪSIM868ģ��\r\n");
		}break;
		case SIMCOM_SIM7000C:	//SIM7000C
		{
			if(pModeInof!=NULL) *pModeInof = "SIM7000Cϵ��";		//ͨ��ģ���ͺ�
			uart_printf("[DTU]ͨ��ģ��ΪSIM7000C\r\n");
		}break;
		case LYNQ_L700:		//LYNQ_L700
		{
			if(pModeInof!=NULL) *pModeInof = "L700ϵ��";			//ͨ��ģ���ͺ�
			uart_printf("[DTU]ͨ��ģ��ΪL700ģ��\r\n");
		}break;
		case SIMCOM_INVALID	:	//��Ч��Ĭ��
		{
			if(pModeInof!=NULL) *pModeInof = "δ֪";				//ͨ��ģ���ͺ�
			uart_printf("[DTU]ͨ��ģ��δ֪!\r\n");
		}break;
	}
}
 
 
 
 
 
/*************************************************************************************************************************
* ����			:	SIMCOM_USER_ERROR SIMCOM_RegisNetwork(SIMCOM_HANDLE *pHandle, u16 Retry, u16 NetworkDelay,const char **pModeInof)
* ����			:	SIMCOMģ���ϵ��ʼ����ע������
* ����			:	pHandle�������Retry:��ʼ�����Դ���>0;NetworkDelay:ע��������ʱʱ��,��λS;pModeInof:����ģ���ͺ���Ϣ(����Ҫ����Ϊ��)
* ����			:	SIMCOM_USER_ERROR	
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2014-08-18
* ����޸�ʱ��	: 	2018-03-24
* ˵��			: 	����ͨ��ģ���ϵ粢��ʼ������		
*************************************************************************************************************************/ 
SIMCOM_USER_ERROR SIMCOM_RegisNetwork(SIMCOM_HANDLE *pHandle, u16 Retry, u16 NetworkDelay,const char **pModeInof)
{
	SIMCOM_NETSTATUS NetStatus;
	SIMCOM_MODE_TYPE ModeType = SIMCOM_INVALID;					//��ʼ��ģ���ͺ���Ч
	SIMCOM_NETMODE_TYPE NetworkMode = SIMCOM_NETMODE_NULL;		//��ʼ��Ϊδ֪ģʽ
	SIMCOM_USER_ERROR Error = SIMCOM_NULL_ERROR;				//��ʼ��״̬
	
	u16 pcnt,cnt;
	u8 NotCnt,SeaCnt,TurCnt,UnkCnt,ErrCnt;
	bool isCart = FALSE;
	u8 SIM_NotReadyCnt = 0;										//SIM��δ׼������������
	
	Retry += 1;													//���Դ�������1��
	//ģ���ϵ�
	for(pcnt = 0;pcnt < Retry;pcnt ++)							//�ϵ�ѭ��
	{
		SIM_NotReadyCnt = 0;									//SIM��δ׼��������������λ
		
		//�ϵ���Ϻ��ʼ��ģ��
		if(pHandle->pGetSTATUS_Pin() == SIMCOM_H_LEVEL)		
		{
			//ģ���ʼ������
			if(NetworkDelay == 0) NetworkDelay = 0xffff;				//Ϊ0,һֱ�ȴ�	
			NotCnt=SeaCnt=TurCnt=UnkCnt=ErrCnt=0;						//��ʼ��ע��״̬������Ϊ0
 
			//��ȡģ���ͺ�
			if(ModeType == SIMCOM_INVALID)
			{
				SIMCOM_SetParametersReturnBool(pHandle, "ATE 0", SIMCOM_DEFAULT_RETRY, 110, "\r\n�ر�AT����ģʽʧ��!\r\n");
				pHandle->pDelayMS(500);
				ModeType = SIMCOM_GetMode(pHandle);						//��ȡģ���ͺ�
				SIMCOM_PrintfModel(pHandle, ModeType, pModeInof);		//��ӡ��ʾͨ��ģ���ͺ�		
			}
			//�ͺŻ�ȡ�ɹ��ˣ�������Ϣ
			if(ModeType!=SIMCOM_INVALID)
			{
				pHandle->SimcomModeType = ModeType;						//�ϵ��ʼ������ͨ��ģ���ͺ�	
			}
			
			//����ģ�����ͽ��г�ʼ���������SIM7000C����Ҫѡ������GSMģʽ����NBIOTģʽ
			if(SIMCOM_NetworkConfig(pHandle, ModeType, &pHandle->NetworkConfig)==FALSE)
			{
				uart_printf("[DTU]��ʼ��ͨ��ģ������ģʽʧ����!\r\n");	
			}
			
			//��ʼ����ȡ������Ϣ
			for(cnt = 0;cnt < NetworkDelay;cnt ++)
			{
				if(pHandle->pGetSTATUS_Pin()==SIMCOM_L_LEVEL) 
				{
					Error = SIMCOM_POWER_UP_ERROR;			//�쳣�ϵ�
					uart_printf("[DTU]�쳣�ϵ���,���鹩���Ƿ��ȶ�!\r\n");	
					break;									//ģ��û���ϵ�,��ʼ��
				}
				
				if(ModeType == SIMCOM_INVALID)
				{
					SIMCOM_SetParametersReturnBool(pHandle, "ATE 0", SIMCOM_DEFAULT_RETRY, 110, "\r\n�ر�AT����ģʽʧ��!\r\n");
					pHandle->pDelayMS(500);
					ModeType = SIMCOM_GetMode(pHandle);								//��ȡģ���ͺ�
					pHandle->SimcomModeType = ModeType;								//�ϵ��ʼ������ͨ��ģ���ͺ�	
					SIMCOM_PrintfModel(pHandle, ModeType, pModeInof);				//��ӡ��ʾͨ��ģ���ͺ�
				}
				
				if(isCart == FALSE)													//��δ��⵽����һֱ���¼��
				{
					if(SIMCOM_GetCPIN(pHandle)==SIM_READY)
					{
						isCart = TRUE;	//�����ɹ�
						uart_printf("\r\nSIM��׼������!\r\n");
					}
					else
					{
						uart_printf("\r\nSIM��δ׼������!\r\n");
						Error = SIMCOM_SIM_NOT_REALYE;								//��δ����
					}
				}
				
				
				//2018-01-18 ����SIM7000C NBģʽ����ע��״̬��ȡ֧��
				if((ModeType == SIMCOM_SIM7000C) && (pHandle->NetworkConfig.NB_EnableMode==1)) //�����SIM7000C�����ҿ�����NBģʽ
				{
					NetStatus = SIMCOM_GetDataNetworkStatus(pHandle);				//��ȡ������������ע��״̬
				}
				else
				{
					NetStatus = SIM900_GetGSMNetworkStatus(pHandle);				//��ȡGSM����ע��״̬
				}
 
				SIMCOM_USER_debug("[DTU]����ע��״̬%d!\r\n",NetStatus);
				//һֱ��������δע���Լ�ע��ܾ�,����,��Щ�����ȳ��־ܾ�,Ȼ�����ע��ɹ�
				switch(NetStatus)
				{
					case SIMCOM_NET_NOT	:	
					{
						NotCnt++;
						//2017-09-09 �����������15��δ��⵽��������δע�ᣬ��ֱ���˳�������δ�忨
						if(SIMCOM_GetCPIN(pHandle)==SIM_NOT_READY)
						{
							Error = SIMCOM_SIM_NOT_REALYE;	//��δ����
							SIM_NotReadyCnt ++;				//SIM��δ׼����������������
						}
						else
						{
							SIM_NotReadyCnt = 0;			//SIM��δ������������λ
						}
					}break;	//δע��
					case SIMCOM_NET_ROA	:					//�Ѿ�ע��,��������
					case SIMCOM_NET_YES	:					//�Ѿ�ע��
					{
						SIM_NotReadyCnt = 0;				//SIM��δ������������λ
						if(pHandle->s_isInitStatus == FALSE)//ģ��û�г�ʼ��
						{
							if(SIMCOM_ModuleInit(pHandle) == FALSE)						//�ϵ���ʼ��ģ���������
							{
								SIMCOM_USER_debug("[DTU]��ʼ��ʧ��!\r\n");	
								if(isCart==TRUE) Error = SIMCOM_INIT_ERROR;				//����ʼ���ɹ��ˣ��ŷ��س�ʼ������
								goto reset;
							}
						}
						
						//��ȡģ����Ϣ
						if( SIMCOM_GetModuleInfo(pHandle, &pHandle->SIMCOM_Info) == TRUE)		//��ȡģ��������Ϣ
						{
							SIMCOM_USER_debug("\r\n������:%s\r\n",pHandle->SIMCOM_Info.Manu);	
							SIMCOM_USER_debug("ģ���ͺ�:%s\r\n",pHandle->SIMCOM_Info.Model);
							SIMCOM_USER_debug("����汾:%s\r\n",pHandle->SIMCOM_Info.Ver);
							SIMCOM_USER_debug("ģ�����к�:%s\r\n",pHandle->SIMCOM_Info.IMEI);
						}
						else
						{
							SIMCOM_USER_debug("\r\n��ȡģ����Ϣʧ��!\r\n");
						}
						//��ȡ��Ӫ����Ϣ
						if(SIMCOM_COPS(pHandle, pHandle->TelecomCarr) == TRUE)		//��ȡ��Ӫ������
						{
							SIMCOM_USER_debug("��Ӫ����Ϣ:%s\r\n", pHandle->TelecomCarr);	
						}
						else
						{
							SIMCOM_USER_debug("��ȡ��Ӫ����Ϣʧ��!\r\n");
						}
						
						//�����SIM7600 SIM7000Cģ��,����Ҫ��ȡ������ʽ
						switch(ModeType)
						{
							case SIMCOM_SIM7600:
							case SIMCOM_SIM7000C:
							{
								NetworkMode = SIM7XXX_GetNetworkMode(pHandle);			//��ȡSIM7XXXϵ��ģ��������ʽ
								pHandle->NetworkMode = NetworkMode;						//��¼ȫ��������ʽ�������ڻ�ȡ�������ĺ���֮ǰ���л�ȡ����Ϊ���ſ��޶������ĺ������ڳ�ʼ��
								if(NetworkMode > 16)
								{
									SIMCOM_USER_debug("������ʽ:��ȡʧ��\r\n");
								}
								else
								{
									SIMCOM_USER_debug("������ʽ:%s\r\n",SIMCOM_NETWORK_NAME[NetworkMode]);
								}
							}break;
							default: //2G
							{
								pHandle->NetworkMode = SIMCOM_NETMODE_GSM;				//����2Gģ��Ĭ��GSM����
							}break;
						}
						
						Error = SIMCOM_INIT_OK;							//��ʼ���ɹ�
						return Error;									//�Ѿ�ע�Ტ��ʼ����
					}
					case SIMCOM_NET_SEA	:	SeaCnt++;break;	//δע��,��������
					case SIMCOM_NET_TUR	:	TurCnt++;break;	//ע�ᱻ�ܾ�
					case SIMCOM_NET_UNK	:	UnkCnt++;break;	//δ֪
					default				:	ErrCnt++;break;	//����
				}
				
				if((TurCnt > 60) || (UnkCnt > 20) || (ErrCnt > 20)) 		//ע�ᱻ�ܾ�,���ߴ���
				{
					if(isCart==TRUE) Error = SIMCOM_REG_ERROR;				//����ʼ���ɹ��ˣ��ŷ��س�ע��ʧ��
					SIMCOM_USER_debug("[DTU]ģ������!\r\n");
					if(SIMCOM_HardwarePowerDOWN(pHandle, TRUE) == FALSE)
					{
						SIMCOM_USER_debug("[DTU]:�ػ�ʧ��!\r\n");
					}
					break;
				}
				
				//SIM��δ������������
				if(SIM_NotReadyCnt > 16)
				{
					
					uart_printf("[DTU]:��μ�⵽��δ����!ģ������!\r\n");
					if(SIMCOM_HardwarePowerDOWN(pHandle, TRUE) == FALSE)
					{
						SIMCOM_USER_debug("[DTU]:�ػ�ʧ��!\r\n");
					}
					break;
				}
							
				//��ʱ
				pHandle->pDelayMS(800);
				if(pHandle->pIWDG_Feed!=NULL) pHandle->pIWDG_Feed();	//ι��
				
			}	
			//����ע��ʧ�ܻ�ģ���ʼ��ʧ��
			
reset:			
			if(SIMCOM_HardwarePowerDOWN(pHandle, TRUE) == TRUE)				//�رյ�Դ
			{
				uart_printf("[DTU]:�����ʼ��ʧ��,ģ��ػ��ɹ�!\r\n");
			}
			else
			{
				uart_printf("[DTU]:�����ʼ��ʧ��,ģ��ػ�ʧ��!\r\n");
			}
		}
		else
		{
			Error = SIMCOM_POWER_UP_ERROR;							//����ʧ��
		}
	}
	
	//��ʾ������Ϣ
	switch(Error)
	{
		case SIMCOM_INIT_OK				:	//��ʼ���ɹ�	
		{
			uart_printf("[DTU]:ģ���ϵ���ʼ�ɹ�!\r\n");
		}break;
		case SIMCOM_POWER_UP_ERROR		:	//�ϵ����
		{
			uart_printf("[DTU]:ģ���ϵ����!\r\n");
		}break;
		case SIMCOM_REG_ERROR			:	//ע�������ʱ��
		{
			uart_printf("[DTU]:ģ��ע�����������ʱ��!\r\n");
		}break;
		case SIMCOM_INIT_ERROR			:	//��ʼ�����ô���
		{
			uart_printf("[DTU]:ģ���ʼ�����ó���!\r\n");
		}break;
		case SIMCOM_SIM_NOT_REALYE		:	//SIM��δ���������ϵ�ʧ��
		{
			uart_printf("[DTU]:ģ��SIM��δ����!\r\n");
		}break;
		default: 
		{
			uart_printf("[DTU]:δ֪�Ĵ���!\r\n");
		}break;
	}
	
	
	return Error;	
}
 
 
 
 
 
/*************************************************************************************************************************
* ����			:	bool SIMCOM_PhoneMessageNumberInitialize(SIMCOM_HANDLE *pHandle, u8 retry)
* ����			:	SIMCOM ��ʼ����ȡ�������ĺ����Լ���������,�ź�ǿ��,CIMI���������ھ��pHandle�У�
* ����			:	pHandle:�����Retry:��ʼ�����Դ���>0
* ����			:	TRUE�ɹ�,FALSE:ʧ��	
* ����			:	�ײ�
* ����			:	cp1300@139.com
* ʱ��			:	2014-08-18
* ����޸�ʱ��	: 	2018-03-24
* ˵��			: 	���������ʼ��֮����г�ʼ����ȡ�����Ϣ,��Ҫ��ǰ��ʼ������ģ���ͺ��Լ�������ʽ
					�������ĺ�������:pHandle->ServiceCenterPhoneNumber;	
					�绰��������:pHandle->LocalPhoneNumber;	
					�ź�ǿ�ȴ����:pHandle->Singal��
					SIM��CIMI��������pHandle->SIM_CIMI��
					2016-06-15:SIM2000Cʹ�õ���SIM��ʱ,�����ȡ���������ʧ��,������ʱ,ȷ����ȡ�ɹ�
					2016-11-16:���ӻ�ȡ�������ĺ�����ʱ
					2017-12-05:NBIO ģ�鲻��ȡ�绰����
*************************************************************************************************************************/ 
bool SIMCOM_PhoneMessageNumberInitialize(SIMCOM_HANDLE *pHandle, u8 retry)
{
	u8 i;				//���Լ�����
	int Singal;
	bool status;
	bool isReadServerNumber = TRUE;	//�Ƿ���Ҫ��ȡ�������ĺ��룬���ſ������ȡ
 
	//����ATָ��
	SIMCOM_Ready(pHandle);
	if(SIMCOM_TestAT(pHandle, 20) != TRUE)
	{
		
		return FALSE;
	}
	
	
	retry += 1;
	//��ȡ�������ĺ���
	if(pHandle->SimcomModeType == SIMCOM_SIM2000)			//����ģ�������ȡ�������ĺ���
	{
		isReadServerNumber = FALSE;
	}
	else if(pHandle->SimcomModeType == LYNQ_L700)			//L700 NBIOT �����ȡ�������ĺ���
	{
		isReadServerNumber = FALSE;
	}
	else if(pHandle->SimcomModeType==SIMCOM_SIM7000C)		//SIM7000C
	{
		isReadServerNumber = FALSE; //sim7000��֧��
	}
	else if(pHandle->SimcomModeType==SIMCOM_SIM7600)		//SIM7600
	{
		switch(pHandle->NetworkMode)						//�鿴������ʽ�������CDMA(���ſ�)����Ҫ��ȡ�������ĺ���
		{
			case SIMCOM_NETMODE_CDMA	://CDMA
			case SIMCOM_NETMODE_EVDO	://EVDO
			case SIMCOM_NETMODE_HYBRID	://HYBRID (CDMA and EVDO)
			case SIMCOM_NETMODE_1XLTE	://1XLTE(CDMA and LTE)
			{
				isReadServerNumber = FALSE;
			}break;
		}
	}
 
	if(isReadServerNumber)				//��Ҫ��ȡ�������ĺ���
	{
		for(i = 0;i < retry;i ++)
		{
			if(SIMCOM_GetServeNumber(pHandle, pHandle->ServiceCenterPhoneNumber) == TRUE)				//��ȡ���ŷ������ĺ���ɹ�
			{
				if(pHandle->ServiceCenterPhoneNumber[0] == 0)
				{
					SIMCOM_USER_debug("�������ĺ���Ϊ��\r\n");
				}
				else
				{
					SIMCOM_USER_debug("�������ĺ���:%s\r\n",pHandle->ServiceCenterPhoneNumber);
				}
				break;
			}
			else
			{
				SIMCOM_WaitSleep(pHandle, 100);
				SIMCOM_Ready(pHandle);
				pHandle->pDelayMS(1000);
			}
		}
		if(i==retry) return FALSE;
	}
	else
	{
		pHandle->pDelayMS(2000);	//�ȴ�2s��
		strcpy(pHandle->ServiceCenterPhoneNumber, "13800818500");	//�������ĺ���-�̶�ֵ����ֹΪ��
		SIMCOM_USER_debug("[ȱʡ]�������ĺ���:%s\r\n",pHandle->ServiceCenterPhoneNumber);
	}
	
	//��ȡ�ź�ǿ��
	for(i = 0;i < retry;i ++)
	{
		Singal = SIMCOM_GetSignal(pHandle);				//��ȡ�ź�ǿ��
		if((Singal > 0) && (Singal!=99)) 
		{
			pHandle->Singal = (u8)Singal;
			SIMCOM_USER_debug("�ź�ǿ�� <%02d>!\r\n" ,Singal);
			break;
		}
		else
		{
			pHandle->Singal = 0;			//û�ж�ȡ������ʱ2��
			pHandle->pDelayMS(2000);		//�ȴ�2s��
		}
		SIMCOM_WaitSleep(pHandle, 100);
		SIMCOM_Ready(pHandle);
		pHandle->pDelayMS(1000);			//�ȴ�1s��
	}
	
	//��ȡ��������
	for(i = 0;i < retry;i ++)
	{
		if(pHandle->NetworkMode == LYNQ_L700)			//L700 NBIOT �޷���ȡ
		{
			pHandle->pDelayMS(1000);					//�ȴ�1s��
			pHandle->LocalPhoneNumber[0] = 0;
			
			return TRUE;
		}
		else if(pHandle->NetworkMode == SIMCOM_SIM7000C)	//SIM7000C �޷���ȡ
		{
			pHandle->pDelayMS(1000);			//�ȴ�1s��
			pHandle->LocalPhoneNumber[0] = 0;
			
			return TRUE;
		}
		else if(pHandle->SimcomModeType==SIMCOM_SIM7600)		//SIM7600��SIM7000C
		{
			switch(pHandle->NetworkMode)						//�鿴������ʽ�������CDMA(���ſ�)����Ҫ��ȡ�������ĺ���
			{
				case SIMCOM_NETMODE_CDMA	://CDMA
				case SIMCOM_NETMODE_EVDO	://EVDO
				case SIMCOM_NETMODE_HYBRID	://HYBRID (CDMA and EVDO)
				case SIMCOM_NETMODE_1XLTE	://1XLTE(CDMA and LTE)
				{
					status = SIMCOM_GetBookNumber(pHandle, 1, pHandle->LocalPhoneNumber);	//�ӵ绰���ж�ȡһ���绰����
				}break;
				default: //������ʽ���Զ�ȡ
				{
					status = SIMCOM_GetPhoneNumber(pHandle, pHandle->LocalPhoneNumber);	//��ȡ��������
				}break; 
			}
		}
		else if(pHandle->SimcomModeType == SIMCOM_SIM2000) //SIM2000
		{
			status = SIMCOM_GetBookNumber(pHandle, 1, pHandle->LocalPhoneNumber);	//�ӵ绰���ж�ȡһ���绰����
		}	
		
		else	//����ģ��
		{
			status = SIMCOM_GetPhoneNumber(pHandle, pHandle->LocalPhoneNumber);	//��ȡ��������
		}
		
		
		if(status== TRUE)			//��ȡ��������ɹ�
		{
			pHandle->LocalPhoneNumber[15] = 0;
			if(pHandle->LocalPhoneNumber[0] == 0)
			{
				SIMCOM_USER_debug("��������Ϊ��\r\n");
			}
			else
			{
				SIMCOM_USER_debug("��������:%s\r\n",pHandle->LocalPhoneNumber);
			}
			break;
		}
		else
		{
			SIMCOM_WaitSleep(pHandle, 100);
			SIMCOM_Ready(pHandle);
			pHandle->pDelayMS(2000);		//�ȴ�2s��
		}
	}
	if(i==retry) return FALSE;
	
	
	//��ȡSIM��CIMI����
	for(i = 0;i < retry;i ++)
	{
		//��ȡSIM��CIMI����(SIM��Ψһid���������)
		if(SIMCOM_GetCIMI(pHandle, pHandle->SIM_CIMI) == TRUE) break;
		
		SIMCOM_WaitSleep(pHandle, 100);
		SIMCOM_Ready(pHandle);
		pHandle->pDelayMS(1000);			//�ȴ�1s��
	}
	if(i==retry) return FALSE;
 
	
	return TRUE;
}

