
#include "packet.h"
#include "ip.h"
#include "socket_helper.h"
#include <cstring>
#include <memory>
#include <semaphore.h>


spyke::communication::communication_protocols::Packet::Packet() : protocol_id( PROTOCOL_ID::INVALID ) {}

spyke::communication::communication_protocols::Packet::Packet( const PROTOCOL_ID& protocol_id )
  : protocol_id( protocol_id ) {}

spyke::communication::communication_protocols::Packet::Packet( Packet&& other ) : protocol_id( other.protocol_id ) {}

const bool spyke::communication::communication_protocols::Packet::is_valid() const { return protocol_id != PROTOCOL_ID::INVALID; }

const size_t spyke::communication::communication_protocols::Packet::get_serialization_size() const { return sizeof( protocol_id ); }

std::unique_ptr< char[] > spyke::communication::communication_protocols::Packet::serialize() const {

  if ( ! is_valid() ) return 0;

  std::unique_ptr< char[] > buffer = std::make_unique< char[] >( get_serialization_size() );

  char* buffer_ptr = buffer.get();

  std::memcpy( buffer_ptr, &protocol_id, sizeof( protocol_id ) ); buffer_ptr += sizeof( protocol_id );

  return std::move( buffer );

}

// template < typename CONNECTION_IP_TYPE >
// const bool spyke::communication::communication_protocols::Packet::send( std::shared_ptr< spyke::communication::connection::Connection< CONNECTION_IP_TYPE > > connection ) { 
//
//   if ( ! is_valid() || ! connection || connection.get()->is_connected() ) return 0;
//
//   sem_wait( &connection.get()->get_locker() );
//
//   const bool send_data_sts = connection::send_data_to( connection.get()->get_socket_id(), serialize().get(), get_serialization_size() ); 
//
//   sem_post( &connection.get()->get_locker() );
//
//   return send_data_sts;
//
// }
//
//
// template < typename CONNECTION_IP_TYPE >
// std::unique_ptr< spyke::communication::communication_protocols::Packet > spyke::communication::communication_protocols::Packet::receive_packet( std::shared_ptr< spyke::communication::connection::Connection< CONNECTION_IP_TYPE > > connection ) {
//
//   if ( ! connection || connection.get()->is_connected() ) return std::make_unique< Packet >();
//
//   sem_wait( &connection.get()->get_read_locker() );
//
//   PROTOCOL_ID protocol_id; 
//   if ( 
//     ! connection::receive_data_from( 
//       connection.get()->get_socket_id(), 
//       reinterpret_cast< char* >( &protocol_id ), 
//       sizeof( protocol_id ) 
//     ) 
//   ) return std::make_unique< Packet >();
//
//   Packet packet = Packet( protocol_id );
//
//   sem_post( &connection.get()->get_read_locker() );
//
//   return std::make_unique< Packet >( std::move( packet ) );
//
// }

// template std::unique_ptr< spyke::communication::communication_protocols::Packet > spyke::communication::communication_protocols::Packet::receive_packet( std::shared_ptr< spyke::communication::connection::Connection< connection::IP_V4 > > );
// template std::unique_ptr< spyke::communication::communication_protocols::Packet > spyke::communication::communication_protocols::Packet::receive_packet( std::shared_ptr< spyke::communication::connection::Connection< connection::IP_V6 > > );
