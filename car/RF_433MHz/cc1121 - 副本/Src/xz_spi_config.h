#ifndef __UTIL_SPI_CONFIG_H__
#define __UTIL_SPI_CONFIG_H__

#include "stm32f0xx.h"
#include "main.h"

//==============================================================================
//���ò��֣���SPI�ӿ�����ֻ��Ҫ�޸����º궨�弴��

//------------------------------------------------------------------------------
//����SPIʱ�Ӽ���
//1:SPI����ʱʱ,SCLKΪ�ߵ�ƽ
//0:SPI����ʱʱ,SCLKΪ�͵�ƽ
#define UTIL_SPI_CPOL                             0
//------------------------------------------------------------------------------
//����SPIʱ����λ
#define UTIL_SPI_CPHA                             0
//CHPAΪ0��SCLK��0�����أ���CS�½��أ���SPI�ӻ�������ʱ���Ƴ�����λ����ʱ������ڣ�SCLK��1������ʱ����������λ����ʱ������ڣ�SCLK�ڶ��α���
//CHPAΪ1������ʱ������ڣ�SCLK��1������ʱ���Ƴ�����λ����ʱ������ڣ�SCLK��2������ʱ����������λ 

//--------------------------------------------------------------------------------------------------------
//����SPI�Ƴ������λ���������λ
#define UTIL_SPI_MOST_BIT_FIRST                   1    


//------------------------------------------------------------------------------
//SPI IO�ڲ���
#define UTIL_SPI_CS_LOW()         HAL_GPIO_WritePin(GPIOA,RF_SPI_NSS,GPIO_PIN_RESET)    
#define UTIL_SPI_CS_HIGH()        HAL_GPIO_WritePin(GPIOA,RF_SPI_NSS,GPIO_PIN_SET)     

#define UTIL_SPI_SCLK_LOW()       HAL_GPIO_WritePin(GPIOA,RF_SPI_SCK,GPIO_PIN_RESET) 
#define UTIL_SPI_SCLK_HIGH()      HAL_GPIO_WritePin(GPIOA,RF_SPI_SCK,GPIO_PIN_SET) 

#define UTIL_SPI_MISO_READ()      HAL_GPIO_ReadPin(GPIOA,RF_SPI_MISO)

#define UTIL_SPI_MOSI_LOW()       HAL_GPIO_WritePin(GPIOA,RF_SPI_MOSI,GPIO_PIN_RESET)                  
#define UTIL_SPI_MOSI_HIGH()      HAL_GPIO_WritePin(GPIOA,RF_SPI_MOSI,GPIO_PIN_SET)                  

//------------------------------------------------------------------------------
//SPI IO������ʱ�����������ȴ�

//SPI CS�½��غ󣬵ȴ���ͨ���ȴ�SPI��ɫ����
//ע�⣺����RFоƬ��CSΪ�ͺ󣬱���ȴ�MISO����GPIO0)Ϊ�ͣ����ܽ�һ������
#define UTIL_SPI_CS_POST_FALL_DELAY()   //while(UTIL_SPI_MISO_READ())  //�ȴ�MISOΪ��

//SPI CS������ǰ���ȴ�
#define UTIL_SPI_CS_PREV_RISE_DELAY()    //do{asm("nop");asm("nop");asm("nop");asm("nop");}while(0)

//SPI CS�����غ󣬵ȴ�
#define UTIL_SPI_CS_POST_RISE_DELAY()   //do{asm("nop");asm("nop");asm("nop");asm("nop");}while(0)

//SPI SCLK������ǰ���ȴ�
#define UTIL_SPI_SCLK_DELAY()          // do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)//do{_delay_us(1);}while(0)


#define UTIL_SPI_CS_PREV_BEGIN()
#define UTIL_SPI_CS_POST_END()

#endif
