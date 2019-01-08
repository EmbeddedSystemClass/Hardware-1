#include "stm32l1xx_rtc.h"
#include "rtc.h"

 unsigned char rtc_wakeup_flag;
void RTC_ClkConfig(void)
{
    static RTC_InitTypeDef RTC_InitStructure;
    static uint32_t AsynchPrediv = 0, SynchPrediv = 0;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /*!< Allow access to RTC */
    PWR_RTCAccessCmd(ENABLE);

    /*!< ʹ���ⲿ����ʱ�� */
    RCC_LSEConfig(RCC_LSE_ON);

    /*!< �ȴ��ⲿ����ʱ�Ӿ��� */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /*!< ����RTCʱ����Դ */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /*!< ʹ��RTCʱ�� */
    RCC_RTCCLKCmd(ENABLE);

    /*!< Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    //��������
    /* ����RTC���ݼĴ����Լ�ʱ�ӷ�Ƶ */
    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;//��ƵΪ1Hz
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24Сʱ��
    /* ���RTC��ʼ�� */
  //  if (RTC_Init(&RTC_InitStructure) == ERROR)
   // {
      //  printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
   // }
}
void RTC_SetTimeDate(RTC_SetTypeDef RTC_SetStructure)
{

    static RTC_TimeTypeDef RTC_TimeStructure;
    static RTC_DateTypeDef RTC_DateStructure;	
//����ʱ�� 9��15��01��
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = 0x09;
    RTC_TimeStructure.RTC_Minutes = 0x15;
    RTC_TimeStructure.RTC_Seconds = 0x01;
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

     RTC_DateStructure.RTC_Date  = RTC_SetStructure.date;
     RTC_DateStructure.RTC_Month = RTC_SetStructure.month;
     RTC_DateStructure.RTC_WeekDay =  RTC_SetStructure.week;
     RTC_DateStructure.RTC_Year = RTC_SetStructure.year;	 
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);	

}
void RTC_WakeUp_IRQInit(void)
{
     static NVIC_InitTypeDef  NVIC_InitStructure;
    static EXTI_InitTypeDef  EXTI_InitStructure;
    //�����Զ���ʱ����
    /* �ж����� *******************************************************/
    EXTI_ClearITPendingBit(EXTI_Line20);
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*ʹ�� RTC Wakeup Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* RTC �����ж�����: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
 
    /* ʹ�� the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);    /* RTC �����ж�����: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */


    /* ʹ�� the Wakeup Interrupt */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*ʹ�� RTC Wakeup Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* ʹ�� the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE); 
}


void RTC_GetTimeDate(RTC_TimeDateTypeDef RTC_TimeDateStructure)
{
	RTC_TimeTypeDef  RTC_TimeStruct;
	RTC_DateTypeDef  RTC_DateStruct;
	uint32_t RTC_Format;
	
	RTC_Format = RTC_Format_BCD;
	RTC_GetTime( RTC_Format,   &RTC_TimeStruct);
	RTC_GetDate( RTC_Format,   &RTC_DateStruct);

	RTC_TimeDateStructure.date = RTC_DateStruct.RTC_Date;
	RTC_TimeDateStructure.month= RTC_DateStruct.RTC_Month;
	RTC_TimeDateStructure.year = RTC_DateStruct.RTC_Year;

	RTC_TimeDateStructure.hour =RTC_TimeStruct.RTC_Hours;
	RTC_TimeDateStructure.minute = RTC_TimeStruct.RTC_Minutes;
	RTC_TimeDateStructure.second  = RTC_TimeStruct.RTC_Seconds;	
}
void RTC_WakeUp_init(uint16_t wakeup_time)
{
    RTC_ITConfig(RTC_IT_WUT, DISABLE); 
    /* RTC �����ж�����: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_SetWakeUpCounter(wakeup_time);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))
        RTC_ITConfig(RTC_IT_WUT, ENABLE); 
}

void SYS_RTCInit(RTC_SetTypeDef RTC_SetStructure)
{

  RTC_ClkConfig();
  RTC_SetTimeDate(RTC_SetStructure);//����rtcʱ��
  RTC_WakeUp_init(300);//���û���ʱ�䣬��λ��
  RTC_WakeUp_IRQInit();
}
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT))
	{
	       rtc_wakeup_flag = 1;
		RTC_ClearFlag(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line20);
	}
}
