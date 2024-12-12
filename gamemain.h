#ifndef __GAMEMAIN_H__ 
#define __GAMEMAIN_H__

#include <DxLib.h>
#include "main.h"
#include "enemy.h"

#define IMG_CHIPSIZE 50
#define HEROSPEED 0.001
#define SLOWSPEED 0.00005
#define MAP_WIDTH 511
#define MAP_HEIGHT 14
#define MAXSTAGE 5
#define SCR_WIDTH 26
#define ANIM_RATE 4
#define SCROLL_STAPOS 500
#define JUMP_POWER 750.0f
#define GRAVITY 30.0f
#define JUMP_FORWARD 200.0f
#define MAX_ENEMY 25
#define MAX_KNIFE 4

extern char g_mapdata[MAXSTAGE][MAP_HEIGHT][MAP_WIDTH + 1];

extern int g_stagenumber;

enum EnemyType{
	ET_CRAB = 2,
	ET_OCT = 3,
	ET_BULLET = 4
};

struct CharaData{
	float x, y;
	double pushSpeed;
	BOOL turn;
	BOOL noground;
	BOOL jumping;
	float jumppower, jumpforward;
	BOOL living;
	EnemyType type;
	int life;
};

struct StageData{
	int stagenum;
	int mapwidth;
	int animcounter;
	CharaData hero;
	float scrollx;
	float scroll_stop;
	CharaData enemies[MAX_ENEMY];
	CharaData knives[MAX_KNIFE];
	BOOL g_spacekey_prev;
	BOOL g_enterkey_prev;
	BOOL g_rkey_prev;
};
extern StageData g_stagedata;

struct AtariInfo{
	BOOL UL, UR, DL, DR, GL, GR;
};

void GameMain();
void InitStage();
void DrawHero(int ac);
void DrawMap();
AtariInfo CheckBlock(float x, float y, float rx);
void DrawKnife(int key);
BOOL IsSpaceKeyTrigger(int key);
BOOL IsEnterKeyTrigger(int key);
BOOL IsRKeyTrigger(int key);

#endif