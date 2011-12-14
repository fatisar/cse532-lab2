#ifndef SCORE_CAL_H
#define SCORE_CAL_H

int * getHandScoreArray (const Hand &hand);
int getHandRank (const Hand &cards);
int arrayToScore (int * array);

#endif
