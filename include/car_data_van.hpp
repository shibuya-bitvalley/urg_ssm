//オドメトリの原点は後輪の中央

//センサの原点は車両中央前方下(地面のとこ）
// ここにセンサの位置を書くときはセンサの原点からを書く
// DEPTHだけ(- FRONTBAMPER_REARWHEEL)してオドメトリの原点に合わせてある


//  !!!値はdouble型にすること!!!
/*

ーーーーーーーーーーーーーー---------------
|			|-      .		
|			|  -   . ←URG_ANGLE
|			|    -.
ーーーー○ーーーーー○ーー●          -
*/
#define URG_ANGLE 40.0		//道路に対してのURGの取付角度(deg)
#define URG_ANGLE2 -1.6		//水平のURGの取付角度(deg) 0が望ましい
#define URG_HEIGHT 1.420	//URGの取付高さ（垂直）
#define URG_WIDTH 1.626/2.0	//URGの原点からの横方向へのずれ(正面から車両を見て右側が正)
#define URG_DEPTH (-0.1 - FRONTBAMPER_REARWHEEL)	//URGの原点からの奥行き方向へのずれ(前方から後方へが正）

#define IBEO_ANGLE 0.0		//道路に対してのIBEOの取付角度(deg)
#define IBEO_HEIGHT 0.8		//IBEOの取付高さ（垂直）
#define IBEO_WIDTH 0.0		//IEBOの原点からの横方向へのずれ(正面から車両を見て右側が正)
#define IBEO_DEPTH (0.1 - FRONTBAMPER_REARWHEEL)	//IBEOの原点からの奥行き方向へのずれ(前方から後方へが正）

#define URGFL_ANGLE 25.0	//前方左のURGFLの角度
#define URGFL_HEIGHT 2.0	//URGFLの取付高さ（垂直）
#define URGFL_WIDTH 1.0		//URGFLの原点からの横方向へのずれ(正面から車両を見て右側が正)
#define URGFL_DEPTH (0.0 - FRONTBAMPER_REARWHEEL)		//URGFLの原点からの奥行き方向へのずれ(前方から後方へが正）

#define URGCL_ANGLE 90.0	//中央左のURGCLの角度
#define URGCL_HEIGHT 2.0	//URGCLの取付高さ（垂直）
#define URGCL_WIDTH 1.0		//URGCLの原点からの横方向へのずれ(正面から車両を見て右側が正)
#define URGCL_DEPTH (5.0 - FRONTBAMPER_REARWHEEL)		//URGCLの原点からの奥行き方向へのずれ(前方から後方へが正）

#define URGRL_ANGLE 135.0	//後方左のURGRLの角度
#define URGRL_HEIGHT 0.65	//URGRLの取付高さ（垂直）
#define URGRL_WIDTH 1.11	//URGRLの原点からの横方向へのずれ(正面から車両を見て右側が正)
#define URGRL_DEPTH (11.45 - FRONTBAMPER_REARWHEEL)	//URGRLの原点からの奥行き方向へのずれ(前方から後方へが正）

#define CAR_HEIGHT 1.990	//車高
#define CAR_WIDTH 1.695		//全幅
#define CAR_DEPTH 4.695		//全長


#define WHEELBASE 2.555		//ホイールベース
#define FRONT_TREAD 1.475	//フロントトレッド
#define REAR_TREAD 1.450	//リアトレッド
#define FRONTBAMPER_REARWHEEL 3.5	//センサの原点（フロントバンパー）から後輪（後4輪の中心・オドメトリの中心）までの距離

//---------取付データ---------
const double cos_urg_angle = cos((double)URG_ANGLE/180.0*M_PI);
const double sin_urg_angle = sin((double)URG_ANGLE/180.0*M_PI);
const double cos_urg_angle2 = cos((double)URG_ANGLE2/180.0*M_PI);
const double sin_urg_angle2 = sin((double)URG_ANGLE2/180.0*M_PI);
const double cos_ibeo_angle = cos((double)IBEO_ANGLE/180.0*M_PI);
const double sin_ibeo_angle = sin((double)IBEO_ANGLE/180.0*M_PI);

const double cos_urgfl_angle = cos((double)URGFL_ANGLE/180.0*M_PI);
const double sin_urgfl_angle = sin((double)URGFL_ANGLE/180.0*M_PI);
const double cos_urgcl_angle = cos((double)URGCL_ANGLE/180.0*M_PI);
const double sin_urgcl_angle = sin((double)URGCL_ANGLE/180.0*M_PI);
const double cos_urgrl_angle = cos((double)URGRL_ANGLE/180.0*M_PI);
const double sin_urgrl_angle = sin((double)URGRL_ANGLE/180.0*M_PI);

