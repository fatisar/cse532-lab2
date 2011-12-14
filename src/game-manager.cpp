// CSE571 - game-manager.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// This class manages all the different connections that a client-side player
// may have.  

#include <iostream>
#include <ace/Tokenizer_T.h>

#include "game-manager.h"
#include "player-game.h"

const static int DEALER_DEFAULT_PORT = 8000;

GameManager* GameManager::instance_ = NULL;

GameManager* GameManager::instance() {
  if (instance_ == NULL) {
    instance_ = new GameManager();
  }
  return instance_;
}

int GameManager::parse_file(char* filename) {
  // Intialize the default host_name and port
  const int HOST_NAME_SIZE = 64;
  char host_name [HOST_NAME_SIZE];
  ::gethostname (host_name, HOST_NAME_SIZE - 1);
  short port = DEALER_DEFAULT_PORT;
  string game_name;
  
  // Attempt to open defintions file for reading
  ifstream def_file;
  def_file.open(filename);
    
  if (!def_file.is_open()) { return -1; }
  
  const int GAME_FLAG = 0, PORT_FLAG = 1, HOST_FLAG = 2;
  int flag;
  
  string line;
  
  // Iterate through the definitions file one line at a time
  while (def_file.good()) {
    getline(def_file, line);
    if (line.size() == 0) continue;
    
    char connection_line[line.size()];
    strcpy(connection_line, line.c_str());
    
    ACE_Tokenizer tokens(connection_line);
    tokens.delimiter_replace(' ',0);        // tokenize 'connection_line' delimited by spaces
        
    bool parse_error = false;
    string error_msg;
    
    // Iterate through the tokens two at a time. The first should be the flag
    // and the second should be the parameter. Possible errors include unknown
    // flags and missing parameters. If an error occurs, an error message is 
    // stored and the loop is broken out of, but the parsing will continue for
    // the next line.
    for(char* tok = tokens.next(); tok; tok = tokens.next()) {
        if (!strcmp(tok,"-g")) flag = GAME_FLAG;
        else if(!strcmp(tok,"-p")) flag = PORT_FLAG;
        else if(!strcmp(tok,"-i")) flag = HOST_FLAG;
        else {
            parse_error = true;
            error_msg = "Unknown flag.";
            break;
        }
        
        tok = tokens.next();    // this should be the parameter for the flag
        
        if ( (tok[0] == '-') || (strlen(tok) == 0) ) {
            parse_error = true;
            error_msg = "Missing parameter for flag.";
            break;
        }
        
        switch(flag) {
            case GAME_FLAG:
                game_name = tok;
                break;
            case PORT_FLAG:
                port = atoi(tok);
                break;
            case HOST_FLAG:
                strcpy(host_name, tok);
                break;
        }
    }
    
    if (game_name.size() == 0) {
        parse_error = true;
        error_msg = "A connection line must define at least a game name.";
    }
    
    if (parse_error) {
        std::cout << "PARSE ERROR IN [" << line << "]:\n - " << error_msg << std::endl;
        std::cout << "Attempting to parse remaining lines." << std::endl;
    }
    else {
        std::cout << "GAME:" << game_name << "/" << std::endl;
        
        PlayerGame* game = new PlayerGame();
        games.push_back(game);
        game->port = port;
        game->host = host_name;
        game->games.push_back(game_name);
        game->handler.game = game;
    }
  }
  return 0;
}

GameManager::~GameManager() {
  for (unsigned int i = 0; i < games.size(); ++i) {
    delete games[i];
  }
}
