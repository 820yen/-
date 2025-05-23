#include "gamemain.h"

char g_mapdata[MAXSTAGE][MAP_HEIGHT][MAP_WIDTH + 1];
StageData g_stagedata;

int g_savepoint = 0;
int g_coincheck = 0;
int g_randamstage;

BOOL g_limitflag = TRUE;	//時間制限でゲームオーバーになるか選べる
BOOL g_debugflag = FALSE;	//デバックの時に分かりやすくするために表示するか選べる
BOOL g_deviceflag = FALSE;	//デバイスが異なる時に変更する

//フェードアウトとフェードイン
int opacity = 0;
BOOL fadein = FALSE;
BOOL fadeout = FALSE;
BOOL savepoint2 = FALSE;
BOOL savepoint3 = FALSE;
BOOL savepoint4 = FALSE;
BOOL savepoint5 = FALSE;

BOOL switch2 = FALSE;
BOOL switch3 = FALSE;
BOOL switch4 = FALSE;
BOOL switch5 = FALSE;

int g_gameoveropacity;	//ゲームオーバーの背景透過変数

//カウントダウン用変数
int g_startTime;			//ゲーム開始時刻
int g_elapsedTime;			//経過した時刻
int remainingTime;			//残り時間

int green = 255;
int red = 0;
int blue = 0;

//ステージ初期化
void InitStage(){
	char buf[256];
	sprintf_s(buf, 256, "media\\stage%d.txt", 1);
	int fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[0][y], 256, fh);
	}
	FileRead_close(fh);
	g_randamstage = 2;// rand() % 3 + 1;
	sprintf_s(buf, 256, "media\\stage%d-%d.txt", 2, g_randamstage);
	fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[1][y], 256, fh);
	}
	FileRead_close(fh);
	g_randamstage = 2;//rand() % 3 + 1;
	sprintf_s(buf, 256, "media\\stage%d-%d.txt", 3, g_randamstage);
	fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[2][y], 256, fh);
	}
	FileRead_close(fh);
	g_randamstage = 2;// rand() % 3 + 1;
	sprintf_s(buf, 256, "media\\stage%d-%d.txt", 4, g_randamstage);
	fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[3][y], 256, fh);
	}
	FileRead_close(fh);
	sprintf_s(buf, 256, "media\\stage%d.txt", 5);
	fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[4][y], 256, fh);
	}
	FileRead_close(fh);
	g_stagedata.mapwidth[0] = (int)strlen(g_mapdata[0][0]);
	g_stagedata.mapwidth[1] = (int)strlen(g_mapdata[0][0]) + (int)strlen(g_mapdata[1][0]);
	g_stagedata.mapwidth[2] = (int)strlen(g_mapdata[0][0]) + (int)strlen(g_mapdata[1][0]) + (int)strlen(g_mapdata[2][0]);
	g_stagedata.mapwidth[3] = (int)strlen(g_mapdata[0][0]) + (int)strlen(g_mapdata[1][0]) + (int)strlen(g_mapdata[2][0]) + (int)strlen(g_mapdata[3][0]);
	g_stagedata.mapwidth[4] = (int)strlen(g_mapdata[0][0]) + (int)strlen(g_mapdata[1][0]) + (int)strlen(g_mapdata[2][0]) + (int)strlen(g_mapdata[3][0]) + (int)strlen(g_mapdata[4][0]);

	//カウントダウンの設定
	g_startTime = GetNowCount();
	g_countDownFlag = FALSE;

	//メインステージ曲の再生
	StopSoundMem(g_sndhandles.title);
	StopSoundMem(g_sndhandles.clear);
	if (CheckSoundMem(g_sndhandles.main) == 0) {
		PlaySoundMem(g_sndhandles.main, DX_PLAYTYPE_LOOP);
	}

	//主人公の位置とステータスを初期化
	g_stagedata.hero.x = 2 * IMG_CHIPSIZE;	//ゴール前 500 * IMG_CHIPSIZE;
	g_stagedata.hero.y = 10 * IMG_CHIPSIZE;
	g_stagedata.hero.pushSpeed = 0;
	g_stagedata.hero.turn = FALSE;
	g_stagedata.hero.jumping = FALSE;
	g_stagedata.hero.noground = FALSE;
	g_stagedata.hero.jumppower = 0;
	g_stagedata.hero.jumpforward = 0;
	g_stagedata.hero.coinCount = 0;
	g_stagedata.hero.deathCount = 0;
	g_savepoint = 0;

	g_gameoveropacity = 0; //ゲームオーバー透過変数初期化
	//フェードイベントの初期化
	opacity = 0;
	fadein = FALSE;
	fadeout = FALSE;
	savepoint2 = FALSE;
	savepoint3 = FALSE;
	savepoint4 = FALSE;
	savepoint5 = FALSE;
	switch2 = FALSE;
	switch3 = FALSE;
	switch4 = FALSE;
	switch5 = FALSE;

	ZeroMemory(g_stagedata.enemies, sizeof(g_stagedata.enemies));
	ZeroMemory(g_stagedata.knives, sizeof(g_stagedata.knives));
	g_stagedata.scrollx = 0;
	//クリア地点 scrollx = 25000;
}

