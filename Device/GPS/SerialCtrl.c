#include "SerialCtrl.h"

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

extern int FD;
extern int OPEN;

/*!
  @brief :シリアル通信を開始する関数

  ボーレートとポートと通信モードを指定できる。

  @param baudrate : ボーレート
  @param port_name: ポート
  @retval  0 : 成功
  @retval -1 : ポートが開けなかったエラー
  @retval -2 : ボーレートが不正な値の場合
*/
int StartSerial(int baudrate, char *port_name)
{
	struct termios newtio;

	OPEN = 0;
	// ポートを開く(読み書き両用、 tty 制御無し)
	if ((FD = open(port_name, O_RDWR | O_NOCTTY)) == -1) {
		perror(port_name);
		return -1;
	}
	OPEN = 1;

	// 現在のシリアルポートの設定を退避する
	tcgetattr(FD, &OLDTIO);

	// 構造体をクリア
	memset(&newtio, 0, sizeof(newtio));

	// ボーレートの設定
	long baudrate_value = -1;
	switch (baudrate) {
	case 4800:
		baudrate_value = B4800;
		break;
	case 9600:
		baudrate_value = B9600;
		break;
	case 19200:
		baudrate_value = B19200;
		break;
	case 38400:
		baudrate_value = B38400;
		break;
	case 57600:
		baudrate_value = B57600;
		break;
	case 115200:
		baudrate_value = B115200;
		break;

	default:
		printf("Invalid Baudrate !");
		return -2;
	}

	// 端末の設定 (制御モード)
	/*
	  baudrate_value : ボーレートの設定
	  CS8            : 8n1 (8 ビット，ノンパリティ，ストップビット 1)
	  CLOCAL         : ローカル接続，モデム制御なし
	  CREAD          : 受信文字(receiving characters)を有効にする．
	*/
	newtio.c_cflag = baudrate_value | CS8 | CREAD | CLOCAL;

	// 端末の設定 (入力モード)
	// 特に何も設定しない
	newtio.c_iflag = 0;

	// 端末の設定 (出力モード)
	// 特に何も設定しない = Raw モード (出力データに何も手を加えない)で出力
	newtio.c_oflag = 0;

	// 端末の設定 (ローカルモード)
	// 特に何も設定しない = 非カノニカルモード
	newtio.c_lflag = 0;	
	
	// タイムアウト設定とread関数の設定
	newtio.c_cc[VTIME] = 0;	// キャラクタ間タイマは未使用
	newtio.c_cc[VMIN]  = 1;	// 1文字で読み込みは満足される

	// 受信したが、読んでないデータをフラッシュ
	tcflush(FD, TCIFLUSH);

	// ポートを newtio の設定にする
	tcsetattr(FD, TCSANOW, &newtio);

	return 0;
}

/*!
  @brief : 開いたポートを閉じる関数
  @retval 0 : 成功
  @retval -1: 失敗
*/
int EndSerial(void)
{
	if (OPEN)
	{
		tcflush(FD, TCIFLUSH);

		tcsetattr(FD, TCSANOW, &OLDTIO);
		return close(FD);
	}
	
	return 0;
}

/*!
  @brief : 1文字を送る関数
  @param character : 送りたい文字
*/
void SendChar(const char character)
{
	write(FD, &character, 1);
	
	return;
}

/*!
  @brief : 1文字を受信する関数
  @param *p_character
*/
void ReceiveChar(char *p_character)
{
	read(FD, p_character, 1);
	
	return;
}

/*!
  @brief : 文字列を送信する関数
  @param char *str : 送りたい文字列
*/
void SendString(char *str)
{
	int i = 0;

	// 1文字ずつ送信
	while (str[i] != '\0') SendChar(str[i++]);
	
	// キャラクタ通信に必要な終端記号を追加
	SendChar('\r');	// CR
	SendChar('\n');	// LF
	
	return;
}

/*!
  @brief : 文字列を受信する関数
  @param char *str : 受信した文字列を格納する配列
*/
void ReceiveString(char *str)
{
	int i = 0;
	char tmp;

	// 1文字ずつ受信
	while (1)
	{
		ReceiveChar(&tmp);
		
		// 終端記号(CR)が来たら、nullを挿入してLFを読みに行く
		if (tmp == '\r')
		{
			str[i] = '\0';
			continue;
		}
		else if (tmp == '\n')	// LFが来たら終了
		{
			break;
		}
		else
		{
			str[i] = tmp;
		}
		
		i++;
	}
	
	return;
}

/*!
  @brief : 指定した長さのバイナリ列を送信する関数
  @param char *str : 送りたいバイナリ列
  @param int len   : バイナリ列の長さ
*/
void SendStringB(char *str, int len)
{
	int i;
	
	for (i = 0; i < len; i++) SendChar(str[i]);
	
	return;
}

/*!
  @brief : 指定した長さのバイナリ列だけ受信する関数
  @param char *str : 受信したバイナリ列を格納する配列
  @param int len   : 受信したいバイナリ列の長さ
*/
void ReceiveStringB(char *str, int len)
{
	int i;
	
	for (i = 0; i < len; i++) ReceiveChar(&str[i]);
	
	return;
}



