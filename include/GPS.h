// GPSデータ構造体
typedef struct
{
	short utc_h;	// UTC時刻[h]
	short utc_m;	// UTC時刻[m]
	float utc_s;	// UTC時刻[s]
	
	double lat;		// 緯度 ddmm.mmm
	double lng;		// 経度 dddmm.mmm
	double alt;		// 標高[m]
	double alt_g;	// ジオイド高度[m]
	
	short status;	// GPSステータス(0:受信不能、1:単独測位、2:DGPS)
	short stl_num;	// 受信衛星数
	float hdop;		// 水平精度劣化指数(小さい方が良い)

	double t_deg;	// 真北に対する進行方向（度）
	double speed;	// 対地速度（km/h）
	
	char GPGGA[128];	// GPGGAメッセージの生データ
	char GPVTG[128];	// GPGGAメッセージの生データ
} gps_t;
