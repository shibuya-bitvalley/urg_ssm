
#include "../../include/GPS.h"

#include <stdio.h>
#include <stdlib.h>
#include <ssm.h>
#include <math.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
	SSM_sid sid_gps;	// GPSのSSMID
	gps_t gpsdata;		// GPSのデータを格納する構造体
	double time_gps;	// GPSのデータのSSM時刻

	initSSM();
	
	sid_gps = openSSM("gps", 0, 0);
	
	while (1)
	{
		readSSM(sid_gps, (char *)&gpsdata, &time_gps, -1);
		
		printf("%s\n", gpsdata.GPGGA);
		printf("%s\n", gpsdata.GPVTG);
		//printf("%lf,%lf\n", gpsdata.t_deg, gpsdata.speed);
		
		usleep(100000);
	}
	
	return 0;
}
		
		
		
		


