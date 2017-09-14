//
//	HOKYO TopURG(右側)と通信を行うプログラム
//	written by Shunichi Akamatsu
//	akamatsu@taka.is.uec.ac.jp
//	matsu.as5@gmail.com
//
//	◯ HOKYO TopURGとether通信を行いSSMに書き込む
//	◯ プログラムのベースは北陽電機のサンプルコード
//

#include <iostream>
#include <cmath>
#include <ssm.hpp>
#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdlib.h>
#include <cstdio>
#include "../../include/LRF.hpp"

using namespace std;

int main(int aArgc, char **appArgv){
   
   urg_t urg;
   int ret;
   long *length_data;
   unsigned short *intensity_data = NULL;
   int length_data_size;
   // "COM1" は、センサが認識されているデバイス名にする必要がある
   const char connect_address[] = "192.168.0.13";
   const long connect_port = 10940;
   urg_connection_type_t connection_type = URG_ETHERNET;
   //const char connect_address[] = "/dev/URG_C";
   //const long connect_port = 115200;
   //urg_connection_type_t connection_type = URG_SERIAL;	
   
   
   //ssmのイニシャライズ（必ず忘れずに）
   
   LRF_ECHO urg_data;	// LRFの（距離+反射強度)*3エコー のデータ 
   initSSM();	
   SSM_sid URG_sid;	//idの作成
   
   double time; //SSM書き込み時間	
   URG_sid = openSSM("URG_R", 0, 0);	//領域の確保（”名前”, 番号, データサイズ, 保持する時間, 周期）
   
   // センサに対して接続を行う。
   ret = urg_open(&urg, connection_type, connect_address, connect_port);
   if (ret < 0) {
      // todo: check error code
      cout << "Can't connect URG 1" <<endl;
      return 1;
   }
   // データ受信のための領域を確保する
   length_data = (long *)malloc(sizeof(long) * urg_max_data_size(&urg) * 3);
   intensity_data = (unsigned short *)malloc(sizeof(unsigned short) * urg_max_data_size(&urg) * 3);
   
   // todo: check length_data is not NULL
   // センサから距離データを取得する。
   //ret = urg_start_measurement(&urg, URG_DISTANCE_INTENSITY, 0, 0);
   ret = urg_start_measurement(&urg, URG_MULTIECHO_INTENSITY, 0, 0);
   if (ret < 0) {
      // todo: check error code
      cout << "Can't connect URG 2" <<endl;
      return 1;
   }
   
   cout << "Connect URG ok" <<endl;
   
   while(1){
      
      //length_data_size = urg_get_distance_intensity(&urg, length_data, intensity_data, NULL);
      length_data_size = urg_get_multiecho_intensity(&urg, length_data, intensity_data, NULL);
      time = gettimeSSM();
      if (length_data_size <= 0) {
	 // todo: check error code
	 cout << "Can't connect URG 3" <<endl;
	 return 1;
      }
      // todo: process length_data array
      // 距離データを X-Y 座標系に変換して表示する
		
      for (int i = 0; i < length_data_size; ++i){
	 urg_data.dist[0].dist[i] = (double)length_data[3*i]/1000.0;
	 urg_data.intensity[0].intensity[i] = (int)intensity_data[3*i];
	 urg_data.dist[1].dist[i] = (double)length_data[3*i+1]/1000.0;
	 urg_data.intensity[1].intensity[i] = (int)intensity_data[3*i+1];
	 urg_data.dist[2].dist[i] = (double)length_data[3*i+2]/1000.0;
	 urg_data.intensity[2].intensity[i] = (int)intensity_data[3*i+2];
	 
	 /*
	  * 
	  // その距離データのラジアン角度を求め、X, Y の座標値を計算する
	  for (int k = 0; k < URG_MAX_ECHO; ++k)
	  {
	  cout << URG_MAX_ECHO << endl;
				//double radian;
	  double length;
	  double intensity;
	  //double x;
	  //double y;
	  //double x_i;
	  //double y_i;
	  //radian = urg_index2rad(&urg, i);
	  length = (double)length_data[(URG_MAX_ECHO * i) + k] / 1000.0;
	  intensity = (int)intensity_data[(URG_MAX_ECHO * i) + k];
	  // \todo check length is valid
	  //x = (double)(length * cos(radian));
	  //y = (double)(length * sin(radian));
	  //x_i = (double)((double)intensity * cos(radian));
	  //y_i = (double)((double)intensity * sin(radian));
	  * 
	  urg_data.dist[k].dist[i] = length;
	  //urg_data.dist[k].x[i] = x;
	  //urg_data.dist[k].y[i] = y;
	  urg_data.intensity[k].intensity[i] = intensity;
	  //urg_data.intensity[k].x[i] = x_i;
	  //urg_data.intensity[k].y[i] = y_i;
	  //cout << x << "," << y << endl;
	  }
	  */
      }
      writeSSM(URG_sid, (char*)&urg_data, time);	//SSMにLRFデータを投げる
      //printf("RIGHT URG data = %lf\n",urg_data.dist[0].dist[540]);
   }
   
   
   // センサとの接続を閉じる。
   urg_stop_measurement(&urg);
   urg_close(&urg);
   cout << "ok connect URG 1" <<endl;

   return 0;
}
