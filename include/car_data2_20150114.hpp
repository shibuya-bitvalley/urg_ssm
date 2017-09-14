#ifndef __CARDATA_H__
#define __CARDATA_H__

#include <cmath>
//オドメトリの原点は後輪の中央

//センサの原点は車両中央前方下(地面のとこ）
// ここにセンサの位置を書くときはセンサの原点からを書く
// DEPTHだけ(- FRONTBAMPER_ODOMETRY)してオドメトリの原点に合わせてある


/*

ーーーーーーーーーーーーーー---------------
|			|-      .		
|			|  -   . ←URG_ANGLE
|			|    -.
ーーーー○ーーーーー○ーー●          -

//座標系

		↑
       ←

    ーーーーーーー
    |           |		
       ○      前        ○
    |           |		
    |           |		
    |           |		
    |           |		
    |           |		
    |	 上     |		
    |           |		
    |           |		
    |           |		
    |           |		
       ○      後        ○
    |           |		
    |           |		
    |           |		
    ーーーーーーー

センサ位置はDEPTHは
(オドメトリ中心からフロントバンパーまでの距離)+(フロントバンパーからセンサまでの距離）
で記述されている
*/

//プログラム中で変更出来るようにあえてconstをつけていない...

//空車時の車両諸元データ
const double m=11885;	//車両重量(kg)
const double I=148810;	//ヨーイング慣性モーメント(kgm2)
const double a=2.58;	//前輪〜重心点距離(m)
const double b=3.324;	//後輪〜重心点距離(m)
const double n=23;	//ステアリング系トータルギヤ比
const double Kf=292971;	//前輪コーナーリングパワー(N/rad)
const double Kr=495218;	//後輪コーナーリングパワー(N/rad)
const double g=9.80665; //重力定数
const double cf=Kf;
const double cr=Kr;
const double Sf = 0.012;	//スタビリティファクタ


double CAR_HEIGHT = 3.34;		//車高
double CAR_WIDTH = 2.49;		//全幅
double CAR_DEPTH = 11.99;		//全長

double WHEELBASE = 5.905;		//ホイールベース
double FRONT_TREAD = 2.055;	//フロントトレッド
double REAR_TREAD = 1.855;	//リアトレッド

//センサ取り付けデータ

//オドメトリの中心は重心位置！
double FRONTBAMPER_ODOMETRY = 8.2-b;	//オドメトリ中心からフロントバンパーまでの距離

//ZMPCAM
double ZMPCAM_HEIGHT = 2.1;
double ZMPCAM_WIDTH = 0.0;
double ZMPCAM_DEPTH = FRONTBAMPER_ODOMETRY - 0.1;

//車線認識用URG
double URG_ANGLE = 56.0;		//道路に対しての取付角度(deg)
double URG_ANGLE2 = 0.8;		//水平のURGの取付角度(deg) 0が望ましい
double URG_HEIGHT = 1.70;	//URGの取付高さ（垂直）
double URG_WIDTH = 1.26;		//オドメトリ原点からの横方向へのずれ
double URG_DEPTH = FRONTBAMPER_ODOMETRY + 0.3;	//オドメトリ原点から前方向へのずれ

//前方IBEO
double IBEO1_ANGLE = 0.0;		//横前後方向取付角度(deg)
double IBEO1_ANGLE2 = 0;		//前後方向取付角度(deg)
double IBEO1_HEIGHT = 0.61;	//IBEOの取付高さ（垂直）
double IBEO1_WIDTH = 0.0;		//オドメトリ原点からの横方向へのずれ
double IBEO1_DEPTH = FRONTBAMPER_ODOMETRY - 0.0;		//オドメトリ原点から前方向へのずれ

//後方IBEO
double IBEO2_ANGLE = 180;	//横方向取付角度(deg)
double IBEO2_ANGLE2 = 0;		//前後方向取付角度(deg)
double IBEO2_HEIGHT = 0.7;	//IBEOの取付高さ（垂直）
double IBEO2_WIDTH = 0.0;		//オドメトリ原点からの横方向へのずれ
double IBEO2_DEPTH = FRONTBAMPER_ODOMETRY - 11.9;		//オドメトリ原点から前方向へのずれ

