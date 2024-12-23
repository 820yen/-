#include "gamemain.h"

char g_mapdata[MAXSTAGE][MAP_HEIGHT][MAP_WIDTH + 1];
StageData g_stagedata;

int g_savepoint = 0;
int g_randamstage;

//�X�e�[�W������
void InitStage(){
	char buf[256];
	sprintf_s(buf, 256, "media\\stage%d.txt", 1);
	int fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[0][y], 256, fh);
	}
	FileRead_close(fh);
	g_randamstage = rand() % 3 + 1;
	sprintf_s(buf, 256, "media\\stage%d-%d.txt", 2, g_randamstage);
	fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[1][y], 256, fh);
	}
	FileRead_close(fh);
	g_randamstage = rand() % 3 + 1;
	sprintf_s(buf, 256, "media\\stage%d-%d.txt", 3, g_randamstage);
	fh = FileRead_open(buf);
	for (int y = 0; y < MAP_HEIGHT; y++){
		FileRead_gets(g_mapdata[2][y], 256, fh);
	}
	FileRead_close(fh);
	g_randamstage = rand() % 3 + 1;
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

	//��l���̈ʒu�ƃX�e�[�^�X��������
	g_stagedata.hero.x = 2 * IMG_CHIPSIZE;
	g_stagedata.hero.y = 10 * IMG_CHIPSIZE;
	g_stagedata.hero.pushSpeed = 0;
	g_stagedata.hero.turn = FALSE;
	g_stagedata.hero.jumping = FALSE;
	g_stagedata.hero.noground = FALSE;
	g_stagedata.hero.jumppower = 0;
	g_stagedata.hero.jumpforward = 0;


	ZeroMemory(g_stagedata.enemies, sizeof(g_stagedata.enemies));
	ZeroMemory(g_stagedata.knives, sizeof(g_stagedata.knives));
	g_stagedata.scrollx = 0;
}


void GameMain(){
	//�A�j���[�V�����J�E���^
	g_stagedata.animcounter++;
	g_stagedata.animcounter &= MAXINT;
	int ac = g_stagedata.animcounter / ANIM_RATE;

	DrawMap();
	DrawHero(ac);
	DrawEnemy(ac);

	//�Q�[���N���A����
	//if (g_stagedata.hero.x >= (g_stagedata.mapwidth - 1) * IMG_CHIPSIZE){
	//	g_gamestate = GAME_CLEAR;
	//	g_timerstart = g_lasttime;	//�^�C�}�[�Z�b�g
	//}
}

void DrawHero(int ac){
	int spaceKey = CheckHitKey(KEY_INPUT_SPACE);
	int enterKey = CheckHitKey(KEY_INPUT_RETURN);
	int rKey = CheckHitKey(KEY_INPUT_R);

	//Enter�L�[���������тɉ���
	if (IsEnterKeyTrigger(enterKey)){
		g_stagedata.hero.pushSpeed += HEROSPEED;
	}
	double mv = 200.0 * g_stagedata.hero.pushSpeed; //�ړ��ʌv�Z
	float hx = g_stagedata.hero.x;
	float hy = g_stagedata.hero.y;

	//�W�����v����
	if (g_stagedata.hero.jumping == TRUE){
		g_stagedata.hero.jumppower -= GRAVITY;
		hy -= (g_stagedata.hero.jumppower * g_frametime);
	}

	//�ړ�����
	hx += mv;
	//������������
	if (g_stagedata.hero.pushSpeed > 0) {
		g_stagedata.hero.pushSpeed -= SLOWSPEED;
		if (g_stagedata.hero.pushSpeed < 0) {
			g_stagedata.hero.pushSpeed = 0;
		}
	}

	//�}�b�v�����蔻��
	AtariInfo atari = CheckBlock(hx, hy, g_stagedata.hero.x);
	if (g_stagedata.hero.turn == FALSE){
		if (atari.DR == TRUE || atari.UR == TRUE){
			hx = g_stagedata.hero.x;
		}
	}
	else {
		if (atari.DL == TRUE || atari.UL == TRUE){
			hx = g_stagedata.hero.x;
		}
	}
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
	//�V��`�F�b�N
	if (g_stagedata.hero.jumping == TRUE){
		if (atari.UL == TRUE || atari.UR == TRUE){
			g_stagedata.hero.jumppower = 0;
			g_stagedata.hero.jumpforward = 0;
			hy = (float)((int)(hy / IMG_CHIPSIZE) * IMG_CHIPSIZE + IMG_CHIPSIZE);
		}
	}
	//�W�����v��������2
	if (g_stagedata.hero.jumping == FALSE){
 		if (IsSpaceKeyTrigger(spaceKey) == TRUE && g_stagedata.hero.noground == FALSE)
		{
			g_stagedata.hero.jumping = TRUE;
			g_stagedata.hero.jumppower = JUMP_POWER;
			g_stagedata.hero.jumpforward = hx;
		}
	}

	//��������
	if (hy > MAP_HEIGHT * IMG_CHIPSIZE){
		hx = 0;
		mv = 0;

		SetFontSize(32);
		DrawString(10, 50, "'R'�L�[�Ń��X�^�[�g", GetColor(255, 255, 255));
		SetFontSize(16);

		if (IsRKeyTrigger(rKey)) {
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
			InitStage();
			g_timerstart = g_lasttime;
		}
	}
	//y���W�̃��Z�b�g
	if (hy < 0.0f || hy > MAP_HEIGHT * IMG_CHIPSIZE) {
		hy = MAP_HEIGHT * IMG_CHIPSIZE;
	}

	//�X�N���[���␳
	if (hx - g_stagedata.scrollx > SCROLL_STAPOS) {
		g_stagedata.scrollx += (hx - g_stagedata.hero.x);
		g_stagedata.scroll_stop = g_stagedata.scrollx;
	}
	else if (hx - g_stagedata.scrollx < -SCROLL_STAPOS && hx >= 4 * IMG_CHIPSIZE) {
		g_stagedata.scrollx -= (g_stagedata.hero.x - hx);
		g_stagedata.scroll_stop = g_stagedata.scrollx;
	}
	g_stagedata.hero.x = hx;
	g_stagedata.hero.y = hy;

	//��ʒ[�ɗ�����X�N���[���X�g�b�v
	//if (hx >(MAP_WIDTH - 448) * IMG_CHIPSIZE){
	//	g_stagedata.scrollx = g_stagedata.scroll_stop;
	//}

	//��l���`��
	DrawRotaGraph2((int)(g_stagedata.hero.x - g_stagedata.scrollx), 
		(int)g_stagedata.hero.y, 0, 0, 1, 0,
		g_imghandles.hero[ac % ANIMFRAME], TRUE, g_stagedata.hero.turn);

	//�X�s�[�h���[�^
	DrawFormatString(100, 200, GetColor(255, 255, 255),
		"�X�s�[�h�F%.2f", mv);
}

