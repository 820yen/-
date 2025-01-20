#ifndef __LOADING_H__
#define __LOADING_H__

#include <DxLib.h>

#define ANIMFRAME 2

struct ImageHandles{
	int hero[ANIMFRAME];
	int monster[2][ANIMFRAME];
	int block, diagblock, knife, bullet;
	int coin[6];
	int title;
	int background[5];
	int builda, buildb, buildc;
	int tsuchi, ganpeki, wood;
};
extern ImageHandles g_imghandles;

struct SoundHandles{
	int coin;
	int jump;
	int drop;
};
extern SoundHandles g_sndhandles;

BOOL LoadGameImage();

#endif