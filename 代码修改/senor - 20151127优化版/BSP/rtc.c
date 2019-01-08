void SYS_RTCInit(void)
{
    static NVIC_InitTypeDef  NVIC_InitStructure;
    static EXTI_InitTypeDef  EXTI_InitStructure;
    static RTC_InitTypeDef RTC_InitStructure;
    static RTC_TimeTypeDef RTC_TimeStructure;
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
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
        printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
    }
    //����ʱ�� 9��15��01��
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = 0x09;
    RTC_TimeStructure.RTC_Minutes = 0x15;
    RTC_TimeStructure.RTC_Seconds = 0x01;
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

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
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* ʹ�� the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WU    RTC_ITConfig(RTC_IT_WUT, EN    /* RTC �����ж�����: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* ʹ�� the Wakeup Interrupt */
    RTC_IT    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*ʹ�� RTC Wakeup Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* RTC �����ж�����: Clock Source: RTCDiv_16, Wakeup Time Base: 4s */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* ʹ�� the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);  
}
void RTC_WKUP_IRQn(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT))
	{
		RTC_ClearFlag(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_	if(RTC_GetITStatus(RTC_IT_WUT))
	{
		RT    RTC_SetWakeUpCounter(0x1FFF);//0x1FFF = 8191; 4s = (8191+1)*(1/(32768/16))

    /* ʹ�� the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

}

void RTC_WKUP_IRQn(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT))
	{
		RTC_ClearFlag(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line20);

	}		EXTI_ClearITPendingBit(		RTC_ClearFlag(RTC_IT_WUT);
		EXTI_Clea    RTC_Format = RTC_Format_BCD; //���ʼ�����ñ���һ�¾Ϳ�
	RTC_GetDate( RTC_Format,  RTC_DateStruct);
	RTC_GetTime( RTC_Format,  RTC_TimeStruct);	RTC_TimeDateStructure.date = RTC_DateStruct.RTC_Date;
	RTC_TimeDateStructure.month = RTC_DateStruct.RTC_Month;
	RTC_TimeDateStructure.year = RTC_DateStruct.RTC_Year;

	RTC_TimeDateStructure.hour = RTC_TimeStruct.RTC_Hours;
	RTC_TimeDateStructure.minute = RTC_TimeStruct.RTC_Minutes;
	RTC_TimeDateStructure.se	RTC_TimeDateStructure.second = RTC_TimeStruct.RTC_Seconds;
}




