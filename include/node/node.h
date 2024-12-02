
#ifndef INCLUDE_NODE_NODE_H
#define INCLUDE_NODE_NODE_H

#include "node_connections_manager.h"

namespace spyke::node {

  enum class NODE_STATUS : char {

    VALID = 0,
    INVALID = 1,

  };

  template < typename SERVER_IP_TYPE >
  struct Node_Settings {

    const Node_Connections_Manager_Settings< SERVER_IP_TYPE > node_connections_manager_settings;

  };

  template < typename SERVER_IP_TYPE >
  class Node {

    private:

      const Node_Settings< SERVER_IP_TYPE > settings;

      Node_Connections_Manager< SERVER_IP_TYPE > connections_manager;

      NODE_STATUS status;

    public:

      Node( const Node& ) = delete;

      Node( Node&& );

      Node();

      Node( const Node_Settings< SERVER_IP_TYPE >& );
      
      const bool is_valid() const;

      void start();

      void stop();

  };

}

#endif
