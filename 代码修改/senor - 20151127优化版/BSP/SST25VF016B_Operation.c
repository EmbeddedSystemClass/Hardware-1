////////////////////////////////////////////////////////////////////////
//                         SST25VF016B��������
//1.Ĭ��ʹ��SPI1����ͨ�ţ�ʱ���ϱ������棬����ǰ���г�ʼ��
//2.STM32��汾v3.5
//3.Ƭѡ�ܽ�NSS����ǰ��ʼ��Ϊ�ߵ�ƽ��ͬʱ����SelectDevice��UnselectDevice����
////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#define SelectDevice GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#define UnselectDevice GPIO_SetBits(GPIOA, GPIO_Pin_4);

u8 SPISend(u8 data)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)!=SET);
	SPI_I2S_SendData(SPI1, data);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)!=SET);
	return(SPI_I2S_ReceiveData(SPI1));	
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
void SST25VF016B_Read(u32 Address,u8 *Data,u32 nCount)
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
void SST25VF016B_Write(u32 Address,u8 *Data,u32 nCount)
{
	u32 temp;
	for (temp=0;temp<nCount;temp++)
	{
		SST25VF016B_WriteEnable();
		SelectDevice;
		SPISend(0x02);
		SPISend((Address&0xffffff)>>16);
		SPISend((Address&0xffff)>>8);
		SPISend(Address&0xff);
		SPISend(Data[temp]);
		UnselectDevice;	
		Address++;
		while(SST25VF016B_IsBusy());
	}
}}

/****************************************************************************
* ��    �ƣ�void FlashReadID(void)
* ��    �ܣ��������뼰�ͺŵĺ���
* ��ڲ�����  
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
unsigned char fac_id,dev_id;

void FlashReadID(void)
{
	while(SST25VF016B_IsBusy());
	SelectDevice;
  	SPISend(0x90);
	SPISend(0x00);
	SPISend(0x00);
	SPISend(0x00);
  	fac_id= SPISend(0x00);;		          //BFH: ������SST
	dev_id= SPISend(0x00);;	              //41H: �����  	fac_id= SPISend(0x00);;		          //BFH: ������SST
	dev_id= SPISend(0x00);;	              //41H: �����ͺ�SST25VF016B     
	UnselectDev	while(SST25VF016B_IsBusy());
	SelectDevice;
  	SPISend(0x90);
	SPISend(0x00);
	SPISend(0x00);
	SPISend(0x00);
  	fac_id= SPISend(0x00);;		          //BFH: ������SST
	dev_id= SPISend(0x00);;	              //41H: �����ͺ�SST25VF016B     
	UnselectDevice;
}

