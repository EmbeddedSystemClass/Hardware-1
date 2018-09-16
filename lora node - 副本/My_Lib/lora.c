#include "stm8l15x.h"
#include "stm8_eval.h"
#include "uart1.h"
#include "Delay.h"
#include "lora.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART1_RX_STA;
//lora��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0;
	USART1_RX_STA=0;
	if((u32)*cmd<=0XFF)
	{
		//while((USART3->SR&0X40)==0);//�ȴ���һ�����ݷ������
		//USART3->DR=(u32)cmd;
		USART_SendData8(EVAL_COM1, (uint8_t)*cmd);


	}
	else u1_printf("%s\r\n",cmd); //��������

	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(lora_check_cmd(ack))
				{
					USART1_RX_STA=0;
					break;//�õ���Ч����
				}
			}
		}
		if(waittime==0)res=1;
	}
	return res;
}
u8* lora_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

unsigned char  lora_init()
{
	u8 retry;
	u8 temp=1;
	retry=3;
	while(retry--)
	{
		if(!lora_send_cmd("AT","OK",70))
		{
			temp=0;//���ɹ�
			break;
		}
	}
	if(retry==0) temp=1;//���ʧ��
	return temp;
}


