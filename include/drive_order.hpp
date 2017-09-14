// ZMP車線認識カメラデータ
typedef struct
{
	int auto_drive;		//0:OFF 1:ON
	int auto_drive_mode;	//1:ZMP_CAMによる自動操舵 2:URG_Cによる自動そうだ 3:自動選択
	int lane_change;	//0:なし 1:左へレーンチェンジ 2:右へレーンチェンジ
} DRIVE_ORDER;

// [0] = 5m
// [1] = 10m
// [2] = 15m
// [3] = 20m
// [4] = 25m
// [5] = 30m
// [6] = 35m
// [7] = 40m
// [8] = 45m
// [9] = 50m
