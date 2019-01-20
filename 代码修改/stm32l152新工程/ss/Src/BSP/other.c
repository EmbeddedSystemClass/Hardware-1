#include "global.h"
#include "other.h"
#include "spi.h"
extern TIM_HandleTypeDef htim2;
unsigned char _menu_tab[]= {0x9e,0xa7,0xc6,0xbc,0xb3,0xf0};
unsigned char _idcl_Tab[]= {0xbf,0xff};    //ID��ʾ��־ ��������
#if (defined Liceng)||(defined Weiyan)||(defined Yali)
unsigned char _flag1_tab[]= {0xfe,0xf7};             //��㴫�����ı�ʾ
#elif (defined Maogan)||(defined Maosuo)
unsigned char _flag2_tab[]= {0xfe,0xbf,0xf7,0xbe};   //�����������ı�ʾ
#endif
unsigned char _data_tab[]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f}; //0123456789.
/**************************************************************
������void NVIC_Config(void)
���ܣ��жϷ������
**************************************************************/
void NVIC_Config(void)
{

    /**************TIM4 ���������ȼ�ʱ�ж�****************/

   HAL_NVIC_SetPriority(TIM4_IRQn, 0x04, 0x01);
   HAL_NVIC_EnableIRQ(TIM4_IRQn);
    /****************TIM2��ͨ��ʱ�жϷ���********************/
   HAL_NVIC_SetPriority(TIM2_IRQn, 0x08, 0x04);
   HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
    /****************PB0��Ӧ��0���жϷ���********************/
   HAL_NVIC_SetPriority(EXTI0_IRQn, 0x0e, 0x0e);
   HAL_NVIC_EnableIRQ(EXTI0_IRQn);	
    /****************PB0��Ӧ��1���жϷ���********************/
   HAL_NVIC_SetPriority(EXTI1_IRQn, 0x0f, 0x0f);
   HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}
/**************************************************************
������void Battery_Monitor_Config (void)
���ܣ���ص�ѹ�������PC1
**************************************************************/
void Battery_Monitor_Config (void)
{


 GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}
/**************************************************************
������uint8_t Battery_Monitor(void)
���ܣ���ص�ѹ��� PC3
**************************************************************/
uint8_t Battery_Monitor(void)
{
    if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1))return 0xCB;   //��ص�ѹ��
    else return 0xCA; //��ص�ѹ����
}
/**************************************************************
������void Power_Control_GPIO(void)
���ܣ� ��������Դ����PC5->3.3V PC2->5V
**************************************************************/
void Power_Control_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
#ifdef Liceng //������������3.3V����
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_RESET);
	
#else
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);
	
#endif
}
/**************************************************************
������void Power_Control(char state)
���ܣ��������������
**************************************************************/
void Power_Control(char state)
{
#ifdef Liceng //������������3.3V����
    if(state)  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_SET);   //�߿�
    else  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_RESET);      //�͹�
#else  //�������������������5V����
    if(state) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_SET);  //�߿�
    else  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);      //�͹�
#endif
}
/**************************************************************
������void Led_Control_GPIO(void)
���ܣ�����ܿ���PC0
**************************************************************/
void Led_Control_GPIO(void)
{

 GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);
	
}
/**************************************************************
������void LED_Control(char state)
���ܣ�����ܵ�Դ����
**************************************************************/
void LED_Control(char state)
{
    if(state)HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET); //�߿�
    else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //�͹�
}
/**************************************************************
������void Timer_Config(uint8_t (*Fuction_Cfg)())
���ܣ���ʱ����ʱ1��
**************************************************************/
void Timer_Config(uint16_t per)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 9999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = per;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
 HAL_TIM_Base_Start(&htim2);


}
/**************************************************************
������void Storage_Data(void)
���ܣ�PB12����595����
**************************************************************/
void Storage_Data(void)
{
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);
}
/**************************************************************
������void Led_Display(uint16_t(*fun)(uint16_t),uint8_t *table,uint8_t i,uint8_t dis_bit)
���ܣ��������ʾ����λ
**************************************************************/
void Led_Display(uint16_t(*fun)(uint16_t),uint8_t *table,uint8_t i,uint8_t dis_bit)
{
    fun(*(table+i));
    fun(dis_bit);
    Storage_Data();
    delay_ms(2);
}
/**************************************************************
������void Led_Display_Data(int channel,int data)
���ܣ��������ʾ����
**************************************************************/
void Led_Display_Data(uint8_t channel,uint16_t data)
{
#if (defined Liceng)||(defined Weiyan)||(defined Yali)
    Led_Display(&SPI2_ReadSend_byte,_flag1_tab,channel,0x08);//��ʾ
#elif (defined Maogan)||(defined Maosuo)
    Led_Display(&SPI2_ReadSend_byte,_flag2_tab,channel,0x08);//��ʾ
#endif
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/100%10,0x04);//��λ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/10%10,0x02); //ʮλ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data%10,0x01); //��λ
#if defined Yali
    Led_Display(&SPI2_ReadSend_byte,_data_tab,10,2);    //С����
#endif
}
/**************************************************************
������void Led_Display_XX(uint8_t sensor_id)
���ܣ��������ʾ������ID/NUM
**************************************************************/
void Led_Display_XX(uint8_t sensor_id)
{
    Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,0,0x08);
    Led_Display(&SPI2_ReadSend_byte,_data_tab,sensor_id/10%10,0x04);//ʮλ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,sensor_id%10,0x02);//��λ
    Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,0,0x01);
}
/**************************************************************
������void Led_Display_Set(char type,uint8_t sensor_id)
���ܣ��������ʾ���������
**************************************************************/
void Led_Display_Set(uint8_t i,uint8_t data)
{
    Led_Display(&SPI2_ReadSend_byte,_menu_tab,i,0x08);
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data/10%10,0x04);//ʮλ
    Led_Display(&SPI2_ReadSend_byte,_data_tab,data%10,0x02);//��λ
    Led_Display(&SPI2_ReadSend_byte,_menu_tab,i+3,0x01);
}
/**************************************************************
������void Led_Display_Set(uint8_t sensor_id)
���ܣ��������ʾ���������
**************************************************************/
void LED_Display_Clear(void)
{
    uint8_t i;
    for(i=0x08; i>=0x01; i=i>>1) //�������λ
    {
        Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,1,i);
    }
    for(i=4; i>=1; i--)       //���С����
    {
        Led_Display(&SPI2_ReadSend_byte,_idcl_Tab,1,i);
    }
}

