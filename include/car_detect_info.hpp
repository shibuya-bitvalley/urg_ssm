//車両検出情報構造体
typedef struct
{
	bool area_detect;
	double range_dist;
	//int car_num;
	//double x[10];
	//double y[10];
	//dist[10];
	//double vel[10];
	//double rel_vel[10];
} CAR_DETECT_INFO;


typedef struct
{
	double x[200];
	double y[200];
	double z[200];
} AREA_DATA;
