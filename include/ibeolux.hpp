//@Ibeo data headerÌ\¢Ì
//ネットワークバイトオーダーになっているので注意！
// 値を使うときは 4byte:ntohl() 2byte:ntohs() でホスト・バイトオーダーに変換すること
typedef struct{
	unsigned int magic_word;
	unsigned int size_pmsg;
	unsigned int size_tmsg;
	unsigned char none;
	unsigned char device_id;
	unsigned short data_type;
	unsigned long long ntp_time;
}IBEO_DATA_HEADER;

//@ibeo LUX scan data: Data type 0x2202Ì\¢Ì
typedef struct{
	unsigned short scan_number;
	unsigned short status;
	unsigned short offset;
	unsigned long long s_ntp;
	unsigned long long e_ntp;
	unsigned short ticks_per_rotation;
	short s_angle;
	short e_angle;
	unsigned short scan_points;
	short yaw;
	short pitch;
	short roll;
	short x;
	short y;
	short z;
	short none;
}IBEO_SCAN_DATA;

// Array of scan points
typedef struct{
	unsigned char layer;
	unsigned char flags;
	short h_angle;
	unsigned short radial_distance;
	unsigned short echo_pulse_width;
	unsigned short none;
}IBEO_SCAN_POINTS;

// Array of scan Data for SSM
typedef struct{
	IBEO_SCAN_DATA scandata;
	IBEO_SCAN_POINTS scanpoints[2000];	//2000個データがあるとは限らない。scandata.scan_pointsで数を確認すること。
}IBEO_DATA_SSM;
