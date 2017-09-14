#ifndef CTRL_GYRO_H
#define CTRL_GYRO_H
/*!
  @file
  @brief 光学式ジャイロセンサー (Tamagawa TAG0007) 用の関数群
  角度分解能は 360 度を 65536 ステップに分解する程度

  @author Kiyoshi MATSUO
*/

#include <ssm.h>

#define gyro_dev_name "/dev/GYRO"
#define gyro_sensorName "TAG0007"
#define gyro_sensorID 7

/*!
    \brief 円周率
\\*/
#define PI 3.14159265358979323846

/*!
    \brief 円周率×２
*/
#define PI2 (3.14159265358979323846 * 2.0)

/*!
    \brief degreeからradianへの変換
*/
#define DEG2RAD (3.14159265358979323846 / 180.0)

/*!
    \brief radianからdegreeへの変換
*/
#define RAD2DEG (180.0 / 3.14159265358979323846)

/*!
  Gyro の分解能 360 / 65536
*/
#define GYRO_RESOLUTION (0.005493164)

int GYRO_OPEN;

// ジャイロ用のデータ構造体
typedef struct _gyro_data{
  double theta;
  double omega;

  double past_theta;
  double delta_theta;

  double time;
}gyroData;

pthread_t Gyrothread;

#define GyroDataLength sizeof(gyroData)

// SSM を用いて、データを読込書きこみするのに必要な情報などの構造体
typedef struct {
  pthread_mutex_t mutex;
  char sensorName[60];
  int sensorID;

  int end_flag;
  int start_flag;

  int scan_times;

  SSM_sid sid;
  SSM_sid readSid;

  char *dev_name;

}SSMID;

/*!
  @brief ジャイロセンサーとの通信を開始する関数

  シリアル通信:baudrate 19200 bps
                       調歩同期
	      データ長 7 bit
	      パリティ 偶数
	      ストップビット 1 bit

  @retval 0 : success
  @retval 1 : can't open port (gyro_dev_name)
  @retval 2 : can't set new port setting
*/
extern int StartSerialForGyro(void);


/*!
  @brief ジャイロセンサーとの通信を終了する関数
*/
extern void EndSerialForGyro(void);

/*!
  @brief ジャイロセンサーから送信されるデータを変更する関数

  @param[in] choice : 角度のみ (choice == 1)
                      角速度のみ (choice == 2)
		      角度+角速度 (choice == 3)

  @retval 0 : success
          1 : fail
*/
extern int ChangeDataModeOfGyro(const int choice);

/*!
  @brief ジャイロセンサーから送信されるデータが何かを教えてくれる関数

  @retval 1 : 角度のみ
          2 : 角速度のみ
	  3 : 角度＋角速度

          -1: gyro is not opened
*/
extern int NowDataModeOfGyro(void);

/*!
  @brief ジャイロセンサーの積算角度を 0 にリセットする関数

  @retval 0 : success
  @retval 1 : fail
*/
extern int ResetGyro(void);

/*!
  @brief ジャイロセンサーから角度データを取得する関数

  @param[out] *p_theta : 角度データ（エラーの場合は変更されない)[-180,180]
  @param[in] mode  : 返す角度データを度数とラジアンどちらか選ぶための変数
                   : 1 ならば度数。その他はすべてラジアン

  @retval 0 : success
  @retval 1 : receive error
  @retval 2 : ジャイロセンサーから角度データが送信されていない場合
*/
extern int GetGyroTheta(double *p_theta, int mode);

/*!
  @brief ジャイロセンサーから角速度データを取得する関数

  @param[out] *p_omega : 角速度データ（エラーの場合は変更されない)[/sec]
  @param[in] mode  : 返す角速度データを度数とラジアンどちらか選ぶための変数
                   : 1 ならば度数。その他はすべてラジアン

  @retval 0 : success
  @retval 1 : receive error
  @retval 2 : ジャイロセンサーから角速度が送信されていない場合
*/
extern int GetGyroOmega(double *p_omega, int mode);

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
extern int GetGyroData(double *p_theta, double *p_omega, int mode);

/*!
  @brief ジャイロセンサーからのデータ垂れ流しを止める関数

  @retval 0 : success
  @retval 1 : failed
*/
extern int StopGyro(void);

/*!
  @brief ジャイロセンサーからのデータ垂れ流しを始める関数

  @retval 0 : success
  @retval 1 : failed
*/
extern int StartGyro(void);

/*!
  @brief ジャイロセンサーから取得するデータの初期値変更関数

  @param[in] offset [rad]
*/
extern void SetInitialTheta(double offset);

/*!
  @brief ジャイロから取得するデータの初期化関数
*/
extern gyroData InitialGyroData(void);

/*!
  @brief ジャイロから取得する角度データを SSM に書き込む関数
*/
extern void *WriteGyroDataSSM(void * arg);

#endif // CTRL_GYRO_H
