#include "main.h"
#include "systick_timer.h"
#include "stm32f4xx_hal.h"


//��������ʱʱ������,
/*    0:LTE������ʱ
        1:433MHZ��ʱʱ��
        2:GPS����Ƶ��
        3:433MHZ����Ƶ��
        4:����
*/

uint32_t Time_delay[systick_count];//��������ʱʱ������

unsigned int   GPS_Time_Interval=10000; //gps����Ƶ������
unsigned int   RF_Time_Interval =500; // 433MHz ����Ƶ������
unsigned int   LTE_Delay_Time =2000;
unsigned int   TXRX_Time =10;
unsigned int   LTE_Heart_Time =20000;
unsigned int   LTE_Power_Time =15000;
unsigned int LTE_Battery_Time=5000;
Module_Sample_Time sample_time_flag;

uint32_t TimingDelay;
//ϵͳ��ʱ��������λ��ms
void delay_us(uint32_t time)
{    
   uint32_t i=0;  
   while(time--)
   {
      i=10;  
      while(i--) ;    
   }
}
//ϵͳ��ʱ��������λ��ms
void delay_ms(uint32_t nms)
{

	TimingDelay = nms;
	while(TimingDelay != 0);
}
//ϵͳ��ʱ����������������λ��ms

void noblock_delay_ms(unsigned char index,uint32_t nms)
{
	Time_delay[index]=nms;
}
uint32_t get_Time_delay(unsigned char index)
{
	return Time_delay[index];
}


uint32_t Time_delay[systick_count];
/*
��ģ�鶨ʱ����ʱ
*/
void Sample_timer_dec()
{
	unsigned char i;
	for(i=0; i<systick_count; i++)
	{
		if(Time_delay[i]>0)
			Time_delay[i]=Time_delay[i]-1;
	}

}



unsigned char  Sample_Time_Interval_Cmp(uint32_t src,uint32_t dst,unsigned char index)
{
	if(src!=dst)
	{
		//EEPROM_Store(dst,index);
		return 1;
	}
	return 0;

}
//���ò���Ƶ�ʡ���ʱʱ��
void Sample_Time_Interval_Setting(unsigned char index,uint32_t value)
{

	switch(index)
	{
	case GPS_INTERVAL_INDEX:
	{
		if(Sample_Time_Interval_Cmp(GPS_Time_Interval,value,index))
			GPS_Time_Interval=value;

	}
	break;


	case TXRX_INDEX:
	{
		if(Sample_Time_Interval_Cmp(TXRX_Time,value,index))
			TXRX_Time=value;
	}
	break;



	case RF_INTERVAL_INDEX:
	{
		if(Sample_Time_Interval_Cmp(RF_Time_Interval,value,index))
			RF_Time_Interval=value;

	}
	break;
	case LTE_DELAY_INDEX:
	{
		if(Sample_Time_Interval_Cmp(LTE_Delay_Time,value,index))
			LTE_Delay_Time=value;

	}
	break;

	case LTE_Heart_INDEX:
	{
		if(Sample_Time_Interval_Cmp(LTE_Heart_Time,value,index))
			LTE_Heart_Time=value;

	}
	case LTE_Power_INDEX:
	{
		if(Sample_Time_Interval_Cmp(LTE_Power_Time,value,index))
			LTE_Power_Time=value;
		

	}
	break;
	case LTE_Battery_INDEX:
	{
		if(Sample_Time_Interval_Cmp(LTE_Battery_Time,value,index))
			LTE_Battery_Time=value;
		//LTE_Battery_INDEX

	}
	break;

	}
	//���������flash�У����籣��


}
Module_Sample_Time Get_Sample_flag_Status()
{
	return sample_time_flag;
}
void Reset_Sample_flag(unsigned char index)
{


	switch (index)
	{
	case GPS_INTERVAL_INDEX:
		sample_time_flag.GPS_sample_flag=0;
		break;
	case TXRX_INDEX:
		sample_time_flag.TXRX_sample_flag=0;
		break;
	case RF_INTERVAL_INDEX:
		sample_time_flag.RF_sample_flag=0;
		break;
	case LTE_DELAY_INDEX:
		sample_time_flag.LTE_delay_flag=0;
		break;
	case LTE_Heart_INDEX:
		sample_time_flag.LTE_heart_flag=0;
		break;
	case LTE_Power_INDEX:
		sample_time_flag.LTE_Power_flag=0;
		break;
	case LTE_Battery_INDEX:
		sample_time_flag.LTE_Battery_flag=0;
		break;		

		
	}

}



