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
	int tsuchic, tsuchi, ganpeki, wood;
	int sogenc, sogenl, sogenr;
	int kumoc, kumol, kumor;
	int gold;
};
extern ImageHandles g_imghandles;

struct SoundHandles{
	int coin;
	int jump;
	int drop;
};
extern SoundHandles g_sndhandles;

BOOL LoadGameImage();
BOOL LoadGameSound();

#endif