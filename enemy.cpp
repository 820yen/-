#include "enemy.h"

void SetEnemy(int mx, int my){
	char c = g_mapdata[0][my][mx];
	//空きを探す
	int i;
	for (i = 0; i < MAX_ENEMY; i++){
		if (g_stagedata.enemies[i].living == FALSE) break;
	}
	if (i < MAX_ENEMY){
		g_stagedata.enemies[i].living = TRUE;
		g_stagedata.enemies[i].x = (float)mx * IMG_CHIPSIZE;
		g_stagedata.enemies[i].y = (float)my * IMG_CHIPSIZE;
		g_stagedata.enemies[i].turn = FALSE;
		g_stagedata.enemies[i].noground = FALSE;
		g_stagedata.enemies[i].jumping = FALSE;
		g_stagedata.enemies[i].jumppower = 0;
		g_stagedata.enemies[i].jumpforward = 0;
		g_stagedata.enemies[i].type = (EnemyType)(c - '0');
		//生命力
		switch (g_stagedata.enemies[i].type)
		{
		case ET_CRAB:
			g_stagedata.enemies[i].life = 4;
			break;
		case ET_OCT:
			g_stagedata.enemies[i].life = 10;
			break;
		}
		//マップデータを空に
		g_mapdata[0][my][mx] = '0';
	}
}

void DrawEnemy(int ac){
	for (int i = 0; i < MAX_ENEMY; i++){
		if (g_stagedata.enemies[i].living == TRUE){
			EnemyType type = g_stagedata.enemies[i].type;
			switch (type)
			{
			case ET_CRAB:
				g_stagedata.enemies[i] =
					MoveCrabMonster(g_stagedata.enemies[i]);
				break;
			case ET_OCT:
				g_stagedata.enemies[i] =
					MoveOctMonster(g_stagedata.enemies[i]);
				break;
			case ET_BULLET:
				g_stagedata.enemies[i] =
					MoveBullet(g_stagedata.enemies[i]);
				break;
			case ET_COIN:
				g_stagedata.enemies[i] =
					CoinSetting(g_stagedata.enemies[i]);
				break;
			}

			
			if (type != ET_BULLET && type != ET_COIN){
				DrawRotaGraph2(
					(int)(g_stagedata.enemies[i].x - g_stagedata.scrollx),
					(int)g_stagedata.enemies[i].y,
					0, 0, 1, 0, g_imghandles.monster[(int)type - 2][ac % ANIMFRAME],
					TRUE, g_stagedata.enemies[i].turn);
			}
			if (type != ET_COIN) {
				AtariHeroAndMonster(i);
			}
		}
	}
}

CharaData MoveCrabMonster(CharaData cd){
	float mv = 100.0f * g_frametime; //移動量計算
	float hx = cd.x;
	float hy = cd.y;

	//ジャンプ処理
	if (cd.jumping == TRUE){
		mv = 0;	//ジャンプ中は自力で移動させない
		cd.jumppower -= GRAVITY;
		hy -= (cd.jumppower * g_frametime);
		hx += (cd.jumpforward * g_frametime);
	}
	//移動
	if (cd.turn == FALSE)	{
		hx -= mv;
	}
	else {
		hx += mv;
	}

	//マップ当たり判定
	AtariInfo atari = CheckBlock(hx, hy, cd.x);
	if (cd.jumping == FALSE){
		if (cd.turn == TRUE){
			if (atari.DR == TRUE || atari.UR == TRUE){
				hx = cd.x;
				cd.turn = !cd.turn;	//反転
			}
		}
		else {
			if (atari.DL == TRUE || atari.UL == TRUE){
				hx = cd.x;
				cd.turn = !cd.turn;	//反転
			}
		}
	}
	//接地チェック
	if (atari.GL == TRUE || atari.GR == TRUE) {
		cd.noground = FALSE;
		cd.jumping = FALSE;
		cd.jumppower = 0;
		cd.jumpforward = 0;
		//ジャンプ後に地面に埋まらないよう補正
		hy = (float)((int)(hy / IMG_CHIPSIZE) * IMG_CHIPSIZE);
	}
	else {
		cd.noground = TRUE;
		//重力で落下
		cd.jumping = TRUE;
	}

	cd.x = hx;
	cd.y = hy;

	return cd;
}

