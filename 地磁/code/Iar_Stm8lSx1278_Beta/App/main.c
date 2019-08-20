

#include "stm8l15x_flash.h"
#include "sx1276.h"
#include "bsp.h"
#include "stm8l15x_it.h"

#include "math.h"
#include "RF.h"
//#define VersionNumber 1.1.1
unsigned char VersionNumber[5] = {'1', '.', '0', '.', '0'} ;

#define TX              1       // ����ģʽ
#define RX              0       // ����ģʽ

//��������

//extern u8 CheckBit;
////Flash �洢
//extern unsigned char Flash_Data[5] ;
//extern unsigned char Flash_Write_Done;
////stm8�������Լ�����״̬��
//extern unsigned char LoraM0Flag;
//extern unsigned char LoraM1Flag;
//extern unsigned char AUX ;
///*����1*/
//extern unsigned char ADDH ;
///*����2*/
//extern unsigned char ADDL;
///*����3*/
//extern u32 SerialRate ;
//extern unsigned char AirRate ;   //�տ��ٶ� Ĭ��2.4k
//
//extern u8 ConfigureDone ;
//extern u8 CurrentMode ;
extern unsigned int pream_long[6] ;
//
//extern u8 LastMode ;
///*����4*/
//extern unsigned char Channel ; //Ĭ��ͨ��Ϊ23ͨ�� 433MHZ
///*����5*/
//extern  unsigned char TranMode;
//extern  unsigned int WakeupTime ;

