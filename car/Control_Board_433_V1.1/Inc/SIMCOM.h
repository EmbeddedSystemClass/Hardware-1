/*************************************************************************************************************
 * �ļ���:			SIMCOM.h
 * ����:			SIMCOM �ײ㶨��
 * ����:			cp1300@139.com
 * ����ʱ��:		2015-02-15
 * ����޸�ʱ��:	2018-03-23
 * ��ϸ:			ע�⣺�ײ�ͨ�Žӿ�ʹ�õ��ǻص����������Ǳ����ṩϵͳ��ʱ���� void SYS_DelayMS(u32 ms);
*************************************************************************************************************/
#ifndef _SIMCOM_H_
#define _SIMCOM_H_
#include "system.h"
 
 
//SIMCOMͨ��ģ�鶨��	
typedef enum
{
	SIMCOM_SIM900	=	0	,		//Ĭ��ΪSIM900
	SIMCOM_SIM800	=	1	,		//SIM800
	SIMCOM_SIM2000	=	2	,		//SIM2000
	SIMCOM_SIM7600	=	3	,		//SIM7600
	SIMCOM_SIM868	=	4	,		//SIM868
	SIMCOM_SIM7000C	=	5	,		//SIM7000C
	LYNQ_L700		=	10	,		//LYNQ_L700
	SIMCOM_INVALID	=	0XFF	,	//��Ч��Ĭ��
}SIMCOM_MODE_TYPE;	
 
//����ע��״̬
typedef enum
{
	SIMCOM_NET_NOT = 0,		//δע��
	SIMCOM_NET_YES = 1,		//�Ѿ�ע��
	SIMCOM_NET_SEA = 2,		//δע��,��������
	SIMCOM_NET_TUR = 3,		//ע�ᱻ�ܾ�
	SIMCOM_NET_UNK = 4,		//δ֪
	SIMCOM_NET_ROA = 5,		//�Ѿ�ע��,��������
	SIMCOM_NET_ERROR=0XFF	//����
}SIMCOM_NETSTATUS;
 
 
 
//SIMCOM������ʽ
typedef enum
{
	SIMCOM_NETMODE_NOT 			= 	0,	//δע��
	SIMCOM_NETMODE_GSM			=	1,	//GSM
	SIMCOM_NETMODE_GPRS			=	2,	//GPRS
	SIMCOM_NETMODE_EGPRS		=	3,	//EGPRS (EDGE)
	SIMCOM_NETMODE_WCDMA		=	4,	//WCDMA
	SIMCOM_NETMODE_HSDPA		=	5,	//HSDPA only(WCDMA)
	SIMCOM_NETMODE_HSUPA		=	6,	//HSUPA only(WCDMA)
	SIMCOM_NETMODE_HSPA			=	7,	//HSPA (HSDPA and HSUPA, WCDMA)
	SIMCOM_NETMODE_LTE			=	8,	//LTE
	SIMCOM_NETMODE_TDS_CDMA		=	9,	//TDS-CDMA
	SIMCOM_NETMODE_TDS_HSDPA	=	10,	//TDS-HSDPA only��SIM7000C ����NBҲ�������
	SIMCOM_NETMODE_TDS_HSUPA	=	11,	//TDS-HSUPA only
	SIMCOM_NETMODE_TDS_HSPA		=	12,	//TDS- HSPA (HSDPA and HSUPA)
	SIMCOM_NETMODE_CDMA			=	13,	//CDMA
	SIMCOM_NETMODE_EVDO			=	14,	//EVDO
	SIMCOM_NETMODE_HYBRID		=	15,	//HYBRID (CDMA and EVDO)
	SIMCOM_NETMODE_1XLTE		=	16,	//1XLTE(CDMA and LTE)
	SIMCOM_NETMODE_NULL			=	0xff,	//δ֪
}SIMCOM_NETMODE_TYPE;
 
 
//SIM������״̬
typedef enum
{
	SIM_READY			=	0,	//SIM������
	SIM_NOT_READY		=	1,	//SIM��δ����
	SIM_UNKNOWN			=	2	//SIM��״̬δ֪
}SIM_CARD_STATUS;
 
//����IO��ƽ����
#define SIMCOM_H_LEVEL		1	//�ߵ�ƽ
#define SIMCOM_L_LEVEL		0	//�͵�ƽ
 
