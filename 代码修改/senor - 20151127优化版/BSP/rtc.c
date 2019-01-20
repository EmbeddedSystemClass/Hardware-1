#include "stm32l1xx.h"
#include "rtc.h"
#include "oled.h"
RTC_InitTypeDef   RTC_InitStructure;;
 unsigned char rtc_wakeup_flag;
/******************************************************************************
��������RTC_GetTimeDate
�������ܣ���ȡrtcʱ�������
******************************************************************************/

RTC_TimeDateTypeDef RTC_GetTimeDate()
{
	RTC_TimeTypeDef  RTC_TimeStruct;
	RTC_DateTypeDef  RTC_DateStruct;
	RTC_TimeDateTypeDef  RTC_TimeDateStructure;
 
	RTC_GetTime(RTC_Format_BCD,  &RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BCD,  &RTC_DateStruct);	


	RTC_TimeDateStructure.date = RTC_DateStruct.RTC_Date;
	RTC_TimeDateStructure.month= RTC_DateStruct.RTC_Month;
	RTC_TimeDateStructure.year = RTC_DateStruct.RTC_Year;

	RTC_TimeDateStructure.hour =RTC_TimeStruct.RTC_Hours;
	RTC_TimeDateStructure.minute = RTC_TimeStruct.RTC_Minutes;
	RTC_TimeDateStructure.second  = RTC_TimeStruct.RTC_Seconds;	
	return RTC_TimeDateStructure;
}
void SYS_RTCInit(RTC_SetTypeDef RTC_SetStructure)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  /* USER CODE BEGIN RTC_Init 0 */
  /* Enable the PWR clock */   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);    /* Allow access to RTC */    
  PWR_RTCAccessCmd(ENABLE);   
  /* Enable the LSE OSC */    
  RCC_LSICmd(ENABLE)  ; 
  /* Wait till LSE is ready */     
    
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)   
  {        
	;  //��ʼ��ʱ��ʧ��,���������⣬32768����������ʱ��1-5S     
  }   
  /* Select the RTC Clock Source */    
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);   
  /* Enable the RTC Clock */    
  RCC_RTCCLKCmd(ENABLE);   
  /* Wait for RTC APB registers synchronisation */    
  RTC_WaitForSynchro();   
  /* Configure the RTC data register and RTC prescaler */    

  /* USER CODE END RTC_Init 0 */



  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  RTC_InitStructure.RTC_AsynchPrediv = 127;
  RTC_InitStructure.RTC_SynchPrediv	=  255;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /**Initialize RTC and set the Time and Date 
  */
  sTime.RTC_Hours= RTC_SetStructure.hour;
  sTime.RTC_Minutes= RTC_SetStructure.minute;
  sTime.RTC_Seconds= RTC_SetStructure.second;
  sTime.RTC_H12=RTC_SetStructure.am_pm;
  
  RTC_SetTime(RTC_Format_BCD, &sTime);
  
  sDate.RTC_WeekDay= RTC_SetStructure.week ;
  sDate.RTC_Month= RTC_SetStructure.month;
  sDate.RTC_Date= RTC_SetStructure.date;
  sDate.RTC_Year= RTC_SetStructure.year;
  RTC_SetDate(RTC_Format_BCD, &sDate);
 // RTC_ITConfig(RTC_IT_WUT, ENABLE);
}
void RtcWakeUpConfig(void)
{    
	RTC_WakeUpCmd(DISABLE);    
	RTC_ClearFlag(RTC_FLAG_WUTF);   
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);     
	//ѡ��ʱ��Ϊ�ⲿ32.768KHz 8��Ƶ    
	RTC_SetWakeUpCounter(20);         //���500ms    
	//RTC_ClearITPendingBit(RTC_IT_WUT);    
	//RTC_ITConfig(RTC_IT_WUT,ENABLE);    //��Ҫʹ���жϣ�����Ҫ�жϺ���
	RTC_IRQConfig();
	RTC_WakeUpCmd(ENABLE);
}

void RTC_IRQConfig(void)
{    
	NVIC_InitTypeDef NVIC_InitStructure;     
	EXTI_InitTypeDef EXTI_InitStructure;  
	/* EXTI configuration *******************************************************/    
	EXTI_ClearITPendingBit(EXTI_Line20);   
	EXTI_InitStructure.EXTI_Line = EXTI_Line20;    
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;    
	EXTI_Init(&EXTI_InitStructure);    
	/* Enable the RTC Wakeup Interrupt */    
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);
       RTC_ClearITPendingBit(RTC_IT_WUT);    
	RTC_ITConfig(RTC_IT_WUT,ENABLE);    //��Ҫʹ���жϣ�����Ҫ�жϺ���
		
}

void RTC_WKUP_IRQHandler(void)
{
 
	
	
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET) 
    {
			  rtc_wakeup_flag=1;
        EXTI_ClearITPendingBit(EXTI_Line20); 
       // PWR_RTCAccessCmd(ENABLE);//���֮ǰ��ֹ�ˣ�Ҫ���´�
        RTC_ClearITPendingBit(RTC_IT_WUT);
        //PWR_RTCAccessCmd(DISABLE);//�����ٹ���
       // RTC_ClearITPendingBit(RTC_IT_WUT);   //����жϱ�־
    }

}
