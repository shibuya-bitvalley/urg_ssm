#===============================================================
# Makefile for program using Gyro API
#---------------------------------------------------------------
# author : Kiyoshi MATSUO
#
# プログラムソースファイルに指定されているプログラムがコンパイルされる
#===============================================================

#
# マクロ定義部
#

# プログラムソースファイル
SRC	= GyroSSM.c

# オブジェクトファイル
OBJ	= ${SRC:.c=.o}

# 実行ファイル
TARGET  = ${SRC:.c=}

# ライブラリのあるディレクトリ
LIB_DIR = gyrolib/

# API name
API_NAME = $(LIB_DIR)CtrlGyro.c
API_SRC = $(API_NAME)

# ヘッダファイル
HEAD	= ${API_SRC:.c=.h}

# API ファイル
API_OBJ = ${API_SRC:.c=.o}

# コンパイラ
CC	= gcc

# コンパイルオプション
CFLAGS	= -g -Wall -Werror -Wmissing-prototypes -I$(LIB_DIR)
LIBS = -lm -lpthread -lssm

#
# 生成規則部
#

# 実行ファイルの生成
all: $(TARGET) subsystem
	 rm -f *.o
subsystem:
	cd $(LIB_DIR) && $(MAKE)
# API の生成


# コンパイル実行
# .c ファイルに対しては次の生成コマンドで .o ファイルを生成せよ
$(OBJ):subsystem $(HEAD)
.c.o:
	$(CC) $(CFLAGS) -c $<

# 最終的な実行ファイルを作る
$(TARGET): subsystem $(OBJ)
	$(CC) $(API_OBJ) $@.o $(LIBS) -o $@

# make clean
clean:
	rm -f $(TARGET) *.o core* && cd $(LIB_DIR) && make clean