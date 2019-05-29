#include "key.h"
#include "main.h"
#include "uartParse.h"


uint16_t UpdateKeyStatus=0;
uint16_t *KeyStaus()//return key status 
{
	return &UpdateKeyStatus;
}

void Gpio_IRQHandler(uint8_t u8Param)//key inttrupt call back function
{
   uint16_t tInchStatus;
	tInchStatus=system_params_get().KEY_H8<<8+system_params_get().KEY_L8;

   switch(u8Param)
   {
		case 0:
		{         

		        if( key_intterupt_trigger(tInchStatus,KEY_DOWN_BIT)==0)//DOWN key down
		        {

					UpdateKeyStatus = UpdateKeyStatus |KEY_DOWN_BIT;	
 				 				
				}
				else 
				{
					if(Gpio_GetIO(KEY_DOWN_PORT, KEY_DOWN_PIN)==0)//key down 
					{
						while(Gpio_GetIO(KEY_DOWN_PORT, KEY_DOWN_PIN))//wait button up
							;
						UpdateKeyStatus = UpdateKeyStatus |KEY_DOWN_BIT;
					}

				}
				
		        if( key_intterupt_trigger(tInchStatus,KEY_BREAK_BIT)==0)//BREAK key down
		        {

					UpdateKeyStatus = UpdateKeyStatus |KEY_BREAK_BIT;					
				}
				else 
				{
					if(Gpio_GetIO(KEY_BREAK_PORT, KEY_BREAK_PIN)==0)//key down 
					{
						while(Gpio_GetIO(KEY_BREAK_PORT, KEY_BREAK_PIN))//wait button up
							;
						UpdateKeyStatus = UpdateKeyStatus |KEY_BREAK_BIT;
					}

				}
				*((uint32_t *)((uint32_t)&M0P_GPIO->P0ICLR + u8Param * 0x40)) = 0;	

		}break;

		case 1:
		{       
		        if( key_intterupt_trigger(tInchStatus,KEY_STOP_BIT)==0)//STOP down
		        {

					UpdateKeyStatus = UpdateKeyStatus |KEY_STOP_BIT;					
				}
				else 
				{
					if(Gpio_GetIO(KEY_STOP_PORT_IRQ, KEY_STOP_PIN)==0)//STOP key down 
					{
						while(Gpio_GetIO(KEY_STOP_PORT_IRQ, KEY_STOP_PIN))//wait button up
							;
						UpdateKeyStatus = UpdateKeyStatus |KEY_STOP_BIT;
					}

				}
			 *((uint32_t *)((uint32_t)&M0P_GPIO->P1ICLR + u8Param * 0x40)) = 0;	

		}break;
				case 3:
				{		
				        //NORTH key down
						if( key_intterupt_trigger(tInchStatus,KEY_NORTH_BIT)==0)
						{
				
							UpdateKeyStatus = UpdateKeyStatus |KEY_NORTH_BIT;					
						}
						else 
						{
							if(Gpio_GetIO(KEY_NORTH_PORT_IRQ, KEY_NORTH_PIN)==0)//key down 
							{
								while(Gpio_GetIO(KEY_NORTH_PORT_IRQ, KEY_NORTH_PIN))//wait button up
									;
								UpdateKeyStatus = UpdateKeyStatus |KEY_NORTH_BIT;
							}
				
						}

						
						//UP key down
						if( key_intterupt_trigger(tInchStatus,KEY_UP_BIT)==0)
						{
				
							UpdateKeyStatus = UpdateKeyStatus |KEY_UP_BIT;					
						}
						else 
						{
							if(Gpio_GetIO(KEY_UP_PORT_IRQ, KEY_UP_PIN)==0)//key down 
							{
								while(Gpio_GetIO(KEY_UP_PORT_IRQ, KEY_UP_PIN))//wait button up
									;
								UpdateKeyStatus = UpdateKeyStatus |KEY_UP_BIT;
							}
				
						}

						
						//SOUTH key down
						if( key_intterupt_trigger(tInchStatus,KEY_SOUTH_BIT)==0)
						{
				
							UpdateKeyStatus = UpdateKeyStatus |KEY_SOUTH_BIT;					
						}
						else 
						{
							if(Gpio_GetIO(KEY_SOUTH_PORT_IRQ, KEY_SOUTH_PIN)==0)//key down 
							{
								while(Gpio_GetIO(KEY_SOUTH_PORT_IRQ, KEY_SOUTH_PIN))//wait button up
									;
								UpdateKeyStatus = UpdateKeyStatus |KEY_SOUTH_BIT;
							}
				
						}

						
						//WEST key down
						if( key_intterupt_trigger(tInchStatus,KEY_WEST_BIT)==0)
						{
				
							UpdateKeyStatus = UpdateKeyStatus |KEY_WEST_BIT;					
						}
						else 
						{
							if(Gpio_GetIO(KEY_SOUTH_PORT_IRQ, KEY_WEST_PIN)==0)//key down 
							{
								while(Gpio_GetIO(KEY_WEST_PORT_IRQ, KEY_WEST_PIN))//wait button up
									;
								UpdateKeyStatus = UpdateKeyStatus |KEY_WEST_BIT;
							}
				
						}
						
						//EAST key down
						if( key_intterupt_trigger(tInchStatus,KEY_EAST_BIT)==0)
						{
				
							UpdateKeyStatus = UpdateKeyStatus |KEY_EAST_BIT;					
						}
						else 
						{
							if(Gpio_GetIO(KEY_EAST_PORT_IRQ, KEY_EAST_PIN)==0)//key down 
							{
								while(Gpio_GetIO(KEY_EAST_PORT_IRQ, KEY_EAST_PIN))//wait button up
									;
								UpdateKeyStatus = UpdateKeyStatus |KEY_EAST_BIT;
							}
				
						}
						
						//START key down
						if( key_intterupt_trigger(tInchStatus,KEY_START_BIT)==0)
						{
				
							UpdateKeyStatus = UpdateKeyStatus |KEY_START_BIT;					
						}
						else 
						{
							if(Gpio_GetIO(KEY_START_PORT_IRQ, KEY_START_PIN)==0)//key down 
							{
								while(Gpio_GetIO(KEY_START_PORT_IRQ, KEY_START_PIN))//wait button up
									;
								UpdateKeyStatus = UpdateKeyStatus |KEY_START_BIT;
							}
				
						}
				 *((uint32_t *)((uint32_t)&M0P_GPIO->P3ICLR + u8Param * 0x40)) = 0;	
				}break;

		
   }


}
/*******************************************************************************
** \brief   GPIO ��ʼ��2 
** 
** \param   [in]  u8Port             IO Port��
** \param   [in]  u8Pin              IO Pin��
** \param   [in]  enDir              IO ��������������
** \param   [in]  bPullup            ��������
** \param   [in]  bPulldown          ��������
** \param   [in]  bOdr               ��©����
** \param   [in]  bDrive             �������� 
**                                   0 = ��
**                                   1 = ��
** \retval     Ok         ���óɹ�
**             ����ֵ     ����ʧ��
******************************************************************************/
/*en_result_t Gpio_InitIOExt(uint8_t u8Port, uint8_t u8Pin,
en_gpio_dir_t enDir,
boolean_t bPullup,
boolean_t bPulldown,
boolean_t bOdr,
boolean_t bDrive)
*/
	boolean_t key_intterupt_trigger(uint16_t inch_status,unsigned char key_num)
	{
		en_gpio_irqtype_t EXTI_Trigger;
	
				 if(inch_status&key_num)
				  EXTI_Trigger = 1;//rising edge trigger
				else 
				  EXTI_Trigger = 0; //low level trigger
		return EXTI_Trigger;
	}

