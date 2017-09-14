#include <cstdio>
#include <iostream>
#include <cmath>

//#include "LRFctrl.h"
#include "GNUctrl.h"
#include "car_data.hpp"

#define GP_PATH "/usr/bin/gnuplot"

FILE *gnu_p;	// Gnuplotへのパイプ用ポインタ

bool ok_flag = false;

void GNU_Init(void)
{
	// Gnuplotのオープン
	gnu_p = popen(GP_PATH, "w");
	fprintf(gnu_p, "set xrange [-5:5]\n");
	fprintf(gnu_p, "set xlabel \"x[m]\"\n");
	fprintf(gnu_p, "set yrange [-2:8]\n");
	fprintf(gnu_p, "set ylabel \"y[m]\"\n");
	fprintf(gnu_p, "set grid\n");
	fprintf(gnu_p, "set size ratio 0.25\n");
	fprintf(gnu_p, "set style fill\n");
	fprintf(gnu_p, "set boxwidth 0.01\n");
	//fprintf(gnu_p, "set size square\n");
	fprintf(gnu_p, "unset key\n");
	
	//fprintf(gnu_p, "set multiplot layout 3,1\n");
}
/*
void GNU_Plot(LRF *urg_data)
{
	int i;

	fprintf(gnu_p, "plot \"-\"\n");	// バッファ出力開始

	// LRFの二次元座標データをバッファに出力
	for (i = 0; i < 682; i++)
	{
		fprintf(gnu_p, "%lf %lf\n", urg_data->x[i], urg_data->y[i]);
	}
	
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	fflush(gnu_p);			// Gnuplotに描画させる
}
*/

