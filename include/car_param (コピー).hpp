//空車時の車両諸元データ
const double m=8890;	//車両重量(kg)
const double I=136000;	//ヨーイング慣性モーメント(kgm2)
const double a=3.045;	//前輪〜重心点距離(m)
const double b=3.369;	//後輪〜重心点距離(m)
const double n=23;	//ステアリング系トータルギヤ比
const double Kf=126192;	//前輪コーナーリングパワー(N/rad)
const double Kr=139014;	//後輪コーナーリングパワー(N/rad)
const double g=9.80665; //重力定数
const double cf=Kf;
const double cr=Kr;

const double Sf = 0.001;	//スタビリティファクタ
