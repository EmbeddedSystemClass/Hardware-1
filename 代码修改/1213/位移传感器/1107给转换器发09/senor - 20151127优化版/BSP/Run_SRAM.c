#include "stm32l1xx.h"


#if defined ( __CC_ARM   )

void

#elif defined ( __ICCARM__ )
__ramfunc void

#elif defined   (  __GNUC__  )
void __attribute__((section(".data")))
#elif defined   (  __TASKING__  )
#pragma section_code_init on
#endif
/*****************************************************
����:LowPowerRunModeSRAM(void)
/*�͹�������ģʽ ����������RAM�У�����10.uA
*****************************************************/
LowPowerRunModeSRAM(void)
{
    FLASH_RUNPowerDownCmd(ENABLE);
    while(((*(__IO uint32_t *)0x40020010) & 0x0001) == 0)
    {
    }
    /* Wait Until Key button pressed */
    while(((*(__IO uint32_t *)0x40020010) & 0x0001) != 0)
    {
    }

    FLASH_RUNPowerDownCmd(DISABLE);
}

#if defined (  __TASKING__  )
#pragma section_code_init restore
#endif