#define ACK_LENGTH      10      // Ӧ���źų���
#define SEND_LENGTH     10      // ��������ÿ���ĳ���
INT8U   SendBuffer[SEND_LENGTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
INT8U   SendFlag = 1;           // =1�������������ݣ�=0������
INT16U  SendCnt = 0;            // �������͵����ݰ���
INT16U  SendTime = 1;           // �������ݷ��ͼ��ʱ��
INT16U  RecvWaitTime = 0;       // ���յȴ�ʱ��
#define RECV_TIMEOUT    800     // ���ճ�ʱ
//��ҪӦ�������
INT8U   AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
INT16U  RecvCnt = 0;            // �������յ����ݰ���

INT16U SingleWait = 0;
unsigned char InitFlag = 0;



void DelayMs(INT8U x)
{
    unsigned int i;
    i=1000;
    for(;x>0;x--)
        for(i=1000;i>0;i--)     
            ;
}





void delay_ms(u16 num)//���Ǻܾ�ȷ
{
    u8 i = 0;
    while(num--)
    {
        for (i=0; i<100; i++);
    }
}







int error_temp = 0;
void main(void)
{
    INT8U length = 0, recv_buffer[128]={ 0 };  //�������ݵĳ����Լ���������洢������
    INT8U tmp_76 = 0, tmp_543 = 0;             //�ֱ������У��λ��������

    char i = 0;

	HardwareInit();

    RF_Initial( );



    while(1)
    {
        SX1276_SendPacket(UsartReceiveData, 3);
                        SX1276_SetPreambleSize((pream_long[GetModuleParams()->AirRate]*(GetModuleParams()->WakeupTime/250)));   //ǰ����Ϊ972 BW 500kHZ SFΪ7 ����д���ʱ��250ms
                        SX1276_SetRxMode();      
        /*�ж�M0 M1 ���벻ͬ�Ĺ���ģʽ
         *M0 = 0  M1 = 1 һ��ģʽ
         *M0 = 1  M1 = 0 ����ģʽ
         *M0 = 0  M1 = 1 ʡ��ģʽ
         *M0 = 1  M1 = 1 ����ģʽ
         */
        GetModuleMode()->LoraM1Flag = GPIO_ReadInputDataBit(PORT_SX127X_M1, PIN_SX127X_M1);
        GetModuleMode()->LoraM0Flag = GPIO_ReadInputDataBit(PORT_SX127X_M0, PIN_SX127X_M0);

        if((GetModuleMode()->LoraM0Flag == 0) && (GetModuleMode()->LoraM1Flag == 0))
            GetModuleMode()->CurrentMode = NormalMode;
        else if((GetModuleMode()->LoraM0Flag != 0) && (GetModuleMode()->LoraM1Flag == 0))
            GetModuleMode()->CurrentMode = WakeMode;
        else if((GetModuleMode()->LoraM0Flag == 0) && (GetModuleMode()->LoraM1Flag != 0))             //��0 ��0x02
            GetModuleMode()->CurrentMode = PowerSavingMode;
        else GetModuleMode()->CurrentMode = SleepMode;

        if(GetModuleMode()->LastMode != GetModuleMode()->CurrentMode) {                               //��ģʽ�����仯ʱ��
            AUX_CONFIGURING();
            GetModuleMode()->ConfigureDone = 0;
            RtcWakeUp = 0;
            RTC_DeInit();                                           //��ʼ��Ĭ��״̬ 
            CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE); //��ֹRTCʱ�� 
            RTC_WakeUpCmd(DISABLE);                                 //��ֹRTC��������
            RTC_ITConfig(RTC_IT_WUT, DISABLE);                      //��ֹRTC�ж�
            GetModuleMode()->LastMode = GetModuleMode()->CurrentMode;
        }

        switch (PowerSavingMode) {
            case NormalMode:
                if(GetModuleMode()->ConfigureDone) {
                    SX1276_SetRxMode();
                    if( UsartReceiveFlag == 1 )
                    {
                        //ǰ���������  ��ǰ�����ģ�鷢�����ݵ����ó�ǰ����ģ�飬���Խ��գ���ǰ�����ģ�鷢�����ݵ����ö�ǰ����ģ�飬�����Խ���
                        SX1276_SetPreambleSize(8);                              //��������ʱ��ǰ�������ö�Щ
                        SX1276_SetRxMode();
                        AUX_CONFIGURING();
                        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
                        //USART_SendData8(USART1, (usart_i-1));
                        //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        for(i = 0; i < (usart_i-1); i++) {
                            USART_SendData8(USART1, UsartReceiveData[i]);
                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        }
                        AUX_CONFIGURED();
                        SX1276_SetPreambleSize(65530);                          //���ݷ�����ɺ�ǰ�������õ����������ջ���ģʽ������
                        usart_i = 0;
                        LED_TOG();
                        UsartReceiveFlag = 0;
                    }
                    AUX_CONFIGURED();
                    if( ExitInterFlag) {
                        ExitInterFlag = 0;
                        length = SX1276_ReceivePacket(recv_buffer);             // ����Ƿ��յ�һ�����ݰ�
                        if( length )
                        {
                            AUX_CONFIGURING();
                            delay_ms(50);
                            for(i = 0; i < length; i++) {
                                USART_SendData8(USART1, recv_buffer[i]);
                                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                            }
                            AUX_CONFIGURED();
                            length = 0;
                            LED_TOG();
                        }
                    }
                } else {
                    AUX_CONFIGURING();
                    Uart1_Init(GetModuleParams()->SerialRate, GetModuleMode()->CheckBit);                           //���ô��ڲ�����
                    USART_SendStr("ʱ��+���ڳ�ʼ�����\r\n");
                    SX1276_SetPreambleSize(65530);
                    GetModuleMode()->ConfigureDone = 1;
                    AUX_CONFIGURED();
                }
                break;
            case WakeMode:
                if(GetModuleMode()->ConfigureDone) {
                    SX1276_SetRxMode();
                    if( UsartReceiveFlag == 1 )
                    {
                        SX1276_SetPreambleSize((pream_long[GetModuleParams()->AirRate]*(GetModuleParams()->WakeupTime/250)));   //ǰ����Ϊ972 BW 500kHZ SFΪ7 ����д���ʱ��250ms
                        SX1276_SetRxMode();
                        SX1276_SendPacket(UsartReceiveData, (usart_i-1));
                        USART_SendData8(USART1, (usart_i-1));
                        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        for(i = 0; i < (usart_i-1); i++) {
                            USART_SendData8(USART1, UsartReceiveData[i]);
                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                        }
                        SX1276_SetPreambleSize(65530);                          //���ݷ�����ɺ�ǰ�������õ����������ջ���ģʽ������
                        usart_i = 0;
                        LED_TOG();
                        UsartReceiveFlag = 0;
                    }
                    if( ExitInterFlag) {
                        ExitInterFlag = 0;
                        length = SX1276_ReceivePacket(recv_buffer);             // ����Ƿ��յ�һ�����ݰ�
                        if( length )
                        {
                            AUX_CONFIGURING();
                            delay_ms(50);
                            //USART_SendData8(USART1, length);
                            //while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                            for(i = 0; i < length; i++) {
                                USART_SendData8(USART1, recv_buffer[i]);
                                while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                            }
                            AUX_CONFIGURED();
                            length = 0;
                            LED_TOG();
                        }
                    }
                } else {
                    AUX_CONFIGURING();
                    Uart1_Init(GetModuleParams()->SerialRate, GetModuleMode()->CheckBit);                           //���ô��ڲ�����
                    USART_SendStr("ʱ��+���ڳ�ʼ�����\r\n");
                    SX1276_SetPreambleSize((pream_long[GetModuleParams()->AirRate]*(GetModuleParams()->WakeupTime/250)));   //ǰ����Ϊ972 BW 500kHZ SFΪ7 ����д���ʱ��250ms
                    GetModuleMode()->ConfigureDone = 1;
                    AUX_CONFIGURED();
                }
                break;
            case PowerSavingMode:
                if(GetModuleMode()->ConfigureDone) {
                    if(RtcWakeUp == 1)
                    {
                        RtcWakeUp = 0;
                        SX1276_CADMode();
                        while (SingleWait <= 20000) {
                            SingleWait ++;
                            if(CadDoneFlag == 1) {
                                RTC_WakeUpCmd(DISABLE);
                                RTC_ITConfig(RTC_IT_WUT, DISABLE);
                                SX1276_SetRxMode();
                                if( ExitInterFlag) {
                                    ExitInterFlag = 0;
                                    length = SX1276_ReceivePacket(recv_buffer);           // ����Ƿ��յ�һ�����ݰ�
                                    if( length )
                                    {
                                        AUX_CONFIGURING();
                                        delay_ms(50);
                                        for(i = 0; i < length; i++) {
                                            USART_SendData8(USART1, recv_buffer[i]);
                                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                        }
                                        AUX_CONFIGURED();
                                        length = 0;
                                        SingleWait = 20001;
                                        CadDoneFlag = 0;
                                    }
                                }
                            }
                        }
                        SingleWait = 0;
                        RTC_ITConfig(RTC_IT_WUT, ENABLE);
                        RTC_WakeUpCmd(ENABLE);
                        while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==0);
                        EnterStopMode();

                        SystemClock_Init();     // ϵͳʱ�ӳ�ʼ��
                        GPIO_Initial();         // ��ʼ��GPIO
                        Uart1_Init(GetModuleParams()->SerialRate, GetModuleMode()->CheckBit);                           //���ô��ڲ�����
                        SPI_Initial();          //SPI��ʼ��
                    }
                } else {
                    AUX_CONFIGURING();
                    Uart1_Init(GetModuleParams()->SerialRate, GetModuleMode()->CheckBit); //���ô��ڲ�����
                    USART_SendStr("ʱ��+���ڳ�ʼ�����\r\n");
                    /*SX1276���ݽ����ⲿ�ж�
                      DIO3��ΪCADDone�ĳ����ж�
                      ��RTC���ж�ʱ
                    */
                    SX1276_SetPreambleSize(65530);
                    RTC_Config(GetModuleParams()->WakeupTime);
                    enableInterrupts();
                    GetModuleMode()->ConfigureDone = 1;
                    AUX_CONFIGURED();
                }
                break;
            case SleepMode:
                if(GetModuleMode()->ConfigureDone) {
                    if ((SleepModeFlag == 1) && (InitFlag == 0))
                    {
                        AUX_CONFIGURING();
                        SystemClock_Init();     // ϵͳʱ�ӳ�ʼ��
                        GPIO_Initial();         // ��ʼ��GPIO
                        SPI_Initial();          //SPI��ʼ��
                        LED_Init();             //����LED��ʼ��
                        RF_Initial( );
                        Uart1_Init(9600, 0);
                        InitFlag = 1;
                        AUX_CONFIGURED();
                    } else if(InitFlag == 1) {
                        if(UsartReceiveFlag == 1) 
                        {
                            switch (UsartReceiveData[0]) {
                                case 0xC0:
                                    FLASH_Unlock(FLASH_MemType_Data);

                                    FLASH_EraseByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
                                    FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS, UsartReceiveData[1]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    ADDH = UsartReceiveData[1];

                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1), UsartReceiveData[2]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    ADDL = UsartReceiveData[2];

                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2), UsartReceiveData[3]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    if (UsartReceiveData[3] >> 6 == 0x01)
                                        GetModuleMode()->CheckBit = 0x06;
                                    else if (UsartReceiveData[3] >> 6 == 0x02)
                                        GetModuleMode()->CheckBit = 0x04;
                                    else GetModuleMode()->CheckBit = 0x00;
                                    if (((UsartReceiveData[3]>>3) & 0x7) < 6)
                                        GetModuleParams()->SerialRate = 1200 * (unsigned int)(pow(2, ((UsartReceiveData[3]>>3) & 0x7)));
                                    else if (((UsartReceiveData[3]>>3) & 0x7) == 6)
                                        GetModuleParams()->SerialRate = 57600;
                                    else GetModuleParams()->SerialRate = 115200;
                                    
                                    GetModuleParams()->AirRate = UsartReceiveData[3]& 0x7;
                                    
                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+3));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3), UsartReceiveData[4]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    GetModuleParams()->Channel =  UsartReceiveData[4]&0x1F;

                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+4));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4), UsartReceiveData[5]);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    TranMode = UsartReceiveData[5]>>7;
                                    GetModuleParams()->WakeupTime = 250*(((UsartReceiveData[5]>>3) & 0x7) + 1);

                                    GetModuleParams()->Flash_Write_Done = 1;
                                    FLASH_EraseByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+5));
                                    FLASH_ProgramByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5), GetModuleParams()->Flash_Write_Done);
                                    FLASH_WaitForLastOperation(FLASH_MemType_Data);
                                    FLASH_Lock(FLASH_MemType_Data);

                                    LED_TOG();
                                break;
                                case 0xC1: //
                                    USART_SendData8(USART1, ADDH);
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                    USART_SendData8(USART1, ADDL);
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);

                                    if(GetModuleMode()->CheckBit == 0x00)
                                        tmp_76 = 0x00;
                                    else if (GetModuleMode()->CheckBit == 0x06)
                                        tmp_76 = 0x01;
                                    else tmp_76 = 0x02;
                                    tmp_543 = (unsigned char) (ceil(log2(GetModuleParams()->SerialRate/1200)));
                                    USART_SendData8(USART1, (tmp_76 << 6 | tmp_543 << 3 | GetModuleParams()->AirRate)); 
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);

                                    USART_SendData8(USART1,  GetModuleParams()->Channel);
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                    
                                    USART_SendData8(USART1, ((TranMode<<7) | (( GetModuleParams()->WakeupTime/250-1)<<3)));
                                    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                    LED_TOG();
                                    break;
                                case 0xC2:
                                    ADDH = UsartReceiveData[1];
                                    
                                    ADDL = UsartReceiveData[2];

                                    if (UsartReceiveData[3] >> 6 == 0x01)
                                          GetModuleMode()->CheckBit = 0x06;
                                    else if (UsartReceiveData[3] >> 6 == 0x02)
                                        GetModuleMode()->CheckBit = 0x04;
                                    else GetModuleMode()->CheckBit = 0x00;
                                    if (((UsartReceiveData[3]>>3) & 0x7) < 6)
                                        GetModuleParams()->SerialRate = 1200 * (unsigned int) (pow(2, ((UsartReceiveData[3]>>3) & 0x7)));
                                    else if (((UsartReceiveData[3]>>3) & 0x7) == 6)
                                        GetModuleParams()->SerialRate = 57600;
                                    else GetModuleParams()->SerialRate = 115200;
                                    
                                    GetModuleParams()->AirRate = UsartReceiveData[3]& 0x7;
                                    
                                    GetModuleParams()->Channel =  UsartReceiveData[4]&0x1F;        

                                    TranMode = UsartReceiveData[5]>>7;
                                    GetModuleParams()->WakeupTime = 250*(((UsartReceiveData[5]>>3) & 0x7) + 1);
                                    //����ģʽû�� IO������ʽû�� FEC����û�� ���书��û��
                                    LED_TOG();
                                    break;
                                case 0xC3:
                                    if((UsartReceiveData[1] == 0xC3) && (UsartReceiveData[2] == 0xC3))
                                    {
                                        for(i = 0; i < 5; i++) {
                                            USART_SendData8(USART1, VersionNumber[i]);
                                            while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==0);
                                        }
                                    }
                                    break; 
                                case 0xC4:
                                    if((UsartReceiveData[1] == 0xC4) && (UsartReceiveData[2] == 0xC4))
                                    {
                                        WWDG->CR = 0x80;
                                    }
                                    break;
                            }
                            UsartReceiveFlag = 0;
                        }
                    }
                } else {
                    GetModuleMode()->ConfigureDone = 1;
                    SleepModeFlag = 0;  //���ڽ������ݹܽ�������ʱ�������ⲿ�жϣ������߽��л��ѣ���SleepModeFlag��1
                    InitFlag = 0;
                    EnterStopMode();
                }
                break;
            default:
                break;
        }
    }
}
