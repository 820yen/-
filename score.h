#ifndef SCORE_H
#define SCORE_H

#define MAXRANKING 5	//�����L���O�̍ő吔
#define SCORE_FILE "media\\score.txt"
#define NUMBER_FILE "media\\number.txt"

struct ScoreData {
	int number;	//���ԁi�v���C���[�ԍ��j
	int score;	//�X�R�A
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