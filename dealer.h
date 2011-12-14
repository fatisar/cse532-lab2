// CSE571 - dealer.h                                                         
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan                         
// Dealer class which holds data about games and players for server.  

#ifndef DEALER_H
#define DEALER_H

#include <vector>
#include <string>
#include <map>

class Player;
class Game;

using namespace std;

class Dealer {

 public:
  static Dealer* instance();
  ~Dealer();
  string name;
  map<string, Player*> players;
  map<string, Game*> games;

 private:
  static Dealer* instance_;

};

#endif
