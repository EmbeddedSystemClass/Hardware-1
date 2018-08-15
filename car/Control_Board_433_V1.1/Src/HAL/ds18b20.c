#include "main.h"
#include "systick_timer.h"
#include "stm32f4xx_hal.h"
#include "ds18b20.h"
#include "ds18b20_BSP.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��λDS18B20
void DS18B20_Rst(void)	   
{   
   unsigned char 	presence;
	//DS18B20_IO_OUT();   //����Ϊ���
  DS18B20_DQ_OUT(1);  //����DQ
  delay_us(50);      //����750us
  DS18B20_DQ_OUT(0);  //DQ=1 
	delay_us(470);       //15US
	 DS18B20_DQ_OUT(1);  //����DQ

             //������ʱ
   delay_us(40);       //15US
    if(DS18B20_DQ_IN()==0)      //���=0���ʼ���ɹ� =1���ʼ��ʧ��
     presence = 1;
    else  
	 presence = 0;
          
    delay_us(450);           //  ��ʱ470us
     DS18B20_DQ_OUT(1);  //����DQ
	
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
unsigned char  DS18B20_Check(void) 	   
{   

}

//��DS18B20��ȡһ��λ
//����ֵ��1/0


//��DS18B20��ȡһ���ֽ�
//����ֵ������������
unsigned char  DS18B20_Read_Byte(void)   
{        
  unsigned char  i,dat;
  dat=0;
    for (i = 8; i > 0; i--) 
	{

      	DS18B20_DQ_OUT(0);
		   //delay_us(1);      //����Ϊ�͵�ƽ
      	dat >>= 1;
      	DS18B20_DQ_OUT(1);       //����Ϊ�ߵ�ƽ(�ͷ�����)   
      	if(DS18B20_DQ_IN())	//��DQ
      		dat |= 0x80;
      	delay_us(80);
      	DS18B20_DQ_OUT(1);		
    }						    
  return dat;
}

//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(unsigned char  dat)     
 {             
    unsigned char  j;
    unsigned char  testb;
  //  DS18B20_IO_OUT();     //����Ϊ���
for (j= 8; j > 0; j--)	 //дһ�ֽ� 
	{DS18B20_DQ_OUT(0);
        testb=dat&0x01;
       
        if(testb)       // д1
        {                           
            DS18B20_DQ_OUT(1);
           // delay_us(60);             
        }
        else            //д0
        {
            DS18B20_DQ_OUT(0);
					}
				delay_us(80);
        DS18B20_DQ_OUT(1);					
				dat=dat>>1;
                                                 
        
    }
}
 
//��ʼ�¶�ת��
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
   // DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
unsigned char  DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_12;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��
 
 	  DS18B20_Rst();
	 // return DS18B20_Check();
}
unsigned char a,b,c,d,e,f,g,h;
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    unsigned char  temp;
    unsigned char  TL,TH;
    short tem;
    DS18B20_Start ();           //��ʼת��
	DS18B20_Write_Byte(0x33);   // skip rom
	a=DS18B20_Read_Byte();
		b=DS18B20_Read_Byte();
		c=DS18B20_Read_Byte();
		d=DS18B20_Read_Byte();
		e=DS18B20_Read_Byte();
		f=DS18B20_Read_Byte();
		g=DS18B20_Read_Byte();
		h=DS18B20_Read_Byte();
		
   // DS18B20_Rst();
  //  DS18B20_Check();	
    DS18B20_Start (); 	
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
    TL=DS18B20_Read_Byte();     // LSB   
    TH=DS18B20_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(double)tem*0.625;//ת��     
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
}