//DCD״̬����
#define DCD_DATA_MODE		0	//����͸��ģʽ
#define DCD_AT_MODE			1	//ATָ��ģʽ
 
//�����Ϣ��������
#define SIMCOM_INFO_SIZE	24	//��Ϣ����
#define SIMCOM_VER_SIZE		24	//����汾���ȶ���
 
//���Դ���,��ֹATָ�����ʧ��
#define SIMCOM_DEFAULT_RETRY	2 
 
 
//SIMCOMģ�������Ϣ
typedef struct
{
	char Manu[SIMCOM_INFO_SIZE+1];					//������
	char Model[SIMCOM_INFO_SIZE+1];					//�ͺ�
	char Ver[SIMCOM_VER_SIZE+1];					//����汾
	char IMEI[SIMCOM_INFO_SIZE+1];					//���к�
}SIMCOM_INFO;
 
 
//NBIOTģʽ����
typedef enum
{
	NB_IOT_MODE = 0,		//NBIOTģʽ
	CAT_M_MODE = 1,			//CAT-Mģʽ
}NBIOT_MODE_TYPE;
 
//����ģʽ����
typedef struct
{
	SIMCOM_MODE_TYPE	ModeType;					//ģ���ͺ�
	NBIOT_MODE_TYPE 	NB_Mode;					//NBģʽ
	s8 NB_EnableMode;								//NBģʽʹ��ģʽ,-1:�������ã�0���ر�NB��ʹ��GSMģʽ��1��ʹ��NBģʽ
	bool isNB_ScarEnable;							//NBģʽ����ʹ��
}NETWORK_CONFIG_TYPE;
 
 
 
//SIMCOMͨ��ģ����
typedef struct
{
	//�������
	SIMCOM_MODE_TYPE	SimcomModeType;				//ģ���ͺ�
	char TelecomCarr[SIMCOM_INFO_SIZE+1];			//��Ӫ������
	SIMCOM_INFO SIMCOM_Info;						//SIMCOMͨ��ģ�������Ϣ�ṹ��
	NETWORK_CONFIG_TYPE NetworkConfig;				//����ģʽ����
	SIMCOM_NETMODE_TYPE NetworkMode;				//��ǰ������ʽ
	u8 Singal;										//�����ź�ǿ��
	char LocalPhoneNumber[16];						//�����绰����
	char ServiceCenterPhoneNumber[16];				//�������ĵ绰����
	char SIM_CIMI[16];								//SIM��ΨһCIMI����
		
	//�ײ�ͨ�Žӿ�
	bool (* pSendData)(u8 *pDataBuff, u16 DataLen);											//�������ݽӿڣ��������ʧ�ܣ�����FALSE,�ɹ�����TRUE;
	int (* pReadData)(u8 **pDataBuff, u8 ByteTimeOutMs, u16 TimeOutMs, u16 *pReceiveDelay);	//�������ݽӿڣ��������ݳ��ȣ����ʧ�ܷ���<=0,�ɹ����������ݳ���
	void (*pClearRxData)(void);																//������ջ�������������������������ݻ���������
	void (*pSetDTR_Pin)(u8 Level);															//DTR���ŵ�ƽ����-���ڿ���sleepģʽ�����˳�͸��ģʽ
	void (*pSetPWRKEY_Pin)(u8 Level);														//PWRKEY�������ŵ�ƽ����-���ڿ���
	u8 (*pGetSTATUS_Pin)(void);																//��ȡSTATUS���ŵ�ƽ-����ָʾģ���ϵ�״̬
	u8 (*pGetDCD_Pin)(void);																//��ȡDCD���ŵ�ƽ-�ߵ�ƽATָ��ģʽ���͵�ƽΪ͸��ģʽ
	//ϵͳ�ӿ�
	void (*pDelayMS)(u32 ms);																//ϵͳ��ʱ����
	void (*pIWDG_Feed)(void);																//���ϵͳ���Ź�(����Ϊ��)
	//�ڲ�״̬����
	bool s_isInitStatus;																	//���ڼ�¼ģ���ʼ��״̬����λ���ϵ���Ϊ��Ч
}SIMCOM_HANDLE;
 
 
 
#endif /*_SIMCOM_H_*/

