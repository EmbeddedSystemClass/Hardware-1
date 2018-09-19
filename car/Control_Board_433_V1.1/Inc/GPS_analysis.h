#ifndef GPS_ANALISIS_H_
#define GPS_ANALISIS_H_
typedef struct
{
	unsigned char year_Hbyte;
	unsigned char year_Lbyte;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
} DATE_TIME;

/*typedef  struct{
    double latitude; //����
    double longitude; //γ��
    int latitude_Degree;    //��
    int        latitude_Cent;   //��
    int     latitude_Second; //��
    int longitude_Degree;    //��
    int        longitude_Cent;  //��
    int     longitude_Second; //��
    float     speed; //�ٶ�
    float     direction; //����
    float     height; //���θ߶�
    int satellite;
    unsigned int len;
    unsigned char     NS;
    unsigned char     EW;
    DATE_TIME D;
}GPS_INFO;*/




typedef  struct
{
	//double latitude; //����
	//double longitude; //γ��
	unsigned char latitude_Degree;    //��
	unsigned char        latitude_Cent;   //��
	unsigned char    latitude_Second; //��
	unsigned char longitude_Degree;    //��
	unsigned char        longitude_Cent;  //��
	unsigned char     longitude_Second; //��
	unsigned char     speed_Hbyte; //�ٶ�
	unsigned char     speed_Lbyte; //�ٶ�
	unsigned char     speed_Dex; //�ٶ�
         unsigned char    direction_Degree_H;    //��
        unsigned char    direction_Degree_L;    //��
	unsigned char    direction_Cent;   //��
	unsigned char    direction_Second; //��	
	unsigned char     height_Hbyte; //����
	unsigned char     height_Lbyte; //����
	// float     height; //���θ߶�
	unsigned char     satellite; //����
	unsigned char     len_Hbyte; //���ȸ��ֽ�
	unsigned char     len_Lbyte; //���ȵ��ֽ�
	unsigned char     NS;
	unsigned char     EW;
	DATE_TIME D;
}GPS_INFO;
int GPS_RMC_Parse(unsigned char *line, GPS_INFO *GPS,char gps_format);
#endif
