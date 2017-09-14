//-------データ配列定義-------//
typedef struct{
	double x[1000];	//0.1mごとのx位置[m]
	double y[1000];	//0.1mごとのy位置[m]
	int reliability;	//信頼度 0=認識不可 1=片側の車線境界線しか見えていない 2=両側の車線境界線が見えている
	double width;	//道路幅
}LANE_RESULT_DATA;

typedef struct{
	LANE_RESULT_DATA left;
	LANE_RESULT_DATA center;
	LANE_RESULT_DATA right;
	double x_pos;
	double y_pos;
	double th_pos;
	int type;	//1:URG 2:ZMP
	
	int left_change;	//左への車線変更が認識されると1増える
	int right_change;	//右への車線変更が認識されると1増える
	
	double left_change_time;	//左へ最新の車線変更した時のSSM時間
	double right_change_time;	//右へ最新の車線変更した時のSSM時間
}LANE_RESULT;


// 1m = 100cm
// 100m = 10000cm
