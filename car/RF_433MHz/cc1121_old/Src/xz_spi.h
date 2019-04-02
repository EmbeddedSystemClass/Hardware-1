#ifndef __UTIL_SPI_H__
#define __UTIL_SPI_H__

#include  "xz_spi_config.h"

#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"


#define RF_SPI_NSS   GPIO_PIN_4 
#define RF_SPI_SCK   GPIO_PIN_5
#define RF_SPI_MISO  GPIO_PIN_6
#define RF_SPI_MOSI  GPIO_PIN_7  
 
#define RF_SPI_RST   GPIO_PIN_2  
#define RF_DATA_IRQ  GPIO_PIN_3

#define bool _Bool
#define true 1
#define false 0



/*!
 *******************************************************************************
 @brief  SPI 4��IO�ڳ�ʼ��������CS,MISO,MOSI,SCLK
 @param  void  ��
 @return void  ��
 *******************************************************************************
 */
void UTIL_SPI_4W_Init(void);

/*!
 *******************************************************************************
 @brief  SPI 3����λ�������ȡһ���ֽڣ�������CS����
 @param  unsigned char data  SPI��λ���������
 @return unsigned char ����SPI��λ��ȡ������
 *******************************************************************************
 */
unsigned char UTIL_SPI_3W_ReadWriteUint8(unsigned char data);


/*!
*******************************************************************************
@brief  SPI 3����λ�����������CS����
@param  unsigned char data  SPI��λ���������
@return void
*******************************************************************************
*/
#define UTIL_SPI_3W_WriteUint8(x) UTIL_SPI_3W_ReadWriteUint8(x)


/*!
*******************************************************************************
@brief  SPI 3�߶�ȡһ���ֽڣ�������CS����
@param  void  ��
@return unsigned char ����SPI��λ��ȡ������
*******************************************************************************
*/
#define UTIL_SPI_3W_ReadUint8()  UTIL_SPI_3W_ReadWriteUint8(0xFF)


/*!
*******************************************************************************
@brief  SPI 3��д��NByte
@param  const unsigned char *data  SPIд�����ݵ���ʼ��ַ
@param  unsigned char size         SPIд�����ݳ���
@return void               ��
*******************************************************************************
*/
void UTIL_SPI_3W_WriteNByte(const unsigned char *data,unsigned char size);

/*!
*******************************************************************************
@brief  SPI 3�߶�ȡNByte
@param  unsigned char *data      SPI��ȡ���ݴ洢��ʼ��ַ
@param  unsigned char size       SPI��ȡ���ݳ���
@return void             ��
*******************************************************************************
*/
void UTIL_SPI_3W_ReadNByte(unsigned char *data,unsigned char size);

/*!
 *******************************************************************************
 @brief  SPI��λ�������ǰ��CS�½��أ�����CS�½��غ������Ҫ�ĵȴ�����
 @param  void ��
 @return void ��
 *******************************************************************************
 */
void UTIL_SPI_CS_Begin(void);

/*!
 *******************************************************************************
 @brief  SPI��λ��������CS�����أ�����CS������ǰ�������Ҫ�ĵȴ�����
 @param  void ��
 @return void ��
 *******************************************************************************
 */
void UTIL_SPI_CS_End(void);

/*!
 *******************************************************************************
 @brief  SPI 3����λ�������ȡһ���ֽڣ�����CS����
 @param  unsigned char data  SPI��λ���������
 @return unsigned char ����SPI��λ��ȡ������
 *******************************************************************************
 */
unsigned char UTIL_SPI_4W_ReadWriteUint8(unsigned char data);


/*!
*******************************************************************************
@brief  SPI 4��д��NByte
@param  const unsigned char *data  SPIд�����ݵ���ʼ��ַ
@param  unsigned char size         SPIд�����ݳ���
@return void               ��
*******************************************************************************
*/
void UTIL_SPI_4W_WriteNByte(const unsigned char *data,unsigned char size);


/*!
*******************************************************************************
@brief  SPI 4�߶�ȡNByte
@param  unsigned char *data      SPI��ȡ���ݴ洢��ʼ��ַ
@param  unsigned char size       SPI��ȡ���ݳ���
@return void             ��
*******************************************************************************
*/
void UTIL_SPI_4W_ReadNByte(unsigned char *data,unsigned char size);


/*!
*******************************************************************************
@brief  SPI SCLK����
@param  void  ��
@return void  ��
*******************************************************************************
*/
void UTIL_SPI_SCLK_InActive(void);



void  BOARD_CC1121_GPIO_Init(void);
void   RF_RECIVE_DATA_GPIO_Init(void);






#endif
