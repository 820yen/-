#include "gamemain.h"

char g_mapdata[MAXSTAGE][MAP_HEIGHT][MAP_WIDTH + 1];
StageData g_stagedata;

int g_savepoint = 0;
int g_coincheck = 0;
int g_randamstage;

BOOL g_limitflag = TRUE;	//���Ԑ����ŃQ�[���I�[�o�[�ɂȂ邩�I�ׂ�
BOOL g_debugflag = FALSE;	//�f�o�b�N�̎��ɕ�����₷�����邽�߂ɕ\�����邩�I�ׂ�
BOOL g_deviceflag = FALSE;	//�f�o�C�X���قȂ鎞�ɕύX����

//�t�F�[�h�A�E�g�ƃt�F�[�h�C��
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

//�J�E���g�_�E���p�ϐ�
int g_startTime;			//�Q�[���J�n����
int g_elapsedTime;			//�o�߂�������
int remainingTime;			//�c�莞��

//�X�e�[�W������
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

	//�J�E���g�_�E���̐ݒ�
	g_startTime = GetNowCount();
	g_countDownFlag = FALSE;

	//���C���X�e�[�W�Ȃ̍Đ�
	StopSoundMem(g_sndhandles.title);
	StopSoundMem(g_sndhandles.clear);
	if (CheckSoundMem(g_sndhandles.main) == 0) {
		PlaySoundMem(g_sndhandles.main, DX_PLAYTYPE_LOOP);
	}

	//��l���̈ʒu�ƃX�e�[�^�X��������
	g_stagedata.hero.x = 2 * IMG_CHIPSIZE;	//�S�[���O 500 * IMG_CHIPSIZE;
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

	//�t�F�[�h�C�x���g�̏�����
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
	//�N���A�n�_ scrollx = 25000;
}

