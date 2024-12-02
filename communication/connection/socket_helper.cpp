
#include "socket_helper.h"
#include "ip.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

void spyke::communication::connection::close_socket_id( const int& socket_id ) { ::close( socket_id ); }

template < typename IP_TYPE >
const int spyke::communication::connection::connect_to( const IP_TYPE& connection_ip ) {
  
  // Need to be mutable for it 
  // depends on the template value
  int socket_id; struct sockaddr_storage hint; memset( &hint, 0, sizeof( sockaddr_storage ) );

  if constexpr( std::is_same< IP_TYPE, connection::IP_V4 >::value ) {

    socket_id = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if( socket_id == -1 ) { /* perror( "socket" ); */ return -1; }

    sockaddr_in* hint_ptr = reinterpret_cast< sockaddr_in* >( &hint ); 

    hint_ptr->sin_family = AF_INET;
    hint_ptr->sin_port = htons( connection_ip.get_port() );
    hint_ptr->sin_addr.s_addr = htonl( connection_ip.get_address() );

  } else {

    socket_id = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
    if( socket_id == -1 ) { /* perror( "socket" ); */ return -1; }

    sockaddr_in6* hint_ptr = reinterpret_cast< sockaddr_in6* >( &hint );

    hint_ptr->sin6_family = AF_INET6;
    hint_ptr->sin6_port = htons( connection_ip.get_port() );

    std::memcpy( hint_ptr->sin6_addr.s6_addr, connection_ip.get_address(), 16 );

  }

  const int sts_connect = connect( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_connect == -1 ) { /* perror( "connect" ); */ close( socket_id ); return -1; }

  return socket_id;

}

template < typename IP_TYPE >
const int spyke::communication::connection::create_server( const IP_TYPE& server_ip ) {

  // Need to be mutable for it 
  // depends on the template value
  int socket_id; struct sockaddr_storage hint; memset( &hint, 0, sizeof( sockaddr_storage ) );

  if constexpr( std::is_same< IP_TYPE, connection::IP_V4 >::value ) {

    socket_id = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if( socket_id == -1 ) { /* perror( "socket" ); */ return -1; }

    sockaddr_in* hint_ptr = reinterpret_cast< sockaddr_in* >( &hint ); 

    hint_ptr->sin_family = AF_INET;
    hint_ptr->sin_port = htons( server_ip.get_port() );
    hint_ptr->sin_addr.s_addr = htonl( server_ip.get_address() );

  } else {

    socket_id = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
    if( socket_id == -1 ) { /* perror( "socket" ); */ return -1; }

    sockaddr_in6* hint_ptr = reinterpret_cast< sockaddr_in6* >( &hint );

    hint_ptr->sin6_family = AF_INET6;
    hint_ptr->sin6_port = htons( server_ip.get_port() );

    std::memcpy( hint_ptr->sin6_addr.s6_addr, server_ip.get_address(), 16 );

  }

  const int sts_bind = bind( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_bind == -1 ) { /* perror( "bind" ); */ close( socket_id ); return -1; }

  const int sts_listen = listen( socket_id, 5 );
  if( sts_listen == -1 ) { /* perror( "listen" ); */ close( socket_id ); return -1; }

  return socket_id;


}

const spyke::communication::connection::Accept_Connection_Request_Return spyke::communication::connection::accept_connection_request( const int& server_socket_id ) {

  struct sockaddr_storage hint; memset( &hint, 0, sizeof( sockaddr_storage ) );
  socklen_t hint_size = sizeof( hint );

  const int socket_id = accept( server_socket_id, ( sockaddr* ) &hint, &hint_size );
  if( socket_id == -1 ) { /* perror( "accept" ); */ return { hint, -1 }; }

  return { hint, socket_id };

}

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE > spyke::communication::connection::convert_connection( const Accept_Connection_Request_Return& accept_connection_request ) {

  return connection::Connection< IP_TYPE >( accept_connection_request.socket_id, IP_TYPE::from_hint( accept_connection_request.hint ) );

}

const bool spyke::communication::connection::send_data_to( const int& socket_id, const char* data, const size_t& data_size ) 
  { const int sts_send = send( socket_id, data, data_size, 0 ); return sts_send != -1 && sts_send != 0; }

const bool spyke::communication::connection::receive_data_from( const int& socket_id, char* data, const size_t& data_size ) 
  { const int sts_recv = recv( socket_id, data, data_size, 0 ); return sts_recv != -1 && sts_recv != 0; }


template const int spyke::communication::connection::connect_to< spyke::communication::connection::IP_V4 >( const connection::IP_V4& );
template const int spyke::communication::connection::create_server< spyke::communication::connection::IP_V4 >( const connection::IP_V4& );
template spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > 
  spyke::communication::connection::convert_connection< spyke::communication::connection::IP_V4 >( const Accept_Connection_Request_Return& );

template const int spyke::communication::connection::connect_to< spyke::communication::connection::IP_V6 >( const connection::IP_V6& );
template const int spyke::communication::connection::create_server< spyke::communication::connection::IP_V6 >( const connection::IP_V6& );
template spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > 
  spyke::communication::connection::convert_connection< spyke::communication::connection::IP_V6 >( const Accept_Connection_Request_Return& );
