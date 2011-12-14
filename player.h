// CSE571 - player.h
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Player class holds data for each player/connection on the server side.   

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "ace/SOCK_Stream.h"

#include "dealer.h"
#include "dealer-svc-handler.h"
#include "hand.h"

using namespace std;

class Card;
class Game;

class Player {

 public:
  ~Player();
  string name;
  vector<string> games;
  vector<pair<string,string> > servers;		// <port,host>
  DealerSvcHandler handler;
  void send_card(Card card);
  Hand hand;
  Game* active_game;
  int score;
 private:

};


#endif
