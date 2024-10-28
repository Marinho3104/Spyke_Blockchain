
#include "connection.h"
#include <unistd.h>


template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::~Connection() { if ( ! is_connected() ) return; close( socket_id ); }

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection( const int& socket_id, const IP_TYPE& ip ) : socket_id( socket_id ), ip( ip ), status( STATUS_CODE::CONNECTED ) {}

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection( Connection< IP_TYPE >&& other ) 
:  socket_id( other.socket_id ), ip( other.ip ), status( STATUS_CODE::CONNECTED ) { other.status = STATUS_CODE::DISCONNECTED; }

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection() : socket_id( 0 ), ip(), status( STATUS_CODE::DISCONNECTED ) {}

template < typename IP_TYPE >
const bool spyke::communication::connection::Connection< IP_TYPE >::is_connected() const { return status == STATUS_CODE::CONNECTED; }

template < typename IP_TYPE >
const bool spyke::communication::connection::Connection< IP_TYPE >::operator==( const Connection< IP_TYPE >& other ) const { return socket_id == other.socket_id && ip == other.ip; }

template < typename IP_TYPE >
const bool spyke::communication::connection::Connection< IP_TYPE >::operator!=( const Connection< IP_TYPE >& other ) const { return ! operator==( other ); }


template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE > spyke::communication::connection::Connection< IP_TYPE >::connect_to( const IP_TYPE& ip ) {

}

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE > spyke::communication::connection::Connection< IP_TYPE >::create_server( const IP_TYPE& ip ) {

}
