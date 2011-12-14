// CSE571 - player-main.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// This file holds the main function that is run by the client program.

#include <iostream>

#include <ace/INET_Addr.h>
#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>

#include "game-manager.h"
#include "player-game.h"
#include "signal-handler.h"

class PlayerSvcHandler;

typedef ACE_Connector<PlayerSvcHandler,ACE_SOCK_CONNECTOR>
    PlayerConnector;

const int MTU = 1500;
extern char buffer[];

int parse_file(char* filename);

int main(int argc, char* argv[]) {

  GameManager* manager = GameManager::instance();
  // Process the command line switches.  Look at every second arg for
  // a recognizable flag.
  const int NAME_FLAG = 0, FILE_FLAG = 1;
  int flag;
  
  bool parse_error = false;
  string error_msg;

  for (int i = 2; i < argc; i += 2) {
    if (!strcmp("-n", argv[i-1])) flag = NAME_FLAG;
    else if (!strcmp("-f", argv[i-1])) flag = FILE_FLAG;
    else {
        parse_error = true;
        error_msg = "Unknown flag.";
        break;
    }
    
    if ((argv[i][0] == '-') || (strlen(argv[i]) == 0)) {
        parse_error = true;
        error_msg = "Missing parameter for flag.";
        break;
    }
    
    switch (flag) {
        case NAME_FLAG:
            manager->name = argv[i];
            break;
        case FILE_FLAG:
            if (manager->parse_file(argv[i]) != 0) { 
                parse_error = true; 
                error_msg = "Could not parse defintion file.";
            }
            break;
    }
  }
  
  if (parse_error) {
    std::cout << "ERROR: " << error_msg << std::endl;
    std::cout << "Usage: client -n <player name> -f <definition file> " << std::endl;
    return -1;
  }
  
  PlayerConnector player_connector(ACE_Reactor::instance());
  
  // keep track of the number of open connections so we can
  // kill the client if no connections were able to be opened
  int num_open_connections = manager->games.size();
  
  for (unsigned int i = 0; i < manager->games.size(); ++i) {
    PlayerGame* game = manager->games[i];
     ACE_INET_Addr peer_addr;
     if (peer_addr.set(game->port, game->host.c_str()) == -1) {
       std::cout << "Unable to set server address for " << game->host << ":" << game->port << std::endl;
       num_open_connections--;
       continue;
     }
     peer_addr.addr_to_string (buffer, MTU);
     std::cout << "Address and port: " << buffer << std::endl;

     PlayerSvcHandler* connector_handler = &(game->handler);
     if (player_connector.connect (connector_handler, peer_addr) == -1) {
       std::cout << "Couldn't connect to server.\n" << std::endl;
       num_open_connections--;
     }
  }
  
  if (num_open_connections == 0) return -1;     // kill the client if no connections were able to be opened

  // Implement signal handler.                                                 
  ACE_Sig_Action sa;
  sa.register_action (SIGINT);
  SigHandler sh (ACE_Reactor::instance());

  while (!SigHandler::sigint_fired()) {
    ACE_Reactor::instance()->handle_events();
  }

  if (GameManager::instance() != NULL) {
    delete GameManager::instance();
  }
  return 0;
}

