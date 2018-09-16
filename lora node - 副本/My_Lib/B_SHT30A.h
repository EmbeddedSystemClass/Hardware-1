#ifndef _SHT30_T_H_
#define _SHT30_T_H_

/*--------------------------ͷ�ļ�����--------------------------------*/
#include "DataType.h"
#include "stm8l15x.h"

/*-----------------------------�ṹ�嶨��---------------------------------*/

/*-----------------------------�궨��---------------------------------*/

//#define         INT0      (0x01<<5)
#define         SCL0      GPIO_Pin_2
#define         SDA0      GPIO_Pin_1

//���Ŷ���
#define SHT30_SCL0_O    GPIO_Init(GPIOB, SCL0, GPIO_Mode_Out_PP_High_Fast)         //�������
#define SHT30_SCL0_H    GPIO_SetBits(GPIOB,SCL0)
#define SHT30_SCL0_L    GPIO_ResetBits(GPIOB,SCL0)

#define SHT30_SCL0_I    GPIO_Init(GPIOB, SCL0, GPIO_Mode_In_FL_No_IT)       //��������
#define SHT30_SCL0_DAT  ( (GPIO_ReadInputDataBit(GPIOB, SCL0))>>2 )

#define SHT30_SDA0_O    GPIO_Init(GPIOB, SDA0, GPIO_Mode_Out_PP_High_Fast)
#define SHT30_SDA0_H    GPIO_SetBits(GPIOB,SDA0)
#define SHT30_SDA0_L   GPIO_ResetBits(GPIOB,SDA0)

#define SHT30_SDA0_I    GPIO_Init(GPIOB, SDA0, GPIO_Mode_In_FL_No_IT)       //��������       //��������
#define SHT30_SDA0_DAT  ( (GPIO_ReadInputDataBit(GPIOB, SDA0))>>1 )


#define	SHT30_SlaveAddress	  		(0x44<<1)						//7λ��ַ0x44 ����1λ 0x45 -- 0x8A

#define noACK 0                                 			//�����ж��Ƿ����ͨѶ 
#define ACK   1                                 			//�������ݴ��� 

//����
#define SHT30_SINGLE_H_MEASURE_EN					0x2C06			
#define SHT30_SINGLE_M_MEASURE_EN					0x2C0D			
#define SHT30_SINGLE_L_MEASURE_EN					0x2C10			
#define SHT30_SINGLE_H_MEASURE_DIS				0x2400			
#define SHT30_SINGLE_M_MEASURE_DIS				0x240B			
#define SHT30_SINGLE_L_MEASURE_DIS				0x2416			

#define SHT30_PERIODOC_H_MEASURE_500MS		0x2032			
#define SHT30_PERIODOC_M_MEASURE_500MS		0x2024			
#define SHT30_PERIODOC_L_MEASURE_500MS		0x202F			
#define SHT30_PERIODOC_H_MEASURE_1S				0x2130			
#define SHT30_PERIODOC_M_MEASURE_1S				0x2126			//
#define SHT30_PERIODOC_L_MEASURE_1S				0x212D			//
#define SHT30_PERIODOC_H_MEASURE_2S				0x2236			//
#define SHT30_PERIODOC_M_MEASURE_2S				0x2220			//
#define SHT30_PERIODOC_L_MEASURE_2S				0x222B			//
#define SHT30_PERIODOC_H_MEASURE_4S				0x2334			//
#define SHT30_PERIODOC_M_MEASURE_4S				0x2322			//
#define SHT30_PERIODOC_L_MEASURE_4S				0x2329			//
#define SHT30_PERIODOC_H_MEASURE_10S			0x2737			//
#define SHT30_PERIODOC_M_MEASURE_10S			0x2721			//
#define SHT30_PERIODOC_L_MEASURE_10S			0x272A			//

#define SHT30_PERIODOC_MEASURE_READ			0xE000			//�ظ�������ȡ����

#define	SHT30_SOFT_RESET									0x30A2			//��λ

#define SHT30_HEATER_EN										0x306D			//����ʹ��
#define SHT30_HEATER_DIS									0x3066			//���ȹر�

#define SHT30_READ_STATUS									0xF32D			//��״̬�Ĵ���
#define SHT30_CLEAR_STATUS								0x3041			//��״̬�Ĵ���

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001



/*---------------------------�ⲿ��������-----------------------------*/

/*-----------------------------��������-------------------------------*/
vid SHT30_DelayMs(uint8 k);
vid SHT30_Delay1us(uint16 vTemp);
vid SHT30_Init(vid);
vid SHT30_Start(vid);                          //��ʼ�ź�
vid SHT30_Stop(vid);                           //ֹͣ�ź�
vid SHT30_SendACK(uint8 ack);                  //Ӧ��ACK
uint8 SHT30_RecvACK(vid);                      //��ack
uint8 SHT30_SendByte(uint8 dat);               //IIC�����ֽ�д
uint8 SHT30_RecvByte(vid);                     //IIC�����ֽڶ�
uint8 SHT30_CheckCrc8(uint8 *vDat, uint8 vLen);//CRC8 У��

uint16 SHT30_ReadSerialNumber(vid);
uint8 SHT30_Single_Measure(uint8 *vBuf);				//���βɼ�����
uint8 SHT30_Periodic_Measure(uint16 vCmd);			//�������ڲɼ�
uint8 SHT30_Periodic_Measure_Read(uint8 *vBuf);	//���ڲɼ���ȡ����
uint8 SHT30_Soft_Reset(vid);												//��λ
uint8 SHT30_Heater(uint16 vCmd);								//����������
uint16 SHT30_ReadStaus(vid);												//��ȡ״̬�Ĵ���
uint8 SHT30_ClearStaus(vid);												//���״̬�Ĵ���
void SHT30_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);	//����ɼ�����
void SHT30_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);							//��ȡ��ʪ������

#endif

