////////////////////////////////////////////////////////////////////////
//                         SST25VF016B��������
//1.Ĭ��ʹ��SPI1����ͨ�ţ�ʱ���ϱ������棬����ǰ���г�ʼ��
//2.STM32��汾v3.5
//3.Ƭѡ�ܽ�NSS����ǰ��ʼ��Ϊ�ߵ�ƽ��ͬʱ����SelectDevice��UnselectDevice����
////////////////////////////////////////////////////////////////////////

#include "stm32l1xx.h"
#include "sst25vf016b.h"
typedef unsigned char  u8;
typedef unsigned int    u16;
typedef uint32_t         u32;
extern SPI_HandleTypeDef hspi1;
#define SelectDevice  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
#define UnselectDevice HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET);
////////////SPI2�ӿ�����/////////////////////////////////////////////////////
void FLASH_SPI_Configuration(void)
{

//		  SPI_InitTypeDef  SPI_InitStructure;
//	  GPIO_InitTypeDef GPIO_InitStructure;
//    //SPI_Rcc_Configuration();
//	  RCC_APB1PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
//	  RCC_APB1PeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7|GPIO_Pin_6 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
//	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);



//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
//	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	  
//  	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4  ;
//	  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//				GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
//	  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	  GPIO_Init(GPIOC, &GPIO_InitStructure);  
//	  GPIO_SetBits(GPIOC, GPIO_Pin_4);
//	  /* SPI2 Config -------------------------------------------------------------*/
//	
//	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //SPI_Direction_2Lines_FullDuplex;
//		//SPI_InitStructure.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
//	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
//		//SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
//	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	  SPI_InitStructure.SPI_CRCPolynomial = 7;
//	  SPI_Init(SPI1, &SPI_InitStructure);

//	  SPI_Cmd(SPI1, ENABLE);
}
u8 SPISend(u8 data)
{
	uint8_t RxData;

	HAL_SPI_TransmitReceive(&hspi1,&data, &RxData, 1, 10);
	return RxData;
}

////////////////////////////////////////////////////
//��ȡ״̬�Ĵ���ֵ
//���ض�ȡֵ
u8 SST25VF016B_ReadReg(void)
{
	u8 Register;
	SelectDevice;
	SPISend(0x05);
	Register=SPISend(0x00);
	UnselectDevice;
	return (Register);
}

/////////////////////////////////////////////////////
//æ���
//����æ״̬��0�����У�!0��æ
u8 SST25VF016B_IsBusy(void)
{
	u8 Busy,Register;
	Register=SST25VF016B_ReadReg();
	Busy=Register & 0x01;
	return (Busy);
}

////////////////////////////////////////////////////////////
//��ȡ����
//u32 Address - ��ʼ��ַ����Χ��0x000000-0x1fffff
//u8 *Data - ��ȡֵ
//u32 nCount - ��ȡ�ֽ���
void SST25VF016B_Read(u8 *Data,u32 Address,u32 nCount)
{
	u32 temp;
	while(SST25VF016B_IsBusy());
	SelectDevice;
	SPISend(0x0b);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	SPISend(0);
	for(temp=0;temp<nCount;temp++)
	{
		Data[temp]=SPISend(0x00);
	}
	UnselectDevice;
}

////////////////////////////////////////////////////
//дʹ��
void SST25VF016B_WriteEnable(void)
{
	SelectDevice;
	SPISend(0x06);
	SPISend(0x50);
	SPISend(0x01);
	SPISend(0x02);
	UnselectDevice;
}

////////////////////////////////////////////////////
//4KB�����
//u32 Address - ��ʼ��ַ����Χ��0-511
void SST25VF016B_4KBErase(u32 Address)
{
	Address<<=12;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x20);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	UnselectDevice;
	while(SST25VF016B_IsBusy());
}

////////////////////////////////////////////////////
//32KB�����
//u32 Address - ��ʼ��ַ����Χ��0-63
void SST25VF016B_32KBErase(u32 Address)
{
	Address<<=15;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x52);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	UnselectDevice;
	while(SST25VF016B_IsBusy());
}

////////////////////////////////////////////////////
//64KB�����
//u32 Address - ��ʼ��ַ����Χ��0-31
void SST25VF016B_64KBErase(u32 Address)
{
	Address<<=16;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0xd8);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	UnselectDevice;
	while(SST25VF016B_IsBusy());
}

