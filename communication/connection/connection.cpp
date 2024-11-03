
#include "connection.h"
#include "ip.h"
#include "socket_helper.h"
#include <unistd.h>

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::~Connection() { if ( ! is_connected() ) return; close( socket_id ); status = STATUS_CODE::DISCONNECTED; }

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection( const int& socket_id, const IP_TYPE& ip ) : socket_id( socket_id ), ip( ip ), status( STATUS_CODE::CONNECTED ) {}

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection( Connection< IP_TYPE >&& other ) 
:  socket_id( other.socket_id ), ip( other.ip ), status( other.status ) { other.status = STATUS_CODE::DISCONNECTED; }

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection() : socket_id( -1 ), ip(), status( STATUS_CODE::DISCONNECTED ) {}

template < typename IP_TYPE >
const bool spyke::communication::connection::Connection< IP_TYPE >::is_connected() const { return status == STATUS_CODE::CONNECTED; }

template < typename IP_TYPE >
const int& spyke::communication::connection::Connection< IP_TYPE >::get_socket_id() const { return socket_id; }

template < typename IP_TYPE >
const bool spyke::communication::connection::Connection< IP_TYPE >::operator==( const Connection< IP_TYPE >& other ) const { return socket_id == other.socket_id && ip == other.ip; }

template < typename IP_TYPE >
const bool spyke::communication::connection::Connection< IP_TYPE >::operator!=( const Connection< IP_TYPE >& other ) const { return ! operator==( other ); }


template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE > spyke::communication::connection::Connection< IP_TYPE >::connect_to( const IP_TYPE& ip ) {

  const int socket_id = connection::connect_to( ip );
  if ( socket_id == -1 ) return Connection< IP_TYPE >();

  return Connection< IP_TYPE >( socket_id, ip );

}

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE > spyke::communication::connection::Connection< IP_TYPE >::create_server( const IP_TYPE& ip ) {

  const int socket_id = connection::create_server( ip );
  if ( socket_id == -1 ) return Connection< IP_TYPE >();

  return Connection< IP_TYPE >( socket_id, ip );

}


template class spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >;
template class spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >;
