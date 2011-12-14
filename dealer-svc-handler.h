// CSE571 - dealer-svc-handler.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Dealer service handler which is used (with an ACCEPT mask) for opening a
// connection and then (with the R/W masks) for managing each socket connection
// reactively.  

#ifndef DEALER_SVC_HANDLER_H
#define DEALER_SVC_HANDLER_H

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"

#include "dealer.h"

class Player;

#define PORT_NUM 10101
#define DATA_SIZE 100

using namespace std;

class DealerSvcHandler:
public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_NULL_SYNCH> {

 public:
  DealerSvcHandler();
  int open(void*);
  int handle_input(ACE_HANDLE);
  int handle_output(ACE_HANDLE);
  int handle_close(ACE_HANDLE);
  int enqueue_card_for_output(char s, char r);
 private:
  bool games_read;
  Player* player;
};



#endif
