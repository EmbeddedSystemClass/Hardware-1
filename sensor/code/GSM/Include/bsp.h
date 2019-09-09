/*===========================================================================
��ַ ��http://yhmcu.taobao.com/
���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾
�ʼ� ��yihe_liyong@126.com
�绰 ��18615799380
===========================================================================*/

#ifndef _BSP_H_
#define _BSP_H_
#include "stm8l15x.h"
#include "STM8l15x_conf.h"
#include "sx1276.h"
#include "mytypedef.h"
#include "STM8l15x_spi.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#include "GSM_BSP.h"
#define ON   1
#define OFF  0
// SPI���Ŷ��� SCLK(PB5), MOSI(PB6), MISO(PB7)
#define PORT_SPI        GPIOB
#define PIN_SCLK        GPIO_Pin_5
#define PIN_MOSI        GPIO_Pin_6
#define PIN_MISO        GPIO_Pin_7

// LED��KEY���Ŷ��壬LED(PB0), KEY(PC1)
#define PORT_LED        GPIOB
#define PIN_LED         GPIO_Pin_3

#define PORT_KEY        GPIOD
#define PIN_KEY         GPIO_Pin_1

#define PORT_POWER_ON        GPIOC
#define PIN_POWER_ON         GPIO_Pin_0

#define PORT_PWRKEY_IN       GPIOD
#define PIN_PWRKEY_IN        GPIO_Pin_4

#define PORT_SENSOR_EN       GPIOB
#define PIN_SENSOR_EN       GPIO_Pin_7
// LED����������(ON)��, (OFF)�رգ�(TOG)��ת
#define LED_ON()        GPIO_ResetBits(PORT_LED, PIN_LED)
#define LED_OFF()       GPIO_SetBits(PORT_LED, PIN_LED)
#define LED_TOG()       GPIO_ToggleBits(PORT_LED, PIN_LED)

#define ADC_BAT_CHANNEL         ADC_Channel_1
#define ADC_SENSOR_CHANNEL      ADC_Channel_2


typedef struct
{
uint32_t 	rx_len;
uint32_t 	rx_len_var;
unsigned char 	receive_flag;
unsigned int      real_index;
unsigned int      loop_index;
unsigned char	usartDMA_rxBuf[buffer_size];

}Uart_Types;


void SClK_Initial(void);                // ��ʼ��ϵͳʱ�ӣ�ϵͳʱ�� = 16MHZ
void GPIO_Initial(void);                // ��ʼ��ͨ��IO�˿�
void SPI_Initial(void);                 // ��ʼ��SPI
INT8U SPI_ExchangeByte(INT8U input);    // ͨ��SPI�������ݽ���
void HardwareInit(void);
void LED_Init(void);
void USART_SendStr(unsigned char *Str);
void USART_SenByte(unsigned char *Str,unsigned char len) ;
uint16_t adcGet(ADC_Channel_TypeDef num);
#endif //_BSP_H_

/*===========================================================================
-----------------------------------�ļ�����----------------------------------
===========================================================================*/
