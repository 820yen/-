#include "score.h"
#include <stdio.h>
#include <DxLib.h>

//スコアをファイルから読みこむ
void LoadScore(ScoreData scores[]){
	FILE* file;
	errno_t err = fopen_s(&file, SCORE_FILE, "r");
	//ファイルがひらけない場合
	if (err != 0 || file == NULL) { 
		for (int i = 0; i < MAXRANKING; i++) {
			scores[i].score = 0;
		}
		return;
	}

	//スコアをファイルから読み込む
	for (int i = 0; i < MAXRANKING; i++) {
		//読みこめたかどうか
		if (fscanf_s(file, "%d", &scores[i].score) != 1) {
			scores[i].score = 0;
		}
	}
	fclose(file);
}

//スコアをファイルに書き出す
void SaveScore(ScoreData scores[]){
	FILE* file;
	errno_t err = fopen_s(&file, SCORE_FILE, "w");
	//ファイルがひらけない場合
	if (err != 0 || file == NULL) {
		return;
	}

	// スコアを書き出す
	for (int i = 0; i < MAXRANKING; i++) {
		fprintf_s(file, "%d\n", scores[i].score);
	}
	fclose(file);
}

//新しいスコアをランキングに追加
void AddScore(ScoreData scores[], int score) {
	//スコアを挿入
	for (int i = 0; i < MAXRANKING; i++) {
		if (score > scores[i].score) {
			//スコアをずらす
			for (int j = MAXRANKING - 1; j > i; j--) {
				scores[j].score = scores[j - 1].score;
			}
			//新しいスコアを挿入
			scores[i].score = score;
			break;
		}
	}
}

//ランキングを描画する
void DrawRanking(const ScoreData scores[]) {
	//右上の位置
	int x = 1000;
	int y = 20;

	//ランキングの出力
	for (int i = 0; i < MAXRANKING; i++) {
		char text[64];
		sprintf_s(text, "%2d. %d", i + 1, scores[i].score);
		DrawString(x, y + i * 20, text, GetColor(255, 255, 255));
	}
}