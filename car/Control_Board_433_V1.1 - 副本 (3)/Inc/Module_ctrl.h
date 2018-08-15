#ifndef Module_H_
#define Module_H_


#define GPS_data_update							0x01   //gps�����и���
#define Temper_data_update					0x02   //�¶������и���
#define UART_data_update						0x04   //�������������и���
#define SPI_Flash_data_update				0x08   //spi�����ݸ���
#define Heart_data_update						0x10   //����������
#define TX_data_update							0x80  //���ݴ����ɣ���Ҫ���� 
#define SPI_Flash_data_update_mask	0xf7  
#define TX_data_update_mask					0x7f
#define Heart_data_update_mask			0xef
#define GPS_data_update_mask			0xfe



 #define GPS_data_index						    0x01  //gps�����и���
 #define Temper_data_index						0x02   //�¶������и���
 #define UART_data_index							0x03   //�������������и���
 #define SPI_Flash_data_index					0x04   //spi�����ݸ���
 #define Heart_data__index						0x05   //����������
 #define TX_data_index							  0x06  //���ݴ����ɣ���Ҫ���� 
 #define  SPI_Flash_data_mask__index	0x07  
 #define TX_data_update_mask_index		0x08
#define  Heart_data_mask_index				0x09



void Modue_Run_Process(void);
unsigned char GetServer_Info_Poll(void);

#endif

