#ifndef __USART_H
#define __USART_H

#define Burate_SETTING      57600
#include "stm32f0xx_hal.h"

extern unsigned char uart_accept_arry[buffer_size];//��ŷ��ص���Ϣ
extern unsigned char uart_accept_end_flag,uart_accept_start_flag,uart_accept_count;//���ڴ洢��Ϣ
extern unsigned int uart_time;//���ڴ洢��Ϣ
 void USART_Config(void);
 void UART_send_byte(uint8_t byte) ;
 void UART_Send(uint8_t *Buffer);
 void USART1_Rec(void);
#endif


