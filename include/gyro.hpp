// ジャイロ用のデータ構造体
typedef struct {
  double theta;
  double omega;

  double past_theta;
  double delta_theta;

  double time;
}gyroData;
