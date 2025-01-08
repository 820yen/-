#ifndef SCORE_H
#define SCORE_H

#define MAXRANKING 5	//�����L���O�̍ő吔
#define SCORE_FILE "media\\score.txt"

struct ScoreData{
	int number;		//����
	int score;		//�X�R�A
};

extern int score;
extern int scoreRanking[MAXRANKING];

void LoadScore(ScoreData scores[]);
void SaveScore(ScoreData scores[]);
void AddScore(ScoreData scores[], int score);
void DrawRanking(const ScoreData scores[]);

#endif