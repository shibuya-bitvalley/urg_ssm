// 滑り角計のデータ
typedef struct
{
	double slip_angle;	//すべり角[deg/s]
	double speed;		//車速[km/h]
	double count_time;	//カウントタイム（データが送られるごとに増える。データの値には意味がない）
} SLIP_ANGLE;

