#include "bsp_adc.h"
#include "adc.h"
#include "gpio.h"
#include "ddl.h"
/**
  * @brief  Configure ADC and Analog watchdog
  * @param  None
  * @retval None
  */
uint16_t ADC_Config(void)
{
       stc_adc_cfg_t      stcAdcCfg;
    stc_adc_norm_cfg_t stcAdcNormCfg;
    
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcNormCfg);
 if (Ok != Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE))
    {
        return Error;
    }

    if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE))
    {
        return Error;
    }
    
    Gpio_SetAnalog(2, 4, TRUE);
    
    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;//BGR����ʹ��
    M0P_BGR->CR_f.TS_EN = 0x0u;

    delay100us(1);
    
    stcAdcCfg.enAdcOpMode = AdcNormalMode;          //���β���ģʽ
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv1;         //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime4Clk;   //4������ʱ��
    stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;   //�ο���ѹ:�ڲ�2.5V(avdd>3V,SPS<=200kHz)  SPS���� = ADCʱ�� / (����ʱ�� + 16CLK) 
    stcAdcCfg.bAdcInBufEn = FALSE;                  //��ѹ���������ʹ�ܣ�SPS�������� <=200K
    stcAdcCfg.u32AdcRegHighThd = 0u;                //�Ƚ���ֵ������
    stcAdcCfg.u32AdcRegLowThd = 0u;                 //�Ƚ���ֵ������
    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;       //ADCת���Զ���������
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);    
    
    stcAdcNormCfg.enAdcNormModeCh = AdcExInputCH0;  //ͨ��0 P24
    stcAdcNormCfg.bAdcResultAccEn = FALSE;
    Adc_ConfigNormMode(&stcAdcCfg, &stcAdcNormCfg);
            
    while(1)
    {
        Adc_Start();
        
        while(FALSE != Adc_PollBusyState());
        
        Adc_GetResult(&u16AdcResult);
        Adc_ClrAccResult();

        delay1ms(1000);
    }




	
    return ADCData;
}
