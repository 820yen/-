#include "main.h"
#include "score.h"
#include<time.h>

//�O���[�o���ϐ�
//���Ԍv���p�ϐ�
int g_lasttime = 0;		//���O�̌v������
float g_frametime = 0;	//1���[�v�ɂ�����������
int g_timerstart;		//�^�C�}�[�p�ϐ�
int g_limittimerstart;	//�^�C�}�[�p�ϐ����Ԑ���
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
int g_smallfont;			//���T�C�Y�t�H���g�n���h��
int g_normalfont;			//�����T�C�Y�t�H���g�n���h��
int g_middlefont;			//���T�C�Y�t�H���g�n���h��
int g_mediumfont;			//����T�C�Y�t�H���g�n���h��
int g_largefont;			//��T�C�Y�t�H���g�n���h��

//�X�R�A�Ǘ��p�ϐ�
ScoreData scores[MAXRANKING] = { 0 };
int playerScore = 0;				//�v���C���[�X�R�A
int playerTimeMin, playerTimeSec;	//�N���A���̎c�莞�Ԃ��L�^
BOOL g_scoreAdded = FALSE;			//�X�R�A���ǉ����ꂽ���ǂ���
BOOL g_scoreTotaled = FALSE;		//�X�R�A�����Z���ꂽ���ǂ���

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
	SetGraphMode(1300, 730, 32);
	//DX���C�u����������
	if (DxLib_Init() == -1) return -1;

	//�X�R�A�Ɣԍ���ǂ݂���
	LoadScore(scores);
	LoadPlayerNumber();

	srand((unsigned int)time(NULL));

	//�摜��ǂݍ���
	if (LoadGameImage() == FALSE) return -1;
	g_largefont = CreateFontToHandle("���C���I", 90, -1, DX_FONTTYPE_ANTIALIASING);
	g_mediumfont = CreateFontToHandle("���C���I", 65, -1, DX_FONTTYPE_ANTIALIASING);
	g_middlefont = CreateFontToHandle("���C���I", 42, -1, DX_FONTTYPE_ANTIALIASING);
	g_normalfont = CreateFontToHandle("���C���I", 30, -1, DX_FONTTYPE_ANTIALIASING);
	g_smallfont = CreateFontToHandle("���C���I", 18, -1, DX_FONTTYPE_ANTIALIASING);

	SetDrawScreen(DX_SCREEN_BACK);
	g_lasttime = GetNowCount() & INT_MAX;	//���ݎ����̋L�^
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		//1���[�v�ɂ����������Ԃ��v��
		int curtime = GetNowCount();
		g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
		g_lasttime = curtime;
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
	DrawGraph(0, 0, g_imghandles.title, FALSE);
	//�e�L�X�g�\��
	DrawStringToHandle(30, 440, "Z�L�[�ŃQ�[���X�^�[�g",
		GetColor(255, 0, 255), g_middlefont);
	DrawStringToHandle(30, 490, "�J�[�\���L�[�ō��E�Ɉړ�",
		GetColor(0, 0, 0), g_middlefont);
	DrawStringToHandle(30, 540, "Z�L�[�ŃW�����v�AX�L�[�Ńi�C�t�����A",
		GetColor(0, 0, 0), g_middlefont);
	//�L�[���`�F�b�N���ĉ�ʂ�؂�ւ�
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (IsAKeyTrigger(key) == TRUE) {
		g_gamestate = GAME_MAIN;
		g_limittimerstart = g_lasttime;
		InitStage();
		playerScore = 0; //�X�R�A�����Z�b�g
	}
}
//�Q�[���{�ҕ`��
void DrawGameMain(){
	GameMain();
}
//�Q�[���N���A��ʕ`��
void DrawGameClear(){
	DrawBox(0, 0, 1300, 730, blackColor, TRUE);

	//�X�R�A����x�������f������
	if (g_scoreTotaled == FALSE){
		//���Ԃ��L�^
		playerTimeMin = g_limittimemin;
		playerTimeSec = g_limittimesec;

		//�X�R�A���v�Z
		playerScore += g_scoretime + g_stagedata.hero.coinCount * 100;
		playerScore -= g_stagedata.hero.deathCount * 10000;

		if (playerScore < 0){
			playerScore = 0;
		}
		g_scoreTotaled = TRUE;			//�X�R�A�����Z���ꂽ���Ƃ��L�^
	}

	//�X�R�A�������L���O�Ɉ�x�����ǉ�
	if (g_scoreAdded == FALSE) {
		AddScore(scores, playerScore);
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
			orangeColor, g_largefont);
	}
	if (g_TextStep >= 4){
		SetFontSize(30);
		if (10 <= playerTimeSec){
			DrawFormatString(162, 200, whiteColor,
				"�c�莞�� : %d:%d", playerTimeMin, playerTimeSec);
		}
		else{
			DrawFormatString(162, 200, whiteColor,
				"�c�莞�� : %d:0%d", playerTimeMin, playerTimeSec);
		}
		SetFontSize(16);
	}
	if (g_TextStep >= 5){
		SetFontSize(30);
		DrawFormatString(162, 280, whiteColor,
			"���S�񐔁F%d", g_stagedata.hero.deathCount);
		SetFontSize(16);
	}
	if (g_TextStep >= 6){
		SetFontSize(30);
		DrawFormatString(162, 360, whiteColor,
			"�R�C�������F%d", g_stagedata.hero.coinCount);
		SetFontSize(16);
	}
	if (g_TextStep >= 10){
		SetFontSize(50);
		DrawFormatString(162, 480, yellowColor,
			"�X�R�A�F%d", playerScore);
		SetFontSize(16);
	}
	if (g_TextStep >= 10){
		DrawStringToHandle(162, 600, "Enter�L�[�Ń^�C�g���ɖ߂�",
			blueColor, g_smallfont);

		//Enter�L�[�Ń^�C�g����ʂ�
		int enterKey = CheckHitKey(KEY_INPUT_RETURN);
		if (IsEnterKeyTrigger(enterKey) == TRUE) {
			g_gamestate = GAME_TITLE;
			//�t���O�����Z�b�g
			g_scoreTotaled = FALSE;
			g_scoreAdded = FALSE;
		}
	}
}
//�Q�[���I�[�o�[��ʕ`��
void DrawGameOver(){

	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (IsAKeyTrigger(key) == TRUE) {
		InitStage();
		g_timerstart = g_lasttime;
	}
	DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);
	//�e�L�X�g�\��
	DrawStringToHandle(100, 200, "�Q�[���I�[�o�[",
		GetColor(255, 0, 0), g_largefont);
	//5�b�o������^�C�g����ʂ�
	if (g_lasttime - g_timerstart > 500) {
		g_gamestate = GAME_TITLE;
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