// CSE571 - dealer-svc-handler.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan  
// Dealer service handler which is used (with an ACCEPT mask) for opening a
// connection and then (with the R/W masks) for managing each socket connection
// reactively.

#include <iostream>

#include <ace/Reactor.h>
#include <ace/Svc_Handler.h>
#include <ace/Acceptor.h>

#include "dealer.h"
#include "game.h"
#include "player.h"
#include "dealer-svc-handler.h"

// Use a single, global buffer for reading data in.  This buffer will be the
// size of the operating system's MTU (as indicated by an IFCONFIG on Linux),
// ensuring that any message we read in from a socket will fit.
const int MTU = 1500;
static char buffer[MTU];


DealerSvcHandler::DealerSvcHandler() {}


int DealerSvcHandler::open(void*){
  // This pointer is later stored within the deal and the memory is cleaned
  // up by the dealer.
  Player* new_player = new Player();

  // Assign this handle to the player's handler and give that handler access
  // to the player that uses it.
  new_player->handler.set_handle(get_handle());
  new_player->handler.player = new_player;

  new_player->handler.peer().disable(ACE_NONBLOCK);
  ACE_Reactor::instance()->register_handler(&(new_player->handler),
					    ACE_Event_Handler::READ_MASK);
  return 0;
}


const int TIMEOUT_VALUE1 = 2;

int DealerSvcHandler::handle_input(ACE_HANDLE){
  Dealer* dealer = Dealer::instance();
  if (player->name.empty()) {  // Expect to receive player's name.
    int retval = peer().recv(buffer, sizeof buffer);
    if (retval  == -1) {
      printf("Error receiving data.");
      return -1;
    } else if (retval == 0) {
      return -1;
    }
    std::cout << "New player connected: " << buffer << std::endl;
    player->name = buffer;
    Dealer::instance()->players.insert(std::pair<string, Player*>
				       (player->name, player));

    int dealer_name_length = dealer->name.length();
    ACE_Message_Block *mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(dealer_name_length + 1), -1);
    mb->copy(dealer->name.c_str(), dealer_name_length + 1);
    ACE_Time_Value tv(TIMEOUT_VALUE1);
    putq(mb, &tv);
    ACE_Reactor::instance ()->mask_ops(this,
				       ACE_Event_Handler::WRITE_MASK |
				       ACE_Event_Handler::READ_MASK,
				       ACE_Reactor::SET_MASK);
  }
  else if (!games_read) {  // Expect to receive games.
    ACE_Time_Value games_timeout(TIMEOUT_VALUE1);
    int retval = peer().recv(buffer, sizeof buffer,
			     &games_timeout);
			     
    if (retval  == -1) {
      std::cout << "Error receiving data.\n" << std::endl;
      return -1;
    } else if (retval == 0) {  // Close down gracefully.
      return -1;
    }
    
    int current_length = strlen(buffer);
    // The end of our list of games will be signaled by a message with a string
    // of length zero. That is, the first byte will be null.  So continue to add
    // games until this is seen.
    if (current_length) {
      player->games.push_back(buffer);
      return 0;
    }

    // If we are here, then the last game has been sent.
    games_read = true;
    std::map<string, Game*>::iterator iter;
    for (unsigned int i = 0; i < player->games.size(); ++i) {
      iter = dealer->games.find(player->games[i]);
      if (iter == dealer->games.end()) {
        Game* game = new Game();
        game->name = player->games[i];
        game->players.push_back(player);
        dealer->games.insert(std::pair<string, Game*>(game->name, game));
      } else {
        iter->second->players.push_back(player);
      }
    }

    // Print out the list of games and the players in each.         
    for(iter = dealer->games.begin(); iter != dealer->games.end(); ++iter) {
      std::cout << iter->first << ":" << std::endl;
      for(unsigned int i = 0; i < iter->second->players.size(); ++i) {
	    std::cout << "\t" << iter->second->players[i]->name << std::endl;
      }
      iter->second->new_player_added();
    }

  }
  else {  // Receive a score from the player.
    int score;
    int retval = peer().recv(&score, (int) sizeof score);
    if (retval  == -1) {
      printf("Error receiving data.");
      return -1;
    } else if (retval == 0) {
      return -1;
    }

    player->score = score;
    player->active_game->score_added();
    
  }

  return 0;
}

const int TIMEOUT_VALUE2 = 90;

int DealerSvcHandler::handle_output(ACE_HANDLE){
  ACE_Message_Block *mb;
  ACE_Time_Value tv(TIMEOUT_VALUE1);
  int queue_length = this->getq(mb, &tv);
  if (queue_length > -1) {
    ACE_Time_Value tv2(TIMEOUT_VALUE2);

    int retval = peer().send(mb->rd_ptr(), mb->length(), &tv2);
    if (retval == -1) {
      std::cout << "Error sending data." << std::endl;
      return 1;
    }
    mb->release();
  }
  if (queue_length == 0) {
    ACE_Reactor::instance ()->mask_ops(this,
  				     ACE_Event_Handler::READ_MASK,
  				     ACE_Reactor::SET_MASK);
  }
  return 0;
}

int DealerSvcHandler::handle_close(ACE_HANDLE) {
  shutdown();
  return 0;
}

const int MSG_BLOCK_SIZE = 2;

int DealerSvcHandler::enqueue_card_for_output(char s, char r) {
  buffer[0] = s;
  buffer[1] = r;
  ACE_Message_Block *mb;
  mb = new ACE_Message_Block(MSG_BLOCK_SIZE);
  mb->copy(buffer, MSG_BLOCK_SIZE);
  ACE_Time_Value tv(TIMEOUT_VALUE1);
  putq(mb, &tv);
  ACE_Reactor::instance ()->mask_ops(this,
				     ACE_Event_Handler::WRITE_MASK |
				     ACE_Event_Handler::READ_MASK,
				     ACE_Reactor::SET_MASK);
  return 0;
}

