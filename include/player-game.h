// CSE571 - player-game.h
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// This class holds data for each game that a client-side player connects to.

#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include <vector>
#include <string>

#include "player-svc-handler.h"
#include "hand.h"

using namespace std;

class PlayerGame {

 public:
  string name;
  string dealer_name;
  vector<string> games;
  PlayerSvcHandler handler;
  int port;
  string host;
  Hand hand;
  int score;
  
};

#endif
