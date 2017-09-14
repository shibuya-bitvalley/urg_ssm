all: gps2ssm gps2ssmViewer

gps2ssm: gps2ssm.c
	gcc -o gps2ssm gps2ssm.c SerialCtrl.c -lssm

gps2ssmViewer: gps2ssmViewer.c
	gcc -o gps2ssmViewer gps2ssmViewer.c -lssm
