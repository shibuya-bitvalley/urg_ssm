/*!
  @file
  @brief gps SSM test

  @author Kiyoshi MATSUO
*/

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <ssm.h>

#include "CtrlGyro.h"

int main (int argc, char **argv)
{
  SSMID gyro;
  strcpy(gyro.sensorName, gyro_sensorName);
  gyro.dev_name = gyro_dev_name;
  //gyro.dev_name = argv[1];
  gyro.sensorID = gyro_sensorID;
  gyro.end_flag = 0;
  initSSM();
  
  gyro.sid = createSSM_time(gyro.sensorName, gyro.sensorID, GyroDataLength, 1, 0.01);
  pthread_mutex_init(&gyro.mutex, NULL);
  pthread_create(&Gyrothread, NULL, WriteGyroDataSSM, &gyro);
  
  while ( gyro.end_flag != 1) 
    {
    /*
      if (getchar() == 'q') 
	{
	  pthread_mutex_lock(&gyro.mutex);
	  gyro.end_flag = 1;
	  pthread_mutex_unlock(&gyro.mutex);
	  break;
	}
	*/
    }
  printf("end\n");
  pthread_join(Gyrothread, NULL);
  
  return 0;
}
