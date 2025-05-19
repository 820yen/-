#include "main.h"
#include "score.h"
#include<time.h>

//�O���[�o���ϐ�
//���Ԍv���p�ϐ�
int g_lasttime = 0;		//���O�̌v������
float g_frametime = 0;	//1���[�v�ɂ�����������
int g_timerstart;		//�^�C�}�[�p�ϐ�
int g_limittimerstart;	//�^�C�}�[�p�ϐ����Ԑ���
int g_jumptimerstart;	//�W�����v�̓ǂݍ��ݎ���
int g_limittimemin;	//���Ԑ�����
int g_limittimesec;	//���Ԑ����b
int g_scoretime;	//�X�R�A�^�C��

//�e�L�X�g�\��
int displayInterval = 30;	//���̃e�L�X�g�\���܂ł̎���
int g_TextTimer;			//�e�L�X�g�\���ϐ�
int g_TextStep;				//���݂̃e�L�X�g�\���̃X�e�b�v

int g_stagenumber;	//�X�e�[�W�ԍ�

GameState g_gamestate = GAME_TITLE;

int g_gametitleimg;			//�^�C�g���C���[�W
int g_heroimg;				//�摜
float g_hx = 0, g_hy = 0;	//���W
//�{�^��
BOOL g_akey_prev;			//���O��A�{�^���̏��
//�t�H���g
int	g_minimumfont;			//�ɏ��T�C�Y�t�H���g�n���h��
int g_smallfont;			//���T�C�Y�t�H���g�n���h��
int g_normalfont;			//�����T�C�Y�t�H���g�n���h��
int g_middlefont;			//���T�C�Y�t�H���g�n���h��
int g_mediumfont;			//����T�C�Y�t�H���g�n���h��
int g_largefont;			//��T�C�Y�t�H���g�n���h��

//�X�R�A�Ǘ��p�ϐ�
ScoreData scores[MAXRANKING] = { 0 };
int playerScore = 0;				//�v���C���[�X�R�A
int playerNumber;
int playerTimeMin, playerTimeSec;	//�N���A���̎c�莞�Ԃ��L�^
BOOL g_scoreAdded = FALSE;			//�X�R�A���ǉ����ꂽ���ǂ���
BOOL g_scoreTotaled = FALSE;		//�X�R�A�����Z���ꂽ���ǂ���
BOOL g_addedFlag = FALSE;

BOOL g_countDownFlag = FALSE;	//�J�E���g�_�E���J�n�t���O
int g_countDownEndTime;			//�J�E���g�_�E���I������

//�F
int blownColor = GetColor(134, 74, 43);
int orangeColor = GetColor(255, 160, 16);
int yellowColor = GetColor(227, 199, 0);
int whiteColor = GetColor(255, 255, 255);
int pinkColor = GetColor(255, 0, 255);
int blackColor = GetColor(0, 0, 0);
int blueColor = GetColor(80, 128, 255);

