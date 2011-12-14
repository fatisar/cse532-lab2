#include "hand.h"
#include "card.h"
#include <cmath>
#include "poker.h"
#include "score-cal.h"

int * getHandScoreArray (const Hand &hand) {
  int * scoreArray = new int[6];
  switch (getHandRank (hand)) {
  case NO_RANK:
    scoreArray[0] = 0;
    break;
  case PAIR:
    scoreArray[0] = 1;
    break;
  case TWO_PAIR:
    scoreArray[0] = 2;
    break;
  case THREE_OF_A_KIND:
    scoreArray[0] = 3;
    break;
  case STRAIGHT:
    scoreArray[0] = 4;
    break;
  case FLUSH:
    scoreArray[0] = 5;
    break;
  case FULL_HOUSE:
    scoreArray[0] = 6;
    break;
  case FOUR_OF_A_KIND:
    scoreArray[0] = 7;
    break;
  case STRAIGHT_FLUSH:
    scoreArray[0] = 8;
    break;
  }

  vector<Card> vec = hand.getHand();
  for (int i = 0; i < 5; ++i) {
    scoreArray[i + 1] = (int) vec[i].c_rank;
  }
  
  return scoreArray;
}

int arrayToScore (int * array) {
  int score = 0;
  for (int i = 0; i < 6; ++i) {
    score += array[i] * std::pow (13, 5-i);
  }
  return score;
}
