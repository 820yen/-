#include "score.h"
#include "main.h"
#include <stdio.h>
#include <DxLib.h>

static int playerNumber = 1; //�v���C���[�ԍ���ێ�

//�ԍ����t�@�C������ǂݍ���
void LoadPlayerNumber() {
	FILE* file;
	errno_t err = fopen_s(&file, NUMBER_FILE, "r");
	if (err == 0 && file != NULL) {
		//�ԍ���ǂݍ���
		fscanf_s(file, "%d", &playerNumber);
		fclose(file);
	}
	else {
		playerNumber = 1; //�t�@�C�����Ȃ��ꍇ��1����n�߂�
	}
}

//�ԍ����t�@�C���ɏ�������
void SavePlayerNumber() {
	FILE* file;
	errno_t err = fopen_s(&file, NUMBER_FILE, "w");
	if (err == 0 && file != NULL) {
		//�ԍ���ۑ�
		fprintf_s(file, "%d\n", playerNumber);
		fclose(file);
	}
	
}

//�X�R�A���t�@�C������ǂ݂���
void LoadScore(ScoreData scores[]) {
	FILE* file;
	errno_t err = fopen_s(&file, SCORE_FILE, "r");
	if (err != 0 || file == NULL) {
		for (int i = 0; i < MAXRANKING; i++) {
			scores[i].score = 0;
			scores[i].number = 0;
		}
		return;
	}

	//�X�R�A�Ɣԍ���ǂݍ���
	for (int i = 0; i < MAXRANKING; i++) {
		if (fscanf_s(file, "%d %d", &scores[i].score, &scores[i].number) != 2) {
			scores[i].score = 0;
			scores[i].number = 0;
		}
	}
	fclose(file);
}

//�X�R�A���t�@�C���ɏ����o��
void SaveScore(ScoreData scores[]) {
	FILE* file;
	errno_t err = fopen_s(&file, SCORE_FILE, "w");
	if (err != 0 || file == NULL) {
		return;
	}

	//�X�R�A�Ɣԍ��������o��
	for (int i = 0; i < MAXRANKING; i++) {
		fprintf_s(file, "%d %d\n", scores[i].score, scores[i].number);
	}
	fclose(file);
}

//�V�����X�R�A�������L���O�ɒǉ�
void AddScore(ScoreData scores[], int score) {
	BOOL scoreAdded = FALSE;	//�X�R�A�������L���O�ɒǉ����ꂽ���ǂ���

	for (int i = 0; i < MAXRANKING; i++) {
		if (score > scores[i].score) {
			//�X�R�A�Ɣԍ������炷
			for (int j = MAXRANKING - 1; j > i; j--) {
				scores[j] = scores[j - 1];
			}
			//�V�����X�R�A�Ɣԍ���}��
			scores[i].score = score;
			scores[i].number = playerNumber;
			scoreAdded = TRUE;
			break;
		}
	}
	
}

//�����L���O��`�悷��
void DrawRanking(const ScoreData scores[]) {
	//�E��̈ʒu
	int x = 1125;
	int y = 80;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawGraph(1085, 0, g_imghandles.ranking, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�����L���O�̏o��
	SetFontSize(16);
	for (int i = 0; i < MAXRANKING; i++) {
		char text[64], text_No[10];
		sprintf_s(text, "%2d. %6d %3d", i + 1, scores[i].score, scores[i].number);
		sprintf_s(text_No, "No.%d", playerNumber);
		DrawString(1200, 250, text_No, GetColor(255, 255, 255), GetColor(0, 0, 0));

		if (scores[i].number == playerNumber){
			DrawStringToHandle(x, y + i * 30, text,
				GetColor(255, 0, 0), g_minimumfont);
		}
		else{
			DrawStringToHandle(x, y + i * 30, text,
				GetColor(0, 0, 0), g_minimumfont);
		}
		
	}
	SetFontSize(30);
	if (g_addedFlag == TRUE){
		playerNumber++;
		g_addedFlag = FALSE;
	}
}