#include "loading.h"

ImageHandles g_imghandles;
SoundHandles g_sndhandles;

BOOL LoadGameImage(){
	if (LoadDivGraph("media\\smp2_chara01.png", 2, 2, 1, 50, 50, g_imghandles.hero) == -1){
		return FALSE;
	}
	if (LoadDivGraph("media\\smp2_chara02.png", 2, 2, 1, 50, 50, g_imghandles.monster[0]) == -1){
		return FALSE;
	}
	if (LoadDivGraph("media\\smp2_chara03.png", 2, 2, 1, 50, 50, g_imghandles.monster[1]) == -1){
		return FALSE;
	}
	if (LoadDivGraph("media\\coin.png", 6, 6, 1, 50, 50, g_imghandles.coin) == -1){
		return FALSE;
	}

	if ((g_imghandles.background[0] = LoadGraph("media\\stage1_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[1] = LoadGraph("media\\stage2_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[2] = LoadGraph("media\\stage3_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[3] = LoadGraph("media\\stage4_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[4] = LoadGraph("media\\stage5_background.jpg")) == -1) return FALSE;

	if ((g_imghandles.block = LoadGraph("media\\smp2_block.png")) == -1) return FALSE;
	if ((g_imghandles.diagblock = LoadGraph("media\\smp2_diagonalblock.png")) == -1) return FALSE;
	if ((g_imghandles.bullet = LoadGraph("media\\smp2_bullet.png")) == -1) return FALSE;
	if ((g_imghandles.knife = LoadGraph("media\\smp2_knife.png")) == -1) return FALSE;
	if ((g_imghandles.title = LoadGraph("media\\title.png")) == -1) return FALSE;

	if ((g_imghandles.builda = LoadGraph("media\\build_black_X.png")) == -1) return FALSE;
	if ((g_imghandles.buildb = LoadGraph("media\\build_win_Y.png")) == -1) return FALSE;
	if ((g_imghandles.buildc = LoadGraph("media\\build_win_Z.png")) == -1) return FALSE;
	if ((g_imghandles.tsuchi = LoadGraph("media\\block_tsuchi_H.png")) == -1) return FALSE;
	if ((g_imghandles.ganpeki = LoadGraph("media\\maptile_ganpeki_I.png")) == -1) return FALSE;
	if ((g_imghandles.wood = LoadGraph("media\\maptile_wood_J.png")) == -1) return FALSE;

	return TRUE;
}

//âπäyÅAå¯â âπ
BOOL LoadGameSound(){
	if ((g_sndhandles.coin = LoadSoundMem("media\\coin.mp3")) == -1) return FALSE;
	if ((g_sndhandles.jump = LoadSoundMem("media\\jump.mp3")) == -1) return FALSE;
	if ((g_sndhandles.drop = LoadSoundMem("media\\drop.mp3")) == -1) return FALSE;

	//âπó ÇÃïœçX(255:í èÌ 0:ñ≥âπ)
	//ChangeVolumeSoundMem(128, g_sndhandles.coin);
	ChangeVolumeSoundMem(200, g_sndhandles.jump);
	//ChangeVolumeSoundMem(170, g_sndhandles.drop);

	return TRUE;
}