//前左URG
double URGFL_ANGLE = 42.0;	//横方向取付角度(deg)
double URGFL_ANGLE2 = 0;	//前後方向取付角度(deg)
double URGFL_HEIGHT = 0.81;	//URGFLの取付高さ（垂直）
double URGFL_WIDTH = 1;	//オドメトリ原点からの横方向へのずれ
double URGFL_DEPTH = FRONTBAMPER_ODOMETRY - 0.1;		//オドメトリ原点から前方向へのずれ

//前右URG
double URGFR_ANGLE = -50.0;	//横方向取付角度(deg)
double URGFR_ANGLE2 = 0;	//前後方向取付角度(deg)
double URGFR_HEIGHT = 0.81;	//URGFLの取付高さ（垂直）
double URGFR_WIDTH = -1;	//オドメトリ原点からの横方向へのずれ
double URGFR_DEPTH = FRONTBAMPER_ODOMETRY - 0.1;		//オドメトリ原点から前方向へのずれ

//中央左URG
double URGCL_ANGLE = 92.0;	//横方向取付角度(deg)
double URGCL_ANGLE2 = 0;	//前後方向取付角度(deg)
double URGCL_HEIGHT = 0.95;	//URGCLの取付高さ（垂直）
double URGCL_WIDTH = 1;	//オドメトリ原点からの横方向へのずれ(正面から車両を見て右側が正)
double URGCL_DEPTH = FRONTBAMPER_ODOMETRY - 5.8;		//オドメトリ原点から前方向へのずれ

//後ろ左URG
double URGRL_ANGLE = 181.0;	//横方向取付角度(deg)
double URGRL_ANGLE2 = 0;	//前後方向取付角度(deg)
double URGRL_HEIGHT = 0.7;	//URGRLの取付高さ（垂直）
double URGRL_WIDTH = 0.7;	//オドメトリ原点からの横方向へのずれ(正面から車両を見て右側が正)
double URGRL_DEPTH = FRONTBAMPER_ODOMETRY - 11.85;	//オドメトリ原点から前方向へのずれ

//後ろ右URG
double URGRR_ANGLE = -128.0;	//横方向取付角度(deg)
double URGRR_ANGLE2 = 0;	//前後方向取付角度(deg)
double URGRR_HEIGHT = 0.76;	//URGRLの取付高さ（垂直）
double URGRR_WIDTH = -1.07;	//オドメトリ原点からの横方向へのずれ(正面から車両を見て右側が正)
double URGRR_DEPTH = FRONTBAMPER_ODOMETRY - 11.6;	//オドメトリ原点から前方向へのずれ


//---------取付データ---------
#define  cos_urg_angle (cos(URG_ANGLE/180.0*M_PI))
#define  sin_urg_angle (sin(URG_ANGLE/180.0*M_PI))
#define  cos_urg_angle2 (cos(URG_ANGLE2/180.0*M_PI))
#define  sin_urg_angle2 (sin(URG_ANGLE2/180.0*M_PI))

#define  cos_ibeo1_angle (cos(IBEO1_ANGLE/180.0*M_PI))
#define  sin_ibeo1_angle (sin(IBEO1_ANGLE/180.0*M_PI))
#define  cos_ibeo1_angle2 (cos(IBEO1_ANGLE2/180.0*M_PI))
#define  sin_ibeo1_angle2 (sin(IBEO1_ANGLE2/180.0*M_PI))

#define  cos_ibeo2_angle (cos(IBEO2_ANGLE/180.0*M_PI))
#define  sin_ibeo2_angle (sin(IBEO2_ANGLE/180.0*M_PI))
#define  cos_ibeo2_angle2 (cos(IBEO2_ANGLE2/180.0*M_PI))
#define  sin_ibeo2_angle2 (sin(IBEO2_ANGLE2/180.0*M_PI))

#define  cos_urgfl_angle (cos(URGFL_ANGLE/180.0*M_PI))
#define  sin_urgfl_angle (sin(URGFL_ANGLE/180.0*M_PI))
#define  cos_urgfl_angle2 (cos(URGFL_ANGLE2/180.0*M_PI))
#define  sin_urgfl_angle2 (sin(URGFL_ANGLE2/180.0*M_PI))

#define  cos_urgfr_angle (cos(URGFR_ANGLE/180.0*M_PI))
#define  sin_urgfr_angle (sin(URGFR_ANGLE/180.0*M_PI))
#define  cos_urgfr_angle2 (cos(URGFR_ANGLE2/180.0*M_PI))
#define  sin_urgfr_angle2 (sin(URGFR_ANGLE2/180.0*M_PI))

