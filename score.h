#ifndef SCORE_H
#define SCORE_H

#define MAXRANKING 5	//ランキングの最大数
#define SCORE_FILE "media\\score.txt"
#define NUMBER_FILE "media\\number.txt"

struct ScoreData {
	int number;	//順番（プレイヤー番号）
	int score;	//スコア
};

extern int score;
extern int scoreRanking[MAXRANKING];

void LoadPlayerNumber();
void SavePlayerNumber();
void LoadScore(ScoreData scores[]);
void SaveScore(ScoreData scores[]);
void AddScore(ScoreData scores[], int score);
void DrawRanking(const ScoreData scores[]);

#endif