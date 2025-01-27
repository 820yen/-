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
extern int g_limittimerstart;	//タイマー用変数時間制限
extern int g_jumptimerstart;	//ジャンプの読み込み時間
extern int g_limittimemin;	//時間制限分
extern int g_limittimesec;	//時間制限秒
extern int g_scoretime;	//スコアタイム
extern BOOL g_countDownFlag; //カウントダウンしたかどうか
extern int g_countDownEndTime; // カウントダウンが終了する時刻

enum GameState{				//ゲーム状態
	GAME_TITLE, GAME_MAIN,
	GAME_CLEAR, GAME_OVER
};
extern GameState g_gamestate;


//ボタン
extern BOOL g_akey_prev;	//直前のAボタンの状態
//フォント
extern int g_smallfont;		//小サイズフォントハンドル
extern int g_normalfont;	//小中サイズフォントハンドル
extern int g_middlefont;	//中サイズフォントハンドル
extern int g_mediumfont;	//中大サイズフォントハンドル
extern int g_largefont;		//大サイズフォントハンドル

//関数プロトタイプ宣言
void DrawGameTitle();
void DrawGameMain();
void DrawGameClear();
void DrawGameOver();
BOOL IsAKeyTrigger(int key);

#endif
