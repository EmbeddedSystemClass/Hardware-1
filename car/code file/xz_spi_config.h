#ifndef __UTIL_SPI_CONFIG_H__
#define __UTIL_SPI_CONFIG_H__

#include "stm32f10x_gpio.h"
#include "ysizet.h"
#include "DLib_Product_string.h"
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
#define UTIL_SPI_CS_LOW()         GPIO_ResetBits(GPIOA,RF_SPI_NSS)    
#define UTIL_SPI_CS_HIGH()        GPIO_SetBits(GPIOA,RF_SPI_NSS)     

#define UTIL_SPI_SCLK_LOW()       GPIO_ResetBits(GPIOA,RF_SPI_SCK) 
#define UTIL_SPI_SCLK_HIGH()      GPIO_SetBits(GPIOA,RF_SPI_SCK) 

#define UTIL_SPI_MISO_READ()      GPIO_ReadInputDataBit(GPIOA,RF_SPI_MISO)

#define UTIL_SPI_MOSI_LOW()       GPIO_ResetBits(GPIOA,RF_SPI_MOSI)                  
#define UTIL_SPI_MOSI_HIGH()      GPIO_SetBits(GPIOA,RF_SPI_MOSI)                  

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
#define UTIL_SPI_SCLK_DELAY()           do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)//do{_delay_us(1);}while(0)


#define UTIL_SPI_CS_PREV_BEGIN()
#define UTIL_SPI_CS_POST_END()

#endif
