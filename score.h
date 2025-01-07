#ifndef SCORE_H
#define SCORE_H

#define MAXRANKING 5	//ランキングの最大数
#define SCORE_FILE "score.txt"

struct ScoreData{
	int score;		//スコア
};

extern int score;
extern int scoreRanking[MAXRANKING];

void LoadScore(ScoreData scores[]);
void SaveScore(ScoreData scores[]);
void AddScore(ScoreData scores[], int score);
void DrawRanking(const ScoreData scores[]);

#endif