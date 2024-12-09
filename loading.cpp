#include "loading.h"

ImageHandles g_imghandles;

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

	if ((g_imghandles.block = LoadGraph("media\\smp2_block.png")) == -1) return FALSE;
	if ((g_imghandles.diagblock = LoadGraph("media\\smp2_diagonalblock.png")) == -1) return FALSE;
	if ((g_imghandles.bullet = LoadGraph("media\\smp2_bullet.png")) == -1) return FALSE;
	if ((g_imghandles.knife = LoadGraph("media\\smp2_knife.png")) == -1) return FALSE;
	if ((g_imghandles.title = LoadGraph("media\\smp2_title.png")) == -1) return FALSE;

	return TRUE;
}