int WINAPI WinMain(HINSTANCE h1, HINSTANCE hP, LPSTR lpC, int nC){
	//�E�B���h�E���[�h�ɂ���
	ChangeWindowMode(FALSE);
	//�E�B���h�E�T�C�Y��ύX����
	SetGraphMode(1300, 730, 32);	//PC�̉𑜓x��1366, 768
	//DX���C�u����������
	if (DxLib_Init() == -1) return -1;

	//�X�R�A�Ɣԍ���ǂ݂���
	LoadScore(scores);
	LoadPlayerNumber();

	srand((unsigned int)time(NULL));

	//�摜��ǂݍ���
	if (LoadGameImage() == FALSE) return -1;
	//BGM,���ʉ��ǂݍ���
	if (LoadGameSound() == FALSE) return -1;

	//�t�H���g�ǂ݂���
	LPCSTR font_path = "media\\Harumaki-R.otf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		return FALSE;
	}
	ChangeFont("�͂�܂� R", DX_CHARSET_DEFAULT);
	g_largefont = CreateFontToHandle("�͂�܂� R", 90, -1, DX_CHARSET_DEFAULT, GetColor(255,255,255));
	g_mediumfont = CreateFontToHandle("�͂�܂� R", 65, -1, DX_CHARSET_DEFAULT, GetColor(255, 255, 255));
	g_middlefont = CreateFontToHandle("�͂�܂� R", 42, -1, DX_CHARSET_DEFAULT, GetColor(255, 255, 255));
	g_normalfont = CreateFontToHandle("�͂�܂� R", 30, -1, DX_CHARSET_DEFAULT, GetColor(255, 255, 255));
	g_smallfont = CreateFontToHandle("�͂�܂� R", 18, -1, DX_CHARSET_DEFAULT, GetColor(255, 255, 255));
	g_minimumfont = CreateFontToHandle("�͂�܂� R", 16, -1, DX_CHARSET_DEFAULT, GetColor(255, 255, 255));
	ChangeFontType(DX_FONTTYPE_EDGE);
	SetFontSize(30);

	SetDrawScreen(DX_SCREEN_BACK);
	g_lasttime = GetNowCount() & INT_MAX;	//���ݎ����̋L�^
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		//1���[�v�ɂ����������Ԃ��v��
		int curtime = GetNowCount();
		g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
		g_lasttime = curtime;
		if (g_lasttime - g_countDownEndTime <= 3000){
			g_limittimerstart = g_lasttime;
		}

		g_limittimemin = (TIMELIMIT - (g_lasttime - g_limittimerstart) / 1000) / 60;
		g_limittimesec = (TIMELIMIT - (g_lasttime - g_limittimerstart) / 1000) % 60;
		g_scoretime = TIMELIMIT * 1000 - ((g_lasttime - g_limittimerstart));

		ClearDrawScreen();
		//��ʕ`��֐��ɐ؂�ւ�
		switch (g_gamestate)
		{
		case GAME_TITLE:
			DrawGameTitle();
			break;
		case GAME_MAIN:
			DrawGameMain();
			break;
		case GAME_CLEAR:
			DrawGameClear();
			break;
		case GAME_OVER:
			DrawGameOver();
			break;
		default:
			break;
		}
		//�����L���O���E��ɕ\��
		DrawRanking(scores);

		ScreenFlip();
	}
	//�Q�[���I�����ɃX�R�A��ۑ�
	SaveScore(scores);
	SavePlayerNumber();

	//�ҋ@
	WaitKey();
	//DX���C�u�����̏I������
	DxLib_End();
	return 0;
}

