/*===========================================================================
* 网址 ：http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* 作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司                 *
* 邮件 ：yihe_liyong@126.com                                                *
* 电话 ：18615799380                                                        *
============================================================================*/
#include "bsp.h"
#include "uart1.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_tim1.h"
#include "uart_hal.h"

#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DR_ADDRESS       (uint16_t)0x5231  /* USART1 Data register Address */

uart_stru uart;
float ADC_RATIO= ((uint16_t) 733); /*ADC_RATIO = ( 3 * 1000 * 1000)/4095 */
extern unsigned int rtctime;//10:9s 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ADCdata = 0;
unsigned char delay_ms(uint32_t num)//不是很精确
{
  u16 i = 0;
  while(num--)
  {
    for (i=0; i<2654; i++);
  }
  return 0;
}




void GPIO_Initial(void)
{ 
  //GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT ); 
  
  // battery quantity ios
  GPIO_Init( BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL1_PIN|
            BATEERY_QUANTITY_LEVEL2_PIN|BATEERY_QUANTITY_LEVEL3_PIN,
            GPIO_Mode_Out_PP_High_Slow );  
  // module status io
  GPIO_Init( MODULE_STATUS_PORT, MODULE_STATUS_PIN, GPIO_Mode_Out_PP_High_Slow );
  //elec lock ios
  GPIO_Init( LOCK_CTRL_PORT, LOCK_CTRL_PIN, GPIO_Mode_Out_PP_High_Slow );  
  GPIO_Init( LOCK_FB_PORT, LOCK_FB_PIN, GPIO_Mode_In_FL_No_IT );   
  //gsm power and gps enbale ios
  GPIO_Init( GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, GPIO_Mode_Out_PP_Low_Slow ); 
  GPIO_Init( GSM_PWR_PORT, GSM_PWR_PIN, GPIO_Mode_Out_PP_Low_Slow ); 
  
}
/*******************************************************************
function: ctronl  gnss enable or disable
@param    newstate:
          this param can be one of the following values:
          ON:           enable gnss
          OFF:          disable gnss
********************************************************************/
void gnss_state(unsigned char newstate)
{
  if(newstate == ON)
  {
    GPIO_WriteBit(GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, SET);   
  }
  else
  {
    GPIO_WriteBit(GNSS_ENABLE_PORT, GNSS_ENABLE_PIN, RESET);  
  }
}
/*******************************************************************
function: ctronl  gsm module power on or off
@param    newstate:
          this param can be one of the following values:
          ON:           module power on
          OFF:          module power off
********************************************************************/
void gsm_power_state(unsigned char newstate)
{
  if(newstate == ON)
  {
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, SET);
    delay_ms(500);
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, RESET);      
  }
  else
  {
    GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, SET);
    delay_ms(500);
     GPIO_WriteBit(GSM_PWR_PORT, GSM_PWR_PIN, RESET);  
  }
}
void lock_state(unsigned char newstate)
{
  if(newstate == ON)
  {
    GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, SET);
    while(GPIO_ReadInputDataBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN)==OFF)
    {
      
      if(delay_ms(500)==0)
      {
        GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, RESET);  
        break;
      }
    }
    GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, RESET);  
    
  }
  else
  {
    GPIO_WriteBit(LOCK_CTRL_PORT, LOCK_CTRL_PIN, RESET);  
  }
}


void RTC_Config(uint16_t time,unsigned char flag) 
{
  if(flag == ON)
  {
    RTC_DeInit(); //初始化默认状态 
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟 
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1); //选择RTC时钟源LSI 38K、2=19K
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
    
    RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE); //允许RTC时钟
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    
    RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值	
    RTC_ITConfig(RTC_IT_WUT, ENABLE); //开启中断
    RTC_ClearITPendingBit(RTC_IT_WUT);
    RTC_WakeUpCmd(ENABLE);
    
  }
  else
  {
    RTC_WakeUpCmd(DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //允许RTC时钟
    //RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_ITConfig(RTC_IT_WUT, DISABLE); //开启中断
    RTC_ClearITPendingBit(RTC_IT_WUT);
    // RTC_SetWakeUpCounter(time); //设置RTC Weakup计算器初值
    
    
  }
  
}



