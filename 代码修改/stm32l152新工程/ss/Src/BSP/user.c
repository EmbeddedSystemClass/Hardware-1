#include "global.h"
#include "adc.h"
#include "other.h"
#include "CC1110.h"
#include "spi.h"
#include "infrared.h"
#include "rtc.h"
UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;
extern RTC_SetTypeDef RTC_SetStructure;
//RCC_ClocksTypeDef RCC_Clocks;
static uint8_t  fac_us=0;
static uint16_t fac_ms=0;
extern WPStatus PD_WAKEUP_Flag;
/**************************************************************
������void Bsp_Config (void)
���ܣ�Ӳ����ʼ��
**************************************************************/
void RCC_Config(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

void ControlGPIO_Initt(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
//	    EXTI_InitTypeDef EXTI_InitStructure;
 __HAL_RCC_GPIOC_CLK_ENABLE();		
  /*Configure GPIO pins : PC7 PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void ControlGPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pins : PB7 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/**************************************************************
������void Wake_Config(void)
���ܣ�������������
**************************************************************/
void Wake_Config(void)
{
	 
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);	
  	RTC_SetStructure.year =0x19;
	RTC_SetStructure.month = 0x01;
	RTC_SetStructure.date = 0x02;
	RTC_SetStructure.week = 0x03;
	RTC_SetStructure.hour = 0x10;
	RTC_SetStructure.minute =0x12;
	RTC_SetStructure.second =0x04;	
	RTC_SetStructure.wakeup_time = 0x05;//����Ϊ��λ����ǰΪ5s
//__HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();	//�����־������ڶ����Ժ��޷���������
//HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, RTC_SetStructure.wakeup_time, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);		
	
}
/**************************************************************
������void EnterLowPower(void)
���ܣ�����͹���ģʽ
**************************************************************/
void EnterLowPower(void)
{
	TIM_HandleTypeDef htim2;
  TIM_HandleTypeDef htim3;
  extern SPI_HandleTypeDef hspi1;
  ADC_HandleTypeDef hadc1;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_PWREx_EnableFastWakeUp();
	__HAL_RCC_PWR_CLK_ENABLE();
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//Ƭ�ڵ�Դ�����ʱ��---ʹ�� 
	HAL_PWREx_EnableUltraLowPower();//���õ͵�Դʹ��
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  //HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|
  	                           GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|
  	                           GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

 __HAL_RCC_GPIOA_CLK_DISABLE();
 __HAL_RCC_GPIOC_CLK_DISABLE();
 __HAL_RCC_GPIOD_CLK_DISABLE();

__HAL_TIM_DISABLE(&htim2) ;
__HAL_TIM_DISABLE(&htim3) ;

   //  TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));     //�رն�ʱ������
  //  TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));     //�رն�ʱ����
   __HAL_SPI_DISABLE(&hspi1);
 __HAL_UART_DISABLE(&huart2);  
 HAL_ADC_Stop(&hadc1);
__HAL_RCC_ADC1_CLK_DISABLE();
 __HAL_RCC_SPI1_CLK_DISABLE();  
 __HAL_RCC_USART1_CLK_DISABLE();  
 __HAL_RCC_TIM2_CLK_DISABLE(); 
 __HAL_RCC_TIM3_CLK_DISABLE();  
  __HAL_RCC_SPI2_CLK_DISABLE(); 
   __HAL_RCC_USART3_CLK_DISABLE(); 
     Wake_Config();	
    //PWR_ClearFlag(PWR_FLAG_WU);
HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);	
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);				//����Ҫ�ӣ�������1.4uA�ĵ���
__HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();	//�����־������ڶ����Ժ��޷���������
}
/**************************************************************
������WPStatus PD_WAKEUP_status (void)
���ܣ��������Ѽ�� ��PB1״̬ ����ߵ�ƽ��⵽����*
**************************************************************/
PDStatus PD_WAKEUP_status (void)
{
    PDStatus status = Low;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))status=High;
    else status=Low;
    return status;
}
/**************************************************************
������void delay_init(void)
���ܣ���ʱ
**************************************************************/
void delay_init(void)
{
	HAL_Init();
}
/**************************************************************
������void delay_us(uint32_t nus)
���ܣ���ʱ΢��
**************************************************************/
void delay_us(uint32_t nus)
{
    HAL_Delay(1);
}
/**************************************************************
������void delay_ms(uint16_t nms)
���ܣ���ʱ���� nms<=0xffffff*8*1000/SYSCLK ��32M������,nms<=4148
**************************************************************/
void delay_ms(uint16_t nms)
{
    HAL_Delay(nms);
}

/**************************************************************
������void Bsp_Config (uint8_t (*Fuction_Cfg)())
���ܣ�Ӳ����ʼ��
**************************************************************/
void Bsp_Config (void)
{
    RCC_Config();

   // RCC_GetClocksFreq(&RCC_Clocks);
    //SysTick_Config(RCC_Clocks.HCLK_Frequency /100);
    Power_Control_GPIO();     //̽ͷ����
    ADC_Config();             //ADC��ʼ��
    Battery_Monitor_Config(); //��ؼ��
    delay_init();             //��ʱ

}

