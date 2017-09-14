// 車両データ構造体
typedef struct
{
	double speed;	//車速[km/h]
	double steer;	//ハンドル角(CANの生データ)[deg]	左回転が正
} CAR_ST;

// 車両データ構造体2
typedef struct
{
	double yawrate;	//ヨーレート[deg/s] 左旋回が正
} CAR_ST2;

// オドメトリ構造体
typedef struct
{
	double x;	// x座標[m]
	double y;	// y座標[m]
	double th;	// 車両角度[rad]
	double vel;	// 車速[m/s]
	double yawrate;	// 実測車両ヨーレート[rad/s]
	double yawrate_est;	// 推定車両ヨーレート[rad/s]
	double steer;	// ハンドル角（進行方向を0として)[rad]
	double beta;	// 推定車両すべり角[rad]
} CAR_ODOMETRY;
