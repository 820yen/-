#ifndef __LOADING_H__
#define __LOADING_H__

#include <DxLib.h>

#define ANIMFRAME 2

struct ImageHandles{
	int hero[ANIMFRAME];
	int monster[2][ANIMFRAME];
	int block, diagblock, knife, bullet;
	int meta[18];
	int ranking, clock;
	int coin[6];
	int kyabecoin;
	int title;
	int background[5];
	int builda, buildb, buildc;
	int tsuchi, ganpeki, wood;
	int sogenc, sogenl, sogenr;
	int kumoc, kumol, kumor;
	int gold;
	int tsuchic, tsuchil, tsuchir, tsuchic_t;
	int kumobigl, kumobir, kumobl;
	int kumobol, kumobor, kumobo;
	int kumotol, kumotor, kumoto;
	int zasso, hana;
	int kosekib, kosekir, half;
};
extern ImageHandles g_imghandles;

struct SoundHandles{
	int coin;
	int kyabetu;
	int jump;
	int drop;
	int title, main, clear;
	int handclap, countdown;
};
extern SoundHandles g_sndhandles;

BOOL LoadGameImage();
BOOL LoadGameSound();

#endif