void Sample_Timer_Init()
{
//	uint32_t *pbuffer;
	unsigned char i;
//  uint32_t value;
	for(i=0; i<systick_count; i++)
	{
	

		if(Time_delay[i]==0)
		{
			switch(i)
			{
			case GPS_INTERVAL_INDEX:
			{
				Time_delay[i] = GPS_Time_Interval;
				sample_time_flag.GPS_sample_flag=0x00;
			}
			break;

			case TXRX_INDEX:
			{
				Time_delay[i] = TXRX_Time;
				sample_time_flag.TXRX_sample_flag=0x00;

			}
			break;
			case RF_INTERVAL_INDEX:
			{
				Time_delay[i] = RF_Time_Interval;
				sample_time_flag.RF_sample_flag=0x00;
			}
			break;

			case LTE_DELAY_INDEX:
			{
				Time_delay[i] = LTE_Delay_Time;
				sample_time_flag.LTE_delay_flag=0x00;
			}
			break;

			case LTE_Heart_INDEX:
			{
				Time_delay[i] = LTE_Heart_Time;
				sample_time_flag.LTE_heart_flag=0x00;
			}
			break;
			case LTE_Power_INDEX:
			{
				Time_delay[i] = LTE_Power_Time;
				sample_time_flag.LTE_Power_flag=0x00;
			}
			break;
			case LTE_Battery_INDEX:
			{
				Time_delay[i] = LTE_Battery_Time;
				sample_time_flag.LTE_Battery_flag=0x00;
			}
			break;

			}
			//Sample_Time_Interval_Setting(i,value);
		}
	}

}

//��ģ�鶨ʱ�����¼�ʱ����
void Sample_Timer_Reload()
{
//	uint32_t *pbuffer;
	unsigned char i;


	for(i=0; i<systick_count; i++)
	{
		if(Time_delay[i]==0)
		{
			switch(i)
			{
			case GPS_INTERVAL_INDEX:
			{
				Time_delay[i] =GPS_Time_Interval ;
				sample_time_flag.GPS_sample_flag=0x01;
			}
			break;

			case TXRX_INDEX:
			{
				Time_delay[i] = TXRX_Time;
				sample_time_flag.TXRX_sample_flag=0x01;

			}
			break;
			case RF_INTERVAL_INDEX:
			{
				Time_delay[i] = RF_Time_Interval;
				sample_time_flag.RF_sample_flag=1;
			}
			break;

			case LTE_DELAY_INDEX:
			{
				Time_delay[i] =LTE_Delay_Time ;
				sample_time_flag.LTE_delay_flag=1;
			}
			break;

			case LTE_Heart_INDEX:
			{
				Time_delay[i] = LTE_Heart_Time;
				sample_time_flag.LTE_heart_flag=1;
			}
			break;
			case LTE_Power_INDEX:
			{
				Time_delay[i] = LTE_Power_Time;
				sample_time_flag.LTE_Power_flag=1;
			}
			break;
			case LTE_Battery_INDEX:
			{
				Time_delay[i] = LTE_Battery_Time;
				sample_time_flag.LTE_Battery_flag=1;
			}
			break;

			}
			//Sample_Time_Interval_Setting(i,value);
		}
	}

}



void HAL_SYSTICK_Callback(void)
{

	if (TimingDelay != 0x00000000)
	{
		TimingDelay--;
	}

	Sample_timer_dec();


}
