// CSE571 - dealer-main.cpp          
// Authors: Jonathan Wald, Daniel Sarfati, Guangle Fan
// This file holds the main function that is run by the server program.

#include <iostream>

#include <ace/INET_Addr.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include "dealer.h"
#include "dealer-svc-handler.h"
#include "signal-handler.h"

typedef ACE_Acceptor<DealerSvcHandler,ACE_SOCK_ACCEPTOR> DealerAcceptor; 

const static int DEALER_DEFAULT_PORT = 8000;

int main(int argc, char* argv[]) {
  int port = DEALER_DEFAULT_PORT;

  const int NAME_FLAG = 0, PORT_FLAG = 1;
  int flag;
  
  bool parse_error = false;
  string error_msg;
  
  // Parse command line arguments.
  for(int i = 2; i < argc; i+=2) {
    if (!strcmp("-n", argv[i-1])) flag = NAME_FLAG;
    else if (!strcmp("-p", argv[i-1])) flag = PORT_FLAG;
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
            Dealer::instance()->name = argv[i];
            break;
        case PORT_FLAG:
            port = atoi(argv[i]);
            break;
    }
  }
  
  if (parse_error) {
    std::cout << "ERROR: " << error_msg << std::endl;
    std::cout << "Usage: ./server -n <dealer name> [-p <server port>]" << std::endl;
    return -1;
  }
  
  std::cout << "Dealer's name: " << Dealer::instance()->name << std::endl; 
  
  // Attempt to set up a socket for Dealer to listen on
  const int HOST_NAME_SIZE = 64;
  ACE_INET_Addr server_addr;
  char address_string [HOST_NAME_SIZE];
  ::gethostname (address_string, HOST_NAME_SIZE - 1);
  if (server_addr.set(port, address_string) == -1) {
    std::cout << "Unable to set server address:" << address_string << std::endl;
    return -1;
  }

  server_addr.addr_to_string (address_string, HOST_NAME_SIZE - 1);
  std::cout << "Address and port: " << address_string << std::endl;

  DealerAcceptor dealer_acceptor(server_addr, ACE_Reactor::instance());

  // Implement signal handler.
  ACE_Sig_Action sa;
  sa.register_action (SIGINT);
  SigHandler sh (ACE_Reactor::instance());

  while(!SigHandler::sigint_fired()) {
    ACE_Reactor::instance()->handle_events();
  }

  if (Dealer::instance() != NULL) {
    delete Dealer::instance();
  }
  return 0;
}
