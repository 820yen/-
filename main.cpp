#include "main.h"
#include "score.h"
#include<time.h>

//グローバル変数
//時間計測用変数
int g_lasttime = 0;		//直前の計測時間
float g_frametime = 0;	//1ループにかかった時間
int g_timerstart;		//タイマー用変数
int g_limittimerstart;	//タイマー用変数時間制限
int g_limittimemin;	//時間制限分
int g_limittimesec;	//時間制限秒
int g_scoretime;	//スコアタイム

//テキスト表示
int displayInterval = 30;	//次のテキスト表示までの時間
int g_TextTimer;			//テキスト表示変数
int g_TextStep;				//現在のテキスト表示のステップ

int g_stagenumber;	//ステージ番号

GameState g_gamestate = GAME_TITLE;

int g_gametitleimg;			//タイトルイメージ
int g_heroimg;				//画像
float g_hx = 0, g_hy = 0;	//座標
//ボタン
BOOL g_akey_prev;			//直前のAボタンの状態
//フォント
int g_smallfont;			//小サイズフォントハンドル
int g_normalfont;			//小中サイズフォントハンドル
int g_middlefont;			//中サイズフォントハンドル
int g_mediumfont;			//中大サイズフォントハンドル
int g_largefont;			//大サイズフォントハンドル

//スコア管理用変数
ScoreData scores[MAXRANKING] = { 0 };
int playerScore = 0;				//プレイヤースコア
int playerTimeMin, playerTimeSec;	//クリア時の残り時間を記録
BOOL g_scoreAdded = FALSE;			//スコアが追加されたかどうか
BOOL g_scoreTotaled = FALSE;		//スコアが加算されたかどうか

//色
int blownColor = GetColor(134, 74, 43);
int orangeColor = GetColor(255, 160, 16);
int yellowColor = GetColor(227, 199, 0);
int whiteColor = GetColor(255, 255, 255);
int pinkColor = GetColor(255, 0, 255);
int blackColor = GetColor(0, 0, 0);
int blueColor = GetColor(80, 128, 255);

int WINAPI WinMain(HINSTANCE h1, HINSTANCE hP, LPSTR lpC, int nC){
	//ウィンドウモードにする
	ChangeWindowMode(FALSE);
	//ウィンドウサイズを変更する
	SetGraphMode(1300, 730, 32);
	//DXライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	//スコアと番号を読みこむ
	LoadScore(scores);
	LoadPlayerNumber();

	srand((unsigned int)time(NULL));

	//画像を読み込み
	if (LoadGameImage() == FALSE) return -1;
	g_largefont = CreateFontToHandle("メイリオ", 90, -1, DX_FONTTYPE_ANTIALIASING);
	g_mediumfont = CreateFontToHandle("メイリオ", 65, -1, DX_FONTTYPE_ANTIALIASING);
	g_middlefont = CreateFontToHandle("メイリオ", 42, -1, DX_FONTTYPE_ANTIALIASING);
	g_normalfont = CreateFontToHandle("メイリオ", 30, -1, DX_FONTTYPE_ANTIALIASING);
	g_smallfont = CreateFontToHandle("メイリオ", 18, -1, DX_FONTTYPE_ANTIALIASING);

	SetDrawScreen(DX_SCREEN_BACK);
	g_lasttime = GetNowCount() & INT_MAX;	//現在時刻の記録
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		//1ループにかかった時間を計測
		int curtime = GetNowCount();
		g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
		g_lasttime = curtime;
		g_limittimemin = (TIMELIMIT - (g_lasttime - g_limittimerstart) / 1000) / 60;
		g_limittimesec = (TIMELIMIT - (g_lasttime - g_limittimerstart) / 1000) % 60;
		g_scoretime = TIMELIMIT * 1000 - ((g_lasttime - g_limittimerstart));

		ClearDrawScreen();
		//画面描画関数に切り替え
		switch (g_gamestate)
		{
		case GAME_TITLE:
			DrawGameTitle();
			break;
		case GAME_MAIN:
			DrawGameMain();
			break;
		case GAME_CLEAR:
			DrawGameClear();
			break;
		case GAME_OVER:
			DrawGameOver();
			break;
		default:
			break;
		}
		//ランキングを右上に表示
        DrawRanking(scores);

		ScreenFlip();
	}
	//ゲーム終了時にスコアを保存
	SaveScore(scores);
	SavePlayerNumber();

	//待機
	WaitKey();
	//DXライブラリの終了処理
	DxLib_End();
	return 0;
}

