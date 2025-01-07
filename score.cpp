#include "score.h"
#include <stdio.h>
#include <DxLib.h>

//�X�R�A���t�@�C������ǂ݂���
void LoadScore(ScoreData scores[]){
	FILE* file;
	errno_t err = fopen_s(&file, SCORE_FILE, "r");
	//�t�@�C�����Ђ炯�Ȃ��ꍇ
	if (err != 0 || file == NULL) { 
		for (int i = 0; i < MAXRANKING; i++) {
			scores[i].score = 0;
		}
		return;
	}

	//�X�R�A���t�@�C������ǂݍ���
	for (int i = 0; i < MAXRANKING; i++) {
		//�ǂ݂��߂����ǂ���
		if (fscanf_s(file, "%d", &scores[i].score) != 1) {
			scores[i].score = 0;
		}
	}
	fclose(file);
}

//�X�R�A���t�@�C���ɏ����o��
void SaveScore(ScoreData scores[]){
	FILE* file;
	errno_t err = fopen_s(&file, SCORE_FILE, "w");
	//�t�@�C�����Ђ炯�Ȃ��ꍇ
	if (err != 0 || file == NULL) {
		return;
	}

	// �X�R�A�������o��
	for (int i = 0; i < MAXRANKING; i++) {
		fprintf_s(file, "%d\n", scores[i].score);
	}
	fclose(file);
}

//�V�����X�R�A�������L���O�ɒǉ�
void AddScore(ScoreData scores[], int score) {
	//�X�R�A��}��
	for (int i = 0; i < MAXRANKING; i++) {
		if (score > scores[i].score) {
			//�X�R�A�����炷
			for (int j = MAXRANKING - 1; j > i; j--) {
				scores[j].score = scores[j - 1].score;
			}
			//�V�����X�R�A��}��
			scores[i].score = score;
			break;
		}
	}
}

//�����L���O��`�悷��
void DrawRanking(const ScoreData scores[]) {
	//�E��̈ʒu
	int x = 1000;
	int y = 20;

	//�����L���O�̏o��
	for (int i = 0; i < MAXRANKING; i++) {
		char text[64];
		sprintf_s(text, "%2d. %d", i + 1, scores[i].score);
		DrawString(x, y + i * 20, text, GetColor(255, 255, 255));
	}
}