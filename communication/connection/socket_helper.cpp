
#include "socket_helper.h"
#include "ip.h"
#include <cstring>
#include <type_traits>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

void spyke::communication::socket_helper::close_socket_id( const int& socket_id ) { ::close( socket_id ); }

template < typename IP_TYPE >
const int spyke::communication::socket_helper::connect_to( const IP_TYPE& connection_ip ) {
  
  const int socket_id = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( socket_id == -1 ) return -1;

  // Need to be mutable for it 
  // depends o nthe template value
  struct sockaddr_storage hint;

  if constexpr( std::is_same< IP_TYPE, connection::IP_V4 >::value ) {

    sockaddr_in* hint_ptr = reinterpret_cast< sockaddr_in* >( &hint ); 

    hint_ptr->sin_family = AF_INET;
    hint_ptr->sin_port = htons( connection_ip.get_port() );
    hint_ptr->sin_addr.s_addr = htonl( connection_ip.get_address() );

  } else {

    sockaddr_in6* hint_ptr = reinterpret_cast< sockaddr_in6* >( &hint );

    hint_ptr->sin6_family = AF_INET6;
    hint_ptr->sin6_port = htons( connection_ip.get_port() );

    std::memcpy( &hint_ptr->sin6_addr, connection_ip.get_address(), sizeof( hint_ptr->sin6_addr ) );

    throw -1; // Not sure if memcpy is correct check before using it

  }

  const int sts_connect = connect( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_connect == -1 ) { close( socket_id ); return -1; }

  return socket_id;


}

template < typename IP_TYPE >
const int spyke::communication::socket_helper::create_server( const IP_TYPE& server_ip ) {

  const int socket_id = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( socket_id == -1 ) return -1;

  // Need to be mutable for it 
  // depends on the template value
  struct sockaddr_storage hint;

  if constexpr( std::is_same< IP_TYPE, connection::IP_V4 >::value ) {

    sockaddr_in* hint_ptr = reinterpret_cast< sockaddr_in* >( &hint ); 

    hint_ptr->sin_family = AF_INET;
    hint_ptr->sin_port = htons( server_ip.get_port() );
    hint_ptr->sin_addr.s_addr = htonl( server_ip.get_address() );

  } else {

    sockaddr_in6* hint_ptr = reinterpret_cast< sockaddr_in6* >( &hint );

    hint_ptr->sin6_family = AF_INET6;
    hint_ptr->sin6_port = htons( server_ip.get_port() );

    std::memcpy( &hint_ptr->sin6_addr, server_ip.get_address(), sizeof( hint_ptr->sin6_addr ) );

    throw -1; // Not sure if memcpy is correct check before using it

  }

  const int sts_bind = bind( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_bind == -1 ) { close( socket_id ); return -1; }

  const int sts_listen = listen( socket_id, 5 );
  if( sts_listen == -1 ) { close( socket_id ); return -1; }

  return socket_id;


}

template const int spyke::communication::socket_helper::connect_to< spyke::communication::connection::IP_V4 >( const connection::IP_V4& );
template const int spyke::communication::socket_helper::create_server< spyke::communication::connection::IP_V4 >( const connection::IP_V4& );
template const int spyke::communication::socket_helper::connect_to< spyke::communication::connection::IP_V6 >( const connection::IP_V6& );
template const int spyke::communication::socket_helper::create_server< spyke::communication::connection::IP_V6 >( const connection::IP_V6& );
