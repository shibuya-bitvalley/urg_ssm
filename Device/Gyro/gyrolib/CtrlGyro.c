/*!
  @file
  @brief :光学式ジャイロセンサー (Tamagawa TAG0007) 用の関数群

  @author Kiyoshi MATSUO
*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>   // fd_set
#include "CtrlGyro.h"

static int GyroFD = -1; // file descripter For Gyro
static int GyroMode = 0; // Data Mode of Gyro
static int GyroFlow = 0; // Gyro Flow flag

static struct termios OLDTIO;
static double theta_initial = 0.0;

/*!
  @brief : 1文字を送る関数
  @param character : 送りたい文字
*/
static void SendChar(const char character)
{
  if (GYRO_OPEN == 1) 
    {
      write(GyroFD, &character, 1);
      usleep(1000);
    }
}

// open device port for Epos
extern int StartSerialForGyro(void)
{
  struct termios tio;
  // initialize EPOS file descriptor
  EndSerialForGyro();
  
  GyroFlow = 0;
  
  // open the device port for EPOS
  GyroFD = open(gyro_dev_name, O_RDWR|O_NOCTTY|O_NONBLOCK);
  if (GyroFD < 0) 
    {
      return 1;
    }
  GYRO_OPEN = 1;
  
  // 現在のシリアルポートの設定を退避する
  tcgetattr(GyroFD, &OLDTIO);
  
  // 構造体をクリア
  memset(&tio, 0, sizeof(tio));
  
  // 端末の設定 (制御モード)
  /*
    BAUDRATE: ボーレートの設定
    CS8     : 7e1 (8 ビット，ノンパリティ，ストップビット 1)
    CLOCAL  : ローカル接続，モデム制御なし
    CREAD   : 受信文字(receiving characters)を有効にする．
  */
  tio.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
  tio.c_cflag |= B19200 | CS7 | CLOCAL | CREAD | PARENB;
  
  // 端末の設定 (入力モード)
  /*
    IGNPAR  : パリティエラーのデータは無視する
    ICRNL   : CR を NL に対応させる
  */
  tio.c_iflag = (IGNPAR | ICRNL);
  
  // 端末の設定 (出力モード)
  // Raw mode での出力
  tio.c_oflag = ~(OPOST | ONLCR);
  
  // 端末の設定 (ローカルモード)
  /*
    ICANON  : カノニカルモード
  */
  tio.c_lflag &=
    ~(ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE);
  
  // 受信時の最小文字数
  tio.c_cc[VMIN] = 1;
  
  // 受信時のタイムアウトの時間 1 * 1/10 [s]
  tio.c_cc[VTIME] = 1;
  
  // 受信したが、読んでないデータをフラッシュ
  tcflush(GyroFD, TCIFLUSH);
  
  // commit parameter
  if (tcsetattr(GyroFD, TCSADRAIN, &tio) != 0) 
    {
      EndSerialForGyro();
      return -1;
    }
  ChangeDataModeOfGyro(3);
  StartGyro();
  ResetGyro();
  StopGyro();
  
  return 0;
}

// close the device port for Gyro
void EndSerialForGyro(void)
{
  if (GyroFD != -1)
    {
      tcflush(GyroFD, TCIOFLUSH);
      close(GyroFD);
      GyroFD= -1;
    }
  if (GYRO_OPEN ==1) 
    {
      // ポートをジャイロとの接続以前の状態に戻す
      tcsetattr(GyroFD, TCSANOW, &OLDTIO);
    }
  GYRO_OPEN = 0;
}

/*!
  @brief ジャイロセンサーから送信されるデータを変更する関数
  
  @param[in] choice : 角度のみ (choice == 1)
  角速度のみ (choice == 2)
  角度+角速度 (choice == 3)
  
  @retval 0 : success
  1 : fail
*/
extern int ChangeDataModeOfGyro(const int choice)
{
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return 1;
    }
  switch(choice) 
    {
    case 1:
      printf("only theta mode");
      SendChar('A');
      printf(" !\n");
      GyroMode = 1;
      break;
    case 2:
      printf("only omega mode");
      SendChar('R');
      printf(" !\n");
      GyroMode = 2;
      break;
    case 3:
      printf("both theta and omega mode");
      SendChar('B');
      printf(" YEAH!\n");
      GyroMode = 3;
      break;
    default:
      printf("iregal choice\n");
      GyroMode = 0;
      return 1;
    }
  return 0;
}

/*!
  @brief ジャイロセンサーから送信されるデータが何かを教えてくれる関数
  
  @retval 1 : 角度のみ
  2 : 角速度のみ
  3 : 角度＋角速度
  
  -1: gyro is not opened
*/
extern int NowDataModeOfGyro(void)
{
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return -1;
    }
  return GyroMode;
}