//タイトル画面描画
void DrawGameTitle(){
	DrawGraph(0, 0, g_imghandles.title, FALSE);
	//テキスト表示
	DrawStringToHandle(30, 440, "Zキーでゲームスタート",
		GetColor(255, 0, 255), g_middlefont);
	DrawStringToHandle(30, 490, "カーソルキーで左右に移動",
		GetColor(0, 0, 0), g_middlefont);
	DrawStringToHandle(30, 540, "Zキーでジャンプ、Xキーでナイフ投げ、",
		GetColor(0, 0, 0), g_middlefont);
	//キーをチェックして画面を切り替え
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (IsAKeyTrigger(key) == TRUE) {
		g_gamestate = GAME_MAIN;
		g_limittimerstart = g_lasttime;
		InitStage();
		playerScore = 0; //スコアをリセット
	}
}
//ゲーム本編描画
void DrawGameMain(){
	GameMain();
}
//ゲームクリア画面描画
void DrawGameClear(){
	DrawBox(0, 0, 1300, 730, blackColor, TRUE);

	//スコアを一度だけ反映させる
	if (g_scoreTotaled == FALSE){
		//時間を記録
		playerTimeMin = g_limittimemin;
		playerTimeSec = g_limittimesec;

		//スコアを計算
		playerScore += g_scoretime + g_stagedata.hero.coinCount * 100;
		playerScore -= g_stagedata.hero.deathCount * 10000;

		if (playerScore < 0){
			playerScore = 0;
		}
		g_scoreTotaled = TRUE;			//スコアが加算されたことを記録
	}

	//スコアをランキングに一度だけ追加
	if (g_scoreAdded == FALSE) {
		AddScore(scores, playerScore);
		g_scoreAdded = TRUE;			//スコアが追加されたことを記録
	}

	//通常描画処理
	g_TextTimer++;	//タイマーを進める
	if (g_TextTimer >= displayInterval){
		g_TextTimer = 0;	//タイマーをリセット
		g_TextStep++;		//ステップを進める
	}

	//テキスト表示
	if (g_TextStep >= 2){
		DrawStringToHandle(200, 50, "ゲームクリア",
			orangeColor, g_largefont);
	}
	if (g_TextStep >= 4){
		SetFontSize(30);
		if (10 <= playerTimeSec){
			DrawFormatString(162, 200, whiteColor,
				"残り時間 : %d:%d", playerTimeMin, playerTimeSec);
		}
		else{
			DrawFormatString(162, 200, whiteColor,
				"残り時間 : %d:0%d", playerTimeMin, playerTimeSec);
		}
		SetFontSize(16);
	}
	if (g_TextStep >= 5){
		SetFontSize(30);
		DrawFormatString(162, 280, whiteColor,
			"死亡回数：%d", g_stagedata.hero.deathCount);
		SetFontSize(16);
	}
	if (g_TextStep >= 6){
		SetFontSize(30);
		DrawFormatString(162, 360, whiteColor,
			"コイン枚数：%d", g_stagedata.hero.coinCount);
		SetFontSize(16);
	}
	if (g_TextStep >= 10){
		SetFontSize(50);
		DrawFormatString(162, 480, yellowColor,
			"スコア：%d", playerScore);
		SetFontSize(16);
	}
	if (g_TextStep >= 10){
		DrawStringToHandle(162, 600, "Enterキーでタイトルに戻る",
			blueColor, g_smallfont);

		//Enterキーでタイトル画面へ
		int enterKey = CheckHitKey(KEY_INPUT_RETURN);
		if (IsEnterKeyTrigger(enterKey) == TRUE) {
			g_gamestate = GAME_TITLE;
			//フラグをリセット
			g_scoreTotaled = FALSE;
			g_scoreAdded = FALSE;
		}
	}
}
//ゲームオーバー画面描画
void DrawGameOver(){

	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (IsAKeyTrigger(key) == TRUE) {
		InitStage();
		g_timerstart = g_lasttime;
	}
	DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);
	//テキスト表示
	DrawStringToHandle(100, 200, "ゲームオーバー",
		GetColor(255, 0, 0), g_largefont);
	//5秒経ったらタイトル画面へ
	if (g_lasttime - g_timerstart > 500) {
		g_gamestate = GAME_TITLE;
	}
}

//キートリガー処理
BOOL IsAKeyTrigger(int key){
	if (key & PAD_INPUT_A){
		if (g_akey_prev == FALSE){
			g_akey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_akey_prev = FALSE;
	}
	return FALSE;
}