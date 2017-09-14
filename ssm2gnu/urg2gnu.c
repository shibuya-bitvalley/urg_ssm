#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "LRFctrl.h"
#include "GNUctrl.h"

int main(int aArgc, char **appArgv)
{
   LRF urg_data;	// LRFの座標データ
   
   LRF_Init(appArgv[1]);	// LRFの初期化
   GNU_Init();				// Gnuplotの初期化
   
   while(1)
     {
	LRF_GetData(&urg_data);	// LRFのデータを取得
	GNU_Plot(&urg_data);	// Gnuplotに描画
	
	usleep(100000);         // 100msec待つ
     }
   
   GNU_Close();	// Gnuplotのクローズ
   return 0;
}
