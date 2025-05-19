#include "score.h"
#include "main.h"
#include <stdio.h>
#include <DxLib.h>

static int playerNumber = 1; //プレイヤー番号を保持

//番号をファイルから読み込む
void LoadPlayerNumber() {
	FILE* file;
	errno_t err = fopen_s(&file, NUMBER_FILE, "r");
	if (err == 0 && file != NULL) {
		//番号を読み込む
		fscanf_s(file, "%d", &playerNumber);
		fclose(file);
	}
	else {
		playerNumber = 1; //ファイルがない場合は1から始める
	}
}

//番号をファイルに書きだす
void SavePlayerNumber() {
	FILE* file;
	errno_t err = fopen_s(&file, NUMBER_FILE, "w");
	if (err == 0 && file != NULL) {
		//番号を保存
		fprintf_s(file, "%d\n", playerNumber);
		fclose(file);
	}
	
}

//スコアをファイルから読みこむ
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

	//スコアと番号を読み込む
	for (int i = 0; i < MAXRANKING; i++) {
		if (fscanf_s(file, "%d %d", &scores[i].score, &scores[i].number) != 2) {
			scores[i].score = 0;
			scores[i].number = 0;
		}
	}
	fclose(file);
}

//スコアをファイルに書き出す
void SaveScore(ScoreData scores[]) {
	FILE* file;
	errno_t err = fopen_s(&file, SCORE_FILE, "w");
	if (err != 0 || file == NULL) {
		return;
	}

	//スコアと番号を書き出す
	for (int i = 0; i < MAXRANKING; i++) {
		fprintf_s(file, "%d %d\n", scores[i].score, scores[i].number);
	}
	fclose(file);
}

//新しいスコアをランキングに追加
void AddScore(ScoreData scores[], int score) {
	BOOL scoreAdded = FALSE;	//スコアがランキングに追加されたかどうか

	for (int i = 0; i < MAXRANKING; i++) {
		if (score > scores[i].score) {
			//スコアと番号をずらす
			for (int j = MAXRANKING - 1; j > i; j--) {
				scores[j] = scores[j - 1];
			}
			//新しいスコアと番号を挿入
			scores[i].score = score;
			scores[i].number = playerNumber;
			scoreAdded = TRUE;
			break;
		}
	}
	
}

//ランキングを描画する
void DrawRanking(const ScoreData scores[]) {
	//右上の位置
	int x = 1125;
	int y = 80;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawGraph(1085, 0, g_imghandles.ranking, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ランキングの出力
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