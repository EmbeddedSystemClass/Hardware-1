

#include "xz_spi.h"
#include "xz_cc1121_init.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#define spi_interface
extern SPI_HandleTypeDef hspi1;
//��ͬ�����µ�SCLK����
#if UTIL_SPI_CPOL==1  //
	#define UTIL_SPI_SCLK_ACTIVE()    UTIL_SPI_SCLK_LOW()
	#define UTIL_SPI_SCLK_INACTIVE()  UTIL_SPI_SCLK_HIGH()
#else
	#define UTIL_SPI_SCLK_ACTIVE()    UTIL_SPI_SCLK_HIGH()
	#define UTIL_SPI_SCLK_INACTIVE()  UTIL_SPI_SCLK_LOW()
#endif

/*!
*******************************************************************************
@brief  SPI 4��IO�ڳ�ʼ��������CS,MISO,MOSI,SCLK
@param  void  ��
@return void  ��
*******************************************************************************
*/
void UTIL_SPI_4W_Init(void)
{
	BOARD_CC1121_GPIO_Init();
}

/*!
*******************************************************************************
@brief  SPI 3����λ�������ȡһ���ֽڣ�������CS����
@param  unsigned char  data  SPI��λ���������
@return unsigned char  ����SPI��λ��ȡ������
*******************************************************************************
*/
#if UTIL_SPI_MOST_BIT_FIRST==1
unsigned char  UTIL_SPI_3W_ReadWriteUint8(unsigned char  data)   //��λ˳��Ϊ��BIT�����Ƴ�
{
	unsigned char i;  //����ʹ��uint8_least,�������ڲ�ͬ��ƽ̨�£������ı������ͽ���ѭ������
	unsigned char  temp;

	temp = 0;

	//CHPAΪ0��SCLK��0�����أ���CS�½��أ���SPI�ӻ�������ʱ���Ƴ�����λ����ʱ�������
	//SCLK��1������ʱ����������λ����ʱ������ڣ�SCLK�ڶ��α���
#if UTIL_SPI_CPHA!=1
	#ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
		for(i = 0; i < 8; i ++)
		{
			//------------------------------------------------------------------------------
			if(data & 0x80)              //SCLK��0������,�Ƴ�����λ
			{
				UTIL_SPI_MOSI_HIGH();
			}
			else
			{
				UTIL_SPI_MOSI_LOW();
			}
			//------------------------------------------------------------------------------

			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			temp <<= 1;                            //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------



			//------------------------------------------------------------------------------
			//SCLK��1������ʱ����������λ
			UTIL_SPI_SCLK_ACTIVE();
			if(UTIL_SPI_MISO_READ())
			{
				temp |= 1U<<0;
			}
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			data <<= 1;                           //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//SCLK��2�������������λ���������λ����һ��ѭ����ʼ��
			UTIL_SPI_SCLK_INACTIVE();
			//------------------------------------------------------------------------------
		}

	#endif
#else
	#ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
	//CHPAΪ1������ʱ������ڣ�SCLK��1������ʱ���Ƴ�����λ
	//��ʱ������ڣ�SCLK��2������ʱ����������λ
		for(i = 0; i < 8; i ++)
		{

			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			temp <<= 1;                  //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------

			//------------------------------------------------------------------------------
			if(data & 0x80)              //SCLK��1������,�Ƴ�����λ
			{
				UTIL_SPI_MOSI_HIGH();
			}
			else
			{
				UTIL_SPI_MOSI_LOW();
			}
			UTIL_SPI_SCLK_ACTIVE();    //SCLK��1������
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			data <<= 1;                  //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//SCLK��2������ʱ����������λ
			if(UTIL_SPI_MISO_READ())
			{
				temp |= 1U<<0;
			}
			UTIL_SPI_SCLK_INACTIVE();   //SCLK��2������
			//------------------------------------------------------------------------------
		}

	#endif
#endif

	return temp;
}

#else

