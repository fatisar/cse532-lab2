// CSE571 - game.h
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Gane class which holds data about a single game on the server side.

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

#include "player.h"

class Deck;

using namespace std;


class Game {

 public:
  Game();
  string name, port, host;
  vector<Player*> players;
  void new_player_added();
  void score_added();
 private:
  void send_hand(Player* player);
  Deck deck;
  bool running;
  int scores_received;  
};

struct player_ptr_less
  : std::binary_function<const Player*, const Player*, bool> {
  bool operator() (const Player* p1, const Player* p2 ) const
  {
    return p1->score > p2->score;
  }
};

#endif
