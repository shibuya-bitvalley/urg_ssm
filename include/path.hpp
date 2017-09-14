//-------データ配列定義-------//
typedef struct{
	double path_x[1000];	//目標経路0.1mごとのx位置[m]
	double path_y[1000];	//目標経路0.1mごとのy位置[m]
	double lane_x[1000];	//自車レーン0.1mごとのx位置[m]
	double lane_y[1000];	//自車レーン0.1mごとのy位置[m]
	double x_pos;
	double y_pos;
	double th_pos;
	int reliability;	//信頼度 0=認識不可 1=片側の車線境界線しか見えていない 2=両側の車線境界線が見えている
}PATH_DATA;