//�u���b�N�Ƃ̓����蔻��
BOOL _CheckBlockSub(float x, float y){
	char blockType;
	int mx = (int)(x / IMG_CHIPSIZE);
	int my = (int)(y / IMG_CHIPSIZE);
	//�}�b�v�͈̔͊O�Ȃ�FALSE
	if ((mx >= 0) && (mx < g_stagedata.mapwidth[0]) && (my < MAP_HEIGHT) && (my > 0)){
		blockType = g_mapdata[0][my][mx];
		g_savepoint = 0;
	}
	else if ((mx >= g_stagedata.mapwidth[0]) && (mx < g_stagedata.mapwidth[1]) && (my < MAP_HEIGHT) && (my > 0)){
		blockType = g_mapdata[1][my][mx - g_stagedata.mapwidth[0]];
		g_savepoint = 1;
	}
	else if ((mx >= g_stagedata.mapwidth[1]) && (mx < g_stagedata.mapwidth[2]) && (my < MAP_HEIGHT) && (my > 0)){
		blockType = g_mapdata[2][my][mx - g_stagedata.mapwidth[1]];
		g_savepoint = 2;
	}
	else if ((mx >= g_stagedata.mapwidth[2]) && (mx < g_stagedata.mapwidth[3]) && (my < MAP_HEIGHT) && (my > 0)){
		blockType = g_mapdata[3][my][mx - g_stagedata.mapwidth[2]];
		g_savepoint = 3;
	}
	else if ((mx >= g_stagedata.mapwidth[3]) && (mx < g_stagedata.mapwidth[4]) && (my < MAP_HEIGHT) && (my > 0)){
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

	//�΂߃u���b�N�i�E�オ��j
	if (blockType == 'B') {
		
	}

	//�΂߃u���b�N�i���オ��j
	if (blockType == 'C') {
		
	}

	if (blockType != '0') return TRUE;
	return FALSE;
}
AtariInfo CheckBlock(float x, float y, float rx){
	AtariInfo result;
	result.UL = _CheckBlockSub(x, y);
	result.UR = _CheckBlockSub(x + IMG_CHIPSIZE - 1, y);
	result.DL = _CheckBlockSub(x, y + IMG_CHIPSIZE -1);
	result.DR = _CheckBlockSub(x + IMG_CHIPSIZE - 1, y + IMG_CHIPSIZE -1);
	result.GL = _CheckBlockSub(rx, y + IMG_CHIPSIZE);
	result.GR = _CheckBlockSub(rx + IMG_CHIPSIZE - 1, y + IMG_CHIPSIZE);

	//�����Ɏ΂߃u���b�N�̕␳

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
			}
			else if (x + sc < g_stagedata.mapwidth[1]){
				cell = g_mapdata[1][y][x + sc - g_stagedata.mapwidth[0]];
			}
			else if (x + sc < g_stagedata.mapwidth[2]){
				cell = g_mapdata[2][y][x + sc - g_stagedata.mapwidth[1]];
			}
			else if (x + sc < g_stagedata.mapwidth[3]){
				cell = g_mapdata[3][y][x + sc - g_stagedata.mapwidth[2]];
			}
			else if (x + sc < g_stagedata.mapwidth[4]){
				cell = g_mapdata[4][y][x + sc - g_stagedata.mapwidth[3]];
			}

			//�u���b�N�`��iA�`Z�j
			if (cell >= 'A' && cell <= 'Z'){
				switch (cell){
				case 'A':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.block, TRUE);
					break;
				case 'B':
					DrawGraph(x * IMG_CHIPSIZE - shiftx, y * IMG_CHIPSIZE,
						g_imghandles.diagblock, TRUE);
					break;
				}
			}
			//�����X�^�[�̌��o (1�`9)
			if (cell >= '1' && cell <= '9') {
				SetEnemy(x + sc, y);  // �G�̈ʒu������ SetEnemy �ɓn��
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