en_gpio_irqtype_t key_intterupt_triggerConfig(uint16_t inch_status,unsigned char key_num)
{
	en_gpio_irqtype_t EXTI_Trigger;

			 if(inch_status&key_num)
			  EXTI_Trigger = GpioIrqFalling;
			else 
			  EXTI_Trigger = GpioIrqLow; 
    return EXTI_Trigger;
}
void key_intterupt_config( uint16_t inch_status)
{
  en_gpio_irqtype_t EXTI_Trigger;
   EXTI_Trigger=key_intterupt_triggerConfig(inch_status,KEY_UP_BIT); 	 	
  //��ʼ���ⲿIO P33
  Gpio_InitIOExt(KEY_UP_PORT,KEY_UP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//up
  Gpio_ClearIrq(KEY_UP_PORT, KEY_UP_PIN);
  Gpio_EnableIrq(KEY_UP_PORT, KEY_UP_PIN, EXTI_Trigger);
  EnableNvic(KEY_UP_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);
  
   EXTI_Trigger=key_intterupt_triggerConfig(KEY_DOWN_BIT);  
  Gpio_InitIOExt(KEY_DOWN_PORT,KEY_DOWN_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//DOWN
  Gpio_ClearIrq(KEY_DOWN_PORT, KEY_DOWN_PIN);
  Gpio_EnableIrq(KEY_DOWN_PORT, KEY_DOWN_PIN, EXTI_Trigger);
  EnableNvic(KEY_DOWN_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   


   EXTI_Trigger=key_intterupt_triggerConfig(KEY_EAST_BIT);     
  Gpio_InitIOExt(KEY_EAST_PORT,KEY_EAST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//EAST
  Gpio_ClearIrq(KEY_EAST_PORT, KEY_EAST_PIN);
  Gpio_EnableIrq(KEY_EAST_PORT, KEY_EAST_PIN, EXTI_Trigger);
  EnableNvic(KEY_EAST_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 

   EXTI_Trigger=key_intterupt_triggerConfig(KEY_WEST_BIT); 
  Gpio_InitIOExt(KEY_WEST_PORT,KEY_WEST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//WEST
  Gpio_ClearIrq(KEY_WEST_PORT, KEY_WEST_PIN);
  Gpio_EnableIrq(KEY_WEST_PORT, KEY_WEST_PIN, EXTI_Trigger);
  EnableNvic(KEY_WEST_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   


   EXTI_Trigger=key_intterupt_triggerConfig(KEY_SOUTH_BIT);  
  Gpio_InitIOExt(KEY_SOUTH_PORT,KEY_SOUTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//SOUTH
  Gpio_ClearIrq(KEY_SOUTH_PORT, KEY_SOUTH_PIN);
  Gpio_EnableIrq(KEY_SOUTH_PORT, KEY_SOUTH_PIN, EXTI_Trigger);
  EnableNvic(KEY_SOUTH_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   


   EXTI_Trigger=key_intterupt_triggerConfig(KEY_START_BIT); 
  Gpio_InitIOExt(KEY_START_PORT,KEY_START_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//START
  Gpio_ClearIrq(KEY_START_PORT, KEY_START_PIN);
  Gpio_EnableIrq(KEY_START_PORT, KEY_START_PIN, EXTI_Trigger);
  EnableNvic(KEY_START_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);  

   EXTI_Trigger=key_intterupt_triggerConfig(KEY_NORTH_BIT);   
  Gpio_InitIOExt(KEY_NORTH_PORT,KEY_NORTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//NORTH
  Gpio_ClearIrq(KEY_NORTH_PORT, KEY_NORTH_PIN);
  Gpio_EnableIrq(KEY_NORTH_PORT, KEY_NORTH_PIN, EXTI_Trigger);
  EnableNvic(KEY_NORTH_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   


   EXTI_Trigger=key_intterupt_triggerConfig(KEY_BREAK_BIT);    
  Gpio_InitIOExt(KEY_BREAK_PORT,KEY_BREAK_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//BREAK
  Gpio_ClearIrq(KEY_BREAK_PORT, KEY_BREAK_PIN);
  Gpio_EnableIrq(KEY_BREAK_PORT, KEY_BREAK_PIN, EXTI_Trigger);
  EnableNvic(KEY_STOP_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 


 EXTI_Trigger=key_intterupt_triggerConfig(KEY_STOP_BIT); 
  Gpio_InitIOExt(KEY_STOP_PORT,KEY_STOP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//STOP
  Gpio_ClearIrq(KEY_STOP_PORT, KEY_STOP_PIN);
  Gpio_EnableIrq(KEY_STOP_PORT, KEY_STOP_PIN, EXTI_Trigger);
  EnableNvic(KEY_BREAK_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 
  
}
void KEY_Init()
{
  params_typedef *params;
  uint16_t EXTI_Trigger;
  //��ʼ���ⲿIO P33
  Gpio_InitIOExt(KEY_UP_PORT,KEY_UP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//up
  Gpio_InitIOExt(KEY_DOWN_PORT,KEY_DOWN_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//DOWN
  Gpio_InitIOExt(KEY_EAST_PORT,KEY_EAST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//EAST
  Gpio_InitIOExt(KEY_WEST_PORT,KEY_WEST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//WEST
  Gpio_InitIOExt(KEY_SOUTH_PORT,KEY_SOUTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//SOUTH
  Gpio_InitIOExt(KEY_START_PORT,KEY_START_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//START
  Gpio_InitIOExt(KEY_NORTH_PORT,KEY_NORTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//NORTH
  Gpio_InitIOExt(KEY_BREAK_PORT,KEY_BREAK_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//BREAK 
  Gpio_InitIOExt(KEY_STOP_PORT,KEY_STOP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//STOP
  
  
  /*
#define INCH					0X01   //�㶯
#define SELFLOCK				0X02 //����
#define INHIBITION			0X03//�໥����
#define NOINHIBITION		0X04//������
  */
  params=system_params_get();
/*  switch(params.KEY_H8 )
  {
  case INCH:			EXTI_Trigger = GpioIrqLow; break;
  case SELFLOCK:		EXTI_Trigger = GpioIrqRising; break;
  case INHIBITION:		EXTI_Trigger = GpioIrqFalling; break;
  case NOINHIBITION:	        EXTI_Trigger = GpioIrqFalling; break;
  default:			EXTI_Trigger = GpioIrqFalling; break;	
  }*/
  EXTI_Trigger = params.KEY_H8<<8+ params.KEY_L8;
  key_intterupt_config(EXTI_Trigger);
  
}