void GameMain(){
	//�w�i�摜�̉��T�C�Y
	int size1x = 1096;
	int size2x = 1096;
	int size3x = 1460;
	int size4x = 1140;
	int size5x = 2393;

	//�w�i�摜
	//���w�i
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

	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̐ݒ�
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

	//�A�j���[�V�����J�E���^
	g_stagedata.animcounter++;
	g_stagedata.animcounter &= MAXINT;
	int ac = g_stagedata.animcounter / ANIM_RATE;

	DrawMap();
	DrawHero(ac);
	DrawEnemy(ac);

	if ((TIMELIMIT - (g_lasttime - g_limittimerstart) / 1000) <= 0 && g_limitflag == TRUE){
		g_gamestate = GAME_OVER;
		g_timerstart = g_lasttime;	//�^�C�}�[�Z�b�g
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

	//�J�E���g�_�E��
	g_elapsedTime = GetNowCount() - g_startTime;
	if (g_lasttime - g_countDownEndTime >= 3000){
		g_countDownFlag = TRUE;
	}
	if (g_countDownFlag == FALSE) {
		SetFontSize(150);
		remainingTime = 3 - g_elapsedTime / 1000;
		DrawFormatString(600, 280, GetColor(0, 0, 0), "%d", remainingTime);
		SetFontSize(16);
	}
	else{
		if (g_elapsedTime < 4000){
			SetFontSize(200);
			DrawString(450, 250, "GO!!", GetColor(0, 0, 0));
			SetFontSize(16);
		}
	}

	double mv = 200.0 * g_stagedata.hero.pushSpeed; //�ړ��ʌv�Z
	float hx = g_stagedata.hero.x;
	float hy = g_stagedata.hero.y;

	//�f�o�C�X�`�F�b�N
	if (Is159KeyTrigger(oneKey, fiveKey, nineKey)){
		if (g_deviceflag == FALSE) {
			g_deviceflag = TRUE;
		}
		else {
			g_deviceflag = FALSE;
		}
	}

	if (g_countDownFlag == TRUE){
		//Enter�L�[���������тɉ���
		if (g_deviceflag == TRUE){
			DrawString(100, 300, "�ʃf�o�C�X�p", GetColor(255, 255, 255));
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
	//�X�s�[�h�����Z
	mv = 200.0 * g_stagedata.hero.pushSpeed;

	//�S�[����̃X�s�[�h����
	if (g_savepoint == 4){
		StopSoundMem(g_sndhandles.main);
		if (CheckSoundMem(g_sndhandles.clear) == 0) {
			PlaySoundMem(g_sndhandles.clear, DX_PLAYTYPE_LOOP);
		}
		g_stagedata.hero.pushSpeed = 0;
		mv = 2;
	}

	//�W�����v����
	if (g_stagedata.hero.jumping == TRUE){
		g_stagedata.hero.jumppower -= GRAVITY * (g_lasttime - g_jumptimerstart) / 1000;
		hy -= (g_stagedata.hero.jumppower * g_frametime);
	}

	//�ړ�����
	hx += mv * (g_lasttime - g_jumptimerstart) / 1000 * 60;
	//������������
	if (g_stagedata.hero.pushSpeed > 0) {
		g_stagedata.hero.pushSpeed -= SLOWSPEED * (g_lasttime - g_jumptimerstart) / 1000 * 60;
	}
	else{
		g_stagedata.hero.pushSpeed = 0;
	}
	if (mv < 0) {
		mv = 0;
	}

	//�}�b�v�����蔻��
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

	//�ڒn�`�F�b�N
	if (atari.GL == TRUE || atari.GR == TRUE) {
		g_stagedata.hero.noground = FALSE;
		g_stagedata.hero.jumping = FALSE;
		g_stagedata.hero.jumppower = 0;
		g_stagedata.hero.jumpforward = 0;
		//�W�����v��ɒn�ʂɖ��܂�Ȃ��悤�␳
		hy = (float)((int)(hy / IMG_CHIPSIZE) * IMG_CHIPSIZE);
	}
	else {
		g_stagedata.hero.noground = TRUE;
		//�d�͂ŗ���
		g_stagedata.hero.jumping = TRUE;
	}
	
	//�W�����v��������2
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
	
	//��������
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
		//��l���̈ʒu�ƃX�e�[�^�X��������
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

	//�X�N���[���␳
	if (hx - g_stagedata.scrollx > SCROLL_STAPOS && hx <= (g_stagedata.mapwidth[4] - 20) * IMG_CHIPSIZE) {
		g_stagedata.scrollx += (hx - g_stagedata.hero.x);
		g_stagedata.scroll_stop = g_stagedata.scrollx;
	}
	else if (hx - g_stagedata.scrollx < -SCROLL_STAPOS && hx >= 4 * IMG_CHIPSIZE) {
		g_stagedata.scrollx -= (g_stagedata.hero.x - hx);
		g_stagedata.scroll_stop = g_stagedata.scrollx;
	}

	//�X�N���[���X�g�b�v
	if (hx > (g_stagedata.mapwidth[4] - 20) * IMG_CHIPSIZE){
		g_stagedata.scrollx = g_stagedata.scroll_stop;
	}


	g_stagedata.hero.x = hx;
	g_stagedata.hero.y = hy;

	SetFontSize(30);

	//��l���`��
	DrawRotaGraph2((int)(g_stagedata.hero.x - g_stagedata.scrollx), 
		(int)g_stagedata.hero.y, 0, 0, 1, 0,
		g_imghandles.hero[ac % ANIMFRAME], TRUE, g_stagedata.hero.turn);

	//�X�s�[�h���[�^
	DrawFormatString(100, 200, GetColor(0, 0, 0),
		"�X�s�[�h�F%.2f", mv);
	//�R�C��������
	DrawFormatString(100, 170, GetColor(0, 0, 0),
		"�R�C���F%d", g_stagedata.hero.coinCount);
	//�c�莞��
	SetFontSize(50);
	DrawBox(585, 45, 720, 100, GetColor(255, 255, 255), TRUE);
	DrawBox(585, 45, 720, 100, GetColor(0, 0, 0), FALSE);
	if (10 <= g_limittimesec){
		DrawFormatString(600, 50, GetColor(0, 0, 0),
			"%d:%d", g_limittimemin, g_limittimesec);
	}
	else{
		DrawFormatString(600, 50, GetColor(0, 0, 0),
			"%d:0%d", g_limittimemin, g_limittimesec);
	}
	SetFontSize(30);
	//�c�苗��
	DrawFormatString(1000, 300, GetColor(0, 0, 0),
		"�ٓ��܂ŁF%.1fkm ��", ((g_stagedata.mapwidth[4] - g_stagedata.hero.x / 50)
		/ (g_stagedata.mapwidth[4] - 2))
		 * 17.5);
	//���S��
	DrawFormatString(100, 230, GetColor(0, 0, 0),
		"���S�񐔁F%d", g_stagedata.hero.deathCount);
	

	//�f�o�b�O���[�h
	if (IsQKeyTrigger(qKey)){
		if (g_debugflag == FALSE) g_debugflag = TRUE;
		else g_debugflag = FALSE;
	}

	//�^�C�����~�b�g�̃I���I�t
	if (IsWKeyTrigger(wKey)){
		if (g_limitflag == FALSE) g_limitflag = TRUE;
		else {
			g_limitflag = FALSE;
		}
	}
	//�^�C�����~�b�g�̃e�L�X�g�\��
	if (g_limitflag == FALSE){
		DrawString(100, 260, "�^�C�����~�b�g�F�I�t", GetColor(255, 255, 255));
	}
	SetFontSize(16);
}

//�u���b�N�Ƃ̓����蔻��
BOOL _CheckBlockSub(float x, float y){
	char blockType;
	int mx = (int)(x / IMG_CHIPSIZE);
	int my = (int)(y / IMG_CHIPSIZE);
	//�}�b�v�͈̔͊O�Ȃ�FALSE
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
		g_gamestate = GAME_CLEAR;
		g_timerstart = g_lasttime;
		return FALSE;
	}

	//�ʏ�u���b�N
	if (blockType == 'A') return TRUE;
	if (blockType != '0') return TRUE;
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

	//�f�o�b�O�A�����蔻�����
	if (g_debugflag == TRUE){
		//�X�N���[���̍��W
		DrawFormatString(100, 260, GetColor(0,0,0),
			"scrollx�F%lf", g_stagedata.scrollx);
		DrawFormatString(1000, 400, GetColor(0, 0, 0),
			"SAVEPOINT�F%d ", g_savepoint);

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

//�}�b�v�`��
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

			//�u���b�N�`��iA�`Z�jz
			if (cell >= 'A' && cell <= 'Z'){
				switch (cell){
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
						g_imghandles.tsuchic, TRUE);
					break;
				case 'L':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.tsuchil, TRUE);
					break;
				case 'M':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.tsuchir, TRUE);
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
			//�����X�^�[�̌��o (1�`9)
			if (cell >= '1' && cell <= '9') {
				SetEnemy(x + sc, y);  //�G�̈ʒu������ SetEnemy �ɓn��
			}
		}
	}
}

//�L�[�g���K�[����
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