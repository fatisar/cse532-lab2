// CSE571 - signal-handler.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// This class handles the SIGINT that it created when CTRL-C is pressed by
// setting a static class variable, which is checked by the appropriate loops.

#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <iostream>
#include "stdio.h"
#include <map>
#include <iterator>
#include <utility>

#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Signal.h"

#include "dealer.h"
#include "game.h"
#include "player.h"

class SigHandler : public ACE_Event_Handler {
 public:
  SigHandler (ACE_Reactor *reactor);
  int handle_signal (int signum, siginfo_t *, ucontext_t *);
  static bool sigint_fired(); 

 private:
  static bool sigint_fired_;
};

#endif
