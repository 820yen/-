#ifndef __MAIN_H__ 
#define __MAIN_H__

#include <DxLib.h>
#include "gamemain.h"
#include "loading.h"

//グローバル変数
//時間計測用変数
extern int g_lasttime;		//直前の計測時間
extern float g_frametime;	//1ループにかかった時間
extern int g_timerstart;	//タイマー用変数

enum GameState{				//ゲーム状態
	GAME_TITLE, GAME_MAIN,
	GAME_CLEAR, GAME_OVER
};
extern GameState g_gamestate;


//ボタン
extern BOOL g_akey_prev;	//直前のAボタンの状態
//フォント
extern int g_middlefont;	//中サイズフォントハンドル
extern int g_largefont;		//大サイズフォントハンドル
extern int g_smallfont;		//小サイズフォントハンドル

//関数プロトタイプ宣言
void DrawGameTitle();
void DrawGameMain();
void DrawGameClear();
void DrawGameOver();
BOOL IsAKeyTrigger(int key);

#endif
