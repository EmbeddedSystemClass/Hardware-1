#include "stm32f10x_can.h"
#include "stm32f10x.h"
#include "platform_config.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stdarg.h"
#include "stm32f10x_spi.h"
#include "oled.h"


#define CAN_BAUDRATE 500
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


__IO uint32_t rec_ff = 0; /* for return of the interrupt handling */
volatile TestStatus TestRx;
int rec_u = 0;
/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
TestStatus CAN_Polling(void);
TestStatus CAN_Interrupt(void);

static __IO uint32_t TimingDelay;
void RCC_Configuration(void);
void Delay(__IO uint32_t nTime);
extern void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char * BMP);
extern unsigned char BMP[];
extern unsigned char fd0[];
extern unsigned char fd1[];
extern unsigned char fd2[];
extern unsigned char fd3[];
extern unsigned char fd4[];
extern char sss;
char r[5];
char m[5];
char n[5];
int ii,k;
extern void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
extern void LCD_P16x16Ch(unsigned char x, unsigned char y, unsigned char N, const unsigned char * ch);
////////////SPI2�ӿ�����/////////////////////////////////////////////////////
void SPI_Configuration(void)
{
	  SPI_InitTypeDef  SPI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);

	  /* SPI2���Ŷ���  PB13--SPI2_SCK  PB15--SPI2_MOSI */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* PB12--OLED RST  ���Ŷ��� */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12  ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
      /* PC6--OLED C/D ���Ŷ��� */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  /* ����SPI2 OLEDƬѡ  PB0 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			  					 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���ģʽ����ٶ�50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //ͨ���������ģʽ
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* SPI2 Config -------------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;   //SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI2, &SPI_InitStructure);
	  /* Enable SPI2 */
	  SPI_Cmd(SPI2, ENABLE);
}

