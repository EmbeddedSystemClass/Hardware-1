#include "bsp.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "systick_timer.h"
#include <string.h>
#include "AT_CMD.h"
#include "lte_hal.h"
#define Network_Thres    30

extern Uart_Types UsartType2,UsartType3;
unsigned char *Establish_TCP_Connection="AT+CIPOPEN=0,\"TCP\",\"zhangmeng5337.iask.in\",15969\r";

unsigned char	NET_STAUS=SIMCOM_NET_NOT;//��������״̬��Ϣ��־
uint32_t  SIMCOM_TimeOut_Count;
unsigned char Network_Intens;

/************************************************************
Function:  init LTE Module
Name:      LTE_M_init()
Parameter: no
************************************************************/
void LTE_M_init()
{

	

//	LTE_hw_init();
//	LTE_power_on();
//	LET_gps_power_ctrl(ON);
//	LET_power_ctrl();
//	LET_reset();
}
//�ҳ�temp��target��λ��
int FindIndex(char *target,char temp)
{
	int i= 0;
	if(target ==NULL)
	{
		//  printf("������Ϊ��...");
		return 0;
	}
	for(i = strlen(target) -1; i>=0; i--)
	{
		if(target[i] == temp)
		{
			return i;
		}
	}
	return -1;  //δ�ҵ��ַ�ƥ��λ��

}
/************************************************************
Function:  string match or not that return froom LTE module
Name:      AT_cmd_ack(unsigned char *src,unsigned char *dest)
Parameter: 1:string match, 0: string not match
************************************************************/
unsigned char AT_cmd_ack(unsigned char *src,unsigned char *ack)
{
	int i= 0,j = 0;
	int srclen = strlen((const char *)src);
	int tarlen=strlen((const char *)ack);
	int temp  =0;
	int index = -1;
	if(src ==NULL || ack ==NULL)
	{
		return 0;
	}

	while(i<srclen)
	{
		//ѭ������
		if(src[i] == ack[j])
		{
			if(j==tarlen-1)
			{
				// printf("ƥ��ɹ�...");
				return 1;
			}
			i++;
			j++;
		}
		else
		{
			//���ֲ����λ��
			temp = tarlen - j + i;  //�ַ�������ĵ�һ���ַ�λ��
			index = FindIndex((char *)ack,src[temp]);
			if(index==-1)
			{
				//δ�ҵ�λ�ã�����
				i = temp+1;
				j = 0;
			}
			else
			{
				//�ҵ�λ��
				i = temp-index;
				j = 0;
			}
		}
	}
//  printf("ƥ��ʧ��..");
	return 0;
}


unsigned char at_cmd_ok(unsigned char *src)
{
	if(AT_cmd_ack(src,(unsigned char *)Respond_OK)==1)//reset
	{
		return 1;
	}
	return 0;

}

unsigned char  SIMCOM_Get_InitReady_Staus(unsigned int waittime)
{
	unsigned char res=0;
	//Send_Comm((const char*)Normal_Mode,strlen((const char*)Normal_Mode));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_Start))
				{
					UsartType2.receive_flag=0;
					res=1;
					break;//�õ���Ч����
				}


			}
		}
		if(waittime==0)
			res=0;
	}
	return res;
}

unsigned char  SIMCOM_Get_Echo_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Echo_Dis,strlen((const char*)Echo_Dis));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_OK))
				{
					UsartType2.receive_flag=0;
					res=1;
					break;//�õ���Ч����
				}

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_NormalMode_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Normal_Mode,strlen((const char*)Normal_Mode));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_OK))
				{
					UsartType2.receive_flag=0;
					res=1;
					break;//�õ���Ч����
				}

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
unsigned int tmpp;
signed char  SIMCOM_Get_SimCard_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Check_SIM,strlen((const char*)Check_SIM));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		tmpp=waittime;
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_CPIN))
				{
					UsartType2.receive_flag=0;
					res=1;
					break;//�õ���Ч����
				}

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_Network_Intensity_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Network_Intensity,strlen((const char*)Network_Intensity));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_Network_Intensity))
				{
					UsartType2.receive_flag=0;
					res=0;
					break;//�õ���Ч����
				}
				else if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_Network_Intensity_eff))
				{
					res=1;
					UsartType2.receive_flag=0;

				}
				break;
				//res=0;

			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_GPRS_Register_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)GPRS_Register,strlen((const char*)GPRS_Register));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}

signed char  SIMCOM_Get_StartGPS_Register_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Start_GPS,strlen((const char*)Start_GPS));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}


signed char  SIMCOM_Get_Transparent_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Pass_Through,strlen((const char*)Pass_Through));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_NetOpen_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Net_Open,strlen((const char*)Net_Open));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_Network_Open2)||
				        AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_Network_Open))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}

			}
			if(waittime==0)res=0;
		}
		return res;
	}
}


signed char  SIMCOM_Get_EstablishTCP_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Establish_TCP_Connection,strlen((const char*)Establish_TCP_Connection));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_TCP_Connect))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����
				}


				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_TCP_Staus(unsigned int waittime)
{
	unsigned char res=0;
	//Send_Comm((const char*)Establish_TCP_Connection,strlen((const char*)Establish_TCP_Connection));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(5);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)TCP_Closed)
					||AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_No_Carrier)
					||AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)TCP_ERROR))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}

				//res=0;TIMEOUT
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
signed char  SIMCOM_Get_TIMEOUT_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)TIMEOUT,strlen((const char*)TIMEOUT));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(5);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)TCP_Closed))
					//||AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)TCP_ERROR))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}

				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}

