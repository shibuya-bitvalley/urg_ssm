OPT =  -I/usr/local/include/urg_c -lurg_c -lm -lssm

all:urg2ssmR urg2ssmL

urg2ssmR:urg2ssm_RIGHT.cpp ../../include/LRF.hpp
	g++ -o urg2ssm_RIGHT urg2ssm_RIGHT.cpp $(OPT)

urg2ssmL:urg2ssm_LEFT.cpp ../../include/LRF.hpp
	g++ -o urg2ssm_LEFT urg2ssm_LEFT.cpp $(OPT)

clean:
	rm *.o

