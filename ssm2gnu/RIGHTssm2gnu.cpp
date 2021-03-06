#include <iostream>
#include <cstdio>
#include <cmath>
#include <ssm.hpp>
#include "../include/LRF.hpp"
#include "GNUctrl2.h"
#include "../include/gnuplot_stream.h"
#include <stdlib.h>
using namespace std;

int main(int aArgc, char **appArgv){
   
   initSSM();	// SSMイニシャライズ
   GNU_Init();	// Gnuplotの初期化
   
   SSM_sid RURG_sid; // SSM管理id
   
   LRF_ECHO urg;	// LRFの座標データ
   
   double time; //SSM書き込み時間
   
   RURG_sid = openSSM("URG_R", 0, 0);	//SSMを開く（”名前”，番号，時間）
   
   int hoge;
   
   while(1){

      hoge = readSSM(RURG_sid, &urg, &time,-1);
      
      //GNU_Plot2(1, &urg);	// Gnuplotに描画
      
      GNU_Plot12(-&urg);	// Gnuplotに描画
      usleep(25000);
   }
   
   GNU_Close();	// Gnuplotのクローズ

   return 0;
}
