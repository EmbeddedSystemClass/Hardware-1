//�������ݽӿ�
static bool GPRS_UART_SendData(u8 DataBuff[], u16 DataLen)
{
	UARTx_SendData(SIMCOM_UART_CH, DataBuff, DataLen);
	
	return TRUE;
}
 
 
 
//�������ݽӿ�
static int GPRS_UART_ReadData(u8 **pDataBuff,u8 ByteTimeOutMs, u16 TimeOutMs, u16 *pReceiveDelayMs)
{
	u32 cnt = 0;
	u16 TempTime;
	
	if(ByteTimeOutMs < 1) ByteTimeOutMs = 1;	//�ֽڳ�ʱʱ�䣬2��֮֡��ļ����Сʱ��
	TimeOutMs /= ByteTimeOutMs;
	TimeOutMs += 1;
	TempTime = TimeOutMs;
	while(TimeOutMs --)
	{
		cnt = UARTx_GetRxCnt(SIMCOM_UART_CH);
		OSTimeDlyHMSM(0,0,0,ByteTimeOutMs);;
		if((cnt > 0) && (cnt == UARTx_GetRxCnt(SIMCOM_UART_CH)))
		{
			if(pReceiveDelayMs!=NULL)	//��Ҫ������ʱ
			{
				*pReceiveDelayMs = (TempTime-TimeOutMs)*ByteTimeOutMs;
			}
			*pDataBuff = g_SIMCOM_Buff;						//���ջ�����
			
			return cnt;
		}
#if SYS_WDG_EN_
		IWDG_Feed();										//ι��
#endif			
	}
	
	return 0;
}
 
 
//������ջ�����
static void GPRS_UART_ClearData(void)
{
	UARTx_ClearRxCnt(SIMCOM_UART_CH);	//������ڻ�����
}
/////////////////////////////////////////////////////////////////////////////////////////////
//SIM900/SIM800ͨ��֧��
//GSMģ����ض���
#define SIMCOM_UART_CH			UART_CH3			//����3
#define SIMCOM_UART_BAUD		115200				//������
#define SIMCOM_UART_BUFF_SIZE	(1024*4)			//���ջ�������С
 
//��ؿ�������
__inline void SIMCOM_SetDTR(u8 Level)		{(PGout(4)=Level);}			//DTR
__inline void SIMCOM_SetPWRKEY(u8 Level)	{(PGout(3)=Level);}			//PWRKEY
__inline u8 SIMCOM_GetSTATUS(void)			{return PGin(5)?1:0;}		//STATUS
__inline u8 SIMCOM_GetDCD(void)				{return PDin(11)?1:0;}		//DCD-�������룬�ߵ�ƽATָ��ģʽ���͵�ƽΪ͸��ģʽ
 
//���ų�ʼ��
__inline void SIMCOM_IO_Init(void)	
{
	SYS_DeviceClockEnable(DEV_GPIOD, TRUE);					//ʹ��GPIODʱ��
	SYS_DeviceClockEnable(DEV_GPIOG, TRUE);					//ʹ��GPIOGʱ��
	SYS_GPIOx_Init(GPIOG, BIT3|BIT4, OUT_PP, SPEED_2M);		//�������
	SYS_GPIOx_OneInit(GPIOD, 11, IN_IPU, IN_NONE);			//DCD ��������
	SYS_GPIOx_OneInit(GPIOG, 5, IN_IPD, IN_NONE);			//STATUS ��������
}