/*!
  @brief ジャイロセンサーの積算角度を 0 にリセットする関数
  
  @retval 0 : success
  @retval 1 : fail
*/
extern int ResetGyro(void)
{
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return 1;
    }
  printf("Gyro Reset\n");
  SendChar('C');
  return 0;
}

/*!
  @brief
  @param[out] data : 受信したデータ
  
  @retval positive : success
  negative : fail
  1        : 角度データ取得
  2        : 角速度データ取得
  3        : 角度＋角速度データ取得
  -1       : GyroMode が不正な値
  -2       : 通信が途中で中断
*/
static int ReceiveData(char data[11])
{
  int i = 0;
  int data_size = 0;
  switch(GyroMode) 
    {
    case 1:
    case 2:
      data_size = 6;
      break;
    case 3:
      data_size = 10;
      break;
    default:
      for(i = 0; i < 10; ++i) 
	{
	  data[i] = 'X';
	}
      data[10] = 0;
      return -1;
    }
  for(i = 0; i < 10; ++i) 
    {
      data[i] = 'Y';
    }
  data[11] = 0;
  fd_set rfds;
  struct timeval tv;

  // 最悪の場合は 20 文字読む必要がある
  char buff[20];
  for (i = 0; i < 19; ++i) 
    {
      buff[i] = 'Z';
    }
  buff[19] = 0;

  // watch a Gyro file descriptor for 100msec
  FD_ZERO(&rfds);
  FD_SET(GyroFD, &rfds);
  tv.tv_sec = 0;
  tv.tv_usec = 1 * 1e5;
  
  // read data
  // 受信したが、読んでないデータをフラッシュ
  tcflush(GyroFD, TCIFLUSH);
  
  int read_size = 0;
  int _read_size = 0;
  int same_count = 0;
  int ret = 0;
  int message_start = 0;
  
  // read data
  while ((read_size != data_size)) 
    {
      if (select(GyroFD + 1, &rfds, NULL, NULL, &tv) <= 0) 
	{
	  tcflush(GyroFD, TCIFLUSH);
	  fprintf(stderr, "Could not read data from Gyro\n");
	  int i = 0;
	  printf("[%d]readed sentence is ", read_size);
	  for (i = 0; i < read_size; ++i) 
	    {
	      printf("%c", data[i]);
	    }
	  printf("\n");
	  return -2;
	}
      ret	= read(GyroFD, (buff + read_size), 1);
      if (buff[read_size] == '$') 
	{
	  message_start = 1;
	}
      if (ret < 0) 
	{
	  continue;
	}
      if (message_start == 1) 
	{
	  read_size += ret;
	}
	
	if( read_size == _read_size )
	{
		same_count++;
		
		if(same_count > 1000)
		{
			//同じ値が1000続いたら読めていない
			printf("Can't Read Data\n");
			return -2;
		}
	}
	else
	{
		same_count = 0;
	}
	_read_size = read_size;
    }
  for (i = 0; i < data_size; ++i) 
    {
      data[i] = buff[i];
    }
  data[data_size] = 0;
  return GyroMode;
}

/*!
  @brief 文字を16進数として解釈し、その値を返す関数
  
  @retval -1 : 16進数として不適切な文字である場合
*/
static unsigned int Hex2Dec(char hex)
{
  unsigned int number = 0;
  
  number = hex - '0';
  if (number < 10) 
    {
      return number;
    }
  
  number = hex -'A';
  if (number < 6) 
    {
      return number + 10;
    }
  return -1;
}

/*!
  @brief ジャイロセンサーから取得した文字列を数値化する関数
*/
static int TranslateGyroData(int *p_value,char data1, char data2, char data3, char data4)
{
  int number[4];
  
  number[0] = Hex2Dec(data4);
  number[1] = Hex2Dec(data3);
  number[2] = Hex2Dec(data2);
  number[3] = Hex2Dec(data1);

  if  ((number[0] < 0) || (number[1] < 0) || (number[2] < 0) || (number[3] < 0)) 
    {
      return -1;
    }
  *p_value = 0;
  
  int i = 0;
  if (number[0] < 16) 
    {
      for (i = 0; i < 4; ++i) 
	{
	  *p_value += (number[i]) << (4 * i);
	}
    }
  if (*p_value == 0) 
    {
      *p_value = 65536;
    }
  
  // 反時計回りで値が増加するようにしている
  *p_value = 65536 - *p_value;
  
  return 0;
}

