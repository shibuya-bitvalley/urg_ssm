#ifndef SERIALCTRL_H
#define SERIALCTRL_H

/*!
  @file  :
  @brief : シリアル通信を行うためのライブラリ
  @autho : Kiyoshi MATSUO
  @date  : 2008/07/02
*/

#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int FD;

// ポートオープンフラグ
int OPEN;

char RECEIVEFLAG;

// 設定を退避させるための構造体
struct termios OLDTIO;

extern int StartSerial(int baudrate, char *port_name);

extern int EndSerial(void);

extern void SendChar(const char character);

extern void ReceiveChar(char *p_character);

#endif // SERIALCTRL_H
