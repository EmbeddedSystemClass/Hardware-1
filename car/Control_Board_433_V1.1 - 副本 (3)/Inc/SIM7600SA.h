#ifndef SIM7600SA_H_
#define SIM7600SA_H_
#include "stm32f4xx_hal.h"
#include "GPS_analysis.h"

#define Ser_Head 0xfdfd
#define Dev_Head 0xfefe

#define SIMCOM_TIME_OUT					20


//����ע��״̬


//����ע��״̬


#define	SIMCOM_NET_NOT 						0    	//����δ���
#define	SIMCOM_READY_YES					1     //�������
#define	SIMCOM_Echo_OFF						2   	//�رջ���
#define	SIMCOM_NORMAL_MODE				3  	//������������ģʽ
#define	SIMCOM_SIMCARD_READY			4  	//���sim���ɹ�
#define	SIMCOM_Network_Intensity_READY		5  	//�ź�����Ҫ��
#define	SIMCOM_GPRS_READY					6     //gprsע��ɹ�
#define	SIMCOM_START_GPS					7  	//����GPS
#define	SIMCOM_TRANSPERENT_MODE				8  	//����͸��ģʽ
#define	SIMCOM_NetOpen_READY				9     //net open
#define	SIMCOM_NET_OK						10 	//���ӳɹ�
#define	SIMCOM_NET_OK2     11
#define	SIMCOM_NET_ERROR					0XFF	//����



typedef struct
{
	unsigned char *data_buff;
	uint32_t  len;
	uint32_t Raddress;
	uint32_t index;

} flash_data_def;

typedef struct
{
	unsigned int  server_head;
	uint32_t  len;
	unsigned char function_status;      //���ò�����0x01;��ѯ����:0x10
	unsigned char dev_sn[12];              //�豸�����к�
	unsigned char *server_IP;			  //������IP��ַ
	//unsigned int  server_port;          //�������˿�
	unsigned char gps_time_interval;    //�ɼ����
	unsigned char rf_dev_sn[4]; 		  //�����豸��
	unsigned char rf_time_interval;	  //���߲���Ƶ��
	unsigned int  crc_value;
} device_com_down;

typedef struct
{
	unsigned int  device_head;

	unsigned long int  len;      			//���ݳ���

	unsigned int   lte_heart;             //��Ч��������0x5aa5������ֵΪҵ������

	unsigned char *gps_pos_info;          //gps��Ϣ
	unsigned int  *tempature;              //�¶���Ϣ��dev_sn(2bytes)+temperature(2bytes)...
	unsigned int  battery_voltage;        //��ص�ѹ


	unsigned char dev_sn[12];//�豸�����к�
	unsigned char *server_IP;	//������IP��ַ
	unsigned int  server_port;//�������˿�
	unsigned char gps_time_interval;//�ɼ����
	unsigned char rf_time_interval;//���߲���Ƶ��
	unsigned int  crc_value;
} device_com_up;




void LTE_M_init(void);
unsigned char Get_Network_status(void);

unsigned char  SIMCOM_GetGPS_Info(unsigned char * buff,GPS_INFO *gps,unsigned char gps_format,unsigned int waittime);
void SIMCOM_Reconnect_status(void);
signed char  SIMCOM_Get_QuitTransparent_Staus(unsigned int waittime);
signed char  SIMCOM_Get_TO_DataMode_Staus(unsigned int waittime);
void SIMCOM_Register_Network(void);
unsigned char Get_Network_Thres(void);
void Set_Network_status(void);
void  Server_IP_Config(unsigned char * pbuff);



#endif
