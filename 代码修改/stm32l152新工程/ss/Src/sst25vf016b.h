#ifndef ss25v_H
#define ss25v_H
#include "stm32l1xx_hal.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef uint32_t u32;
void FLASH_SPI_Configuration(void);
void SPI_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void FlashReadID(unsigned char fac_id,unsigne #define SST25_FastReadData                      0x0B  
 #define SST25_4KByte_BlockERASE             0x20  
 #define SST25_32KByte_BlockErase             0x52  
 #define SST25_64KByte_BlockErase             0xD8  
 #define SST25_ChipErase                           0xC7  
 #define SST25_ByteProgram                      0x02  
 #define SST25_AAI_WordProgram              0xAD  
 #define SST25_ReadStatusReg                   0x05  
 #define SST25_EnableWriteStatusReg         0x50  
 #define SST25_WriteStatusReg                  0x01  
 #define SST25_WriteEnable                       0x06  
 #define SST25_WriteDisable                      0x04  
 #define SST25_ManufactDeviceID              0x90  
 #define SST25_JedecDeviceID                   0x9F  
 #define SST25_EBSY                                0x70  
 #define SST25_DBSY                                0x80   
  
 void SPI_Flash_Init(void);  
 u16  SPI_Flash_ReadID(void);                //��ȡFLASH ID  
 u8     SPI_Flash_ReadSR(void);              //��ȡ״̬�Ĵ���  
void SPI_FLASH_Write_SR(u8 sr);          //д״̬�Ĵ���  
void SPI_FLASH_Write_Enable(void);     //дʹ��  
void SPI_FLASH_Write_Disable(void);     //д����  
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash  
 void SPI_Flash_Erase_Chip(void);                                                         //��Ƭ����  
void SPI_Flash_Erase_Sector(u32 Dst_Addr);                                        //��������  
void SPI_Flash_Wait_Busy(void);                                                         //�ȴ�����  
void SST25V_EBSY(void);  
 void SST25V_DBSY(void);  
 void Flash_WriteByte(u8* pBuffer,u32 WriteAddr);                                       //д��1Byte����  
void AutoAddressIncrement_WordProgramA(u8 Byte1, u8 Byte2, u32 Addr);  //��ַ�Զ����ӵ�д����A  
 void AutoAddressIncrement_WordProgramB(u8 state,u8 Byte1, u8 Byte2);     //��ַ�Զ����ӵ�д����B  
 void SPI_Flash_Write(u8 pBuffer[],u32 WriteAddr,u16 NumByteToWrite);       //���AB���ɵĵ�ַ�Զ����ӵ��������ݵ�д��  


typedef unsigned char u8;
typedef unsigned int u16;
typedef uint32_t u32;
void FLASH_SPI_Configuration(void);
void SPI_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void FlashReadID(unsigned char fac_id,unsigned char dev_id1,unsigned char dev_id2);

#endif

