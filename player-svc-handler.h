// CSE571 - player-svc-handler.h
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// Player service handler which is used for opening a connection and then (with
// R/W masks) for managing each socket connection reactively.

#ifndef PLAYER_SVC_HANDLER_H
#define PLAYER_SVC_HANDLER_H

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Acceptor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"

class PlayerGame;

#define PORT_NUM 10101
#define DATA_SIZE 100

using namespace std;

class PlayerSvcHandler:
public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {

 public:
  PlayerSvcHandler();
  int open(void*);
  int handle_input(ACE_HANDLE);
  int handle_output(ACE_HANDLE);
  int handle_close (ACE_HANDLE=ACE_INVALID_HANDLE, ACE_Reactor_Mask=ACE_Event_Handler::ALL_EVENTS_MASK);
  PlayerGame* game;
  int enqueue_string_for_output(string s);

 private:
};



#endif
