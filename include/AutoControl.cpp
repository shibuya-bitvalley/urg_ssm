#include <iostream>
#include <cstdio>
#include <cmath>
#include <ssm.hpp>
#include <stdlib.h>
#include "../include/car.hpp"
#include "../include/car_data2.hpp"
#include "../include/AutoControl.hpp"
#include "../include/servo_motor.hpp"
#include "../include/path.hpp"
#include "../include/drive_order.hpp"
#include "calc.h"

using namespace std;

const double STEER_GAIN = 1;

//2次予測でt秒後の車両位置を求める関数
void get_espos( double *espos_x, double *espos_y, const double vel, const double yawrate , const double es_time )
{
	double x;
	double y;

	if( yawrate == 0.0 )
	{
		x = vel * es_time;
		y = 0.0;
	}
	else
	{
		double wt = yawrate * es_time;
		double r = vel / yawrate;
	
		x = r*sin(wt);
		y = r - r*cos(wt);
	}
	
	*espos_x = x;
	*espos_y = y;
}


int main(int aArgc, char **appArgv)
{
	//SSM関連
	SSMApi<CAR_ODOMETRY> ODOMETRY_SSM("ODOMETRY", 0);
	SSMApi<PATH_DATA> PATH_DATA_SSM("PATH_DATA", 0);
	SSMApi<AUTO_CONTROL> AUTO_CONTROL_SSM("AUTO_CONTROL", 0);
	SSMApi<SERVO_ORDER> SERVO_ORDER_SSM("SERVO_ORDER", 0);
	SSMApi<SERVO_STATUS> SERVO_STATUS_SSM("SERVO_STATUS", 0);
	SSMApi<DRIVE_ORDER> DRIVE_ORDER_SSM("DRIVE_ORDER", 0);

	//SSMの初期化
	initSSM();
	
	//SSM Create,Open
	ODOMETRY_SSM.open(SSM_READ);
	PATH_DATA_SSM.open(SSM_READ);
	AUTO_CONTROL_SSM.create(1.0, 0.001);
	SERVO_ORDER_SSM.create(1.0, 0.001);
	SERVO_STATUS_SSM.open(SSM_READ);
	DRIVE_ORDER_SSM.open(SSM_READ);
	
	bool init_flag = false;
	
	//ハンドル指令角
	double steer_target = 0;
	double _steer_target = 0;
	//偏差を計算
	double target_dist = 0;
	double _target_dist = 0;
	double front_dist = 0;
	
	double steer_target_plus = 0;
	
	//前回レーン偏差値
	double last_lane_y = 0;

	//制御開始
	while(1)
	{
		//SSMから読み込み
		ODOMETRY_SSM.readNew();
		PATH_DATA_SSM.readNew();
		SERVO_STATUS_SSM.readNew();
		DRIVE_ORDER_SSM.readNew();
		
		if( PATH_DATA_SSM.data.reliability > 0 )
		{
			//経路データを２次曲線でフィッティングして曲がり具合から
			//前方注視時間を決定する
			double kai[5];
			double std_err = calc(PATH_DATA_SSM.data.path_x, PATH_DATA_SSM.data.path_y, kai, 1000, 3);	//2次曲線
			//前方注視時間の変化は1〜2秒
			double look_ahead_time = 1.0 * ( 0.0008/fabs(kai[2]) );
			if( look_ahead_time > 2.0 )
			{
				look_ahead_time = 2.0;
			}
			else if( look_ahead_time < 1.0 )
			{
				look_ahead_time = 1.0;
			}
			look_ahead_time = 2.0;
		
			cout << "前方注視時間:" << look_ahead_time << endl;
		
		
		
			/*
			//追従点は2.0秒先の位置とする
			double mytarget_dist = ODOMETRY_SSM.data.vel * 2.0;
			if( mytarget_dist < 20.0 )
			{
				//最低20m先を見る
				mytarget_dist = 20.0;
			}
			cout << "Dist:" << mytarget_dist << endl;
			*/
		
			//2.0秒先の車両位置の2次予測を行う
			double espos_x;
			double espos_y;
			
			double esvel;
			//時速20km未満の場合は20kmとして計算する
			//時速20kmで1秒先=20÷3.6*1.0=5.5
			//つまり少なくとも5.5m前方を見る
			//20km/h=5.55m/s
			if( ODOMETRY_SSM.data.vel < 5.5 )	//40000÷3600=11.1
			{
				esvel = 5.5;
			}
			else
			{
				esvel = ODOMETRY_SSM.data.vel;
			}
		
			get_espos( &espos_x, &espos_y, esvel, ODOMETRY_SSM.data.yawrate, look_ahead_time );
			
			//1次予測
			//espos_x = ODOMETRY_SSM.data.vel * look_ahead_time;
			//espos_y = 0;
			
			
			
			cout << "E:" << espos_x << "," << espos_y << endl;
			
			//2次予測に一番近い目標位置を算出
			double dist = -1;
			int dist_num = 0;
			for( int i=0; i<1000; i++ )
			{
				double tmp_dist = sqrt( pow(espos_x-PATH_DATA_SSM.data.path_x[i],2)+pow(espos_y-PATH_DATA_SSM.data.path_y[i],2) );
				if( dist == -1 || tmp_dist < dist )
				{
					dist = tmp_dist;
					dist_num = i;
				}
			}
			double mytarget_x = PATH_DATA_SSM.data.path_x[dist_num];
			double mytarget_y = PATH_DATA_SSM.data.path_y[dist_num];
			cout << "T:" << mytarget_x << "," << mytarget_y << endl;

			//偏差を計算
			front_dist = espos_x;
			target_dist = mytarget_y - espos_y;			
			
			//ターゲット情報をSSMに投げる
			//ワールド座標
			AUTO_CONTROL_SSM.data.target_x = cos(PATH_DATA_SSM.data.th_pos)*mytarget_x - sin(PATH_DATA_SSM.data.th_pos)*mytarget_y + PATH_DATA_SSM.data.x_pos;
			AUTO_CONTROL_SSM.data.target_y = sin(PATH_DATA_SSM.data.th_pos)*mytarget_x + cos(PATH_DATA_SSM.data.th_pos)*mytarget_y + PATH_DATA_SSM.data.y_pos;
			AUTO_CONTROL_SSM.data.espos_x = cos(ODOMETRY_SSM.data.th)*espos_x - sin(ODOMETRY_SSM.data.th)*espos_y + ODOMETRY_SSM.data.x;
			AUTO_CONTROL_SSM.data.espos_y = sin(ODOMETRY_SSM.data.th)*espos_x + cos(ODOMETRY_SSM.data.th)*espos_y + ODOMETRY_SSM.data.y;
		}
		else
		{
			//ターゲット情報をSSMに投げる
			//ワールド座標
			cout << "車線情報なし" << endl;
			cout << "車線情報なし" << endl;
			AUTO_CONTROL_SSM.data.target_x = 0;
			AUTO_CONTROL_SSM.data.target_y = 0;
			AUTO_CONTROL_SSM.data.espos_x = 0;
			AUTO_CONTROL_SSM.data.espos_y = 0;
			
			//偏差を計算
			target_dist = 0;
		}
		
		//命令を投げてもいいか判断
		if( PATH_DATA_SSM.data.reliability > 0 && DRIVE_ORDER_SSM.data.auto_drive == 1)
		{
			/*
			///////////////////////////////////////////////////////////////
			//----------------------候補2----------------------------------
			//時速3.6km(1m/s)以上なら操舵角を計算する
			if( ODOMETRY_SSM.data.vel > 1.0 )
			{			
				double main_dist = -1;
				
				double main_target_x = 0;
				double main_target_y = 0;
				double main_espos_x = 0;
				double main_espos_y = 0;
			
				//+180〜-180度までのハンドル角で一番命令ヨーレートに近い操舵角を求める
				for( double i=-1.0; i<=1.0; i+=0.01 )
				{
					// i はハンドル切り足し量
					double steer_deg = ODOMETRY_SSM.data.steer * 180 / M_PI;
					double beta = ODOMETRY_SSM.data.beta;
					double yawrate = ODOMETRY_SSM.data.yawrate;
					
					double x=0;
					double y=0;
					double theta=0;
					
					double _theta = 0;
					double _beta = beta;
				
					//2秒後の車両位置を計算　10ms*200 = 2000ms
					for( int j=0; j<200; j++ )
					{
						//車両モデル
						double steer_rad_i = steer_deg / 180.0 * M_PI;
						double vel_ms = fabs( ODOMETRY_SSM.data.vel );
						double a11 = -(2.0*(cf+cr))/(m*vel_ms);
						double a12 = -1.0-( (2.0*(a*cf-b*cr))/(m*pow(vel_ms,2)) );
						double a21 = -(2.0*(a*cf-b*cr))/I;
						double a22 = -(2.0*(pow(a,2)*cf+pow(b,2)*cr))/(I*vel_ms);
						double b11 = (2.0*cf)/(m*vel_ms);
						double b21 = (2.0*a*cf)/I;
			
						double beta_dot = a11 * beta + a12 * yawrate + b11 * steer_rad_i/n;
						double yawrate_dot = a21 * beta + a22 * yawrate + b21 * steer_rad_i/n;
					
						beta = beta + (beta_dot*0.01);
						yawrate = yawrate + (yawrate_dot*0.01);
						
						x += ( ODOMETRY_SSM.data.vel*cos(theta+beta) + ODOMETRY_SSM.data.vel*cos(_theta+_beta) ) * 0.01 /2.0;
						y += ( ODOMETRY_SSM.data.vel*sin(theta+beta) + ODOMETRY_SSM.data.vel*sin(_theta+_beta) ) * 0.01 /2.0;
						theta += yawrate * 0.01;
						
						steer_deg += i;
						_theta = theta;
						_beta = beta;					
					}
					
					
					//2次予測に一番近い目標位置を算出
					double dist = -1;
					int dist_num = 0;
					for( int j=0; j<1000; j++ )
					{
						double tmp_dist = sqrt( pow(x-PATH_DATA_SSM.data.path_x[j],2)+pow(y-PATH_DATA_SSM.data.path_y[j],2) );
						if( dist == -1 || tmp_dist < dist )
						{
							dist = tmp_dist;
							dist_num = j;
						}
					}
					
					double mytarget_x = PATH_DATA_SSM.data.path_x[dist_num];
					double mytarget_y = PATH_DATA_SSM.data.path_y[dist_num];
					
					
					
					if( main_dist == -1 || dist < main_dist )
					{
						main_dist = dist;
						main_target_x = mytarget_x;
						main_target_y = mytarget_y;
						main_espos_x = x;
						main_espos_y = y;
						steer_target = ODOMETRY_SSM.data.steer * 180 / M_PI + i;
					}
						
										
				}
				
				cout << "dist:" << main_dist << endl;
				//ターゲット情報をSSMに投げる
				//ワールド座標
				AUTO_CONTROL_SSM.data.target_x = cos(PATH_DATA_SSM.data.th_pos)*main_target_x - sin(PATH_DATA_SSM.data.th_pos)*main_target_y + PATH_DATA_SSM.data.x_pos;
				AUTO_CONTROL_SSM.data.target_y = sin(PATH_DATA_SSM.data.th_pos)*main_target_x + cos(PATH_DATA_SSM.data.th_pos)*main_target_y + PATH_DATA_SSM.data.y_pos;
				AUTO_CONTROL_SSM.data.espos_x = cos(ODOMETRY_SSM.data.th)*main_espos_x - sin(ODOMETRY_SSM.data.th)*main_espos_y + ODOMETRY_SSM.data.x;
				AUTO_CONTROL_SSM.data.espos_y = sin(ODOMETRY_SSM.data.th)*main_espos_x + cos(ODOMETRY_SSM.data.th)*main_espos_y + ODOMETRY_SSM.data.y;
			}
			else
			{
				steer_target = 0;
			}
			//----------------------候補2----------------------------------
			///////////////////////////////////////////////////////////////
			*/
			
			
			///////////////////////////////////////////////////////////////
			//----------------------候補１----------------------------------
			//目標ヨーレート[rad/s]
			double yawrate_star = 2 * ODOMETRY_SSM.data.vel / pow(front_dist,2) * target_dist;
					
			//命令よーレート [rad/s]
			double yawrate_order = ODOMETRY_SSM.data.yawrate + yawrate_star;
			//double yawrate_order = yawrate_star;
			
			cout << "Y:" << ODOMETRY_SSM.data.yawrate << "  Ys" << yawrate_star << endl;
			
			
			//時速3.6km(1m/s)以上なら操舵角を計算する
			if( ODOMETRY_SSM.data.vel > 1.0 )
			{			
				double dist = -1;
			
				//+180〜-180度までのハンドル角で一番命令ヨーレートに近い操舵角を求める
				for( double i=-180.0; i<=180.0; i+=0.1 )
				{
					//車両モデル
					double steer_rad_i = (double)i / 180.0 * M_PI;
					double vel_ms = fabs( ODOMETRY_SSM.data.vel );
					double a11 = -(2.0*(cf+cr))/(m*vel_ms);
					double a12 = -1.0-( (2.0*(a*cf-b*cr))/(m*pow(vel_ms,2)) );
					double a21 = -(2.0*(a*cf-b*cr))/I;
					double a22 = -(2.0*(pow(a,2)*cf+pow(b,2)*cr))/(I*vel_ms);
					double b11 = (2.0*cf)/(m*vel_ms);
					double b21 = (2.0*a*cf)/I;
			
					double beta_dot = a11 * ODOMETRY_SSM.data.beta + a12 * ODOMETRY_SSM.data.yawrate + b11 * steer_rad_i/n;
					double yawrate_dot = a21 * ODOMETRY_SSM.data.beta + a22 * ODOMETRY_SSM.data.yawrate + b21 * steer_rad_i/n;
					
					double beta = ODOMETRY_SSM.data.beta + (beta_dot*0.1);
					double yawrate = ODOMETRY_SSM.data.yawrate + (yawrate_dot*0.1);
					
					double tmp_dist = fabs( yawrate_order - yawrate );
					if( dist == -1 || tmp_dist < dist )
					{
						dist = tmp_dist;
						steer_target = i;
					}					
				}
				
				cout << "dist:" << dist << endl;
			}
			else
			{
				steer_target = 0;
			}
			//----------------------候補１----------------------------------
			///////////////////////////////////////////////////////////////
										
			
			//操舵角制限
			if( steer_target > 100 )
			{
				steer_target = 100;
				cout << "操舵角制限(100deg)" << endl;
			}
			else if( steer_target < -100 )
			{
				steer_target = -100;
				cout << "操舵角制限(-100deg)" << endl;
			}
			else
			{
				cout << "操舵角制限なし" << endl;
			}
			
			
			//操舵角速度制限
			//１秒で180deg
			//0.01秒で1.8deg)			
			if( steer_target - _steer_target > 1.8 )
			{
				steer_target = _steer_target+1.8;
				cout << "操舵角速度制限(1080deg/s)" << endl;
			}
			else if( steer_target - _steer_target < -1.8 )
			{
				steer_target = _steer_target-1.8;
				cout << "操舵角速度制限(1080deg/s)" << endl;
			}
			else
			{
				cout << "操舵角速度制限なし" << endl;
			}
			_steer_target = steer_target;
				
			SERVO_ORDER_SSM.data.send_on = true;
			SERVO_ORDER_SSM.data.servo_on = true;
			SERVO_ORDER_SSM.data.clear_alarm = false;
			SERVO_ORDER_SSM.data.position_data_reset = false;
			
			//モータ指令値は右回転が正なので符号を反転する
			SERVO_ORDER_SSM.data.motor_position = (int)(-steer_target * 36.0);
		}
		else
		{
			SERVO_ORDER_SSM.data.send_on = true;
			SERVO_ORDER_SSM.data.servo_on = false;
			SERVO_ORDER_SSM.data.clear_alarm = false;
			SERVO_ORDER_SSM.data.position_data_reset = false;
			init_flag = false;
			cout << "車線情報なし or 操舵指令なし" << endl;
			cout << "車線情報なし or 操舵指令なし" << endl;
			
			steer_target = 0;
			SERVO_ORDER_SSM.data.motor_position = 0;
		}
		
		cout << "TargetDist:" << target_dist << endl;
		cout << "StearTarget:" << (double)SERVO_ORDER_SSM.data.motor_position/36.0 << endl;
		cout << "横偏差:" << PATH_DATA_SSM.data.lane_y[0] << endl;


		cout << endl;
		//SSMに投げる
		SERVO_ORDER_SSM.write();
		AUTO_CONTROL_SSM.write();

		//cout << "stear:" << car_data.steer << " deg    "<< "speed:" << car_data.speed << " km/n" << endl;
		usleep(10000);
	}
	return 0;
}
