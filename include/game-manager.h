// CSE571 - game-manager.h
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// This class manages all the different connections that a client-side player
// may have.           

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>


class PlayerGame;

using namespace std;

class GameManager {

 public:
  ~GameManager();
  string name;
  static GameManager* instance(); 
  vector<PlayerGame*> games;
  int parse_file(char * filename);

 private:
  static GameManager* instance_;

};


#endif
