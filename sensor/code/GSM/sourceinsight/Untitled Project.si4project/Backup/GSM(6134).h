#ifndef GSM_H
#define GSM_H
#define Ser_Head 0xfdfd
#define Dev_Head 0xfefe

#define SIMCOM_TIME_OUT					20

#define buffer_size  256

typedef struct
{
uint32_t 	rx_len;
uint32_t 	rx_len_var;
unsigned char 	receive_flag;
unsigned int      real_index;
unsigned int      loop_index;
unsigned char	UsartReceiveData[buffer_size];

}Uart_Types;


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
#define	SIMCOM_NetClose_MODE				9  	//����͸��ģʽ
#define	SIMCOM_CIPClose_MODE                            10

#define	SIMCOM_NetOpen_READY				11     //net open
#define	SIMCOM_NET_OK						12 	//���ӳɹ�
#define	SIMCOM_NET_OK2     13
#define	SIMCOM_NET_ERROR					0XFF	//����
#endif
