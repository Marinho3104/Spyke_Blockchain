
#include "socket_helper.h"
#include "ip.h"
#include <cstring>
#include <type_traits>
#include <arpa/inet.h>
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
  struct sockaddr_storage hint; memset( &hint, 0, sizeof( sockaddr_storage ) );

  if constexpr( std::is_same< IP_TYPE, connection::IP_V4 >::value ) {

    sockaddr_in* hint_ptr = reinterpret_cast< sockaddr_in* >( &hint ); 

    hint_ptr->sin_family = AF_INET;
    hint_ptr->sin_port = htons( connection_ip.get_port() );
    hint_ptr->sin_addr.s_addr = htonl( connection_ip.get_address() );

  } else {

    sockaddr_in6* hint_ptr = reinterpret_cast< sockaddr_in6* >( &hint );

    hint_ptr->sin6_family = AF_INET6;
    hint_ptr->sin6_port = htons( connection_ip.get_port() );

    if( inet_pton(AF_INET6, connection_ip.get_address(), &hint_ptr->sin6_addr) <= 0 ) 
      { close( socket_id ); return -1; }

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
  struct sockaddr_storage hint; memset( &hint, 0, sizeof( sockaddr_storage ) );

  if constexpr( std::is_same< IP_TYPE, connection::IP_V4 >::value ) {

    sockaddr_in* hint_ptr = reinterpret_cast< sockaddr_in* >( &hint ); 

    hint_ptr->sin_family = AF_INET;
    hint_ptr->sin_port = htons( server_ip.get_port() );
    hint_ptr->sin_addr.s_addr = htonl( server_ip.get_address() );

  } else {

    sockaddr_in6* hint_ptr = reinterpret_cast< sockaddr_in6* >( &hint );

    hint_ptr->sin6_family = AF_INET6;
    hint_ptr->sin6_port = htons( server_ip.get_port() );

    if( inet_pton(AF_INET6, server_ip.get_address(), &hint_ptr->sin6_addr) <= 0 ) 
      { close( socket_id ); return -1; }

  }

  const int sts_bind = bind( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_bind == -1 ) { close( socket_id ); return -1; }

  const int sts_listen = listen( socket_id, 5 );
  if( sts_listen == -1 ) { close( socket_id ); return -1; }

  return socket_id;


}

const spyke::communication::socket_helper::Accept_Connection_Request_Return spyke::communication::socket_helper::accept_connection_request( const int& server_socket_id ) {

  struct sockaddr_storage hint; memset( &hint, 0, sizeof( sockaddr_storage ) );
  socklen_t hint_size = sizeof( hint );

  const int socket_id = accept( server_socket_id, ( sockaddr* ) &hint, &hint_size );
  if( socket_id == -1 ) return { hint, -1 };

  return { hint, socket_id };

}

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE > spyke::communication::socket_helper::convert_connection( const Accept_Connection_Request_Return& accept_connection_request ) {

  return connection::Connection< IP_TYPE >( accept_connection_request.socket_id, IP_TYPE::from_hint( accept_connection_request.hint ) );

}

template const int spyke::communication::socket_helper::connect_to< spyke::communication::connection::IP_V4 >( const connection::IP_V4& );
template const int spyke::communication::socket_helper::create_server< spyke::communication::connection::IP_V4 >( const connection::IP_V4& );
template spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > 
  spyke::communication::socket_helper::convert_connection< spyke::communication::connection::IP_V4 >( const Accept_Connection_Request_Return& );

template const int spyke::communication::socket_helper::connect_to< spyke::communication::connection::IP_V6 >( const connection::IP_V6& );
template const int spyke::communication::socket_helper::create_server< spyke::communication::connection::IP_V6 >( const connection::IP_V6& );
template spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > 
  spyke::communication::socket_helper::convert_connection< spyke::communication::connection::IP_V6 >( const Accept_Connection_Request_Return& );