//�^�C�g����ʕ`��
void DrawGameTitle(){
	StopSoundMem(g_sndhandles.clear);
	if (CheckSoundMem(g_sndhandles.title) == 0) {
		PlaySoundMem(g_sndhandles.title, DX_PLAYTYPE_LOOP);
	}
	DrawGraph(0, 0, g_imghandles.title, FALSE);
	//�L�[���`�F�b�N���ĉ�ʂ�؂�ւ�
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (IsAKeyTrigger(key) == TRUE) {
		PlaySoundMem(g_sndhandles.countdown, DX_PLAYTYPE_BACK);
		g_gamestate = GAME_MAIN;
		g_limittimerstart = g_lasttime;
		g_countDownEndTime = g_lasttime;
		InitStage();
		playerScore = 0; //�X�R�A�����Z�b�g
		g_TextStep = 0;	 //�e�L�X�g�X�e�b�v�����Z�b�g
	}
}
//�Q�[���{�ҕ`��
void DrawGameMain(){
	GameMain();
}
//�Q�[���N���A��ʕ`��
void DrawGameClear(){

	int qKey = CheckHitKey(KEY_INPUT_Q);

	//�X�R�A����x�������f������
	if (g_scoreTotaled == FALSE){
		//���Ԃ��L�^
		playerTimeMin = g_limittimemin;
		playerTimeSec = g_limittimesec;

		//�X�R�A���v�Z
		playerScore += g_scoretime + g_stagedata.hero.coinCount * 500;
		playerScore -= g_stagedata.hero.deathCount * 10000;

		if (playerScore < g_stagedata.hero.coinCount * 500){
			playerScore = g_stagedata.hero.coinCount * 500;
		}
		g_scoreTotaled = TRUE;			//�X�R�A�����Z���ꂽ���Ƃ��L�^
	}

	//�X�R�A�������L���O�Ɉ�x�����ǉ�
	if (g_scoreAdded == FALSE && g_TextStep >= 20) {
		AddScore(scores, playerScore);
		//������Đ�
		PlaySoundMem(g_sndhandles.handclap, DX_PLAYTYPE_BACK);

		g_scoreAdded = TRUE;			//�X�R�A���ǉ����ꂽ���Ƃ��L�^
	}

	//�ʏ�`�揈��
	g_TextTimer++;	//�^�C�}�[��i�߂�
	if (g_TextTimer >= displayInterval){
		g_TextTimer = 0;	//�^�C�}�[�����Z�b�g
		g_TextStep++;		//�X�e�b�v��i�߂�
	}

	//�e�L�X�g�\��
	if (g_TextStep >= 2){
		DrawStringToHandle(200, 50, "�Q�[���N���A",
			whiteColor, g_largefont, blackColor);
	}
	if (g_TextStep >= 6){
		if (10 <= playerTimeSec){
			char text[64];
			sprintf_s(text, "�c�莞�� : %d:%d", playerTimeMin, playerTimeSec);
			DrawString(162, 200, text, whiteColor, blackColor);
		}
		else{
			char text[64];
			sprintf_s(text, "�c�莞�� : %d:0%d", playerTimeMin, playerTimeSec);
			DrawString(162, 200, text, whiteColor, blackColor);
		}
	}
	if (g_TextStep >= 10){
		char text[64];
		sprintf_s(text, "�����񐔁F%d", g_stagedata.hero.deathCount);
		DrawString(162, 280, text, whiteColor, blackColor);
	}
	if (g_TextStep >= 14){
		DrawGraph(162, 353, g_imghandles.kyabecoin, TRUE);
		char text[64];
		sprintf_s(text, "�~%d", g_stagedata.hero.coinCount);
		DrawString(202, 360, text, whiteColor, blackColor);
	}
	if (g_TextStep >= 20){
		SetFontSize(100);
		char text[64];
		sprintf_s(text, "�X�R�A�F%d", playerScore);
		DrawString(162, 450, text, whiteColor, blackColor);
		SetFontSize(30);
	}
	if (g_TextStep >= 20){
		//Z�L�[�Ń^�C�g����ʂ�
		int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if (IsQKeyTrigger(qKey)) {
			g_gamestate = GAME_TITLE;
			g_addedFlag = TRUE;
			//�t���O�����Z�b�g
			g_scoreTotaled = FALSE;
			g_scoreAdded = FALSE;
		}
	}
}
//�Q�[���I�[�o�[��ʕ`��
void DrawGameOver(){
	int qKey = CheckHitKey(KEY_INPUT_Q);

	StopSoundMem(g_sndhandles.main);
	if (CheckSoundMem(g_sndhandles.title) == 0) {
		PlaySoundMem(g_sndhandles.title, DX_PLAYTYPE_LOOP);
	}

	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (g_gameoveropacity < 255){
		g_gameoveropacity += 5;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, g_gameoveropacity);
	DrawBox(0, 0, 1300, 730, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	g_limittimesec = 0;
	g_limittimemin = 0;
	DrawStringToHandle(300, 100, "�^�C���I�[�o�[",
		GetColor(255, 0, 0), g_largefont);

	//������
	DrawFormatString(282, 280, GetColor(255, 255, 255),
		"�����񐔁F%d", g_stagedata.hero.deathCount);
	//�R�C������
	DrawGraph(282, 353, g_imghandles.kyabecoin, TRUE);
	DrawFormatString(322, 360, GetColor(255, 255, 255),
		"�~%d", g_stagedata.hero.coinCount);
	//�c�苗��
	DrawFormatString(282, 440, GetColor(255, 255, 255),
		"�c�苗���F%.1fkm", (g_stagedata.mapwidth[4] - g_stagedata.hero.x / 50)
		/ (g_stagedata.mapwidth[4] - 2)
		* 17.5);
	
	//Q�L�[�������ꂽ��^�C�g����ʂ�
	if (IsQKeyTrigger(qKey)){
		g_gamestate = GAME_TITLE;
		g_addedFlag = TRUE;
		//�t���O�����Z�b�g
		g_scoreTotaled = FALSE;
		g_scoreAdded = FALSE;
	}
}

//�L�[�g���K�[����
BOOL IsAKeyTrigger(int key){
	if (key & PAD_INPUT_A){
		if (g_akey_prev == FALSE){
			g_akey_prev = TRUE;
			return TRUE;
		}
	}
	else {
		g_akey_prev = FALSE;
	}
	return FALSE;
}