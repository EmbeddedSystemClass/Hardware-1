

#include "xz_spi.h"
#include "xz_cc1121_init.h"

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
@param  uint8 data  SPI��λ���������
@return uint8 ����SPI��λ��ȡ������
*******************************************************************************
*/
#if UTIL_SPI_MOST_BIT_FIRST==1
OPTI_S uint8 UTIL_SPI_3W_ReadWriteUint8(uint8 data)   //��λ˳��Ϊ��BIT�����Ƴ�
{
  uint8_least i;  //����ʹ��uint8_least,�������ڲ�ͬ��ƽ̨�£������ı������ͽ���ѭ������
  uint8 temp;
  
  temp = 0;
  
  //CHPAΪ0��SCLK��0�����أ���CS�½��أ���SPI�ӻ�������ʱ���Ƴ�����λ����ʱ������� 
  //SCLK��1������ʱ����������λ����ʱ������ڣ�SCLK�ڶ��α���
#if UTIL_SPI_CPHA!=1         
  for(i = 0;i < 8;i ++)
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
  
#else 
  
  //CHPAΪ1������ʱ������ڣ�SCLK��1������ʱ���Ƴ�����λ  
  //��ʱ������ڣ�SCLK��2������ʱ����������λ 
  for(i = 0;i < 8;i ++)
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
  
  return temp;
}

#else

OPTI_S uint8 UTIL_SPI_3W_ReadWriteUint8(uint8 data)   //��λ˳��Ϊ��BIT�����Ƴ�
{
  uint8_least i;  //����ʹ��uint8_least,�������ڲ�ͬ��ƽ̨�£������ı������ͽ���ѭ������
  uint8 temp;
  
  temp = 0;
  
    //CHPAΪ0��SCLK��0�����أ���CS�½��أ���SPI�ӻ�������ʱ���Ƴ�����λ����ʱ������� 
  //SCLK��1������ʱ����������λ����ʱ������ڣ�SCLK�ڶ��α���
#if UTIL_SPI_CPHA!=1         
  for(i = 0;i < 8;i ++)
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
  
#else 
  
  //CHPAΪ1������ʱ������ڣ�SCLK��1������ʱ���Ƴ�����λ  
  //��ʱ������ڣ�SCLK��2������ʱ����������λ 
  for(i = 0;i < 8;i ++)
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
  
  return temp;
}
#endif


/*!
*******************************************************************************
@brief  SPI 3��д��NByte
@param  const uint8 *data  SPIд�����ݵ���ʼ��ַ
@param  uint8 size         SPIд�����ݳ���
@return void               ��
*******************************************************************************
*/
OPTI_S void UTIL_SPI_3W_WriteNByte(const uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
   {
     UTIL_SPI_3W_ReadWriteUint8(data[i]);
   }
}


/*!
*******************************************************************************
@brief  SPI 3�߶�ȡNByte
@param  uint8 *data      SPI��ȡ���ݴ洢��ʼ��ַ
@param  uint8 size       SPI��ȡ���ݳ���
@return void             ��
*******************************************************************************
*/
OPTI_S void UTIL_SPI_3W_ReadNByte(uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
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
OPTI_S void UTIL_SPI_CS_Begin(void)
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
OPTI_S void UTIL_SPI_CS_End(void)
{
  UTIL_SPI_CS_PREV_RISE_DELAY();
  
  UTIL_SPI_CS_HIGH();
  
  UTIL_SPI_CS_POST_RISE_DELAY();
  UTIL_SPI_CS_POST_END();
}


/*!
*******************************************************************************
@brief  SPI 3����λ�������ȡһ���ֽڣ�����CS����
@param  uint8 data  SPI��λ���������
@return uint8 ����SPI��λ��ȡ������
*******************************************************************************
*/
OPTI_S uint8 UTIL_SPI_4W_ReadWriteUint8(uint8 data)
{
  uint8 temp;
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
@param  const uint8 *data  SPIд�����ݵ���ʼ��ַ
@param  uint8 size         SPIд�����ݳ���
@return void               ��
*******************************************************************************
*/
OPTI_S void UTIL_SPI_4W_WriteNByte(const uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
   {
     UTIL_SPI_4W_ReadWriteUint8(data[i]);
   }
}


/*!
*******************************************************************************
@brief  SPI 4�߶�ȡNByte
@param  uint8 *data      SPI��ȡ���ݴ洢��ʼ��ַ
@param  uint8 size       SPI��ȡ���ݳ���
@return void             ��
*******************************************************************************
*/
OPTI_S void UTIL_SPI_4W_ReadNByte(uint8 *data,uint8 size)
{
   for(uint8_least i=0;i<size;i++)
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
OPTI_S void UTIL_SPI_SCLK_InActive(void)
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
   RCC_APB2PeriphClockCmd(
                         RCC_APB2Periph_GPIOA                           
                           |RCC_APB2Periph_GPIOC,                               
                              ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = RF_SPI_NSS|RF_SPI_SCK|RF_SPI_MOSI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  
  GPIO_SetBits(GPIOA,RF_SPI_NSS|RF_SPI_MOSI); 
  GPIO_ResetBits(GPIOA,RF_SPI_SCK);
  
  GPIO_InitStructure.GPIO_Pin = RF_SPI_MISO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   

  GPIO_InitStructure.GPIO_Pin = RF_SPI_RST;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  GPIO_SetBits(GPIOC,RF_SPI_RST); 
  
  GPIO_InitStructure.GPIO_Pin = RF_DATA_IRQ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //  ��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
}


void   RF_RECIVE_DATA_GPIO_Init(void)
{
  
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
  
  EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
  EXTI_Init(&EXTI_InitStructure); //��ʼ���ж��߲���
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}



void __ISR_EXTI9_5_IRQn(void)
{
  EXTI_ClearITPendingBit(EXTI_Line5);
  RF_RECEIVE_DATA.rf_receive_end_flag=true;
  asm("nop");
}

