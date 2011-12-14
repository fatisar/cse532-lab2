// CSE571 - player-svc-handler.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Player service handler which is used for opening a connection and then (with
// R/W masks) for managing each socket connection reactively.  

#include <iostream>

#include "card.h"
#include "game.h"
#include "game-manager.h"
#include "player-game.h"
#include "player-svc-handler.h"
#include "score-cal.h"

// Use a single, global buffer for reading data in.  This buffer will be the
// size of the operating system's MTU (as indicated by an IFCONFIG on Linux),
// ensuring that any message we read in from a socket will fit.
const int MTU = 1500;
char buffer[MTU];

PlayerSvcHandler::PlayerSvcHandler() {}

int PlayerSvcHandler::open(void*){

  GameManager* manager = GameManager::instance();
  // manager->games.push_back(game);
  int player_name_length = manager->name.length();
  ACE_Message_Block *mb = new ACE_Message_Block(player_name_length + 1);
  
  mb->copy(manager->name.c_str(), player_name_length + 1);
  ACE_Time_Value tv(2);
  game->handler.putq(mb, &tv);
  ACE_Reactor::instance()->register_handler(this,
  					    ACE_Event_Handler::READ_MASK |
                                            ACE_Event_Handler::WRITE_MASK);

  return 0;
}

int PlayerSvcHandler::handle_input(ACE_HANDLE){
  if (game->dealer_name.empty()) {
    ACE_Time_Value timeout(2);
    int retval = peer().recv(buffer, sizeof buffer, &timeout);
    if (retval == -1) {
      std::cout << "Error receiving data." << std::endl;
      return -1;
    } else if (retval == 0) {  // Close down gracefully.
      return -1;
    }
    std::cout << "Connected to dealer: " << buffer << std::endl;
    game->dealer_name = buffer;

    for (unsigned int i = 0; i < game->games.size(); ++i) {
      enqueue_string_for_output(game->games[i]);
    }

    // Dealer waits to see an empty string before it stops looking for game
    // names.
    string empty = "";
    enqueue_string_for_output(empty);

    ACE_Reactor::instance ()->mask_ops(this,
				       ACE_Event_Handler::WRITE_MASK |        
				       ACE_Event_Handler::READ_MASK,          
				       ACE_Reactor::SET_MASK);   
  } else {
    // We are reading cards in from dealer.
    ACE_Time_Value timeout(3);
    int retval = peer().recv(buffer, 2, &timeout);
    if (retval == -1) {
      std::cout << "Error receiving data." << std::endl;
      return -1;
    } else if (retval == 0) {  // Close down gracefully.
      return -1;
    }
    char s = buffer[0];
    char r = buffer[1];
    Card card;
    card.set_values(s, r);
    
    game->hand.add_card(card);
    if (game->hand.size() == 5) {
      std::cout << "New hand for game " << game->games[0] << " :" << game->hand
                << std::endl;
    
 
      int * scoreArray = getHandScoreArray (game->hand);

      int score = arrayToScore (scoreArray);

      int size = sizeof score;
      std::cout << "Size of score: " << size << std::endl;
      // ADD TO QUEUE
      ACE_Message_Block *mb = new ACE_Message_Block(size);
      mb->copy((char*)&score, size);
      ACE_Time_Value tv(2);
      putq(mb, &tv);

      // SET WRITE MASK
      ACE_Reactor::instance()->register_handler(this,
						ACE_Event_Handler::READ_MASK |
						ACE_Event_Handler::WRITE_MASK);
      std::cout << "SCORE: " << score << std::endl;
 
    }
 
  }
  return 0;
}

int PlayerSvcHandler::handle_output(ACE_HANDLE){
  ACE_Message_Block *mb;
  ACE_Time_Value tv(2);
  int queue_length = this->getq(mb, &tv);
  if (queue_length > -1) {
    ACE_Time_Value tv2(90);
    int retval = peer().send_urg(mb->rd_ptr(), mb->length()+1, &tv2);
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

int PlayerSvcHandler::handle_close (ACE_HANDLE, ACE_Reactor_Mask) {
  shutdown();
  return 0;
}

int PlayerSvcHandler::enqueue_string_for_output(string s) {
  int length = s.length(); 
  ACE_Message_Block *mb;
  mb = new ACE_Message_Block(length+1);
  mb->copy(s.c_str(), length+1);
  ACE_Time_Value tv(2);
  putq(mb, &tv);
  return 0;
}
