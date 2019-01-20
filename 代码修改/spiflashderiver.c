/*void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
  
 SPI_FLASH_Write_SR(0x02);//ʹ��״̬�Ĵ����е�д�洢��  
  
         SST25V_DBSY();  
 SPI_FLASH_Write_SR(0x02);//ʹ��״̬�Ĵ����е�д�洢��  
         SST25V_DBSY();  
ʵ��Ŀ�ģ�������д���ⲿFLASH�У�Ȼ���ٶ�������ʾ��LCD��  
 ʵ��ƽ̨������STM32F103C8T6�Ĳ���������  
FLASH��SST25VF016B  
  
 flash_SST.c  */
  
 #include "flash_SST.h"  
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "sst25vf016b.h" 

 typedef unsigned char	u8;
 typedef unsigned int	 u16;
 typedef uint32_t		  u32;
 extern SPI_HandleTypeDef hspi1;
#define SelectDevice  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
#define UnselectDevice HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET);


 u8 SPIx_ReadWriteByte(u8 data)
{
	uint8_t RxData;

	HAL_SPI_TransmitReceive(&hspi1,&data, &RxData, 1, 10);
	return RxData;
} 
 //4KbytesΪһ��Sector  
 //16������Ϊ1��Block  
 //SST25VF016B  
 //����Ϊ2M�ֽ�,����32��Block(��),512��Sector��������  
//��ʼ��SPI FLASH��IO��  
//�޸�״̬�Ĵ���������оƬ�洢����д  
void SPI_Flash_Init(void)  
 {  
 //RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��          
//GPIOA->CRH&=0XFFFFFF0F;  
// GPIOA->CRH|=0X00000030;    //PA9 ����           
//GPIOA->ODR|=1<<9;          //PA9����  
//SPIx_Init();               //��ʼ��SPI  
 SPI_FLASH_Write_SR(0x02);  //ʹ��״̬�Ĵ����е�д�洢��  
SST25V_DBSY();  
 }   
  
 //��ȡSPI_FLASH��״̬�Ĵ���  
//BIT7  6   5   4   3   2   1   0  
 //SPR   RV  TB BP2 BP1 BP0 WEL BUSY  
 //SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��  