void GameMain(){
	//背景画像の横サイズ
	int size1x = 1096;
	int size2x = 1096;
	int size3x = 1460;
	int size4x = 1140;
	int size5x = 2393;

	//背景画像
	//白背景
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0, 0, 1300, 730, GetColor(255, 255, 255), TRUE);
	//stage1
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawGraph(0 - g_stagedata.scrollx / 5, 0, g_imghandles.background[0], TRUE);
		DrawGraph(size1x - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[0], TRUE);

	//stage2 1800
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawGraph(size1x * 2 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[1], TRUE);
		DrawGraph(size1x * 3 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[1], TRUE);
		DrawGraph(size1x * 4 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[1], TRUE);

	//stage3 3300
	if (switch3 == TRUE){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(0, 0, 1300, 730, GetColor(255, 255, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawGraph(2000 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[2], TRUE);
		DrawGraph(2000 + size3x - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[2], TRUE);
		DrawGraph(2000 + size3x * 2 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[2], TRUE);
	}

	//stage4 4800
	if (switch4 == TRUE){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(0, 0, 1300, 730, GetColor(255, 255, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawGraph(3500 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[3], TRUE);
		DrawGraph(3500 + size4x - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[3], TRUE);
		DrawGraph(3500 + size4x * 2 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[3], TRUE);
	}

	//stage5 6300
	if (switch5 == TRUE){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(0, 0, 1300, 730, GetColor(255, 255, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawGraph(4700 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[4], TRUE);
		DrawGraph(5000 + size5x - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[4], TRUE);
		DrawGraph(5000 + size5x * 2 - int(g_stagedata.scrollx / 5), 0, g_imghandles.background[4], TRUE);
	}

	//フェードイン、フェードアウトの設定
	if (savepoint3 == FALSE){
		if (g_savepoint == 2){
			if (fadein == FALSE){
				opacity += 5;
				if (opacity == 255){
					fadein = TRUE;
					switch3 = TRUE;
				}
			}
			if (fadein == TRUE){
				opacity -= 5;
				if (opacity == 0){
					fadein = FALSE;
					savepoint3 = TRUE;
				}
			}
		}

	}
	if (savepoint4 == FALSE){
		if (g_savepoint == 3){
			if (fadein == FALSE){
				opacity += 5;
				if (opacity == 255){
					fadein = TRUE;
					switch4 = TRUE;
				}
			}
			if (fadein == TRUE){
				opacity -= 5;
				if (opacity == 0){
					fadein = FALSE;
					savepoint4= TRUE;
				}
			}
		}

	}
	if (savepoint5 == FALSE){
		if (g_savepoint == 4){
			if (fadein == FALSE){
				opacity += 5;
				if (opacity == 255){
					fadein = TRUE;
					switch5 = TRUE;
				}
			}
			if (fadein == TRUE){
				opacity -= 5;
				if (opacity == 0){
					fadein = FALSE;
					savepoint5 = TRUE;
				}
			}
		}

	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, opacity);
	DrawBox(0, 0, 1300, 730, GetColor(255, 255, 255), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//アニメーションカウンタ
	g_stagedata.animcounter++;
	g_stagedata.animcounter &= MAXINT;
	int ac = g_stagedata.animcounter / ANIM_RATE;

	DrawMap();
	DrawHero(ac);
	DrawEnemy(ac);
	//ゲームオーバー時の画面表示
	if ((TIMELIMIT - (g_lasttime - g_limittimerstart) / 1000) <= 0 && g_limitflag == TRUE && g_savepoint != 4){
		DrawGameOver();
	}

	
}

void DrawHero(int ac){
	int spaceKey = CheckHitKey(KEY_INPUT_SPACE);
	int enterKey = CheckHitKey(KEY_INPUT_RETURN);
	int rKey = CheckHitKey(KEY_INPUT_R);
	int qKey = CheckHitKey(KEY_INPUT_Q);
	int wKey = CheckHitKey(KEY_INPUT_W);

	int oneKey = CheckHitKey(KEY_INPUT_1);
	int fiveKey = CheckHitKey(KEY_INPUT_5);
	int nineKey = CheckHitKey(KEY_INPUT_9);

	int metaGraph;

	//カウントダウン
	g_elapsedTime = GetNowCount() - g_startTime;
	if (g_lasttime - g_countDownEndTime >= 3000){
		g_countDownFlag = TRUE;
	}
	if (g_countDownFlag == FALSE) {
		SetFontSize(150);
		remainingTime = 3 - g_elapsedTime / 1000;
		DrawFormatString(600, 280, GetColor(0, 0, 0), "%d", remainingTime);
		SetFontSize(30);
	}
	else{
		if (g_elapsedTime < 4000){
			SetFontSize(200);
			DrawString(450, 250, "GO!!", GetColor(0, 0, 0));
			SetFontSize(30);
		}
	}

	double mv = 200.0 * g_stagedata.hero.pushSpeed; //移動量計算
	float hx = g_stagedata.hero.x;
	float hy = g_stagedata.hero.y;

	//デバイスチェック
	if (Is159KeyTrigger(oneKey, fiveKey, nineKey)){
		if (g_deviceflag == FALSE) {
			g_deviceflag = TRUE;
		}
		else {
			g_deviceflag = FALSE;
		}
	}

	if (g_countDownFlag == TRUE){
		//Enterキーを押すたびに加速
		if (g_deviceflag == TRUE){
			DrawString(100, 300, "別デバイス用", GetColor(255, 255, 255));
			if (IsEnterKeyTrigger(enterKey) && g_savepoint != 4){
				if (mv <= 3){
					g_stagedata.hero.pushSpeed += HEROSPEED / 2;
				}
				else if (mv <= 5){
					g_stagedata.hero.pushSpeed += HEROSPEED / 4;
				}
				else if (mv <= 7){
					g_stagedata.hero.pushSpeed += HEROSPEED / 6;
				}
				else if (mv <= 9){
					g_stagedata.hero.pushSpeed += HEROSPEED / 8;
				}
				else if (mv <= 10){
					g_stagedata.hero.pushSpeed += HEROSPEED / 10;
				}
				else if (mv <= 12){
					g_stagedata.hero.pushSpeed += HEROSPEED / 14;
				}
				else if (mv <= 13){
					g_stagedata.hero.pushSpeed += HEROSPEED / 18;
				}
				else if (mv <= 15){
					g_stagedata.hero.pushSpeed += HEROSPEED / 20;
				}
				else{
					g_stagedata.hero.pushSpeed += HEROSPEED / 200;
				}
			}
		}
		else{
			if (IsEnterKeyTrigger(enterKey) && g_savepoint != 4){
				if (mv <= 3){
					g_stagedata.hero.pushSpeed += HEROSPEED;
				}
				else if (mv <= 5){
					g_stagedata.hero.pushSpeed += HEROSPEED / 2;
				}
				else if (mv <= 7){
					g_stagedata.hero.pushSpeed += HEROSPEED / 3;
				}
				else if (mv <= 9){
					g_stagedata.hero.pushSpeed += HEROSPEED / 4;
				}
				else if (mv <= 10){
					g_stagedata.hero.pushSpeed += HEROSPEED / 5;
				}
				else if (mv <= 12){
					g_stagedata.hero.pushSpeed += HEROSPEED / 7;
				}
				else if (mv <= 13){
					g_stagedata.hero.pushSpeed += HEROSPEED / 9;
				}
				else if (mv <= 15){
					g_stagedata.hero.pushSpeed += HEROSPEED / 10;
				}
				else{
					g_stagedata.hero.pushSpeed += HEROSPEED / 100;
				}
			}
		}

	}
	//スピードを加算
	mv = 200.0 * g_stagedata.hero.pushSpeed;

	//ゴール後のスピード調整
	if (g_savepoint == 4){
		StopSoundMem(g_sndhandles.main);
		if (CheckSoundMem(g_sndhandles.clear) == 0) {
			PlaySoundMem(g_sndhandles.clear, DX_PLAYTYPE_LOOP);
		}
		g_stagedata.hero.pushSpeed = 0;
		if ((g_stagedata.mapwidth[4] - g_stagedata.hero.x / 50 - 1) / (g_stagedata.mapwidth[4] - 2) > 0){
			 	mv = 2;
		}
		else{
			mv = 0;
		}
	}
	//ジャンプ処理
	if (g_stagedata.hero.jumping == TRUE){
		g_stagedata.hero.jumppower -= GRAVITY * (g_lasttime - g_jumptimerstart) / 1000;
		hy -= (g_stagedata.hero.jumppower * g_frametime);
	}

	//ゲームオーバー時にスピードを0に
	if ((TIMELIMIT - (g_lasttime - g_limittimerstart) / 1000) <= 0 && g_limitflag == TRUE && g_savepoint != 4){
		mv = 0;
		hy = g_stagedata.hero.y;
	}

	//移動処理
	hx += mv * (g_lasttime - g_jumptimerstart) / 1000 * 60;
	//加速減少処理
	if (g_stagedata.hero.pushSpeed > 0) {
		g_stagedata.hero.pushSpeed -= SLOWSPEED * (g_lasttime - g_jumptimerstart) / 1000 * 60;
	}
	else{
		g_stagedata.hero.pushSpeed = 0;
	}
	if (mv < 0) {
		mv = 0;
	}

	//マップ当たり判定
	AtariInfo atari = CheckBlock(hx, hy, g_stagedata.hero.x);
	if (g_stagedata.hero.turn == FALSE){
		if (atari.UL == TRUE){
			if (atari.ULU == TRUE){
				g_stagedata.hero.jumppower = -GRAVITY * (g_lasttime - g_jumptimerstart) / 1000;
			}
		}
		if (atari.UR == TRUE){
			if (atari.URU == TRUE){
				g_stagedata.hero.jumppower = -GRAVITY * (g_lasttime - g_jumptimerstart) / 1000;
			}
			if (atari.URR == TRUE){
				hx = g_stagedata.hero.x;
				g_stagedata.hero.pushSpeed -= SLOWSPEED * 2;
			}
		}
		if (atari.DR == TRUE){
			if (atari.DRR == TRUE){
				hx = g_stagedata.hero.x;
				g_stagedata.hero.pushSpeed -= SLOWSPEED * 2;
			}
		}
	}
	g_jumptimerstart = g_lasttime;

	//接地チェック
	if (atari.GL == TRUE || atari.GR == TRUE) {
		g_stagedata.hero.noground = FALSE;
		g_stagedata.hero.jumping = FALSE;
		g_stagedata.hero.jumppower = 0;
		g_stagedata.hero.jumpforward = 0;
		//ジャンプ後に地面に埋まらないよう補正
		hy = (float)((int)(hy / IMG_CHIPSIZE) * IMG_CHIPSIZE);
	}
	else {
		g_stagedata.hero.noground = TRUE;
		//重力で落下
		g_stagedata.hero.jumping = TRUE;
	}
	
	//ジャンプ処理その2
	if (g_countDownFlag == TRUE){
		if (g_stagedata.hero.jumping == FALSE){
			if (IsSpaceKeyTrigger(spaceKey) == TRUE && g_stagedata.hero.noground == FALSE && g_savepoint != 4)
			{
				PlaySoundMem(g_sndhandles.jump, DX_PLAYTYPE_BACK);

				g_stagedata.hero.jumping = TRUE;
				g_stagedata.hero.jumppower = JUMP_POWER;
				g_stagedata.hero.jumpforward = hx;
			}
		}
	}
	
	//落下判定
	if (hy > MAP_HEIGHT * IMG_CHIPSIZE - IMG_CHIPSIZE){
		mv = 0;
		g_stagedata.hero.deathCount++;
		PlaySoundMem(g_sndhandles.drop, DX_PLAYTYPE_BACK);

		if (g_savepoint == 0){
			hx = 2 * IMG_CHIPSIZE;
		}
		else if (g_savepoint == 1){
			hx = (g_stagedata.mapwidth[0] + 2) * IMG_CHIPSIZE;
		}
		else if (g_savepoint == 2){
			hx = (g_stagedata.mapwidth[1] + 2) * IMG_CHIPSIZE;
		}
		else if (g_savepoint == 3){
			hx = (g_stagedata.mapwidth[2] + 2) * IMG_CHIPSIZE;
		}
		else if (g_savepoint == 4){
			hx = (g_stagedata.mapwidth[3] + 2) * IMG_CHIPSIZE;
		}
		hy = 10 * IMG_CHIPSIZE;
		//主人公の位置とステータスを初期化
		g_stagedata.hero.y = 10 * IMG_CHIPSIZE;
		g_stagedata.hero.pushSpeed = 0;
		g_stagedata.hero.turn = FALSE;
		g_stagedata.hero.jumping = FALSE;
		g_stagedata.hero.noground = FALSE;
		g_stagedata.hero.jumppower = 0;
		g_stagedata.hero.jumpforward = 0;
		g_stagedata.scrollx = hx - 2 * IMG_CHIPSIZE;
		g_timerstart = g_lasttime;
	}

	//スクロール補正
	if (hx - g_stagedata.scrollx > SCROLL_STAPOS && hx <= (g_stagedata.mapwidth[4] - 20) * IMG_CHIPSIZE) {
		g_stagedata.scrollx += (hx - g_stagedata.hero.x);
		g_stagedata.scroll_stop = g_stagedata.scrollx;
	}
	else if (hx - g_stagedata.scrollx < -SCROLL_STAPOS && hx >= 4 * IMG_CHIPSIZE) {
		g_stagedata.scrollx -= (g_stagedata.hero.x - hx);
		g_stagedata.scroll_stop = g_stagedata.scrollx;
	}

	//スクロールストップ
	if (hx > (g_stagedata.mapwidth[4] - 20) * IMG_CHIPSIZE){
		g_stagedata.scrollx = g_stagedata.scroll_stop;
	}


	g_stagedata.hero.x = hx;
	g_stagedata.hero.y = hy;

	//主人公描画
	DrawRotaGraph2((int)(g_stagedata.hero.x - g_stagedata.scrollx), 
		(int)g_stagedata.hero.y, 0, 0, 1, 0,
		g_imghandles.hero[ac % ANIMFRAME], TRUE, g_stagedata.hero.turn);

	green = 255 - mv * 34 + 300;
	red = mv * 34;
	if (green < 0){
		green = 0;
	}
	if (green > 255){
		green = 255;
	}
	if (red < 0){
		red = 0;
	}
	if (red > 255){
		red = 255;
	}
	
	if (g_savepoint != 4){
		//スピードメータ
		DrawBox(10, 10, 500, 60, GetColor(red, green, 0), TRUE);
		DrawBoxGrad(15, 15, 207, 55, GetColor(0, 255, 0), GetColor(255, 255, 0),GetColor(0, 255, 0), GetColor(255, 255, 0));
		if (mv >= 6){
			DrawBoxGrad(207, 15, 335, 55, GetColor(255, 255, 0), GetColor(255, 240, 0), GetColor(255, 255, 0), GetColor(255, 240, 0));
		}
		if (mv >= 10){
			DrawBoxGrad(335, 15, 495, 55, GetColor(255, 240, 0), GetColor(255, 0, 0), GetColor(255, 240, 0), GetColor(255, 0, 0));
		}
		DrawBox(495, 15, 495 - 32 * (15 - mv), 55, GetColor(0, 0, 0), TRUE);
		DrawBox(10, 10, 500, 60, GetColor(0, 0, 0), FALSE);
		DrawBox(15, 15, 495, 55, GetColor(0, 0, 0), FALSE);
		for (int i = 1; i < 15; i++){
			DrawLine(15 + i * 32, 15, 15 + i * 32, 55, GetColor(0, 0, 0));
		}
		
		//メーター描画
		//metaGraph = mv * (18 / 15);
		//if (metaGraph < 9){
		//	metaGraph *= 2;
		//}
		//else{
		//	metaGraph = metaGraph - 8 + (metaGraph - 9);
		//}
		//DrawGraph(10, 10, g_imghandles.meta[(int)metaGraph], TRUE);

		//コイン所持数
		DrawGraph(20, 65, g_imghandles.kyabecoin, TRUE);
		char text_coin[64];
		sprintf_s(text_coin, "×%d", g_stagedata.hero.coinCount);
		DrawString(60, 72, text_coin, GetColor(255, 255, 255), GetColor(0, 0, 0));
		//残り時間
		SetFontSize(50);
		DrawBox(585, 45, 730, 100, GetColor(255, 255, 255), TRUE);
		DrawBox(585, 45, 730, 100, GetColor(0, 0, 0), FALSE);
		DrawGraph(530, 50, g_imghandles.clock, TRUE);
		if (10 <= g_limittimesec){
			char text[64];
			sprintf_s(text, "%d:%d", g_limittimemin, g_limittimesec);
			DrawString(592, 45, text, GetColor(255, 255, 255), GetColor(0, 0, 0));
		}
		else{
			char text[64];
			sprintf_s(text, "%d:0%d", g_limittimemin, g_limittimesec);
			DrawString(592, 45, text, GetColor(255, 255, 255), GetColor(0, 0, 0));
		}
		SetFontSize(30);
	}

	if (g_debugflag == TRUE){
		//スピード（数値）
		DrawFormatString(100, 150, GetColor(0, 0, 0),
			"speed：%lf", mv);
	}
	
	//残り距離
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	char text_distance[64];
	sprintf_s(text_distance, "ゴール：%.1fkm →", ((g_stagedata.mapwidth[4] - g_stagedata.hero.x / 50)
		/ (g_stagedata.mapwidth[4] - 2)) * 17.5);
	DrawString(950, 300, text_distance, GetColor(255, 255, 255), GetColor(0, 0, 0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//デバッグモード
	//if (IsQKeyTrigger(qKey)){
	//	if (g_debugflag == FALSE) g_debugflag = TRUE;
	//	else g_debugflag = FALSE;
	//}

	//タイムリミットのオンオフ
	if (IsWKeyTrigger(wKey)){
		if (g_limitflag == FALSE) g_limitflag = TRUE;
		else {
			g_limitflag = FALSE;
		}
	}
	//タイムリミットのテキスト表示
	if (g_limitflag == FALSE){
		DrawString(100, 260, "タイムリミット：オフ", GetColor(255, 255, 255));
	}
}

//ブロックとの当たり判定
BOOL _CheckBlockSub(float x, float y){
	char blockType;
	int mx = (int)(x / IMG_CHIPSIZE);
	int my = (int)(y / IMG_CHIPSIZE);
	//マップの範囲外ならFALSE
	if ((mx >= 0) && (mx < g_stagedata.mapwidth[0]) && (my < MAP_HEIGHT) && (my <= MAP_HEIGHT * IMG_CHIPSIZE)){
		blockType = g_mapdata[0][my][mx];
		g_savepoint = 0;
	}
	else if ((mx >= g_stagedata.mapwidth[0]) && (mx < g_stagedata.mapwidth[1]) && (my < MAP_HEIGHT) && (my <= MAP_HEIGHT * IMG_CHIPSIZE)){
		blockType = g_mapdata[1][my][mx - g_stagedata.mapwidth[0]];
		g_savepoint = 1;
	}
	else if ((mx >= g_stagedata.mapwidth[1]) && (mx < g_stagedata.mapwidth[2]) && (my < MAP_HEIGHT) && (my <= MAP_HEIGHT * IMG_CHIPSIZE)){
		blockType = g_mapdata[2][my][mx - g_stagedata.mapwidth[1]];
		g_savepoint = 2;
	}
	else if ((mx >= g_stagedata.mapwidth[2]) && (mx < g_stagedata.mapwidth[3]) && (my < MAP_HEIGHT) && (my <= MAP_HEIGHT * IMG_CHIPSIZE)){
		blockType = g_mapdata[3][my][mx - g_stagedata.mapwidth[2]];
		g_savepoint = 3;
	}
	else if ((mx >= g_stagedata.mapwidth[3]) && (mx < g_stagedata.mapwidth[4]) && (my < MAP_HEIGHT) && (my <= MAP_HEIGHT * IMG_CHIPSIZE)){
		blockType = g_mapdata[4][my][mx - g_stagedata.mapwidth[3]];
		g_savepoint = 4;
	}
	else if (mx < g_stagedata.mapwidth[4]){
		return FALSE;
	}
	if (mx >= g_stagedata.mapwidth[4]) {
		return FALSE;
	}

	//通常ブロック
	if (blockType == 'A') return TRUE;
	if (blockType != '0' && blockType != '1' && blockType != '2' && blockType != 'a') return TRUE;
	return FALSE;
}
AtariInfo CheckBlock(float x, float y, float rx){
	int debugAtari_x[10] = { x, x + IMG_CHIPSIZE - 1,						//UL,UR
							x, x + IMG_CHIPSIZE - 1,						//DL,DR
							rx, rx + IMG_CHIPSIZE - 1,						//GL,GR
							x + 10, x + IMG_CHIPSIZE - 10,					//ULU,URU
							x + IMG_CHIPSIZE + 30, x + IMG_CHIPSIZE + 30 };	//URR,DRR

	int debugAtari_y[10] = { y, y,											//UL,UR
							y + IMG_CHIPSIZE - 1, y + IMG_CHIPSIZE - 1,		//DL,DR
							y + IMG_CHIPSIZE, y + IMG_CHIPSIZE, 			//GL,GR
							y - 30, y - 30, 								//ULU,URU
							y + 10, y + IMG_CHIPSIZE - 1 };					//URR,DRR

	AtariInfo result;
	result.UL = _CheckBlockSub(debugAtari_x[0], debugAtari_y[0]);
	result.UR = _CheckBlockSub(debugAtari_x[1], debugAtari_y[1]);
	result.DL = _CheckBlockSub(debugAtari_x[2], debugAtari_y[2]);
	result.DR = _CheckBlockSub(debugAtari_x[3], debugAtari_y[3]);
	result.GL = _CheckBlockSub(debugAtari_x[4], debugAtari_y[4]);
	result.GR = _CheckBlockSub(debugAtari_x[5], debugAtari_y[5]);

	result.ULU = _CheckBlockSub(debugAtari_x[6], debugAtari_y[6]);
	result.URU = _CheckBlockSub(debugAtari_x[7], debugAtari_y[7]);
	result.URR = _CheckBlockSub(debugAtari_x[8], debugAtari_y[8]);
	result.DRR = _CheckBlockSub(debugAtari_x[9], debugAtari_y[9]);

	//デバッグ、当たり判定可視化
	if (g_debugflag == TRUE){
		//スクロールの座標
		DrawFormatString(100, 260, GetColor(0,0,0),
			"scrollx：%lf", g_stagedata.scrollx);
		DrawFormatString(1000, 400, GetColor(0, 0, 0),
			"SAVEPOINT：%d ", g_savepoint);

		DrawFormatString(150, 300, GetColor(0, 0, 0), "UL:%d\nUR:%d\nDL:%d\nDR:%d\nGL:%d\nGR:%d", result.UL, result.UR, result.DL, result.DR, result.GL, result.GR);
		DrawFormatString(100, 300, GetColor(0, 0, 0), "ULU:%d\nURU:%d\nURR:%d\nDRR:%d", result.ULU, result.URU, result.URR, result.DRR);

		DrawBox(debugAtari_x[0] - 2 - g_stagedata.scrollx, debugAtari_y[0] - 2, debugAtari_x[0] + 2 - g_stagedata.scrollx, debugAtari_y[0] + 2, GetColor(255, 0, 0), TRUE);//UL
		DrawBox(debugAtari_x[1] - 2 - g_stagedata.scrollx, debugAtari_y[1] - 2, debugAtari_x[1] + 2 - g_stagedata.scrollx, debugAtari_y[1] + 2, GetColor(255, 0, 0), TRUE);//UR
		DrawBox(debugAtari_x[2] - 2 - g_stagedata.scrollx, debugAtari_y[2] - 2, debugAtari_x[2] + 2 - g_stagedata.scrollx, debugAtari_y[2] + 2, GetColor(255, 0, 0), TRUE);//DL
		DrawBox(debugAtari_x[3] - 2 - g_stagedata.scrollx, debugAtari_y[3] - 2, debugAtari_x[3] + 2 - g_stagedata.scrollx, debugAtari_y[3] + 2, GetColor(255, 0, 0), TRUE);//DR

		DrawBox(debugAtari_x[4] - 2 - g_stagedata.scrollx, debugAtari_y[4] - 2, debugAtari_x[4] + 2 - g_stagedata.scrollx, debugAtari_y[4] + 2, GetColor(0, 0, 255), FALSE);//GL
		DrawBox(debugAtari_x[5] - 2 - g_stagedata.scrollx, debugAtari_y[5] - 2, debugAtari_x[5] + 2 - g_stagedata.scrollx, debugAtari_y[5] + 2, GetColor(0, 0, 255), FALSE);//GR

		DrawBox(debugAtari_x[6] - 2 - g_stagedata.scrollx, debugAtari_y[6] - 2, debugAtari_x[6] + 2 - g_stagedata.scrollx, debugAtari_y[6] + 2, GetColor(255, 0, 0), FALSE);//ULU
		DrawBox(debugAtari_x[7] - 2 - g_stagedata.scrollx, debugAtari_y[7] - 2, debugAtari_x[7] + 2 - g_stagedata.scrollx, debugAtari_y[7] + 2, GetColor(255, 0, 0), FALSE);//URU
		DrawBox(debugAtari_x[8] - 2 - g_stagedata.scrollx, debugAtari_y[8] - 2, debugAtari_x[8] + 2 - g_stagedata.scrollx, debugAtari_y[8] + 2, GetColor(255, 0, 0), FALSE);//URR
		DrawBox(debugAtari_x[9] - 2 - g_stagedata.scrollx, debugAtari_y[9] - 2, debugAtari_x[9] + 2 - g_stagedata.scrollx, debugAtari_y[9] + 2, GetColor(255, 0, 0), FALSE);//DRR
	}

	return result;
}

//マップ描画
void DrawMap(){
	char cell;
	int sc = (int)(g_stagedata.scrollx / IMG_CHIPSIZE);
	int shiftx = (int)g_stagedata.scrollx % IMG_CHIPSIZE;
	for (int y = 0; y < MAP_HEIGHT; y++){
		for (int x = 0; x < SCR_WIDTH + 1; x++){
			if (x + sc < g_stagedata.mapwidth[0]){
				cell = g_mapdata[0][y][x + sc];
				g_coincheck = 0;
			}
			else if (x + sc < g_stagedata.mapwidth[1]){
				cell = g_mapdata[1][y][x + sc - g_stagedata.mapwidth[0]];
				g_coincheck = 1;
			}
			else if (x + sc < g_stagedata.mapwidth[2]){
				cell = g_mapdata[2][y][x + sc - g_stagedata.mapwidth[1]];
				g_coincheck = 2;
			}
			else if (x + sc < g_stagedata.mapwidth[3]){
				cell = g_mapdata[3][y][x + sc - g_stagedata.mapwidth[2]];
				g_coincheck = 3;
			}
			else if (x + sc < g_stagedata.mapwidth[4]){
				cell = g_mapdata[4][y][x + sc - g_stagedata.mapwidth[3]];
				g_coincheck = 4;
			}

			//ブロック描画（A〜Z）z
			if ((cell >= 'a' && cell <= 'z') || (cell >= 'A' && cell <= 'Z') || (cell >= '1' && cell <= '9')){
				switch (cell){
				case '1':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.hana, TRUE);
					break;
				case '2':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.zasso, TRUE);
					break;
				case '3':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kosekib, TRUE);
					break;
				case '4':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kosekir, TRUE);
					break;

				case'a':
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130);
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.tsuchic_t, TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					break;

				case 'A':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.block, TRUE);
					break;
				case 'B':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.sogenc, TRUE);
					break;
				case 'C':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.sogenl, TRUE);
					break;
				case 'D':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.sogenr, TRUE);
					break;
				case 'E':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumoc, TRUE);
					break;
				case 'F':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumol, TRUE);
					break;
				case 'G':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumor, TRUE);
					break;
				case 'H':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.tsuchi, TRUE);
					break;
				case 'I':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.ganpeki, TRUE);
					break;
				case 'J':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.wood, TRUE);
					break;
				case 'K':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumotor, TRUE);
					break;
				case 'L':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.tsuchil, TRUE);
					break;
				case 'M':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.tsuchir, TRUE);
					break;
				case 'N':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumotol, TRUE);
					break;
				case 'O':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumoto, TRUE);
					break;
				case 'P':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumobor, TRUE);
					break;
				case 'Q':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumobo, TRUE);
					break;
				case 'R':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumobl, TRUE);
					break;
				case 'S':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumobol, TRUE);
					break;
				case 'T':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumobir, TRUE);
					break;
				case 'U':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.kumobigl, TRUE);
					break;
				case 'V':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.half, TRUE);
					break;
				case 'W':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.gold, TRUE);
					break;
				case 'X':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.builda, TRUE);
					break;
				case 'Y':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.buildb, TRUE);
					break;
				case 'Z':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.buildc, TRUE);
					break;
				}
			}
			//モンスターの検出 (1〜9)
			if (cell == '5') {
				SetEnemy(x + sc, y);  //敵の位置だけを SetEnemy に渡す
			}
		}
	}
	if (g_savepoint == 4){
		DrawGameClear();
	}
}