unsigned char  UTIL_SPI_3W_ReadWriteUint8(unsigned char  data)   //��λ˳��Ϊ��BIT�����Ƴ�
{
	uint8_least i;  //����ʹ��uint8_least,�������ڲ�ͬ��ƽ̨�£������ı������ͽ���ѭ������
	unsigned char  temp;

	temp = 0;

	//CHPAΪ0��SCLK��0�����أ���CS�½��أ���SPI�ӻ�������ʱ���Ƴ�����λ����ʱ�������
	//SCLK��1������ʱ����������λ����ʱ������ڣ�SCLK�ڶ��α���
#if UTIL_SPI_CPHA!=1
	#ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
		for(i = 0; i < 8; i ++)
		{
			//------------------------------------------------------------------------------
			if(data & 0x01)              //SCLK��0������,�Ƴ�����λ
			{
				UTIL_SPI_MOSI_HIGH();
			}
			else
			{
				UTIL_SPI_MOSI_LOW();
			}
			//------------------------------------------------------------------------------



			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			temp >= 1;                            //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------



			//------------------------------------------------------------------------------
			//SCLK��1������ʱ����������λ
			UTIL_SPI_SCLK_ACTIVE();
			if(UTIL_SPI_MISO_READ())
			{
				temp |= 1U<<7;
			}
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			data >>= 1;                           //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//SCLK��2�������������λ���������λ����һ��ѭ����ʼ��
			UTIL_SPI_SCLK_INACTIVE();
			//------------------------------------------------------------------------------
		}
    #endif
#else
    #ifdef spi_interface
		HAL_SPI_Transmit(&hspi1, &data,1, 2);
//		HAL_SPI_Receive(&hspi1, &data, 1, 10);
	#else
	//CHPAΪ1������ʱ������ڣ�SCLK��1������ʱ���Ƴ�����λ
	//��ʱ������ڣ�SCLK��2������ʱ����������λ
		for(i = 0; i < 8; i ++)
		{

			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			temp >>= 1;                  //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------

			//------------------------------------------------------------------------------
			if(data & 0x01)              //SCLK��1������,�Ƴ�����λ
			{
				UTIL_SPI_MOSI_HIGH();
			}
			else
			{
				UTIL_SPI_MOSI_LOW();
			}
			UTIL_SPI_SCLK_ACTIVE();    //SCLK��1������
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//��ʱ�������
			UTIL_SPI_SCLK_DELAY() ;
			data >>= 1;                  //������λ���㣬�ֱ�������������ڵȴ�����ƽ��SCLK�ߵ͵�ƽʱ��
			//------------------------------------------------------------------------------


			//------------------------------------------------------------------------------
			//SCLK��2������ʱ����������λ
			if(UTIL_SPI_MISO_READ())
			{
				temp |= 1U<<7;
			}
			UTIL_SPI_SCLK_INACTIVE();   //SCLK��2������
			//------------------------------------------------------------------------------
		}
    #endif
#endif

	return temp;
}
#endif


/*!
*******************************************************************************
@brief  SPI 3��д��NByte
@param  const unsigned char  *data  SPIд�����ݵ���ʼ��ַ
@param  unsigned char  size         SPIд�����ݳ���
@return void               ��
*******************************************************************************
*/
void UTIL_SPI_3W_WriteNByte(const unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
	{
		UTIL_SPI_3W_ReadWriteUint8(data[i]);
	}
}


/*!
*******************************************************************************
@brief  SPI 3�߶�ȡNByte
@param  unsigned char  *data      SPI��ȡ���ݴ洢��ʼ��ַ
@param  unsigned char  size       SPI��ȡ���ݳ���
@return void             ��
*******************************************************************************
*/
void UTIL_SPI_3W_ReadNByte(unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
	{
		data[i]=UTIL_SPI_3W_ReadWriteUint8(0xFF);
	}
}



/*!
*******************************************************************************
@brief  SPI��λ�������ǰ��CS�½��أ�����CS�½��غ������Ҫ�ĵȴ�����
@param  void ��
@return void ��
*******************************************************************************
*/
void UTIL_SPI_CS_Begin(void)
{

	UTIL_SPI_CS_PREV_BEGIN();

	UTIL_SPI_CS_LOW();

	UTIL_SPI_CS_POST_FALL_DELAY();
}

