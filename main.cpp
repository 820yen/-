#include "main.h"
#include "score.h"
#include<time.h>

//グローバル変数
//時間計測用変数
int g_lasttime = 0;		//直前の計測時間
float g_frametime = 0;	//1ループにかかった時間
int g_timerstart;		//タイマー用変数
int g_limittimerstart;	//タイマー用変数時間制限
int g_jumptimerstart;	//ジャンプの読み込み時間
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
int playerNumber;
int playerTimeMin, playerTimeSec;	//クリア時の残り時間を記録
BOOL g_scoreAdded = FALSE;			//スコアが追加されたかどうか
BOOL g_scoreTotaled = FALSE;		//スコアが加算されたかどうか
BOOL g_addedFlag = FALSE;

BOOL g_countDownFlag = FALSE;	//カウントダウン開始フラグ
int g_countDownEndTime;			//カウントダウン終了時刻

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
	SetGraphMode(1300, 730, 32);	//PCの解像度は1366, 768
	//DXライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	//スコアと番号を読みこむ
	LoadScore(scores);
	LoadPlayerNumber();

	srand((unsigned int)time(NULL));

	//画像を読み込み
	if (LoadGameImage() == FALSE) return -1;
	//BGM,効果音読み込み
	if (LoadGameSound() == FALSE) return -1;

	//フォント読みこみ
	//LPCSTR font_path = "media\\Buildingsandundertherailwaytracksfree_ver.otf"; // 読み込むフォントファイルのパス
	//if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	//}
	//else {
	//	return FALSE;
	//}
	//ChangeFont("Buildingsandundertherailwaytracksfree_ver", DX_CHARSET_DEFAULT);
	g_largefont = CreateFontToHandle("メイリオ", 90, -1, DX_FONTTYPE_ANTIALIASING);
	g_mediumfont = CreateFontToHandle("メイリオ", 65, -1, DX_FONTTYPE_ANTIALIASING);
	g_middlefont = CreateFontToHandle("メイリオ", 42, -1, DX_FONTTYPE_ANTIALIASING);
	g_normalfont = CreateFontToHandle("メイリオ", 30, -1, DX_FONTTYPE_ANTIALIASING);
	g_smallfont = CreateFontToHandle("メイリオ", 18, -1, DX_FONTTYPE_ANTIALIASING);
	SetFontSize(30);

	SetDrawScreen(DX_SCREEN_BACK);
	g_lasttime = GetNowCount() & INT_MAX;	//現在時刻の記録
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		//1ループにかかった時間を計測
		int curtime = GetNowCount();
		g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
		g_lasttime = curtime;
		if (g_lasttime - g_countDownEndTime <= 3000){
			g_limittimerstart = g_lasttime;
		}

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
	StopSoundMem(g_sndhandles.clear);
	if (CheckSoundMem(g_sndhandles.title) == 0) {
		PlaySoundMem(g_sndhandles.title, DX_PLAYTYPE_LOOP);
	}
	DrawGraph(0, 0, g_imghandles.title, FALSE);
	//キーをチェックして画面を切り替え
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (IsAKeyTrigger(key) == TRUE) {
		PlaySoundMem(g_sndhandles.countdown, DX_PLAYTYPE_BACK);
		g_gamestate = GAME_MAIN;
		g_limittimerstart = g_lasttime;
		g_countDownEndTime = g_lasttime;
		InitStage();
		playerScore = 0; //スコアをリセット
		g_TextStep = 0;	 //テキストステップをリセット
	}
}
//ゲーム本編描画
void DrawGameMain(){
	GameMain();
}
//ゲームクリア画面描画
void DrawGameClear(){

	int qKey = CheckHitKey(KEY_INPUT_Q);

	//スコアを一度だけ反映させる
	if (g_scoreTotaled == FALSE){
		//時間を記録
		playerTimeMin = g_limittimemin;
		playerTimeSec = g_limittimesec;

		//スコアを計算
		playerScore += g_scoretime + g_stagedata.hero.coinCount * 500;
		//playerScore -= g_stagedata.hero.deathCount * 10000;

		if (playerScore < 0){
			playerScore = 0;
		}
		g_scoreTotaled = TRUE;			//スコアが加算されたことを記録
	}

	//スコアをランキングに一度だけ追加
	if (g_scoreAdded == FALSE && g_TextStep >= 20) {
		AddScore(scores, playerScore);
		//拍手を再生
		PlaySoundMem(g_sndhandles.handclap, DX_PLAYTYPE_BACK);

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
			blackColor, g_largefont);
	}
	if (g_TextStep >= 6){
		if (10 <= playerTimeSec){
			DrawFormatString(162, 200, blackColor,
				"残り時間 : %d:%d", playerTimeMin, playerTimeSec);
		}
		else{
			DrawFormatString(162, 200, blackColor,
				"残り時間 : %d:0%d", playerTimeMin, playerTimeSec);
		}
	}
	if (g_TextStep >= 10){
		DrawFormatString(162, 280, blackColor,
			"落下回数：%d", g_stagedata.hero.deathCount);
	}
	if (g_TextStep >= 14){
		DrawGraph(162, 353, g_imghandles.kyabecoin, TRUE);
		DrawFormatString(202, 360, GetColor(0, 0, 0),
			"×%d", g_stagedata.hero.coinCount);
	}
	if (g_TextStep >= 20){
		SetFontSize(100);
		DrawFormatString(162, 450, blackColor,
			"スコア：%d", playerScore);
		SetFontSize(30);
	}
	if (g_TextStep >= 20){
		//Zキーでタイトル画面へ
		int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if (IsQKeyTrigger(qKey)) {
			g_gamestate = GAME_TITLE;
			g_addedFlag = TRUE;
			//フラグをリセット
			g_scoreTotaled = FALSE;
			g_scoreAdded = FALSE;
		}
	}
}
//ゲームオーバー画面描画
void DrawGameOver(){
	int qKey = CheckHitKey(KEY_INPUT_Q);

	StopSoundMem(g_sndhandles.main);
	if (CheckSoundMem(g_sndhandles.title) == 0) {
		PlaySoundMem(g_sndhandles.title, DX_PLAYTYPE_LOOP);
	}

	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (IsAKeyTrigger(key) == TRUE) {
		InitStage();
		g_timerstart = g_lasttime;
	}
	DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);
	//テキスト表示
	DrawStringToHandle(100, 200, "タイムオーバー",
		GetColor(255, 0, 0), g_largefont);
	//5秒経ったらタイトル画面へ
	if (IsQKeyTrigger(qKey)){
		g_gamestate = GAME_TITLE;
		g_addedFlag = TRUE;
		//フラグをリセット
		g_scoreTotaled = FALSE;
		g_scoreAdded = FALSE;
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