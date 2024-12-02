

#ifndef INCLUDE_CLIENT_NODE_CLIENT_H
#define INCLUDE_CLIENT_NODE_CLIENT_H

#include "node.h"

namespace spyke::client::node {

  template < typename SERVER_IP_TYPE >
  static spyke::node::Node< SERVER_IP_TYPE > node_instance;

  static sem_t exiting_done;

  template < typename SERVER_IP_TYPE >
  const bool setup( const spyke::node::Node_Settings< SERVER_IP_TYPE >& );

  template < typename SERVER_IP_TYPE >
  void start();

  void input_loop();

  const bool set_signal_mask();

  template < typename SERVER_IP_TYPE >
  void SIGINT_handler( int );

}

#endif