#define  cos_urgcl_angle (cos(URGCL_ANGLE/180.0*M_PI))
#define  sin_urgcl_angle (sin(URGCL_ANGLE/180.0*M_PI))
#define  cos_urgcl_angle2 (cos(URGCL_ANGLE2/180.0*M_PI))
#define  sin_urgcl_angle2 (sin(URGCL_ANGLE2/180.0*M_PI))

#define  cos_urgrl_angle (cos(URGRL_ANGLE/180.0*M_PI))
#define  sin_urgrl_angle (sin(URGRL_ANGLE/180.0*M_PI))
#define  cos_urgrl_angle2 (cos(URGRL_ANGLE2/180.0*M_PI))
#define  sin_urgrl_angle2 (sin(URGRL_ANGLE2/180.0*M_PI))

#define  cos_urgrr_angle (cos(URGRR_ANGLE/180.0*M_PI))
#define  sin_urgrr_angle (sin(URGRR_ANGLE/180.0*M_PI))
#define  cos_urgrr_angle2 (cos(URGRR_ANGLE2/180.0*M_PI))
#define  sin_urgrr_angle2 (sin(URGRR_ANGLE2/180.0*M_PI))




/*
double cos_urg_angle = cos(URG_ANGLE/180.0*M_PI);
double sin_urg_angle = sin(URG_ANGLE/180.0*M_PI);
double cos_urg_angle2 = cos(URG_ANGLE2/180.0*M_PI);
double sin_urg_angle2 = sin(URG_ANGLE2/180.0*M_PI);

double cos_ibeo1_angle = cos(IBEO1_ANGLE/180.0*M_PI);
double sin_ibeo1_angle = sin(IBEO1_ANGLE/180.0*M_PI);
double cos_ibeo1_angle2 = cos(IBEO1_ANGLE2/180.0*M_PI);
double sin_ibeo1_angle2 = sin(IBEO1_ANGLE2/180.0*M_PI);

double cos_ibeo2_angle = cos(IBEO2_ANGLE/180.0*M_PI);
double sin_ibeo2_angle = sin(IBEO2_ANGLE/180.0*M_PI);
double cos_ibeo2_angle2 = cos(IBEO2_ANGLE2/180.0*M_PI);
double sin_ibeo2_angle2 = sin(IBEO2_ANGLE2/180.0*M_PI);

double cos_urgfl_angle = cos(URGFL_ANGLE/180.0*M_PI);
double sin_urgfl_angle = sin(URGFL_ANGLE/180.0*M_PI);
double cos_urgfl_angle2 = cos(URGFL_ANGLE2/180.0*M_PI);
double sin_urgfl_angle2 = sin(URGFL_ANGLE2/180.0*M_PI);

double cos_urgfr_angle = cos(URGFR_ANGLE/180.0*M_PI);
double sin_urgfr_angle = sin(URGFR_ANGLE/180.0*M_PI);
double cos_urgfr_angle2 = cos(URGFR_ANGLE2/180.0*M_PI);
double sin_urgfr_angle2 = sin(URGFR_ANGLE2/180.0*M_PI);

double cos_urgcl_angle = cos(URGCL_ANGLE/180.0*M_PI);
double sin_urgcl_angle = sin(URGCL_ANGLE/180.0*M_PI);
double cos_urgcl_angle2 = cos(URGCL_ANGLE2/180.0*M_PI);
double sin_urgcl_angle2 = sin(URGCL_ANGLE2/180.0*M_PI);

double cos_urgrl_angle = cos(URGRL_ANGLE/180.0*M_PI);
double sin_urgrl_angle = sin(URGRL_ANGLE/180.0*M_PI);
double cos_urgrl_angle2 = cos(URGRL_ANGLE2/180.0*M_PI);
double sin_urgrl_angle2 = sin(URGRL_ANGLE2/180.0*M_PI);

double cos_urgrr_angle = cos(URGRR_ANGLE/180.0*M_PI);
double sin_urgrr_angle = sin(URGRR_ANGLE/180.0*M_PI);
double cos_urgrr_angle2 = cos(URGRR_ANGLE2/180.0*M_PI);
double sin_urgrr_angle2 = sin(URGRR_ANGLE2/180.0*M_PI);
*/

#endif __CARDATA_H__
