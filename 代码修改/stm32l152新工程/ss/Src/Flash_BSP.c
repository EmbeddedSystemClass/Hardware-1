#include "main.h"
#include "Flash_BSP.h"
#include "stm32l1xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//SPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/16/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern SPI_HandleTypeDef hspi1;

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{
 W25QXX_CS(1);
    //SPI1_ReadWriteByte(0Xff);                           //��������
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
unsigned char SPI1_ReadWriteByte(unsigned char TxData)
{
    unsigned char Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 10);       
 	return Rxdata;          		    //�����յ�������		
}


void W25QXX_CS(unsigned char flag)
{
  switch(flag)
  {
      case 0:   
      {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
      }break;
      case 1:
      {
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
     
      } break;  
  }


}


