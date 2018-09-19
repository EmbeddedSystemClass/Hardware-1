#include "stm32f1xx_hal.h"
#include "CRC.h"

/******************************************************************************/
unsigned int calc_crc(const unsigned char *buf, const unsigned int len)
{
  unsigned int  i;
  unsigned int cksum;
  unsigned int crc16_tab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
  };
  cksum = 0;
  for (i = 0;  i < len;  i++) {
    cksum = crc16_tab[((cksum>>8) ^ *buf++) & 0xFF] ^ (cksum << 8);
  }
  return cksum;
}
unsigned int sum_crc=0;
unsigned char tmp,crc_result,ll;
uint32_t i;
unsigned int CRC_decode(unsigned char *payload_buf,unsigned char CRC_len)
{
  ll=CRC_len;
  i=0;
  sum_crc=0;
  if(CRC_len>=2)
  {
    crc_init(&sum_crc);	
    for(i=0;i<(CRC_len);i++) //CRC_len=11
    {
   
      //uart_tx(2,&payload_buf[i],1);     
      crc_accumulate((payload_buf+i),&sum_crc);//CRCУ��
   // uart1_tx(payload_buf+i,1);
    }
    tmp=MSG_CRC;
    crc_accumulate(&tmp,&sum_crc);
	return sum_crc;
    //crc_result=sum_crc;
    /*if((payload_buf[CRC_len-1]==(sum_crc&0x00ff))&&(payload_buf[CRC_len-2]==(sum_crc>>8)))
      crc_result = 1;
    else
      crc_result = 0;*/ 
  }
  
  return crc_result;
}
void CRC_cal(unsigned char *payload_buf,unsigned char *crc_pbuffer,unsigned char CRC_len)
{
  unsigned char i;
  unsigned int sum_crc=0;
  unsigned char tmp; 
  crc_init(&sum_crc);
  payload_buf[CRC_len]=MSG_CRC;
  for(i=0;i<(CRC_len+1);i++)
  {
    crc_accumulate((payload_buf+i),&sum_crc);//CRCУ��
  }
  tmp = sum_crc&0xff;	
  payload_buf[CRC_len]=tmp;
  tmp = sum_crc>>8;		
  payload_buf[CRC_len+1]=tmp;  
}
/**
* @brief Accumulate the X.25 CRC by adding one char at a time.
*
* The checksum function adds the hash of one char at a time to the
* 16 bit checksum (uint16_t).
*
* @param data new char to hash
* @param crcAccum the already accumulated checksum
**/
void crc_accumulate(unsigned char *data, unsigned int *crcAccum)
{
  /*Accumulate one byte of data into the CRC*/
  unsigned char tmp;
  
  tmp = (*data) ^ (unsigned char)(*crcAccum &0xff);
  tmp ^= (tmp<<4);
  *crcAccum = (*crcAccum>>8) ^ (tmp<<8) ^ (tmp <<3) ^ (tmp>>4);
}
/********************************************************************************/
/**
* @brief Initiliaze the buffer for the X.25 CRC
*
* @param crcAccum the 16 bit X.25 CRC
*/
void crc_init(unsigned int* crcAccum)
{
  *crcAccum = X25_INIT_CRC;
}
/********************************************************************************/
/**
* @brief Calculates the X.25 checksum on a byte buffer
*
* @param  pBuffer buffer containing the byte array to hash
* @param  length  length of the byte array
* @return the checksum over the buffer bytes
**/
unsigned int crc_calculate(unsigned char *pBuffer, unsigned int length)
{
  unsigned int crcTmp=0,i;
  unsigned char tmp;
  crc_init(&crcTmp);
//  while (length--) 
//  {
//    uart1_tx(pBuffer,1);
//    crc_accumulate(pBuffer++, &crcTmp);
//  }
  
     crc_init(&sum_crc);	
    for(i=0;i<(length);i++) //CRC_len=11
    {
         //   uart_tx(2,&(pBuffer[i]),1);
      crc_accumulate(&(pBuffer[i]), &crcTmp);

    }
  //  tmp=MSG_CRC;
 //   crc_accumulate(&tmp,&crcTmp); 
  
  
  
  return crcTmp;
}