//TB,BP2,BP1,BP0:FLASH����д��������  
//WEL:дʹ������  
//BUSY:æ���λ(1,æ;0,����)  
 //Ĭ��:0x00  
 u8 SPI_Flash_ReadSR(void)    
 {   
 u8 byte=0;    
 SelectDevice;                            //ʹ������    
SPIx_ReadWriteByte(SST25_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������     
byte=SPIx_ReadWriteByte(0Xff);             //��ȡһ���ֽ�   
UnselectDevice;                                 //ȡ��Ƭѡ      
return byte;    
 }  
  
 //дSPI_FLASH״̬�Ĵ���  
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!  
void SPI_FLASH_Write_SR(u8 sr)    
 {    
 SelectDevice;    //Ƭѡ  
SPIx_ReadWriteByte(SST25_EnableWriteStatusReg);  //ʹ��д״̬�Ĵ�������    
UnselectDevice;                                                 //ȡ��Ƭѡ  
SelectDevice;                                                  //Ƭѡ                          
SPIx_ReadWriteByte(SST25_WriteStatusReg);   //����дȡ״̬�Ĵ�������     
SPIx_ReadWriteByte(sr);                                //д��һ���ֽ�   
UnselectDevice;                                        //ȡ��Ƭѡ                   
 }   
  
  
 //SPI_FLASHдʹ��  
//��WEL��λ    
void SPI_FLASH_Write_Enable(void)    
 {  
 SelectDevice;                            //ʹ������    
SPIx_ReadWriteByte(SST25_WriteEnable);      //����дʹ��   
UnselectDevice;                            //ȡ��Ƭѡ                 
 }  
  
 //SPI_FLASHд��ֹ  
//��WEL����   
void SPI_FLASH_Write_Disable(void)    
 {   
 SelectDevice;                                       //ʹ������    
SPIx_ReadWriteByte(SST25_WriteDisable);     //����д��ָֹ��     
UnselectDevice;                                       //ȡ��Ƭѡ                 
 }                               
  
 //��ȡоƬID SST25VF016���� 0XBF41  
 u16 SPI_Flash_ReadID(void)  
 {  
 u16 Temp = 0;        
 SelectDevice;   
      
 //���Ͷ�ȡID����                             
SPIx_ReadWriteByte(0x90);  
  
 //����24λ�ĵ�ַ          
SPIx_ReadWriteByte(0x00);               
 SPIx_ReadWriteByte(0x00);               
 SPIx_ReadWriteByte(0x00);  
  
 //��ȡ���ص�16λֵ                                
Temp=SPIx_ReadWriteByte(0xFF)<<8;  //��8λ����  
Temp+=SPIx_ReadWriteByte(0xFF);      //�װ�λ����  
UnselectDevice;                                     
 return Temp;  
 }  
  
 //��ȡSPI FLASH   
 //��ָ����ַ��ʼ��ȡָ�����ȵ�����  
//pBuffer:���ݴ洢��  
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)  
 //NumByteToRead:Ҫ��ȡ���ֽ���(���65535��64k)  
 void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)    
 {  
 u16 i;                                                                                                                  
 SelectDevice;                                       //ʹ������    
SPIx_ReadWriteByte(SST25_ReadData);         //���Ͷ�ȡ����  
  
//����24bit��ַ     
SPIx_ReadWriteByte((u8)((ReadAddr)>>16));     
 SPIx_ReadWriteByte((u8)((ReadAddr)>>8));    
 SPIx_ReadWriteByte((u8)ReadAddr);  
 for(i=0;i<NumByteToRead;i++)  
 {  
   pBuffer=SPIx_ReadWriteByte(0XFF);   //ѭ������   
}  
UnselectDevice;                            //ȡ��Ƭѡ                 
 }   
  
 //��ַ�Զ����ӵ�д����A  
 void AutoAddressIncrement_WordProgramA(u8 Byte1, u8 Byte2, u32 Addr)  
 {  
 SPI_FLASH_Write_Enable();  
 SelectDevice;  
 SPIx_ReadWriteByte(SST25_AAI_WordProgram);  
  
 //������Ҫд���ݵ���ʼ��ַ  
SPIx_ReadWriteByte((Addr & 0xFF0000) >> 16);  
 SPIx_ReadWriteByte((Addr & 0xFF00) >> 8);  
 SPIx_ReadWriteByte(Addr & 0xFF);                                  //�����������������  
SPIx_ReadWriteByte(Byte1);  
 SPIx_ReadWriteByte(Byte2);  
 UnselectDevice;  
 SPI_Flash_Wait_Busy();  
 }  
  
 //��ַ�Զ����ӵ�д����B  
 void AutoAddressIncrement_WordProgramB(u8 state,u8 Byte1, u8 Byte2)  
 {  
 SPI_FLASH_Write_Enable();  
 SelectDevice;  
 SPIx_ReadWriteByte(SST25_AAI_WordProgram);  
 SPIx_ReadWriteByte(Byte1);  
 SPIx_ReadWriteByte(Byte2);  
 UnselectDevice;  
 SPI_Flash_Wait_Busy();  
 if(state==1)  
 {  
   SPI_FLASH_Write_Disable();  
 }  
 SPI_Flash_Wait_Busy();  
 }  
  
 //���AB���ɵĵ�ַ�Զ����ӵ��������ݵ�д��  
