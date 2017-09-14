// dSPACE　→ モータ
typedef struct
{
	//命令を送るかどうか
	bool send_on;			// 1:送信する	0:送信しない

	//モータ位置
	int motor_position;		//[Pules]    1(deg)= 36(Pulse)

	
	//サーボコマンド
	bool servo_on;			//ServoOn
	bool clear_alarm;		//Clear Alarm
	bool position_data_reset;	//Position Data Reset
} SERVO_ORDER;

// モータ　→　dSPACE
typedef struct
{
	//モータ電流
	double motor_current;		//[Arms]
	
	//モータ位置
	int motor_position;		//[Pules]    1(deg)= 36(Pulse)
	
	//サーボステータス
	bool servo_on;			//ServoOn
	bool servo_ready;		//ServoReady
	bool fault;			//Fault状態
	bool current_limit;		//電流リミット
	bool speed_limit;		//速度リミット
	bool position_deviation_over;	//位置偏差過大
	bool drive_voltage_over;	//駆動電圧過大
	bool drive_voltage_lower;	//駆動電圧過小
	bool temperature_anomaly;	//温度異常
	bool overload;			//過負荷
	bool sensor_anomaly;		//センサ異常
	bool current_control_anomaly;	//電流制御異常
	bool bettry_warning;		//Bettry Warning
} SERVO_STATUS;