signed char  SIMCOM_Get_QuitTransparent_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Quit_transparent,strlen((const char*)Quit_transparent));
  
	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(1);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					delay_ms(200);
					UsartType2.receive_flag=0;
					break;//�õ���Ч����
				}


				//res=0;
			}
		}
		if(waittime==0)
			res=0;
	}
	return res;
}
signed char  SIMCOM_Get_TO_DataMode_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)TO_DATA_MODE,strlen((const char*)TO_DATA_MODE));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_TCP_Connect))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}


				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}
unsigned char  SIMCOM_Get_Trans_Staus(unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)Pass_Through,strlen((const char*)Pass_Through));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],(unsigned char*)Respond_OK))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}


				//res=0;
			}
		}
		if(waittime==0)res=0;
	}
	return res;
}


//��sim900a��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
unsigned char  SIMCOM_SendAT(unsigned char *cmd,unsigned char *ack,unsigned int waittime)
{
	unsigned char res=0;
	Send_Comm((unsigned char*)cmd,strlen((const char*)cmd));

	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(UsartType2.receive_flag==1)//���յ��ڴ���Ӧ����
			{
				if(AT_cmd_ack(&UsartType2.usartDMA_rxBuf[UsartType2.real_index],ack))
				{
					res=1;
					UsartType2.receive_flag=0;
					break;//�õ���Ч����

				}


			}
		}
		if(waittime==0)res=0;
	}
	return res;
}


void SIMCOM_ReConnect()
{
	if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK) //����ϵͳ��ʱ���ƣ���֤ϵͳ�ܹ��ڳ�ʱ��������������
		SIMCOM_TimeOut_Count++;
	else if(NET_STAUS==SIMCOM_NET_OK)
		SIMCOM_TimeOut_Count=0;
	if(SIMCOM_TimeOut_Count>=SIMCOM_TIME_OUT)
	{
		NET_STAUS=SIMCOM_NET_ERROR;
	}

}
void SIMCOM_Register_Network()
{
	SIMCOM_ReConnect();


	switch(NET_STAUS)
	{
	case SIMCOM_NET_NOT:
	{
		if(SIMCOM_Get_InitReady_Staus(1000)==1)
			NET_STAUS=SIMCOM_READY_YES;
	}
	break;
	case SIMCOM_READY_YES:
	{
		if( SIMCOM_Get_Echo_Staus(500)==1)
			NET_STAUS=SIMCOM_Echo_OFF;
	}
	break;
	case SIMCOM_Echo_OFF:
	{
		if(SIMCOM_Get_NormalMode_Staus(500)==1)
			NET_STAUS=SIMCOM_NORMAL_MODE;
	}
	break;
	case SIMCOM_NORMAL_MODE:
	{
		if(SIMCOM_Get_SimCard_Staus(500)==1)
			NET_STAUS=SIMCOM_SIMCARD_READY;
	}
	break;

	case SIMCOM_SIMCARD_READY:
	{
		if(SIMCOM_Get_Network_Intensity_Staus(500)==1)
			NET_STAUS=SIMCOM_Network_Intensity_READY;
	}
	break;
	case SIMCOM_Network_Intensity_READY:
	{
		if(SIMCOM_Get_GPRS_Register_Staus(200)==1)
			NET_STAUS=SIMCOM_GPRS_READY;
	}
	break;
//	case SIMCOM_GPRS_READY:
//	{
//		if(SIMCOM_Get_StartGPS_Register_Staus(200)==1)
//			NET_STAUS=SIMCOM_START_GPS;
//	}
//	break;
	case SIMCOM_GPRS_READY:
	{
		if(SIMCOM_Get_Transparent_Staus(200)==1)
			NET_STAUS=SIMCOM_TRANSPERENT_MODE;
	}
	break;
	case SIMCOM_TRANSPERENT_MODE:
	{
		if(SIMCOM_Get_NetOpen_Staus(200)==1)
			NET_STAUS=SIMCOM_NetOpen_READY;
	}
	break;
	case SIMCOM_NetOpen_READY:
	{
		if(SIMCOM_Get_EstablishTCP_Staus(500)==1)
			NET_STAUS=SIMCOM_NET_OK;
	}
	break;
	case SIMCOM_NET_OK:
	{
		if(SIMCOM_Get_TCP_Staus(40)==1)
			NET_STAUS=SIMCOM_NetOpen_READY;
	}
	break;
	case SIMCOM_NET_ERROR:
	{
		//LET_reset();                    //��λ����
		NET_STAUS=SIMCOM_NET_NOT;       //״̬����λ
	}
	break;
	}
	if(NET_STAUS!=SIMCOM_NET_NOT&&NET_STAUS!=SIMCOM_NET_OK)
	{
		Network_Intens=Network_Intens++;
	}
	else
	{
		Network_Intens=0;

	}
}
void SIMCOM_Reconnect_status()
{
	NET_STAUS=SIMCOM_NetOpen_READY;
}
void  Server_IP_Config(unsigned char * pbuff)
{
	Establish_TCP_Connection=pbuff;
}
unsigned char Get_Network_status()
{
	return NET_STAUS;
}
void Set_Network_status()
{
	NET_STAUS=SIMCOM_NetOpen_READY;
}

unsigned char Get_Network_Thres()
{
	if(Network_Intens>=Network_Thres)
		return 1;
	else
		return 0;
}


