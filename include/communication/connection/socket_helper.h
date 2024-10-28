

#ifndef INCLUDE_COMMUNICATION_CONNECTION_SOCKET_HELPER_H
#define INCLUDE_COMMUNICATION_CONNECTION_SOCKET_HELPER_H

namespace spyke::communication::socket_helper {

  void close_socket_id( const int& );

  template < typename IP_TYPE >
  const int connect_to( const IP_TYPE& );

  template < typename IP_TYPE >
  const int create_server( const IP_TYPE& );

}

#endif