/*!
  @brief ジャイロセンサーから角度データを取得する関数
  
  @retval 0 : success
  @retval 1 : receive error
  @retval 2 : ジャイロセンサーから角度データが送信されていない場合
*/
extern int GetGyroTheta(double *p_theta, int mode)
{
  // Gyro との通信が始まってない場合の処理
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return 1;
    }
  
  // Data mode が不正の場合の処理
  if (GyroMode != 1) 
    {
      printf("gyro is not theta only mode %d\n", GyroMode);
      return 2;
    }
  
  // 垂れ流しモードでない場合は、単発でデータをもらおうとする
	// 現状では、ハンドシェイクでデータがもらえてないのでエラー処理にする
  if (GyroFlow != 1) 
    {
      //		SendChar('D');
      return 2;
    }
  
  char data[11];
  
  int ret = 0;
  ret = ReceiveData(data);
  
  if(ret < 0) 
    {
      printf("Gyro receive error\n");
      return 1;
    }
  
  int value = -1;
  // 得たデータを数値に変換する
  if (TranslateGyroData(&value, data[1], data[2], data[3], data[4]) < 0) 
    {
      printf("received data from Gyro is not valid\n");
      return 1;
    }
  
  // 0.005493164 は 360 / 65536 である
  *p_theta = (double) value * 0.005493164;
  
  // offset を入れる
  *p_theta += theta_initial;
  
  if (*p_theta > 180.0) 
    {
      *p_theta -= 360.0;
    }
  
  // mode 1 は 度数で返す
  if (mode == 1) 
    {
      return 0;
    }
  
  // default では ラジアンで返す
  *p_theta *= DEG2RAD;
  return 0;
}

/*!
  @brief ジャイロセンサーから角速度データを取得する関数
  
  @param[out] *p_omega : 取得された角速度の値 度数かラジアンかは mode で決まる
  @param[in] mode      : 取得された角速度の値を度数、
  ラジアンのどちらで返すかを決める変数 mode 1 の場合は度数、それ以外の場合はラジアン
  
  @retval 0 : success
  @retval 1 : receive error
  @retval 2 : ジャイロセンサーから角速度が送信されていない場合
*/
extern int GetGyroOmega(double *p_omega, int mode)
{
  // そもそも Gyro と通信が始まってないよ Tony
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return 1;
    }
  
  // Data mode 不正じゃい
  if (GyroMode != 2) 
    {
      printf("gyro is not theta only mode %d\n", GyroMode);
      return 2;
    }
  
  // 垂れ流しモードでない場合は、単発でいただきさま
  // ぶっちゃけ、ハンドシェイクまじやばくね？
  if (GyroFlow != 1) 
    {
      //		SendChar('D');
      return 2;
    }
  char data[11];
  
  int ret = 0;
  ret = ReceiveData(data);
  
  if(ret < 0) 
    {
      printf("Gyro receive error\n");
      return 1;
    }
  
  int value = -1;
  // 得たデータを数値に変換する
  if (TranslateGyroData(&value, data[1], data[2], data[3], data[4]) < 0) 
    {
      printf("received data from Gyro is not valid\n");
      return 1;
    }
  
  // 角度に変換する
  *p_omega = (double) value * GYRO_RESOLUTION;
  
  if (*p_omega > 180.0)
    {
      *p_omega -= 360.0;
    }
  
  // mode 1 は 度数で返す
  if (mode == 1) 
    {
      return 0;
    }
  
  // default では ラジアンで返す
  *p_omega *= DEG2RAD;
  
  return 0;
}

/*!
  @brief ジャイロセンサーから角度データと角速度データを取得する関数
  
  @param[out] *p_theta : 角度データ [-180,180](エラーの場合は変更されない)
  @param[out] *p_omega : 角速度データ [/sec]（エラーの場合は変更されない)
  @param[in] mode  : 返すデータを度数とラジアンどちらか選ぶための変数
  : 1 ならば度数。その他はすべてラジアン
  
  @retval 0 : success
  @retval 1 : receive error
  @retval 2 : ジャイロセンサーから角度＋角速度が送信されていない場合
*/
extern int GetGyroData(double *p_theta, double *p_omega, int mode)
{
  // そもそも Gyro と通信が始まってないよ Tony
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return 1;
    }
  
  // Data mode 不正じゃい
  if (GyroMode != 3) 
    {
      printf("gyro is not theta only mode %d\n", GyroMode);
      return 2;
    }
  
  // 垂れ流しモードでない場合は、単発でいただきさま
  // ぶっちゃけ、ハンドシェイクまじやばくね？
  if (GyroFlow != 1) 
    {
      //		SendChar('D');
      return 2;
    }
  char data[11];
  
  int ret = 0;
  ret = ReceiveData(data);
  
  if(ret < 0) 
    {
      printf("Gyro receive error\n");
      return 1;
    }
  
  int value1 = -1;
	// 得たデータを数値に変換する
  if (TranslateGyroData(&value1, data[1], data[2], data[3], data[4]) < 0) 
    {
      printf("received data from Gyro is not valid\n");
      return 1;
    }
  int value2 = -1;
  if (TranslateGyroData(&value2, data[5], data[6], data[7], data[8]) < 0) 
    {
      printf("received data from Gyro is not valid\n");
      return 1;
    }
  
  // 角度に変換する
  *p_theta = (double) value1 * GYRO_RESOLUTION;
  // offset を入れる
  *p_theta += theta_initial;
  
  *p_omega = (double) value2 * GYRO_RESOLUTION;
  
  if (*p_theta > 180.0) 
    {
      *p_theta -= 360.0;
    }
  
  if (*p_omega > 180.0) 
    {
      *p_omega -= 360.0;
    }
  
  // mode 1 は 度数で返す
  if (mode == 1) 
    {
      return 0;
    }
  
  // default では ラジアンで返す
  *p_theta *= DEG2RAD;
  *p_omega *= DEG2RAD;
  
  return 0;
}

