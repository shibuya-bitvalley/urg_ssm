//空車時の車両諸元データ
const double m=11885;	//車両重量(kg)
const double I=148810;	//ヨーイング慣性モーメント(kgm2)
const double a=2.58;	//前輪〜重心点距離(m)
const double b=3.324;	//後輪〜重心点距離(m)
const double n=23;	//ステアリング系トータルギヤ比
const double Kf=292971;	//前輪コーナーリングパワー(N/rad)
const double Kr=495218;	//後輪コーナーリングパワー(N/rad)
const double g=9.80665; //重力定数
const double cf=Kf;
const double cr=Kr;

const double Sf = 0.001;	//スタビリティファクタ