void GNU_Plot(int *peak, double *x, double *y, double *intensity, int num)
{
	int i;
	
	fprintf(gnu_p, "plot \"-\" with boxes,  \"-\" lc rgb \"blue\" with dot, \"-\" lc rgb \"green\" with points \n");	// バッファ出力開始
	
	// 反射強度のデータをバッファに出力
	for (i = 0; i < num; i++)
	{
		// 地面から±0.15mのときのみ表示(0.15m=15cm)
		//if( y[i] < 0.15 && y[i] > -0.15 )
		{
			fprintf(gnu_p, "%lf %lf\n", x[i], intensity[i]);
		}
		
		/*
		if(ok_flag == false)
		{
			printf("%lf %lf\n", x[i], intensity[i]*10);
		}
		*/
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	/*
	if(ok_flag == false)
	{
		printf("\n\n\n");
	}
	*/
	
	// LRFの二次元座標データをバッファに出力
	for (i = 0; i < num; i++)
	{
		fprintf(gnu_p, "%lf %lf\n", x[i], y[i]);
		
		/*
		if(ok_flag == false)
		{
			//ok_flag = true;
			printf("%lf %lf\n", x[i], y[i]);
		}
		*/
		
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	/*
	if(ok_flag == false)
	{
		printf("\n\n\n");
	}
	*/
	
	
	// 白線の候補をバッファに出力
	for (i = 0; i < 10; i++)
	{
		if(peak[i] != 0.0)
		{
			fprintf(gnu_p, "%lf %lf\n", x[ peak[i] ], intensity[ peak[i] ]);
		}		
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了		

	fflush(gnu_p);			// Gnuplotに描画させる
}

void GNU_Plot2(int *peak, double *dist, double left, double right, double left_dist, double right_dist, double *x, double *y, double *intensity, int num)
{
	int i;
	
	fprintf(gnu_p, "set terminal x11 noraise 0\n");
	
	fprintf(gnu_p, "set xrange [-10:10]\n");
	fprintf(gnu_p, "set yrange [-2:8]\n");
	
	fprintf(gnu_p, "plot \"-\" with boxes,  \"-\" lc rgb \"blue\" with dot, \"-\" lc rgb \"green\" lw 10, \"-\" lc rgb \"yellow\" lw 10 , \"-\" lc rgb \"blue\" lw 5 \n");	// バッファ出力開始
		
	// 反射強度のデータをバッファに出力
	for (i = 0; i < num; i++)
	{
		// 地面から±0.15mのときのみ表示(0.15m=15cm)
		//if( y[i] < 0.15 && y[i] > -0.15 )
		{
			fprintf(gnu_p, "%lf %lf\n", x[i], intensity[i]);
		}
		
		/*
		if(ok_flag == false)
		{
			printf("%lf %lf\n", x[i], intensity[i]*10);
		}
		*/
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	/*
	if(ok_flag == false)
	{
		printf("\n\n\n");
	}
	*/
	
	// LRFの二次元座標データをバッファに出力
	for (i = 0; i < num; i++)
	{
		fprintf(gnu_p, "%lf %lf\n", x[i], y[i]);
		
		/*
		if(ok_flag == false)
		{
			//ok_flag = true;
			printf("%lf %lf\n", x[i], y[i]);
		}
		*/
		
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	/*
	if(ok_flag == false)
	{
		printf("\n\n\n");
	}
	*/
	
	
	// 白線をバッファに出力
	if(left != 0.0)
	{
		fprintf(gnu_p, "%lf %lf\n", left, 0.0);
	}
	if(right != 0.0)
	{
		fprintf(gnu_p, "%lf %lf\n", right, 0.0);
	}	
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	// 白線の候補をバッファに出力
	for(i=0; i<10; i++)
	{
		if(peak[i] != 0 && dist[peak[i]] != left && dist[peak[i]] != right)
		{
			fprintf(gnu_p, "%lf %lf\n", dist[peak[i]], 0.0);
		}
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	// 障害物の候補をバッファに出力
	if(left_dist != 0.0)
	{
		fprintf(gnu_p, "%lf %lf\n", left_dist, 0.0);
	}
	if(right_dist != 0.0)
	{
		fprintf(gnu_p, "%lf %lf\n", right_dist, 0.0);
	}	
	fprintf(gnu_p, "e\n");	// バッファ出力終了

	fflush(gnu_p);			// Gnuplotに描画させる
}

void GNU_Plot3(double *left_lane, double *right_lane, double *left_dist, double *right_dist)
{
	int i;
	
	//まず左の車線
	fprintf(gnu_p, "set terminal x11 noraise 1\n");
	fprintf(gnu_p, "set xrange [0:200]\n");
	fprintf(gnu_p, "set yrange [-10:10]\n");
	//fprintf(gnu_p, "plot \"-\" with dots,\"-\" with dots,\"-\" lc rgb \"blue\" with dots,\"-\" lc rgb \"blue\" with dots\n");
	fprintf(gnu_p, "plot \"-\",\"-\",\"-\" lc rgb \"blue\",\"-\" lc rgb \"blue\"\n");
	for (i = 0; i < 200; i++)
	{
		if( left_lane[i] != 0.0)
		{
			fprintf(gnu_p, "%d %lf\n", i, left_lane[i]);
		}
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	//つぎ右の車線
	for (i = 0; i < 200; i++)
	{
		if( right_lane[i] != 0.0)
		{
			fprintf(gnu_p, "%d %lf\n", i, right_lane[i]);
		}	
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	//左の障害物
	for (i = 0; i < 200; i++)
	{
		fprintf(gnu_p, "%d %lf\n", i, left_dist[i]);	
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	//右の障害物
	for (i = 0; i < 200; i++)
	{
		fprintf(gnu_p, "%d %lf\n", i, right_dist[i]);	
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	
	fflush(gnu_p);			// Gnuplotに描画させる	
}


void GNU_Plot4(double *x, double *y)
{
	int i;

	fprintf(gnu_p, "set size square\n");
	fprintf(gnu_p, "set xrange [-30:30]\n");
	fprintf(gnu_p, "set yrange [-30:30]\n");

	fprintf(gnu_p, "plot \"-\" lc rgb \"blue\", \"-\" lc rgb \"red\"\n");	// バッファ出力開始

	// LRFの二次元座標データをバッファに出力
	for (i = 0; i < 3240; i++)
	{
		if( y[i] < -3.0-(CAR_WIDTH/2.0) || y[i] > 3.0+(CAR_WIDTH/2.0))
		{
			fprintf(gnu_p, "%lf %lf\n", y[i], -x[i]);
		}
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	// LRFの二次元座標データをバッファに出力
	for (i = 0; i < 3240; i++)
	{
		if( y[i] >= -3.0 -(CAR_WIDTH/2.0) && y[i] <= 3.0+(CAR_WIDTH/2.0))
		{
			fprintf(gnu_p, "%lf %lf\n", y[i], -x[i]);
		}
	}
	fprintf(gnu_p, "e\n");	// バッファ出力終了
	
	fflush(gnu_p);			// Gnuplotに描画させる
}


void GNU_Close(void)
{	
	fclose(gnu_p);
}