void RCC_Configuration(void){

  SystemInit();   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
  						, ENABLE);
}
/****************************************************************************
* ��    �ƣ�void Delay(__IO uint32_t nTime)
* ��    �ܣ���ʱ��ʱ���� 1msΪ��λ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
/****************************************************************************
* ��    �ƣ�void TimingDelay_Decrement(void)
* ��    �ܣ���ȡ���ĳ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t TxBuffer1[] = "USART Interrupt Example: This is USART1 DEMO";  
uint8_t RxBuffer1[],tx_flag;
extern uint8_t rec_f;
__IO uint8_t TxCounter1 = 0x00;
__IO uint8_t RxCounter1 = 0x00; 

uint32_t Rec_Len;


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

void Delay(__IO uint32_t nCount);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);
void USART_Config(USART_TypeDef* USARTx);


GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStruct;
USART_ClockInitTypeDef USART_ClockInitStruct;

/****************************************************************************
* ��    �ƣ�void ili9325_DrawPicture(u16 StartX,u16 StartY, u8 Dir��u8 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*			Dir		   ͼ����ʾ����       
*           pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ  ȡģ���img2LCD
* ���÷�����ili9325_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void USART_Config(USART_TypeDef* USARTx){
  USART_InitStructure.USART_BaudRate = 9600;						//����9600bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//���ô��ڲ�������
 
  
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    //ʹ�ܽ����ж�
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);						//ʹ�ܷ��ͻ�����ж�   

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);	
}
int Value[5]= {0xFF,0xFF,0x01,0xAA,0xBB};
void CC1110_Sendd(void)
{
    int i;
    USART1->SR = (uint16_t)~0x0040;
    for(i=0; i<5; i++)
    {
        USART1->DR = (*(Value+i) & (uint16_t)0x01FF);
        while((USART1->SR & 0x0080) == RESET); //������ɱ�־
    }
}	
/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ��ܶ���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
int main(void)
{

   uint8_t a=0;
  /* System Clocks Configuration */
  RCC_Configuration();											  //ϵͳʱ������
       
  /*Ƕ�������жϿ����� 
      ˵����USART1��ռ���ȼ�����0�����1λ�� ���������ȼ�����0�����7λ�� */ 
  NVIC_Configuration();											  //�ж�Դ����

  /*�Կ���LEDָʾ�Ƶ�IO�ڽ����˳�ʼ�������˿�����Ϊ������������������ٶ�Ϊ50Mhz��PA9,PA10�˿ڸ���Ϊ����1��TX��RX��
  ������ĳ������ʱ������Ӧ�������ڵĶ˿ڵ�ʱ�ӽ���ʹ�ܡ������޷����óɹ��������õ��˶˿�B�� ���Ҫ������˿ڵ�ʱ��
  ����ʹ�ܣ�ͬʱ�����õ�����IO�ڹ����������ô��ڡ���˻�Ҫʹ��AFIO�����ù���IO��ʱ�ӡ�*/
  GPIO_Configuration();											  //�˿ڳ�ʼ��

	 //RCC_Configuration();	           		//ϵͳʱ�����ü�������ʱ��ʹ��
   if (SysTick_Config(3600))		    //ʱ�ӽ����ж�ʱ50usһ��  ���ڶ�ʱ 
   { 
     Capture error */ 
     while (1);
   }
   SPI_Configuration();		  
   Select_OLED
   LCD_Init();

  USART_Config(USART1);											  //����1��ʼ��
  
   while (1)
  {
		LCD_CLS();
		LCD_P16x16Ch(0,0,8,fd1);  
		//LCD_P16x16Ch(16,2,2,fd1);  //
		//LCD_P16x16Ch(16,4,2,fd2);  //
		//LCD_P16x16Ch(16,6,2,fd3);  //
		//LCD_P8x16Str(64,2,"24.5"); 
		//LCD_P8x16Str(64,4,"12.5"); 
		//LCD_P8x16Str(64,6,"6.2"); 
		LCD_P8x16Str(10,2,r);
		LCD_P8x16Str(50,2,m);
		LCD_P8x16Str(100,2,n);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);
		Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);Delay(2000);

		//CC1110_Sendd();
		TestRx = CAN_Interrupt();
		//LCD_P8x16Str(64,6,array="RxBuffer1[RxCounter1-2]");
	    //LCD_P16x16Ch(56,0,1,fd0);  //
		//LCD_P16x16Ch(16,2,2,fd1);  //
	if(rec_f==1){												  //�ж��Ƿ��յ�һ֡��Ч����
		rec_f=0;
		//USART_OUT(USART1,"\r\n�����͵���ϢΪ: \r\n");    
		//USART_OUT(USART1,&TxBuffer1[0]);
		CC1110_Sendd();
		//TestRx = CAN_Interrupt();
		if(a==0) {GPIO_SetBits(GPIOB, GPIO_Pin_5); a=1;}          //LED1  V6��V3�壩 V2��MINI�壩 ������˸                
		else {GPIO_ResetBits(GPIOB, GPIO_Pin_5);a=0;  }
	}
  }
}

