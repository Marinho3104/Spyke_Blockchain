
#include "connection.h"
#include "ip.h"
#include "packet.h"
#include "socket_helper.h"
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::~Connection() 
  { if ( ! is_connected() ) return; sem_close( &write_locker ); sem_close( &read_locker ); if ( socket_id != -1 ) close( socket_id ); status = STATUS_CODE::DISCONNECTED; }

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection( const int& socket_id, const IP_TYPE& ip ) : socket_id( socket_id ), ip( ip ), status( STATUS_CODE::CONNECTED ) 
  { sem_init( &write_locker, 0, 1 ); sem_init( &read_locker, 0, 1 ); }

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE >::Connection( Connection< IP_TYPE >&& other ) 
:  socket_id( other.socket_id ), ip( other.ip ), status( other.status ), write_locker( std::move( other.write_locker ) ), read_locker( std::move( other.read_locker ) ) { other.status = STATUS_CODE::DISCONNECTED; }

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
const bool spyke::communication::connection::Connection< IP_TYPE >::send_packet( std::unique_ptr< communication_protocols::Packet > packet ) {

  if ( ! is_connected() || ! packet->is_valid() ) return false;

  sem_wait( &write_locker );

  std::unique_ptr< char[] > data_serialized = std::move( packet->serialize() );
  const size_t data_serialized_size = packet->get_serialization_size();

  const bool send_sts = spyke::communication::connection::send_data_to( socket_id, data_serialized.get(), data_serialized_size );

  sem_post( &write_locker );

  return send_sts;

}

template < typename IP_TYPE >
std::unique_ptr< spyke::communication::communication_protocols::Packet > spyke::communication::connection::Connection< IP_TYPE >::receive_packet() {

  if( ! is_connected() ) return std::make_unique< spyke::communication::communication_protocols::Packet >();
  
  sem_wait( &read_locker );

  communication_protocols::PROTOCOL_ID protocol_id; 
  if ( 
    ! connection::receive_data_from( 
      socket_id, 
      reinterpret_cast< char* >( &protocol_id ), 
      sizeof( protocol_id )
    ) 
  ) return std::make_unique< communication_protocols::Packet >();

  sem_post( &read_locker );

  return std::make_unique< communication_protocols::Packet >( protocol_id );

}


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