extern u8 CurrentMode ;
void EnterStopMode(void) 
{
  disableInterrupts(); 
  GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOA,  GPIO_Pin_2, GPIO_Mode_In_FL_No_IT ); 
  GPIO_Init( GPIOA,  GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast );
  GPIO_Init( GPIOA,  GPIO_Pin_4, GPIO_Mode_Out_OD_Low_Fast );   
  GPIO_Init( GPIOA,  GPIO_Pin_5, GPIO_Mode_In_PU_No_IT ); 
  GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT );
  GPIO_Init( GPIOC, GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4, GPIO_Mode_In_PU_No_IT );
  
  
}


static void DMA_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
  
  /* Deinitialize DMA channels */
  DMA_GlobalDeInit();
  
  DMA_DeInit(DMA1_Channel1);
  DMA_DeInit(DMA1_Channel2);
  
  /* DMA channel Rx of USART Configuration */
  DMA_Init(USART_DMA_CHANNEL_RX, (uint16_t)uart.rxbuffer , (uint16_t)USART_DR_ADDRESS,
           BUFFERSIZE, DMA_DIR_PeripheralToMemory, DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc, DMA_Priority_Low, DMA_MemoryDataSize_Byte);
  

  /* Enable the USART Tx/Rx DMA requests */
  USART_DMACmd(USART1, USART_DMAReq_RX, ENABLE);
  /* Global DMA Enable */
  DMA_GlobalCmd(ENABLE);
  
  /* Enable the USART Tx DMA channel */
  // DMA_Cmd(USART_DMA_CHANNEL_TX, ENABLE);
  /* Enable the USART Rx DMA channel */
  //DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);         
}
void DMA_START_RX(void)
{   
  DMA_ClearFlag(DMA1_FLAG_TC2);
  DMA_ClearFlag(DMA1_FLAG_HT2);
  DMA_Cmd(USART_DMA_CHANNEL_RX, DISABLE);
  DMA_SetCurrDataCounter(USART_DMA_CHANNEL_RX, BUFFERSIZE); 
  DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);
}


void HardwareInit()
{
  disableInterrupts();
  SystemClock_Init();     // 系统时钟初始化
  GPIO_Initial(); 
  lock_state(OFF);
  
  Uart1_Init(9600);// 初始化GPIO
  DMA_Config();
  LED_Init(LEVEL_ALL_LED,OFF);
  enableInterrupts();
  gnss_state(OFF);
  gsm_power_state(ON);
    
}

/*******************************************************************
function: ctronl leds
@param    num:specify the led num
@param    newstate:specify the led on or off           
********************************************************************/
void LED_Init(unsigned char num,unsigned char newstate)
{

  switch(num)
  {
  case LEVEL1_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL1_PIN, RESET);     
      }
      else
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL1_PIN, SET);
      }
    }break;
  case LEVEL2_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL2_PIN, RESET);     
      }
      else
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL2_PIN, SET);
      }
    }break;
    
  case LEVEL3_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL3_PIN, RESET);     
      }
      else
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT, BATEERY_QUANTITY_LEVEL3_PIN, SET);
      }
    }break;    
  case STATUS_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, RESET);     
      }
      else
      {
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, SET);
      }
    }break; 

  case LEVEL_ALL_LED:
    {
      if(newstate == ON)
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL1_PIN, RESET);
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL2_PIN, RESET);        
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL3_PIN, RESET);       
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, RESET);
      }
      else
      {
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL1_PIN, SET);
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL2_PIN, SET);        
        GPIO_WriteBit(BATEERY_QUANTITY_PORT,BATEERY_QUANTITY_LEVEL3_PIN, SET);
        GPIO_WriteBit(MODULE_STATUS_PORT, MODULE_STATUS_PIN, SET);
      }
    }break;     
  }
  
}