/****************************************************************************
* ��    �ƣ�void Delay(__IO uint32_t nCount)
* ��    �ܣ���ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
//void Delay(__IO uint32_t nCount)
//{
 // for(; nCount != 0; nCount--);
//}

/****************************************************************************
* ��    �ƣ�void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* ��    �ܣ���ʽ�������������
* ��ڲ�����USARTx:  ָ������
			Data��   ��������
			...:     ��������
* ���ڲ�������
* ˵    ������ʽ�������������
        	"\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
			"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
* ���÷������� 
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //�ַ���
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //ʮ����
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}

/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1����--PB5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A�˿� 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //���ÿ�©����
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 //A�˿� 
}

/****************************************************************************
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void NVIC_Configuration(void)
{
  /*  �ṹ����*/
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��   ��������1�� 7 */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;		   //��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			   //�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//���ô���1�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	     	//��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;				//�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ��
  NVIC_Init(&NVIC_InitStructure);
}
int k=0;
int l,v,s,w,a,b,id,z;
void USART1_IRQHandler(void)      //����1 �жϷ������
{
  unsigned int i;
	
	//char sss;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	   //�ж϶��Ĵ����Ƿ�ǿ�
  {	   
    RxBuffer1[RxCounter1++] = USART_ReceiveData(USART1);   //�����Ĵ��������ݻ��浽���ջ�������
		//int(RxBuffer1[RxCounter1-2])
		//RxBuffer1[RxCounter1-1]=(RxBuffer1[RxCounter1-2]<<8)+RxBuffer1[RxCounter1-1];
		//RxBuffer1[1] = USART_ReceiveData(USART1);   //�����Ĵ��������ݻ��浽���ջ�������
		//RxBuffer1[2] = USART_ReceiveData(USART1);   //�����Ĵ��������ݻ��浽���ջ�������
	  if(k==0)
		{
		 id = RxBuffer1[RxCounter1-1];
		 *itoa(id,r,10);
		 k++;
		}
		else if(k==1)
		{
     l=RxBuffer1[RxCounter1-1];
		 k++;
		}	
		else if(k==2)
		{
		 v=(l<<8)+RxBuffer1[RxCounter1-1];
		 *itoa(v,m,10);
		 k++;
		}
		else if(k==3)
		{
	   s=RxBuffer1[RxCounter1-1];
			if(s==0x07)
			{
			s=0x01;
			}
		 k++;
		}
		else if(k==4)
		{
		 w=(s<<8)+RxBuffer1[RxCounter1-1];
		 *itoa(w,n,10);
		 k++;
		}
		else if(k==5)
		{
		z=RxBuffer1[RxCounter1-1];	
		k=0;	
		}
		//k=RxBuffer1[RxCounter1-1];
    //if(RxBuffer1[RxCounter1-2]==0x0d&&RxBuffer1[RxCounter1-1]==0x0a)     //�жϽ�����־�Ƿ���0x0d 0x0a
		//if(RxBuffer1[RxCounter1-1]==0x02) 
    //{
	  for(i=0; i< RxCounter1; i++) TxBuffer1[i]	= RxBuffer1[i]; 	     //�����ջ�����������ת�����ͻ�������׼��ת��
	  rec_u=1;															 //���ճɹ���־
	  TxBuffer1[RxCounter1]=0;		                                     //���ͻ�����������    
	  //RxCounter1=0;	  
    //}
  }
  
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                   //�����Ϊ�˱���STM32 USART ��һ���ֽڷ�����ȥ��BUG 
  { 
     USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					     //��ֹ�����������жϣ� 
  }	
  
}

/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/
TestStatus CAN_Interrupt(void)
{
	
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;  
  GPIO_InitTypeDef  GPIO_InitStructure;
  CanTxMsg TxMessage;
  //CanRxMsg RxMessage;
  //uint8_t TransmitMailbox = 0;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			             //LCD����ر�	

  GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);		     //�˿ڸ���ΪCAN1   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	                 //PB8:CAN-RX 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			     //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					 //PB9:CAN-TX 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CAN�Ĵ�����ʼ�� */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN��Ԫ��ʼ�� */
  CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  ʱ�䴥��ͨ��ģʽʹ��
  CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  �Զ����߹��� 
  CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  �Զ�����ģʽ
  //CAN_InitStructure.CAN_NART=ENABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  0-�Զ��ش�   1-����ֻ��һ��
  CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  0-�Զ��ش�   1-����ֻ��һ��
  CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  0-���ʱ�±��ĻḲ��ԭ�б���  1-���ʱ���±��Ķ���
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  //CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� 0-���ȼ�ȡ���ڱ��ı�ʾ�� 1-���ȼ�ȡ���ڷ��������˳��
  //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;	   //BTR-SILM/LBKM   CAN����ģʽ
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW ����ͬ����Ծ��� 1��ʱ�䵥Ԫ
  CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;		   //BTR-TS1 ʱ���1 ռ����2��ʱ�䵥Ԫ
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ
  
