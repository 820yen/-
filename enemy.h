#ifndef __ENEMY_H__ 
#define __ENEMY_H__

#include "gamemain.h"

struct CharaData;

void SetEnemy(int mx, int my);
void DrawEnemy(int ac);
CharaData CoinSetting(CharaData cd);

#endif