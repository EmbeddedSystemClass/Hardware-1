#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"




void main(void)
{
 
  HardwareInit();
  module_prams_init();
  
  while(1)
  {
  //delay_ms(5000);
    SIMCOM_Register_Network();
    module_process(); 
  }
}
