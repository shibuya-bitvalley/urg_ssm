/* LRF構造体 SSM用 */
//SimpleURGは683、TopURGは1081
typedef struct{
	double dist[761];	/* 距離データ[m] */
	//double x[1081];		/* x座標[m] */
	//double y[1081];		/* y座標[m] */
}LRF_T;

typedef struct{
	int intensity[761];	/* 反射強度 */
	//double x[1081];		/* x座標 */
	//double y[1081];		/* y座標 */
}LRF_I_T;

typedef struct{
	LRF_T dist[3];		/* 距離データ*3 */
	LRF_I_T intensity[3];	/* 反射強度*3 */
}LRF_ECHO_T;
