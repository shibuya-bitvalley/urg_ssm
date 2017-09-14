#ifndef URG_SENSOR_H
#define URG_SENSOR_H

/*!
  \file
  \brief URG センサ制御

  URG 用の基本的な関数を提供します。


  \brief URG sensor

  URG 用の基本的な関数を提供します。

  \author Satofumi KAMIMURA

  $Id: urg_sensor.h,v 540bc11f70c8 2011/05/08 23:04:49 satofumi $
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "urg_connection.h"


    /*!
      \brief 計測タイプ
    */
    typedef enum {
        URG_DISTANCE,           /*!< 距離 */
        URG_DISTANCE_INTENSITY, /*!< 距離 + 強度 */
        URG_MULTIECHO,          /*!< マルチエコーの距離 */
        URG_MULTIECHO_INTENSITY, /*!< マルチエコーの(距離 + 強度) */
        URG_STOP,                /*!< 計測の停止 */
        URG_UNKNOWN,             /*!< 不明 */
    } urg_measurement_type_t;

    /*!
      \brief 距離を何 byte で表現するかの指定
    */
    typedef enum {
        URG_COMMUNICATION_3_BYTE, /*!< 距離を 3 byte で表現する */
        URG_COMMUNICATION_2_BYTE, /*!< 距離を 2 byte で表現する */
    } urg_range_data_byte_t;


    enum {
        URG_SCAN_INFINITY = 0,  /*!< 無限回のデータ取得 */
        URG_MAX_ECHO = 3, /*!< マルチエコーの最大エコー数 */
    };


    /*! エラーハンドラ */
    typedef urg_measurement_type_t
    (*urg_error_handler)(const char *status, void *urg);


    /*!
      \brief URG センサ管理

      \brief URG sensor
    */
    typedef struct
    {
        int is_active;
        int last_errno;
        urg_connection_t connection;

        int first_data_index;
        int last_data_index;
        int front_data_index;
        int area_resolution;
        long scan_usec;
        int min_distance;
        int max_distance;
        int scanning_first_step;
        int scanning_last_step;
        int scanning_skip_step;
        int scanning_skip_scan;
        urg_range_data_byte_t range_data_byte;

        int timeout;
        int specified_scan_times;
        int scanning_remain_times;
        int is_laser_on;

        int received_first_index;
        int received_last_index;
        int received_skip_step;
        urg_range_data_byte_t received_range_data_byte;
        int is_sending;

        urg_error_handler error_handler;

        char return_buffer[80];
    } urg_t;


    /*!
      \brief 接続

      指定したデバイスに接続し、距離を計測できるようにする。

      \param[in,out] urg URG センサ管理
      \param[in] connection_type 通信タイプ
      \param[in] device_or_address 接続デバイス名
      \param[in] baudrate_or_port 接続ボーレート [bps] / TCP/IP ポート

      \retval 0 正常
      \retval <0 エラー

      connection_type には、以下の項目が指定できます。

      - #URG_SERIAL
      - シリアル、USB 接続

      - #URG_ETHERNET
      - イーサーネット接続

      Example
      \code
      urg_t urg;

      if (urg_open(&urg, URG_SERIAL, "/dev/ttyACM0", 115200) < 0) {
      return 1;
      }

      ...

      urg_close(&urg); \endcode

      \attention URG C ライブラリの他の関数を呼び出す前に、この関数を呼び出す必要があります。

      \see urg_close()
    */
    extern int urg_open(urg_t *urg, urg_connection_type_t connection_type,
                        const char *device_or_address,
                        long baudrate_or_port);


    /*!
      \brief 切断

      レーザを消灯し、URG との接続を切断します。

      \param[in,out] urg URG センサ管理

      \see urg_open()
    */
    extern void urg_close(urg_t *urg);


    /*!
      \brief タイムアウト時間の設定

      \param[in,out] urg URG センサ管理
      \param[in] msec タイムアウトする時間 [msec]

      \attention urg_open() を呼び出すと timeout の設定値はデフォルト値に初期化されるため、この関数は urg_open() 後に呼び出すこと。
    */
    extern void urg_set_timeout_msec(urg_t *urg, int msec);


    /*! タイムスタンプモードの開始 */
    extern int urg_start_time_stamp_mode(urg_t *urg);


    /*!
      \brief タイムスタンプの取得

      \param[in,out] urg URG センサ管理

      \retval >=0 タイムスタンプ [msec]
      \retval <0 エラー

      Example
      \code
      urg_start_time_stamp_mode(&urg);

      before_ticks = get_pc_msec_function();
      time_stamp = urg_time_stamp(&urg);
      after_ticks = get_pc_msec_function();

      // タイムスタンプについての計算
      ...

      urg_stop_time_stamp_mode(&urg); \endcode

      詳しくは \ref sync_time_stamp.c を参照して下さい。
    */
    extern long urg_time_stamp(urg_t *urg);


    /*! タイムスタンプモードの終了 */
    extern int urg_stop_time_stamp_mode(urg_t *urg);


    /*!
      \brief 距離データの取得を開始

      距離データの取得を開始します。実際のデータは urg_get_distance(), urg_get_distance_intensity(), urg_get_multiecho(), urg_get_multiecho_intensity() で取得できます。

      \param[in,out] urg URG センサ管理
      \param[in] type データ・タイプ
      \param[in] scan_times データの取得回数
      \param[in] skip_scan データの取得間隔

      \retval 0 正常
      \retval <0 エラー

      type には取得するデータの種類を指定します。

      - #URG_DISTANCE ... 距離データ
      - #URG_DISTANCE_INTENSITY ... 距離データと強度データ
      - #URG_MULTIECHO ... マルチエコー版の距離データ
      - #URG_MULTIECHO_INTENSITY ... マルチエコー版の(距離データと強度データ)

      scan_times は何回のデータを取得するかを 0 以上の数で指定します。ただし、0 または #URG_SCAN_INFINITY を指定した場合は、無限回のデータを取得します。\n
      開始した計測を中断するには urg_stop_measurement() を使います。

      skip_scan はミラーの回転数のうち、１回のスキャン後に何回スキャンしないかを指定します。skip_scan に指定できる範囲は [0, 9] です。

      \image html skip_scan_image.png 何回に１回だけ計測するか

      たとえば、ミラーの１回転が 100 [msec] のセンサで skip_scan に 1 を指定した場合、データの取得間隔は 200 [msec] になります。

      Example
      \code
      enum { CAPTURE_TIMES = 10 };
      urg_start_measurement(&urg, URG_DISTANCE, CAPTURE_TIMES, 0);

      for (i = 0; i < CAPTURE_TIMES; ++i) {
      int n = urg_get_distance(&urg, data, &time_stamp);

      // 受信したデータの利用
      ...
      } \endcode

      \see urg_get_distance(), urg_get_distance_intensity(), urg_get_multiecho(), urg_get_multiecho_intensity(), urg_stop_measurement()
    */
    extern int urg_start_measurement(urg_t *urg, urg_measurement_type_t type,
                                     int scan_times, int skip_scan);


    /*!
      \brief 距離データの取得

      センサから距離データを取得します。事前に urg_start_measurement() を #URG_DISTANCE 指定で呼び出しておく必要があります。

      \param[in,out] urg URG センサ管理
      \param[out] data 距離データ [mm]
      \param[out] time_stamp タイムスタンプ [msec]

      \retval >=0 受信したデータ個数
      \retval <0 エラー

      data には、センサから取得した距離データが格納されます。data はデータを格納するのサイズを確保しておく必要があります。data に格納されるデータ数は urg_max_data_size() で取得できます。

      time_stamp には、センサ内部のタイムスタンプが格納されます。time_stamp を取得したくない場合 NULL を指定して下さい。

      Example
      \code
      long *data = (long*)malloc(urg_max_data_size(&urg) * sizeof(data[0]));

      ...

      // データのみ取得する
      urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
      int n = urg_get_distance(&urg, data, NULL);

      ...

      // データとタイムスタンプを取得する
      long time_stamp;
      urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
      n = urg_get_distance(&urg, data, &time_stamp); \endcode

      \see urg_start_measurement(), urg_max_data_size()
    */
    extern int urg_get_distance(urg_t *urg, long data[], long *time_stamp);


    /*!
      \brief 距離と強度データの取得

      urg_get_distance() に加え、強度データの取得ができる関数です。事前に urg_start_measurement() を #URG_DISTANCE_INTENSITY 指定で呼び出しておく必要があります。

      \param[in,out] urg URG センサ管理
      \param[out] data 距離データ [mm]
      \param[out] intensity 強度データ
      \param[out] time_stamp タイムスタンプ [msec]

      \retval >=0 受信したデータ個数
      \retval <0 エラー

      強度データとは、距離計算に使った波形の反射強度であり、センサのシリーズ毎に特性が異なります。 強度データを使うことで、物体の反射率や環境の大まかな濃淡を推測できます。

      data, time_stamp については urg_get_distance() と同じです。

      intensity には、センサから取得した強度データが格納されます。intensity はデータを格納するのサイズを確保しておく必要があります。intensity に格納されるデータ数は urg_max_data_size() で取得できます。

      Example
      \code
      int data_size = urg_max_data_size(&urg);
      long *data = malloc(data_size * sizeof(long));
      long *intensity = malloc(data_size * sizeof(unsigned short));

      ...

      urg_start_measurement(&urg, URG_DISTANCE_INTENSITY, 1, 0);
      int n = urg_get_distance_intensity(&urg, data, intesnity, NULLL); \endcode

      \see urg_start_measurement(), urg_max_data_size()
    */
    extern int urg_get_distance_intensity(urg_t *urg, long data[],
                                          unsigned short intensity[],
                                          long *time_stamp);


    /*!
      \brief 距離データの取得 (マルチエコー版)

      マルチエコー版の距離データ取得関数です。事前に urg_start_measurement() を #URG_MULTIECHO 指定で呼び出しておく必要があります。

      \param[in,out] urg URG センサ管理
      \param[out] data_multi 距離データ [mm]
      \param[out] time_stamp タイムスタンプ [msec]

      \retval >=0 受信したデータ個数
      \retval <0 エラー

      マルチエコーとは複数の距離データです。 マルチエコーは、１つのレーザ発光において複数の距離データが得られたときに得られます。

      \image html multiecho_image.png マルチエコーのイメージ図

      time_stamp については urg_get_distance() と同じです。

      data_multi には、センサから取得した距離データが１つの step あたり最大で #URG_MAX_ECHO (3 つ)格納されます。マルチエコーが存在しない項目のデータ値は -1 が格納されています。

      \verbatim
      data_multi[0] ... step n の距離データ (1 つめ)
      data_multi[1] ... step n の距離データ (2 つめ)
      data_multi[2] ... step n の距離データ (3 つめ)
      data_multi[3] ... step (n + 1) の 距離データ (1 つめ)
      data_multi[4] ... step (n + 1) の 距離データ (2 つめ)
      data_multi[5] ... step (n + 1) の 距離データ (3 つめ)
      ... \endverbatim

      格納順は、各 step において urg_get_distance() のときと同じ距離のデータが (3n + 0) の位置に格納され、それ以外のデータが (3n + 1), (3n + 2) の位置に降順に格納されます。\n
      つまり data_multi[3n + 1] >= data_multi[3n + 2] になることは保証されますが data_multi[3n + 0] と data_multi[3n + 1] の関係は未定義です。(data_multi[3n + 1] == data_multi[3n + 2] が成り立つのはデータ値が -1 のとき。)

      Example
      \code
      long *data_multi = malloc(3 * urg_max_data_size(&urg) * sizeof(long));

      ...

      urg_start_measurement(&urg, URG_MULTIECHO, 1, 0);
      int n = urg_get_distance_intensity(&urg, data_multi, NULLL); \endcode

      \see urg_start_measurement(), urg_max_data_size()
    */
    extern int urg_get_multiecho(urg_t *urg, long data_multi[], long *time_stamp);


    /*!
      \brief 距離と強度データの取得 (マルチエコー版)

      urg_get_multiecho() に加え、強度データの取得できる関数です。事前に urg_start_measurement() を #URG_MULTIECHO_INTENSITY 指定で呼び出しておく必要があります。

      \param[in,out] urg URG センサ管理
      \param[out] data_multi 距離データ [mm]
      \param[out] intensity_multi 強度データ
      \param[out] time_stamp タイムスタンプ [msec]

      \retval >=0 受信したデータ個数
      \retval <0 エラー

      data_multi, time_stamp については urg_get_multiecho() と同じです。

      intensity_multi のデータの並びは data_multi と対応したものになります。intensity_multi に格納されるデータ数は urg_max_data_size() で取得できます。

      Example
      \code
      int data_size = urg_max_data_size(&urg);
      long *data_multi = malloc(3 * data_size * sizeof(long));
      long *intensity_multi = malloc(3 * data_size * sizeof(unsigned short));

      ...

      urg_start_measurement(&urg, URG_DISTANCE_INTENSITY, 1, 0);
      int n = urg_get_multiecho_intensity(&urg, data_multi,
      intesnity_multi, NULLL); \endcode

      \see urg_start_measurement(), urg_max_data_size()
    */
    extern int urg_get_multiecho_intensity(urg_t *urg, long data_multi[],
                                           unsigned short intensity_multi[],
                                           long *time_stamp);


    /*!
      \brief 計測を中断し、レーザを消灯させます

      \ref urg_start_measurement() の計測を中断します。

      \param[in,out] urg URG センサ管理

      \retval 0 正常
      \retval <0 エラー

      Example
      \code
      urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0);
      for (int i = 0; i < 10; ++i) {
      urg_get_distance(&urg, data, NULL);
      }
      urg_stop_measurement(&urg); \endcode

      \see urg_start_measurement()
    */
    extern int urg_stop_measurement(urg_t *urg);


    /*!
      \brief 計測範囲を設定します

      センサが計測する範囲を step 値で指定します。urg_get_distance() などの距離データ取得の関数で返されるデータ数は、ここで指定した範囲で制限されます。

      \param[in,out] urg URG センサ管理
      \param[in] first_step 計測の開始 step
      \param[in] last_step 計測の終了 step
      \param[in] skip_step 計測データをグルーピングする個数

      \retval 0 正常
      \retval <0 エラー

      センサの step は、センサ正面を 0 とし、センサ上部から見て反時計まわりの向きが正の値となる順に割り振られます。

      \image html sensor_angle_image.png センサと step の関係

      step の間隔と、最大値、最小値はセンサ依存です。step 値の最大値、最小値は urg_step_min_max() で取得できます。\n

      first_step, last_step でデータの計測範囲を指定します。計測範囲は [first_step, last_step] となります。

      skip_step は、計測データをグルーピングする個数を指定します。指定できる値は [0, 99] です。\n
      skip_step は、指定された数のデータを 1 つにまとめることで、センサから受信するデータ量を減らし、距離取得を行う関数の応答性を高めるときに使います。ただし、データをまとめるため、得られるデータの分解能は減ります。

      例えば以下のような距離データが得られる場合に
      \verbatim
      100, 101, 102, 103, 104, 105, 106, 107, 108, 109
      \endverbatim

      skip_step に 2 を指定すると、得られるデータは
      \verbatim
      \endverbatim

      データは、まとめるデータのうち、一番小さな値のデータが用いられます。

      Example
      \code
      urg_set_scanning_parameter(&urg, urg_deg2step(&urg, -45),
      urg_deg2step(&urg, +45), 1);
      urg_start_measurement(&urg, URG_DISTANCE, 0);
      int n = urg_get_distance(&urg, data, NULL);
      for (int i = 0; i < n; ++i) {
      printf("%d [mm], %d [deg]\n", data[i], urg_index2deg(&urg, i));
      } \endcode

      \see urg_step_min_max(), urg_rad2step(), urg_deg2step()
    */
    extern int urg_set_scanning_parameter(urg_t *urg, int first_step,
                                          int last_step, int skip_step);


    /*!
      \brief 通信データのサイズ変更

      距離データをセンサから受信の際のデータサイズを変更します。

      \param[in,out] urg URG センサ管理
      \param[in] data_byte 距離値を表現するデータのバイト数

      \retval 0 成功
      \retval <0 エラー

      data_byte には

      - URG_COMMUNICATION_3_BYTE ... 距離を 3 byte で表現する
      - URG_COMMUNICATION_2_BYTE ... 距離を 2 byte で表現する

      を指定できます。\n
      初期状態では距離を 3 byte で表現するようになっています。この設定を 2 byte に設定することで、センサから受信するデータ数は 2/3 になります。ただし、取得できる距離の最大値が 4095 になるため、観測したい対象が 4 [m] 以内の範囲に存在する場合のみ利用して下さい。
    */
    extern int urg_set_communication_data_size(urg_t *urg,
                                               urg_range_data_byte_t data_byte);


    /*! レーザを発光させる */
    extern int urg_laser_on(urg_t *urg);


    /*! レーザを消灯する */
    extern int urg_laser_off(urg_t *urg);


    /*! センサを再起動する */
    extern int urg_reboot(urg_t *urg);


    /*!
      \brief センサを低消費電力の状態に遷移させる

      低消費電力のモードでは、スキャナの回転が停止し計測も中断されます。

      - 低消費電力のモード
        - レーザが消灯して計測が中断される。
        - スキャナの回転が停止する。

      低消費電力のモードから抜けるためには \ref urg_wakeup() 関数を呼び出して下さい。

      \see urg_wakeup()
    */
    extern void urg_sleep(urg_t *urg);


    /*!
      \brief センサを低消費電力のモードから通常の状態に遷移させる

      \see urg_sleep()
    */
    extern void urg_wakeup(urg_t *urg);

    /*!
      \brief センサが計測できる状態かを返す

      \retval 1 センサが計測できる状態にある
      \retval 0 センサが計測できる状態にない

      起動直後でスキャナの回転が安定していない場合や、何らかのエラーで計測できない場合、この関数は 0 を返します。
    */
    extern int urg_is_stable(urg_t *urg);


    /*!
      \brief センサ型式を文字列で返す

      センサの型式を文字列で返す。返される文字列はセンサ依存となる。

      \param[in] urg URG センサ管理

      \return センサ型式の文字列
    */
    extern const char *urg_sensor_product_type(urg_t *urg);


    /*!
      \brief センサのシリアル ID 文字列を返す

      センサのシリアル ID 文字列を返す。返される文字列はセンサ依存となる。

      \param[in] urg URG センサ管理

      \return シリアル ID 文字列
    */
    extern const char *urg_sensor_serial_id(urg_t *urg);


    /*!
      \brief センサのバージョン文字列を返す

      センサのソフトウェア・バージョン文字列を返す。返される文字列はセンサ依存となる。

      \param[in] urg URG センサ管理

      \return バージョン文字列
    */
    extern const char *urg_sensor_firmware_version(urg_t *urg);


    /*!
      \brief センサのステータス文字列を返す

      センサのステータス文字列を返す。返される文字列はセンサ依存となる。

      \param[in] urg URG センサ管理
      \return ステータス文字列
    */
    extern const char *urg_sensor_status(urg_t *urg);


    /*!
      \brief センサの状態を返す

      センサのステータス文字列を返す。返される文字列はセンサ依存となる。

      \param[in] urg URG センサ管理
      \return 状態を示す文字列

      \attention 状態については SCIP の通信仕様書を参照のこと。
    */
    extern const char *urg_sensor_state(urg_t *urg);


    /*!
      \brief 計測用のエラーハンドラを登録する

      エラーハンドラは Gx, Mx 系のコマンドの応答が "00" か "99" 以外のときに呼び出される。
    */
    extern void urg_set_error_handler(urg_t *urg, urg_error_handler handler);


    /*!
      \brief SCIP 文字列のデコードを行う

      \param[in] data SCIP 文字列
      \param[in] data の byte サイズ

      \retval デコード後の数値
    */
    extern long urg_scip_decode(const char data[], int size);


#ifdef __cplusplus
}
#endif

#endif /* !URG_SENSOR_H */