//キートリガー処理
BOOL IsSpaceKeyTrigger(int key){
 	if (key){
		if (g_stagedata.g_spacekey_prev == FALSE){
			g_stagedata.g_spacekey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_stagedata.g_spacekey_prev = FALSE;
	}
	return FALSE;
}
BOOL IsEnterKeyTrigger(int key){
	if (key){
		if (g_stagedata.g_enterkey_prev == FALSE){
			g_stagedata.g_enterkey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_stagedata.g_enterkey_prev = FALSE;
	}
	return FALSE;
}
BOOL IsRKeyTrigger(int key){
	if (key){
		if (g_stagedata.g_rkey_prev == FALSE){
			g_stagedata.g_rkey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_stagedata.g_rkey_prev = FALSE;
	}
	return FALSE;
}
BOOL IsQKeyTrigger(int key){
	if (key){
		if (g_stagedata.g_qkey_prev == FALSE){
			g_stagedata.g_qkey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_stagedata.g_qkey_prev = FALSE;
	}
	return FALSE;
}

BOOL IsWKeyTrigger(int key){
	if (key){
		if (g_stagedata.g_wkey_prev == FALSE){
			g_stagedata.g_wkey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_stagedata.g_wkey_prev = FALSE;
	}
	return FALSE;
}

BOOL Is159KeyTrigger(int key1, int key2, int key3){
	if (key1 && key2 && key3){
		if (g_stagedata.g_159key_prev == FALSE){
			g_stagedata.g_159key_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_stagedata.g_159key_prev = FALSE;
	}
	return FALSE;
}

// グラデーションカラーの四角形を描画する ( 各頂点の色を指定する: 左上, 右上, 左下, 右下 )
int DrawBoxGrad(int x1, int y1, int x2, int y2, int Colorlt, int Colorrt, int Colorlb, int Colorrb)
{
	const int		VERT_X = 8;						// 小さいグラデーションに分割する数
	const int		VERT_Y = 8;
	int				red[4], green[4], blue[4];		// 4つの頂点の色
	unsigned short	indices[(VERT_Y - 1) * (VERT_X - 1) * 6];
	VERTEX2D		vertex[VERT_Y][VERT_X];			// ポリゴンの頂点の情報

	if (Colorlb == 0) Colorlb = Colorlt;		// 四角形の左下の色が指定されていなければ上の色をコピーする
	if (Colorrb == 0) Colorrb = Colorrt;		// 四角形の右下の色が指定されていなければ上の色をコピーする
	GetColor2(Colorlt, &red[0], &green[0], &blue[0]);
	GetColor2(Colorrt, &red[1], &green[1], &blue[1]);
	GetColor2(Colorlb, &red[2], &green[2], &blue[2]);
	GetColor2(Colorrb, &red[3], &green[3], &blue[3]);

	for (int iy = 0; iy < VERT_Y; iy++)
	{
		for (int ix = 0; ix < VERT_X; ix++)
		{
			vertex[iy][ix].pos.z = 0.0f;			// 各頂点の情報をセット
			vertex[iy][ix].rhw = 1.0f;
			vertex[iy][ix].dif.a = 0xff;
			vertex[iy][ix].u = 0.0f;
			vertex[iy][ix].v = 0.0f;

			vertex[iy][ix].pos.x = static_cast<float>(x2 - x1) * ix / (VERT_X - 1) + static_cast<float>(x1);
			vertex[iy][ix].pos.y = static_cast<float>(y2 - y1) * iy / (VERT_Y - 1) + static_cast<float>(y1);

			vertex[iy][ix].dif.r = static_cast<BYTE>(
				(((red[0] * ((VERT_X - 1) - ix) + red[1] * ix) / (VERT_X - 1) * ((VERT_Y - 1) - iy)
				+ (red[2] * ((VERT_X - 1) - ix) + red[3] * ix) / (VERT_X - 1) * iy)) / (VERT_Y - 1));
			vertex[iy][ix].dif.g = static_cast<BYTE>(
				(((green[0] * ((VERT_X - 1) - ix) + green[1] * ix) / (VERT_X - 1) * ((VERT_Y - 1) - iy)
				+ (green[2] * ((VERT_X - 1) - ix) + green[3] * ix) / (VERT_X - 1) * iy)) / (VERT_Y - 1));
			vertex[iy][ix].dif.b = static_cast<BYTE>(
				(((blue[0] * ((VERT_X - 1) - ix) + blue[1] * ix) / (VERT_X - 1) * ((VERT_Y - 1) - iy)
				+ (blue[2] * ((VERT_X - 1) - ix) + blue[3] * ix) / (VERT_X - 1) * iy)) / (VERT_Y - 1));
		}
	}

	int i = 0;
	for (int iy = 0; iy < VERT_Y - 1; iy++)
	{
		for (int ix = 0; ix < VERT_X - 1; ix++)
		{
			indices[i] = static_cast<unsigned short>(iy * VERT_X + ix);					// 頂点インデックスを作成
			indices[i + 1] = static_cast<unsigned short>(iy * VERT_X + ix + 1);
			indices[i + 2] = static_cast<unsigned short>((iy + 1) * VERT_X + ix);
			indices[i + 3] = static_cast<unsigned short>((iy + 1) * VERT_X + ix + 1);
			indices[i + 4] = static_cast<unsigned short>((iy + 1) * VERT_X + ix);
			indices[i + 5] = static_cast<unsigned short>(iy * VERT_X + ix + 1);
			i += 6;
		}
	}

	return DrawPolygonIndexed2D(&vertex[0][0], VERT_X * VERT_Y, indices, (VERT_Y - 1) * (VERT_X - 1) * 2, DX_NONE_GRAPH, FALSE);
}