////////////////////////////////////////////////////
//��Ƭ����
void SST25VF016B_AllErase(void)
{
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x60);
	UnselectDevice;
	while(SST25VF016B_IsBusy());	
}

////////////////////////////////////////////////////
//д������
//u32 Address - ��ʼ��ַ����Χ��0x000000-0x1fffff
//u8 *Data - ��дֵ
//u32 nCount - д���ֽ���
void SST25VF016B_Write_Page(u8 *Data,u32 Address,u32 nCount)
{
	u32 temp;
	SST25VF016B_WriteEnable();
	SelectDevice;
	SPISend(0x02);
	SPISend((Address&0xffffff)>>16);
	SPISend((Address&0xffff)>>8);
	SPISend(Address&0xff);
	for (temp=0;temp<nCount;temp++)
	{
		SPISend(Data[temp]);
		Address++;	
	}
	UnselectDevice;
	while(SST25VF016B_IsBusy());	
}


//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���0x1fffff)
//CHECK OK
void SST25VF016B_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=4096-WriteAddr%4096; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SST25VF016B_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>4096)pageremain=4096; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 


//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���0x1fffff)   
u8 SST25VF016B_BUFFER[4096];	
u8 WriteAddressPostion[3];
void SST25VF016B_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * SST25VF016B_BUF;	  
   	SST25VF016B_BUF=SST25VF016B_BUFFER;	
	 	secpos=WriteAddr/4096;//������ַ  
		secoff=WriteAddr%4096;//�������ڵ�ƫ��
		secremain=4096-secoff;//����ʣ��ռ��?
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SST25VF016B_Read(SST25VF016B_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SST25VF016B_BUFFER[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SST25VF016B_4KBErase(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SST25VF016B_BUF[i+secoff]=pBuffer[i];	  
			}
			SST25VF016B_Write_NoCheck(SST25VF016B_BUFFER,secpos*4096,4096);//д����������  

		}else SST25VF016B_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}

void Read_AddressWrite()
{
	SST25VF016B_Read(WriteAddressPostion,0,3);
}
void SPI_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
   u32 tmp;
    SST25VF016B_Read(WriteAddressPostion,0,3);
   if(WriteAddressPostion[0]==0xff&&WriteAddressPostion[1]==0xff&&
   	 WriteAddressPostion[2]==0xff)//�жϸõ�ַ�ռ��Ƿ�д��
  	 tmp = 	 NumByteToWrite;
   else
   	tmp = WriteAddressPostion[0]<<16+ WriteAddressPostion[1]<<8+ 
   	     WriteAddressPostion[2] + NumByteToWrite ;
   if(tmp>=0x200000)
   {
   
	tmp = tmp - 0x200000 + 3;
   	WriteAddressPostion[0]=tmp>>16;
   	WriteAddressPostion[1]=tmp>>8;
   	WriteAddressPostion[2]=tmp;  
   }
   else
   {
   	WriteAddressPostion[0]=tmp>>16;
   	WriteAddressPostion[1]=tmp>>8;
   	WriteAddressPostion[2]=tmp; 
   }
   SST25VF016B_Write(WriteAddressPostion,0,3)  ;//��ַ0---2�洢��ǰflash�п����õ�ַ�ռ���׵�ַ
   SST25VF016B_Write( pBuffer, WriteAddr, NumByteToWrite)  ; 	//WriteAddr��ַ��Χ0x000003---0x1ffffff
    
}
/****************************************************************************
* ��    �ƣ�void FlashReadID(void)
* ��    �ܣ��������뼰�ͺŵĺ���
* ��ڲ�����  
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
unsigned char fac_id,dev_id;

void FlashReadID(unsigned char fac_id,unsigned char dev_id1,unsigned char dev_id2)
{
 // while(SST25VF016B_IsBusy());
  SelectDevice;
  SPISend(0x9f);
	//SPISend(0x00);
	//SPISend(0x00);
	//SPISend(0x00);
  fac_id= SPISend(0x00);;		          //BFH: ������SST
	dev_id= SPISend(0x00);;	              //41H: �����ͺ�SST25VF016B     
	dev_id= SPISend(0x00);;	              //H: �����ͺ�SST25VF016B   	
	UnselectDevice;
}
