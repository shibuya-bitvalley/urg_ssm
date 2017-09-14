//高速道路
static const double ROAD_WIDTH = 3.5;
//日野テストコース
//static const double ROAD_WIDTH = 4.0;
//JARI
//static const double ROAD_WIDTH = 3.7;

//-------データ配列定義-------//
typedef struct{
	double xx[3000];	/* 距離データ[m] */
	double yy[3000];		/* x座標[m] */
	double zz[3000];		/* y座標[m] */
	int intensity[3000];
	double x_pos;
	double y_pos;
	double th_pos;
}SAMPLED_DATA_URGC;

//一時格納用配列
typedef struct{
	double x;	/* 距離データ[m] */
	double y;		/* x座標[m] */
	double kai[5];	/* 三次近似曲線の係数 */
	
	double odometry_x;
	double odometry_y;
	double odometry_th;
}LANE;

// レーンデータの構造体
typedef struct{
	LANE left2;	//左車線のデータ
	LANE left;	//左車線のデータ
	LANE right;	//右車線のデータ
	LANE right2;	//右車線のデータ
}LANES;

// オドメトリレーンデータの構造体
typedef struct{
	LANE odometry;	//左車線のデータ
	LANE left;	//左車線のデータ
	LANE right;	//右車線のデータ
	LANE left2;	//右車線のデータ
	LANE right2;	//右車線のデータ
}LANES_ODOMETRY;

// オドメトリレーンデータの構造体
typedef struct{
	double distance;
	double ESTIMATE_MAX_RANGE;
}LANE_INFO;

typedef struct{
	int hayashi[3000];
}HAYASHI;

// SSM用レーンデータ
typedef struct{
	LANES real[50];
	LANES estimate50[50];
	LANES estimate100[50];
	LANE_INFO data_l2;
	LANE_INFO data_l;
	LANE_INFO data_r;
	LANE_INFO data_r2;
	SAMPLED_DATA_URGC plot_sampleddata_urgc;
	HAYASHI hayashi;
	
	int left_change;	//左への車線変更が認識されると1増える
	int right_change;	//右への車線変更が認識されると1増える
	
	double left_change_time;	//左へ最新の車線変更した時のSSM時間
	double right_change_time;	//右へ最新の車線変更した時のSSM時間
}LANE_DATA;
	
