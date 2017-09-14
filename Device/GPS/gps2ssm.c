
#include "../../include/GPS.h"
#include "SerialCtrl.h"

#include <stdio.h>
#include <stdlib.h>
#include <ssm.h>
#include <unistd.h>
#include <string.h>


// GPSとの通信を開始する
void openGPS(char *device_name)
{
   // シリアルポートを開く
   if (StartSerial(4800, device_name) < 0)
     {
	printf("Can't open %s.\n", device_name);
	exit(1);
     }
   
   printf("Connected to GPS.\n");
}

// チェックサムを計算する関数
char calcCheckSumGPS(char *data)
{
   char checksum = 0x00;	// チェックサム初期化
   
   data++;	// $の分を飛ばす
   
   while (*data != '*')
     {
	checksum ^= *data;
	data++;
     }
   
   return checksum;
}

// GPSに送信するデータにチェックサムをつけて送信する
void sendGPScmd(char *cmd)
{
   char txdata[256];	// 実際に送信するデータ
   
   // 送信データを作成する
   sprintf(txdata, "%s%x", cmd, calcCheckSumGPS(cmd));
   
   SendString(txdata);	// データを送信する
}

void initGPS(void)
{
   sendGPScmd("$PSRF103,00,00,01,01*");	// GPGGAメッセージを1秒おきに出力
   sendGPScmd("$PSRF103,01,00,00,01*");	// GPGLLメッセージを出力しない
   sendGPScmd("$PSRF103,02,00,00,01*");	// GPGSAメッセージを出力しない
   sendGPScmd("$PSRF103,03,00,00,01*");	// GPGSVメッセージを出力しない
   sendGPScmd("$PSRF103,04,00,00,01*");	// GPRMCメッセージを出力しない
   sendGPScmd("$PSRF103,05,00,01,01*");	// GPVTGメッセージを1秒おきに出力
   sendGPScmd("$PSRF103,06,00,00,01*");	// GPMISS?メッセージを出力しない
   sendGPScmd("$PSRF103,08,00,00,01*");	// GPZDAメッセージを出力しない
}


// GPSデータを取得する関数
// 返り値はGPSデータ受信直後のSSM時刻
double getGPSdata(gps_t *gpsdata)
{
   char rxdata[128];	// 受信データ
   char dummy[64];		// 後半の不要な文字列が格納されるダミー領域
   double ssm_time;
   double none;
   
   ReceiveString(rxdata);		// GPSデータを受信する
   ssm_time = gettimeSSM();	// GPSデータを受信直後にSSM時刻を取得する
   
   // 各種パラメータを数値化して構造体に格納
   if(rxdata[3] == 'G' && rxdata[4] == 'G' && rxdata[5] == 'A')
     {
	strcpy(gpsdata->GPGGA, rxdata);	// GPGGAセンテンスの生データ
	sscanf(
	       rxdata,
	       "$GPGGA,%2hd%2hd%f,%lf,N,%lf,E,%hd,%hd,%f,%lf,M,%lf,M,*",
	       &gpsdata->utc_h,
	       &gpsdata->utc_m,
	       &gpsdata->utc_s,
	       &gpsdata->lat,
	       &gpsdata->lng,
	       &gpsdata->status,
	       &gpsdata->stl_num,
	       &gpsdata->hdop,
	       &gpsdata->alt,
	       &gpsdata->alt_g
	      );
	
	ReceiveString(rxdata);		// GPSデータを受信する
	
	if(rxdata[3] == 'V' && rxdata[4] == 'T' && rxdata[5] == 'G')
	  {
	     strcpy(gpsdata->GPVTG, rxdata);	// GPGGAセンテンスの生データ
	     sscanf(
		    rxdata,
		    "$GPVTG,%lf,T,,M,%lf,N,%lf,K,M,*",
		    &gpsdata->t_deg,
		    &none,
		    &gpsdata->speed
		   );
	  }
	else
	  {
	     return -1.0;
	  }
     }
   else
     {
	return -1.0;
     }
   
   return ssm_time;	// GPSデータ取得直後のSSM時刻を返す
}




int main(int argc, char *argv[])
{
   double time_gps;	// GPSのデータを取得したSSM時刻
   SSM_sid sid_gps;	// GPSのSSMID
   gps_t gpsdata;		// GPSのデータを格納する構造体
   
   // シリアルポートを開く（実行時に指定が無ければ/dev/GPSを開く）
   openGPS((argc < 2) ? "/dev/GPS" : argv[1]);
   
   initGPS();	// GPSとの通信方法を指定する
   initSSM();	// SSMの初期化
   
   sid_gps = createSSM("gps", 0, sizeof(gps_t), 5, 1);
   
   while (1)
     {
	time_gps = getGPSdata(&gpsdata);	// GPSデータを取得する
	if( time_gps != -1.0)
	  {
	     writeSSM(sid_gps, (char *)&gpsdata, time_gps);	// GPSデータをSSMに書き込む
	  }
     }
   
   return 0;
}
