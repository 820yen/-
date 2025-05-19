#include "enemy.h"

void SetEnemy(int mx, int my){
	char c;
	if (g_coincheck == 0){
		c = g_mapdata[0][my][mx];
	}
	else if (g_coincheck == 1){
		c = g_mapdata[1][my][mx - g_stagedata.mapwidth[0]];
	}
	else if (g_coincheck == 2){
		c = g_mapdata[2][my][mx - g_stagedata.mapwidth[1]];
	}
	else if (g_coincheck == 3){
		c = g_mapdata[3][my][mx - g_stagedata.mapwidth[2]];
	}
	else if (g_coincheck == 4){
		c = g_mapdata[4][my][mx - g_stagedata.mapwidth[3]];
	}

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

		//マップデータを空に
		if (g_coincheck == 0){
			g_mapdata[0][my][mx] = '0';
		}
		else if (g_coincheck == 1){
			g_mapdata[1][my][mx - g_stagedata.mapwidth[0]] = '0';
		}
		else if (g_coincheck == 2){
			g_mapdata[2][my][mx - g_stagedata.mapwidth[1]] = '0';
		}
		else if (g_coincheck == 3){
			g_mapdata[3][my][mx - g_stagedata.mapwidth[2]] = '0';
		}
		else if (g_coincheck == 4){
			g_mapdata[4][my][mx - g_stagedata.mapwidth[3]] = '0';
		}
	}
}

void DrawEnemy(int ac){
	for (int i = 0; i < MAX_ENEMY; i++){
		if (g_stagedata.enemies[i].living == TRUE){
			EnemyType type = g_stagedata.enemies[i].type;
			switch (type)
			{
			case ET_COIN:
				g_stagedata.enemies[i] =
					CoinSetting(g_stagedata.enemies[i]);
				break;
			}
		}
	}
}

CharaData CoinSetting(CharaData cd) {
	//アニメーションフレームの計算
	//int coinAnimFrame = g_stagedata.animcounter / ANIM_RATE % 6;

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
		PlaySoundMem(g_sndhandles.kyabetu, DX_PLAYTYPE_BACK);
		cd.living = FALSE;
		g_stagedata.hero.coinCount++;
		return cd;
	}

	//コインを描画する
	//DrawGraph(
	//	(int)(cd.x - g_stagedata.scrollx), (int)cd.y,
	//	g_imghandles.coin[coinAnimFrame], TRUE);

	DrawGraph(
		(int)(cd.x - g_stagedata.scrollx), (int)cd.y,
		g_imghandles.kyabecoin, TRUE);

	return cd;
}