/*!
*******************************************************************************
@brief  SPI��λ��������CS�����أ�����CS������ǰ�������Ҫ�ĵȴ�����
@param  void ��
@return void ��
*******************************************************************************
*/
void UTIL_SPI_CS_End(void)
{
	UTIL_SPI_CS_PREV_RISE_DELAY();

	UTIL_SPI_CS_HIGH();

	UTIL_SPI_CS_POST_RISE_DELAY();
	UTIL_SPI_CS_POST_END();
}


/*!
*******************************************************************************
@brief  SPI 3����λ�������ȡһ���ֽڣ�����CS����
@param  unsigned char  data  SPI��λ���������
@return unsigned char  ����SPI��λ��ȡ������
*******************************************************************************
*/
unsigned char  UTIL_SPI_4W_ReadWriteUint8(unsigned char  data)
{
	unsigned char  temp;
	UTIL_SPI_CS_PREV_BEGIN();
	//UTIL_SPI_CS_LOW();
	UTIL_SPI_CS_POST_FALL_DELAY();

	temp=UTIL_SPI_3W_ReadWriteUint8(data);

	UTIL_SPI_CS_PREV_RISE_DELAY();
	//UTIL_SPI_CS_HIGH();
	UTIL_SPI_CS_POST_RISE_DELAY();
	UTIL_SPI_CS_POST_END();

	return temp;
}


/*!
*******************************************************************************
@brief  SPI 4��д��NByte
@param  const unsigned char  *data  SPIд�����ݵ���ʼ��ַ
@param  unsigned char  size         SPIд�����ݳ���
@return void               ��
*******************************************************************************
*/
void UTIL_SPI_4W_WriteNByte(const unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
	{
		UTIL_SPI_4W_ReadWriteUint8(data[i]);
	}
}


/*!
*******************************************************************************
@brief  SPI 4�߶�ȡNByte
@param  unsigned char  *data      SPI��ȡ���ݴ洢��ʼ��ַ
@param  unsigned char  size       SPI��ȡ���ݳ���
@return void             ��
*******************************************************************************
*/
void UTIL_SPI_4W_ReadNByte(unsigned char  *data,unsigned char  size)
{
	unsigned char i;
	for( i=0; i<size; i++)
	{
		data[i]=UTIL_SPI_4W_ReadWriteUint8(0xFF);
	}
}


/*!
*******************************************************************************
@brief  SPI SCLK����
@param  void  ��
@return void  ��
*******************************************************************************
*/
void UTIL_SPI_SCLK_InActive(void)
{
	UTIL_SPI_SCLK_INACTIVE();
}

/*!
*******************************************************************************
@brief  BOARD_CC1121_GPIO_Init
@param  void  ��
@return void  ��
*******************************************************************************
*/


void  BOARD_CC1121_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();


	GPIO_InitStruct.Pin =  RF_SPI_NSS|RF_SPI_SCK|RF_SPI_MOSI;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,RF_SPI_NSS|RF_SPI_MOSI,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,RF_SPI_SCK,GPIO_PIN_RESET);



	GPIO_InitStruct.Pin = RF_SPI_MISO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //�������룬����
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RF_SPI_RST;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //�������룬����
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	GPIO_InitStruct.Pin = RF_SPI_RST;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //�������룬����
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,RF_SPI_RST,GPIO_PIN_SET);

	GPIO_InitStruct.Pin = RF_DATA_IRQ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //�������룬����
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);


}


void   RF_RECIVE_DATA_GPIO_Init(void)
{


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_2)
	{
		//nIRQ=0;
		RF_RECEIVE_DATA.rf_receive_end_flag=true;
//		if(tx_rx_switch==0)
//		{
//			rf_rx_FLAG=1;
//		}
		//rf_rx_irq();
	}
}