/*!
  @brief ジャイロセンサーからのデータ垂れ流しを止める関数
  
  @retval 0 : success
  @retval 1 : failed
*/
extern int StopGyro(void)
{
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return 1;
    }
  
  // 垂れ流しモードでないならそのままにする
  if (GyroFlow == 1) 
    {
      printf("stop Flow\n");
      SendChar('S');
    }
  GyroFlow = 0;
  return 0;
  
}

/*!
  @brief ジャイロセンサーからのデータ垂れ流しを始める関数

  @retval 0 : success
  @retval 1 : failed
*/
extern int StartGyro(void)
{
  if (GYRO_OPEN != 1) 
    {
      printf("Gyro is not opened\n");
      return 1;
    }
  
  // 垂れ流しモードならそのままにする
  if (GyroFlow == 1) 
    {
      return 0;
    }
  printf("start Flow");
  SendChar('T');
  printf(" !\n");
  GyroFlow = 1;
  return 0;
}

/*!
  @brief ジャイロから取得する角度データの初期値変更関数
  
  @param[in] offset [rad]
*/
extern void SetInitialTheta(double offset)
{
  theta_initial = offset * RAD2DEG;
  
  return;
}

/*!
  @brief ジャイロから取得するデータの初期化関数
*/
extern gyroData InitialGyroData(void)
{
  gyroData gyro_data;
  
  gyro_data.theta = theta_initial * DEG2RAD;
  gyro_data.omega = 0.0;
  
  gyro_data.past_theta = theta_initial * DEG2RAD;
  gyro_data.delta_theta = 0.0;
  
  gyro_data.time = gettimeSSM();
  
  return gyro_data;
}

/*!
  @brief ジャイロから取得する角度データを SSM に書き込む関数
*/
extern void *WriteGyroDataSSM(void * arg)
{
  SSMID *p_gyro = (SSMID *) arg;
  
  printf("GYRO START");
  if(GYRO_OPEN != 1) 
    {
      printf("gyro has not opened\n");
      if (StartSerialForGyro() != 0) 
	{
	  printf("start Error\n");
	  p_gyro->end_flag = 1;
	  pthread_exit(NULL);
	}
    }
  printf(" !\n");
  
  StartGyro();
  p_gyro->start_flag = 1;
  gyroData gyro_data;
  
  if (GetGyroData(&gyro_data.theta, &gyro_data.omega, 0) == 2) 
    {
      ChangeDataModeOfGyro(3);
    }
  
  double past_theta = 0.0;
  double past_time = gettimeSSM();
  int ret = 0;
  
  while(p_gyro->end_flag == 0) 
    {
      past_theta = gyro_data.theta;
      ret = GetGyroData(&gyro_data.theta, &gyro_data.omega, 0);
      if (ret == 0) 
	{
	  gyro_data.time = gettimeSSM();
	  gyro_data.past_theta = past_theta;
	  gyro_data.delta_theta = gyro_data.theta - gyro_data.past_theta;
	}
	else
	{
	  printf("Receive error!!!!!");
	  p_gyro->end_flag = 1;
	  break;
	}
      if (gyro_data.time != past_time) 
	{
	  if(gyro_data.theta>PI)
	    gyro_data.theta = gyro_data.theta - PI2;
	  if(gyro_data.theta<=-PI)
	    gyro_data.theta = gyro_data.theta + PI2;
	  //printf("theta = %lf\n",gyro_data.theta);
	  writeSSM(p_gyro->sid, (char *)&gyro_data, gyro_data.time);
	  past_time = gyro_data.time;
	}
    }
  
  EndSerialForGyro();
  p_gyro->start_flag = 0;
  pthread_exit(NULL);
}