#if CAN_BAUDRATE == 1000 /* 1MBps */
  CAN_InitStructure.CAN_Prescaler =6;			   //BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+2+3)/6=1Mbps
#elif CAN_BAUDRATE == 500 /* 500KBps */
  CAN_InitStructure.CAN_Prescaler =12;
#elif CAN_BAUDRATE == 250 /* 250KBps */
  CAN_InitStructure.CAN_Prescaler =24;
#elif CAN_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_Prescaler =48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
  CAN_InitStructure.CAN_Prescaler =60;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
  CAN_InitStructure.CAN_Prescaler =120;
#elif  CAN_BAUDRATE == 20 /* 20KBps */
  CAN_InitStructure.CAN_Prescaler =300;
#elif  CAN_BAUDRATE == 10 /* 10KBps */
  CAN_InitStructure.CAN_Prescaler =600;
#endif  

  //CAN_InitStructure.CAN_TXFP=DISABLE;
  //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
//  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
//  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
//  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
  //CAN_InitStructure.CAN_Prescaler=1;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN��������ʼ�� */
  CAN_FilterInitStructure.CAN_FilterNumber=0;						//
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;		//FM1R  ��������0�Ĺ���ģʽ��
  																	//0: ��������x��2��32λ�Ĵ��������ڱ�ʶ������λģʽ�� 
																	//1: ��������x��2��32λ�Ĵ��������ڱ�ʶ���б�ģʽ��
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//FS1R ��������0(13��0)��λ��
  																	//0��������λ��Ϊ2��16λ�� 1��������λ��Ϊ����32λ��
  
  /* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */
  CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)0x1234<<3)&0xFFFF0000)>>16;				//Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)0x1234<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;//Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xffff;
  
//  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;					
//  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//  /* ȫΪ0�������˱��ı�ʶ���б� */
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;				//FFAx : ������λ������ ������ͨ����ĳ�������Ĺ��˺�
  																	//������ŵ��������FIFO�С� 0����������������FIFO0�� 1����������������FIFO1��
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;				//FACTx : ���������� �����ĳλ����1��������Ӧ�Ĺ�������ֻ�ж�FACTxλ��0��
  																	//���CAN_FMR�Ĵ�����FINITλ����1�󣬲����޸���Ӧ�Ĺ������Ĵ���
																	//x(CAN_FxR[0:1])�� 0�������������ã� 1�������������
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN FIFO0 �����ж�ʹ�� */ 
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  /* ����һ֡���� */
  TxMessage.StdId=0x00;
  TxMessage.ExtId=id;
  TxMessage.IDE=CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.DLC=5;
  //TxMessage.Data[0]=0xDE;
	//TxMessage.Data[0]=RxBuffer1[RxCounter1-2];
	TxMessage.Data[0]=l;
	TxMessage.Data[1]=v;
	TxMessage.Data[2]=s;
	TxMessage.Data[3]=w;
	TxMessage.Data[4]=z;
  //TxMessage.Data[1]=0xCA;
	//TxMessage.Data[1]=RxBuffer1[RxCounter1-1];
	RxCounter1=0;
  if(rec_u==1)
	{												  //�ж��Ƿ��յ�һ֡��Ч����
		rec_u=0;
	  CAN_Transmit(CAN1, &TxMessage);
//		CAN_Transmit(CAN1,RxBuffer1[RxCounter1-1]);
	}
  /* �ȴ����ճɹ���־��λ */
  rec_ff = 0xFF;	 
  //while(rec_ff == 0x000000FF);

  /* �����жϽ�ֹ*/
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  return (TestStatus)rec_ff;
}

/******************* (C) COPYRIGHT 2013 �ܶ�STM32 *****END OF FILE****/
