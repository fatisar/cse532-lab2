// CSE571 - dealer.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Dealer class which holds data about games and players for server.

#include <iostream>
#include "stdio.h"
#include <map>
#include <iterator>
#include <utility>

#include "dealer.h"
#include "player.h"
#include "signal-handler.h"

Dealer* Dealer::instance_ = NULL;

Dealer* Dealer::instance() {
  if (instance_ == NULL) {
    instance_ = new Dealer();
  }
  return instance_;
}

// Dealer destructor.  We need to clean up the memory we have dynamically
// allocated, the pointers to which are stored within the dealer.
Dealer::~Dealer() {
  std::map<string, Game*>::iterator game_iter;
  for(game_iter = games.begin(); game_iter != games.end(); ++game_iter) {
    delete game_iter->second;
  }
  std::map<string, Player*>::iterator player_iter;
  for(player_iter = players.begin(); player_iter != players.end();
      ++player_iter) {
    delete player_iter->second;
  }
}
