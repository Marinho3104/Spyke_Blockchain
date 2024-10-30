

#ifndef INCLUDE_COMMUNICATION_CONNECTION_SOCKET_HELPER_H
#define INCLUDE_COMMUNICATION_CONNECTION_SOCKET_HELPER_H

#include "connection.h"
#include <sys/socket.h>
namespace spyke::communication::socket_helper {

  struct Accept_Connection_Request_Return {

    const sockaddr_storage hint;
    const int socket_id;

  };

  void close_socket_id( const int& );

  template < typename IP_TYPE >
  const int connect_to( const IP_TYPE& );

  template < typename IP_TYPE >
  const int create_server( const IP_TYPE& );

  const Accept_Connection_Request_Return accept_connection_request( const int& );

  template < typename IP_TYPE >
  connection::Connection< IP_TYPE > convert_connection( const Accept_Connection_Request_Return& );

}

#endif