//�����Ȳ�����д����Ĺ���  
//pBuffer��Ϊ��д������  
//WriteAddr����д���ݵ���ʼ��ַ  
//NumByteToWrite����Ҫд�����ݵĳ���  
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)  
 {  
 u16 i,temp;  
 u32 secpos;  
 u16 secoff;  
 u16 secremain;       
 //���´���Ϊ������д����Ĵ���  
secpos=WriteAddr/4096;                                           //������4K����ַ0~511 for     SST25VF016  
 secoff=WriteAddr@96;                                                             //�������ڵ�ƫ��  
secremain=4096-secoff;                                                           //����ʣ��ռ��С  
if(NumByteToWrite<secremain)                                                  //ʣ��ռ������������  
{  
   temp=1;  
 }  
 else                                                                 //ʣ��ռ�С����������  
{  
   i=NumByteToWrite-secremain;                           //�жϻ�ռ�˼�������  
if(i%96==0)  
        temp=i/4096+1;  
 else  
        temp=i/4096+2;  
 }  
 for(i=0;i<temp;i++)  
 {  
   SPI_Flash_Erase_Sector((secpos+i)*4096);                   //������Ҫд�����ݵ�����     
 }  
   //���´���Ϊ������д��ָ����ַ�Ĵ���  
if(NumByteToWrite%2==0)  
 {  
   temp=NumByteToWrite/2-1;  
 }  
 else  
 {  
   temp=NumByteToWrite/2;  
 }  
 AutoAddressIncrement_WordProgramA(pBuffer[0], pBuffer[1],WriteAddr );                                        //��ʼд����  
for(i=1;i<temp;i++)  
 {  
   AutoAddressIncrement_WordProgramB(0,pBuffer[2*i], pBuffer[2*i+1]);  
 }  
 if(NumByteToWrite%2==0)  
 {  
   AutoAddressIncrement_WordProgramB(1,pBuffer[NumByteToWrite-2], pBuffer[NumByteToWrite-1]);   //����д����  
}  
else  
 {  
   AutoAddressIncrement_WordProgramB(1,pBuffer[NumByteToWrite-1],0);                                         //����д����  
}  
  
 //д��1Byte����  
//pBuffer:��д������  
//WriteAddr����д���ݵĵ�ַ  
void Flash_WriteByte(u8* pBuffer,u32 WriteAddr)  
 {  
 u32 secpos;  
 secpos=WriteAddr/4096;                                   //������ַ 0~511 for w25x16  4096=4k  
 SPI_Flash_Erase_Sector(secpos);                        //�����������  
SPI_FLASH_Write_Enable();                              //SET WEL  
 SelectDevice;                                            //ʹ������    
SPIx_ReadWriteByte(SST25_ByteProgram );       //����дҳ����  
  
//����24bit��ַ     
SPIx_ReadWriteByte((u8)((WriteAddr)>>16));    
 SPIx_ReadWriteByte((u8)((WriteAddr)>>8));    
 SPIx_ReadWriteByte((u8)WriteAddr);                  //���ʹ�д������  
SPIx_ReadWriteByte(pBuffer[0]);  
 UnselectDevice;   
 SPI_Flash_Wait_Busy();                                     //�ȴ�д���  
}  
  
 //��������оƬ  
//��Ƭ����ʱ��:  
 //W25X16:25s  
 //W25X32:40s  
 //W25X64:40s  
 //�ȴ�ʱ�䳬��...  
void SPI_Flash_Erase_Chip(void)    
 {                                              
 SPI_FLASH_Write_Enable();                          //SET WEL  
 SPI_Flash_Wait_Busy();    
 SelectDevice;                                       //ʹ������    
SPIx_ReadWriteByte(SST25_ChipErase);       //����Ƭ��������   
UnselectDevice;                                     //ȡ��Ƭѡ                 
SPI_Flash_Wait_Busy();                              //�ȴ�оƬ��������  
}  
  
 //����һ������  
//Dst_Addr:������ַ 0~511 for w25x16  
 //����һ��ɽ��������ʱ��:150ms  
 void SPI_Flash_Erase_Sector(u32 Dst_Addr)    
 {    
 SPI_FLASH_Write_Enable();                                       //SET WEL             
 SPI_Flash_Wait_Busy();    
 SelectDevice;                                                   //ʹ������    
SPIx_ReadWriteByte(SST25_4KByte_BlockERASE);      //������������ָ��  
SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));            //����24bit��ַ     
SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));    
 SPIx_ReadWriteByte((u8)Dst_Addr);   
 UnselectDevice;                                            //ȡ��Ƭѡ                 
SPI_Flash_Wait_Busy();                                    //�ȴ��������  
}   
  
 //�ȴ�����  
void SPI_Flash_Wait_Busy(void)    
 {    
 while ((SPI_Flash_ReadSR()&0x01)==0x01);       // �ȴ�BUSYλ���  
}  
  
void SST25V_EBSY(void)  
 {  
 SelectDevice;  
 SPIx_ReadWriteByte( SST25_EBSY);  
 UnselectDevice;     
 }  
  
 void SST25V_DBSY(void)  
 {  
 SelectDevice;  
 SPIx_ReadWriteByte( SST25_DBSY);  
 UnselectDevice;     
 }  
  
 
 
