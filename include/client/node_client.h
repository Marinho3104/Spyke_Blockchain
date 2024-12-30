

#ifndef INCLUDE_CLIENT_NODE_CLIENT_H
#define INCLUDE_CLIENT_NODE_CLIENT_H

#include "node.h"
#include <semaphore.h>

namespace spyke::client::node {

  // Global instance of a node to be used 
  // everywhere in the code as needed
  // Meant to only be used by this client
  // logic and not outside of it
  template < typename SERVER_IP_TYPE >
  static spyke::node::Node< SERVER_IP_TYPE > node_instance;

  // Funcion to setup the global ref of node instance
  template < typename SERVER_IP_TYPE >
  const bool setup( spyke::node::Node_Settings< SERVER_IP_TYPE >& );

  // Entry point to start the node instance
  // will exit when the node instance stops runnning
  template < typename SERVER_IP_TYPE >
  void start();

  // Used to obtain input from the user
  // to perform any available task on a 
  // running node
  template < typename SERVER_IP_TYPE >
  void input_loop();

  // Sets the signal mask to ignore everything
  // but the SIGINT signal 
  const bool set_signal_mask();

  // Hanlder to stop the node instance
  template < typename SERVER_IP_TYPE >
  void SIGINT_handler( int );

}

#endif
