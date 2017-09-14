//
//	SSMCreateプログラム
//	written by Shunichi Akamatsu
//	akamatsu@taka.is.uec.ac.jp
//	matsu.as5@gmail.com
//
//	◯ SSMの領域を作成する。
//

#include <iostream>
#include <cstdio>
#include <cmath>
#include <ssm.hpp>
#include <stdlib.h>
#include <fstream>
#include "../include/cam_f.hpp"
#include "../include/car.hpp"
#include "../include/ibeolux.hpp"
#include "../include/LRF.hpp"
#include "../include/LRF_T.hpp"
#include "../include/gyro.hpp"
#include "../include/zmp_cam.hpp"
#include "../include/lane_data.hpp"
#include "../include/servo_motor.hpp"
#include "../include/drive_order.hpp"
#include "../include/lane_result.hpp"
#include "../include/path.hpp"
#include "../include/AutoControl.hpp"
#include "../include/GPS.h"
#include "../include/slip_angle.hpp"

using namespace std;

//メイン関数
int main(int aArgc, char **appArgv){
   
   //SSM定義
   //SSMApi<CAM_F> CAM_F_SSM("CAM_F", 0);
   //SSMApi<CAR_ST> CARDATA_SSM("CARDATA", 0);
   //SSMApi<CAR_ST2> CARDATA2_SSM("CARDATA2", 0);
   //SSMApi<CAR_ODOMETRY> ODOMETRY_SSM("ODOMETRY", 0);
   //SSMApi<IBEO_DATA_SSM> IBEOLUX_F_SSM("IBEOLUX_F", 0);
   //SSMApi<IBEO_DATA_SSM> IBEOLUX_R_SSM("IBEOLUX_R", 0);
   
   //SSMApi<LRF_ECHO> URG_C_SSM("URG_C", 0);
   SSMApi<LRF_ECHO> URG_L_SSM("URG_L", 0);
   SSMApi<LRF_ECHO> URG_R_SSM("URG_R", 0);
   //SSMApi<LRF_ECHO> URG_RL_SSM("URG_RL", 0);
   //SSMApi<LRF_ECHO> URG_FR_SSM("URG_FR", 0);
   //SSMApi<LRF_ECHO> URG_CR_SSM("URG_CR", 0);
   //SSMApi<LRF_ECHO> URG_RR_SSM("URG_RR", 0);
   //SSMApi<LRF_ECHO_T> Tough_URG_SSM("Tough_URG", 0);
   
   SSMApi<gps_t> gps_SSM("gps", 0);
   SSMApi<gyroData> gyroData_SSM("TAG0007", 7);
   
   //SSMApi<ZMP_CAM> ZMP_CAM_SSM("ZMP_CAM", 0);
   //SSMApi<LANE_DATA> LANE_DATA_SSM("LANE_DATA", 0);
   
   //SSMApi<SERVO_ORDER> SERVO_ORDER_SSM("SERVO_ORDER", 0);
   //SSMApi<DRIVE_ORDER> DRIVE_ORDER_SSM("DRIVE_ORDER", 0);
   //SSMApi<LANE_RESULT> LANE_RESULT_SSM("LANE_RESULT", 0);
   //SSMApi<PATH_DATA> PATH_DATA_SSM("PATH_DATA", 0);
   //SSMApi<AUTO_CONTROL> AUTO_CONTROL_SSM("AUTO_CONTROL", 0);
   //SSMApi<SERVO_STATUS> SERVO_STATUS_SSM("SERVO_STATUS", 0);
   
   //SSMApi<SLIP_ANGLE> SLIP_ANGLE_SSM("SLIP_ANGLE", 0);
   
   
   //SSM初期化
   initSSM();
   //CAM_F_SSM.create(1.0, 0.1);
   //CARDATA_SSM.create(1.0, 0.01);
   //CARDATA2_SSM.create(1.0, 0.01);
   //ODOMETRY_SSM.create(1.0, 0.01);
   //IBEOLUX_F_SSM.create(1.0, 0.05);
   //IBEOLUX_R_SSM.create(1.0, 0.05);
   
   //URG_C_SSM.create(1.0, 0.025);
   URG_L_SSM.create(1.0, 0.025);
   URG_R_SSM.create(1.0, 0.025);
   //URG_RL_SSM.create(1.0, 0.025);
   //URG_FR_SSM.create(1.0, 0.025);
   //URG_CR_SSM.create(1.0, 0.025);
   //URG_RR_SSM.create(1.0, 0.025);
   //Tough_URG_SSM.create(1.0, 0.05);
   
   gps_SSM.create(3.0, 1.0);
   gyroData_SSM.create(1.0, 0.01);
   
   //ZMP_CAM_SSM.create(1.0, 0.01);
   //LANE_DATA_SSM.create(1.0, 0.025);
   
   //SERVO_ORDER_SSM.create(1.0, 0.01);
   //DRIVE_ORDER_SSM.create(1.0, 0.01);
   //LANE_RESULT_SSM.create(1.0, 0.01);
   //PATH_DATA_SSM.create(1.0, 0.01);
   //AUTO_CONTROL_SSM.create(1.0, 0.01);
   //SERVO_STATUS_SSM.create(1.0, 0.01);
   //SLIP_ANGLE_SSM.create(1.0, 0.01);
   
   cout << "create area -> URG_L, URG_R, Gyro, GPS" << endl;
   
   while(1){
      sleep(100000);
   }
   
   return 0;
}





