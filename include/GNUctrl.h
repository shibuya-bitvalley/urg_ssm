
extern void GNU_Init(void);
//extern void GNU_Plot(LRF *urg_data);
extern void GNU_Plot(int *peak, double *x, double *y, double *intensity, int num);
extern void GNU_Plot2(int *peak, double *dist, double left, double right, double left_dist, double right_dist, double *x, double *y, double *intensity, int num);
extern void GNU_Plot3(double *left_lane, double *right_lane, double *left_dist, double *right_dist);
extern void GNU_Plot4(double *x, double *y);
extern void GNU_Close(void);
