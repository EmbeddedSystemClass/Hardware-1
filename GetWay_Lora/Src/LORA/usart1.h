#ifndef __USART1_H
#define __USART1_H	 
//#include "sys.h"  
#include "stdio.h"
#include "main.h"
#include "stm32f1xx_hal.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2016/6/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	
#include "sys.h"
#define USART1_MAX_RECV_LEN		1024					//�����ջ����ֽ���
#define USART1_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART1_RX_EN 			1					//0,������;1,����.

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART1_RX_STA;   						//��������״̬

extern UART_HandleTypeDef UART1_Handler;//UART���


void usart1_set(u8 bps,u8 parity);
void usart1_init(u32 bound);				//����3��ʼ�� 
void u1_printf(char* fmt, ...);
void usart1_set(u8 bps,u8 parity);    
void usart1_rx(u8 enable);

#endif



