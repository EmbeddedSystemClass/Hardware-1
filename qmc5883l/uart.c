#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{     
// uint8_t temp[1];
//  temp[0]=ch;	
//   HAL_UART_Transmit(&huart2, temp, 1, 2);
//}
#endif 