CharaData MoveOctMonster(CharaData cd){
	int time = g_stagedata.animcounter % 300;
	if (time == 0 || time == 10){
		//空きを探す
		int i;
		for (i = 0; i < MAX_ENEMY; i++){
			if (g_stagedata.enemies[i].living == FALSE) break;
		}
		if (i < MAX_ENEMY){
			g_stagedata.enemies[i].living = TRUE;
			g_stagedata.enemies[i].x = cd.x - IMG_CHIPSIZE / 2;
			g_stagedata.enemies[i].y = cd.y;
			g_stagedata.enemies[i].type = ET_BULLET;
			g_stagedata.enemies[i].life = 1;	//100にしてもいい
		}
	}

	return cd;
}

CharaData MoveBullet(CharaData cd){
	float mv = 280.0f * g_frametime; //移動量計算
	cd.x -= mv;
	//マップ当たり判定
	AtariInfo atari = CheckBlock(cd.x, cd.y, cd.x);
	if (atari.DL == TRUE || atari.UL == TRUE) cd.living = FALSE;
	DrawGraph((int)(cd.x - g_stagedata.scrollx), (int)cd.y,
		g_imghandles.bullet, TRUE);

	return cd;
}

CharaData CoinSetting(CharaData cd) {
	//アニメーションフレームの計算
	int coinAnimFrame = g_stagedata.animcounter / ANIM_RATE % 6;


	//主人公との当たり判定
	float ax1 = g_stagedata.hero.x + 10;
	float ay1 = g_stagedata.hero.y + 10;
	float ax2 = g_stagedata.hero.x + IMG_CHIPSIZE - 10;
	float ay2 = g_stagedata.hero.y + IMG_CHIPSIZE - 10;
	float bx1 = cd.x + 10;
	float by1 = cd.y + 10;
	float bx2 = cd.x + IMG_CHIPSIZE - 10;
	float by2 = cd.y + IMG_CHIPSIZE - 10;

	//コイン接触時の処理
	if ((ax1 < bx2) && (bx1 < ax2) && (ay1 < by2) && (by1 < ay2)) {
		cd.living = FALSE;
		g_stagedata.hero.coinCount++;
		return cd;
	}

	//コインを描画する
	DrawGraph(
		(int)(cd.x - g_stagedata.scrollx), (int)cd.y,
		g_imghandles.coin[coinAnimFrame], TRUE);

	return cd;
}

void AtariHeroAndMonster(int idx){
	//コインは当たり判定の対象外
	if (g_stagedata.enemies[idx].type == ET_COIN) {
		return;
	}
	//主人公との当たり判定
	float ax1 = g_stagedata.hero.x + 10;
	float ay1 = g_stagedata.hero.y + 10;
	float ax2 = g_stagedata.hero.x + IMG_CHIPSIZE - 10;
	float ay2 = g_stagedata.hero.y + IMG_CHIPSIZE - 10;
	float bx1 = g_stagedata.enemies[idx].x + 10;
	float by1 = g_stagedata.enemies[idx].y + 10;
	float bx2 = g_stagedata.enemies[idx].x + IMG_CHIPSIZE - 10;
	float by2 = g_stagedata.enemies[idx].y + IMG_CHIPSIZE - 10;

	//敵接触時の処理
	if ((ax1 < bx2) && (bx1 < ax2) && (ay1 < by2) && (by1 < ay2)){
		g_gamestate = GAME_OVER;
		g_timerstart = g_lasttime;
	}

	//ナイフとモンスターの当たり判定
	for (int i = 0; i < MAX_KNIFE; i++){
		if (g_stagedata.knives[i].living == FALSE) continue;
		float cx1 = g_stagedata.knives[i].x + 10;
		float cy1 = g_stagedata.knives[i].y + 30;
		float cx2 = g_stagedata.knives[i].x + IMG_CHIPSIZE - 10;
		float cy2 = g_stagedata.knives[i].y + IMG_CHIPSIZE - 30;

		if ((cx1 < bx2) && (bx1 < cx2) && (cy1 < by2) && (by1 < cy2)){
			g_stagedata.enemies[idx].life--;
			if (g_stagedata.enemies[idx].life < 1){
				g_stagedata.enemies[idx].living = FALSE;
			}
			g_stagedata.knives[i].living = FALSE;
		}
	}
}