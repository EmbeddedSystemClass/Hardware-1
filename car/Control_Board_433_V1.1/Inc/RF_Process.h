#ifndef RF_PROCESS_H_

#define RF_PROCESS_H_

typedef  struct{
	unsigned int len;
	unsigned int dev_sn;//�豸�����к�  
	unsigned int temperature;
}RF_INFO_DEF;//��ص�ѹ��GPSλ�����ݣ���������������433����ģ����յ����������ݵȱ�Ҫ��Ϣ

void RF_Process(void);
unsigned char RF_Param(unsigned char *pbuff, RF_INFO_DEF rf_info);

#endif
