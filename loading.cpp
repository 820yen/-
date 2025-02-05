#include "loading.h"

ImageHandles g_imghandles;
SoundHandles g_sndhandles;

BOOL LoadGameImage(){
	if (LoadDivGraph("media\\kyabetsutaro.png", 2, 2, 1, 50, 50, g_imghandles.hero) == -1){
		return FALSE;
	}
	if (LoadDivGraph("media\\coin.png", 6, 6, 1, 50, 50, g_imghandles.coin) == -1){
		return FALSE;
	}
	if (LoadDivGraph("media\\meta.png", 18, 2, 9, 500, 83, g_imghandles.meta) == -1){
		return FALSE;
	}
	if ((g_imghandles.kyabecoin = LoadGraph("media\\kyabecoin.png")) == -1) return FALSE;
	if ((g_imghandles.ranking = LoadGraph("media\\ranking.png")) == -1) return FALSE;
	if ((g_imghandles.clock = LoadGraph("media\\clock.png")) == -1) return FALSE;


	if ((g_imghandles.background[0] = LoadGraph("media\\stage1_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[1] = LoadGraph("media\\stage2_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[2] = LoadGraph("media\\stage3_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[3] = LoadGraph("media\\stage4_background.jpg")) == -1) return FALSE;
	if ((g_imghandles.background[4] = LoadGraph("media\\stage5_background.png")) == -1) return FALSE;

	if ((g_imghandles.block = LoadGraph("media\\smp2_block.png")) == -1) return FALSE;
	if ((g_imghandles.title = LoadGraph("media\\title.png")) == -1) return FALSE;

	if ((g_imghandles.sogenc = LoadGraph("media\\jimen_sogen_center_B.png")) == -1) return FALSE;
	if ((g_imghandles.sogenl = LoadGraph("media\\jimen_sogen_left_C.png")) == -1) return FALSE;
	if ((g_imghandles.sogenr = LoadGraph("media\\jimen_sogen_right_D.png")) == -1) return FALSE;
	if ((g_imghandles.kumoc = LoadGraph("media\\kumo.center_E.png")) == -1) return FALSE;
	if ((g_imghandles.kumol = LoadGraph("media\\kumo.left_F.png")) == -1) return FALSE;
	if ((g_imghandles.kumor = LoadGraph("media\\kumo.right_G.png")) == -1) return FALSE;
	if ((g_imghandles.tsuchi = LoadGraph("media\\tsuchi.center_H.png")) == -1) return FALSE;
	if ((g_imghandles.ganpeki = LoadGraph("media\\maptile_ganpeki_I.png")) == -1) return FALSE;
	if ((g_imghandles.wood = LoadGraph("media\\maptile_wood_J.png")) == -1) return FALSE;
	if ((g_imghandles.tsuchil = LoadGraph("media\\tsuchi.left_L.png")) == -1) return FALSE;
	if ((g_imghandles.tsuchir = LoadGraph("media\\tsuchi.right_M.png")) == -1) return FALSE;
	if ((g_imghandles.gold = LoadGraph("media\\block_gold_W.png")) == -1) return FALSE;
	if ((g_imghandles.builda = LoadGraph("media\\build_black_X.png")) == -1) return FALSE;
	if ((g_imghandles.buildb = LoadGraph("media\\build_win_Y.png")) == -1) return FALSE;
	if ((g_imghandles.buildc = LoadGraph("media\\build_win_Z.png")) == -1) return FALSE;

	if ((g_imghandles.kumobigl = LoadGraph("media\\kumo.big_left_U.png")) == -1) return FALSE;
	if ((g_imghandles.kumobir = LoadGraph("media\\kumo.big_right_T.png")) == -1) return FALSE;
	if ((g_imghandles.kumobl = LoadGraph("media\\kumo.block_R.png")) == -1) return FALSE;

	if ((g_imghandles.kumobol = LoadGraph("media\\kumo.bottom_left_S.png")) == -1) return FALSE;
	if ((g_imghandles.kumobor = LoadGraph("media\\kumo.bottom_right_P.png")) == -1) return FALSE;
	if ((g_imghandles.kumobo = LoadGraph("media\\kumo.bottom_Q.png")) == -1) return FALSE;

	if ((g_imghandles.kumotol = LoadGraph("media\\kumo.top_left_N.png")) == -1) return FALSE;
	if ((g_imghandles.kumotor = LoadGraph("media\\kumo.top_right_K.png")) == -1) return FALSE;
	if ((g_imghandles.kumoto = LoadGraph("media\\kumo.top_O.png")) == -1) return FALSE;

	if ((g_imghandles.zasso = LoadGraph("media\\zasso_touka_2.png")) == -1) return FALSE;
	if ((g_imghandles.hana = LoadGraph("media\\hana_touka_1.png")) == -1) return FALSE;
	
	if ((g_imghandles.kosekib = LoadGraph("media\\maptile_ganpeki_koseki_lightblue_3.png")) == -1) return FALSE;
	if ((g_imghandles.kosekir = LoadGraph("media\\maptile_ganpeki_koseki_red_4.png")) == -1) return FALSE;
	if ((g_imghandles.half = LoadGraph("media\\half_V.png")) == -1) return FALSE;
	
	if ((g_imghandles.tsuchic_t = LoadGraph("media\\tsuchi_center_toumei_a.png")) == -1) return FALSE;
	

	return TRUE;
}

//âπäyÅAå¯â âπ
BOOL LoadGameSound(){
	//if ((g_sndhandles.coin = LoadSoundMem("media\\coin.mp3")) == -1) return FALSE;
	if ((g_sndhandles.kyabetu = LoadSoundMem("media\\paku_low.mp3")) == -1) return FALSE;
	if ((g_sndhandles.jump = LoadSoundMem("media\\jump.mp3")) == -1) return FALSE;
	if ((g_sndhandles.drop = LoadSoundMem("media\\drop.mp3")) == -1) return FALSE;
	if ((g_sndhandles.handclap = LoadSoundMem("media\\handclap.mp3")) == -1) return FALSE;
	if ((g_sndhandles.countdown = LoadSoundMem("media\\countdown.mp3")) == -1) return FALSE;

	if ((g_sndhandles.title = LoadSoundMem("media\\BGM_title.mp3")) == -1) return FALSE;
	if ((g_sndhandles.main = LoadSoundMem("media\\BGM_main.mp3")) == -1) return FALSE;
	if ((g_sndhandles.clear = LoadSoundMem("media\\BGM_clear.mp3")) == -1) return FALSE;

	//âπó ÇÃïœçX(255:í èÌ 0:ñ≥âπ)
	ChangeVolumeSoundMem(128, g_sndhandles.title);
	ChangeVolumeSoundMem(120, g_sndhandles.main);
	ChangeVolumeSoundMem(130, g_sndhandles.clear);
	ChangeVolumeSoundMem(128, g_sndhandles.coin);
	ChangeVolumeSoundMem(100, g_sndhandles.jump);
	ChangeVolumeSoundMem(150, g_sndhandles.drop);

	return TRUE;
}
