

#ifndef INCLUDE_COMMUNICATION_CONNECTION_SOCKET_HELPER_H
#define INCLUDE_COMMUNICATION_CONNECTION_SOCKET_HELPER_H

#include "connection.h"
#include <sys/socket.h>

namespace spyke::communication::connection {

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

  const bool send_data_to( const int&, const char*, const size_t& );

  const bool receive_data_from( const int&, char*, const size_t& );

}

#endif
