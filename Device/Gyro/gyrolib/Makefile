#=================================================
# Makefile for gyro API
#-------------------------------------------------
# Copyright (C) 2008
#
#=================================================

# API source file
API_SRC = CtrlGyro.c

# HEADER file
HEAD	= ${API_SRC:.c=.h}

# API object
API_OBJ = ${API_SRC:.c=.o}

# compiler
CC	= gcc

# compile option
CFLAGS	= -g -O2 -Wall -Werror -Wmissing-prototypes -Wno-unused-result

all:$(API_OBJ)

$(API_OBJ):$(HEAD)
.c.o:
	$(CC) $(CFLAGS) -c $<

# clean
clean:
	rm -f *.o core*
