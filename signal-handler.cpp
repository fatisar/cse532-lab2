// CSE571 - signal-handler.cpp
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// This class handles the SIGINT that it created when CTRL-C is pressed by
// setting a static class variable, which is checked by the appropriate loops.

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
#include "player.h"
#include "signal-handler.h"

bool SigHandler::sigint_fired_ = 0;

SigHandler::SigHandler(ACE_Reactor *reactor) {
  if (reactor->register_handler (SIGINT, this) == -1) {
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
  }
}
  
bool SigHandler::sigint_fired() {
  return sigint_fired_;
}

int SigHandler::handle_signal (int, siginfo_t *,
			       ucontext_t *) {
  sigint_fired_ = 1;
  return 0;
}
