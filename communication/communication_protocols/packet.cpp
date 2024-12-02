
#include "packet.h"
#include "socket_helper.h"
#include <cstring>
#include <memory>


spyke::communication::communication_protocols::Packet::Packet() : protocol_id( 0 ), data_size( 0 ) {}

spyke::communication::communication_protocols::Packet::Packet( const unsigned char& protocol_id, unsigned char data[], const size_t& data_size )
  : data( std::make_unique< unsigned char[] >( data_size ) ), protocol_id( protocol_id ), data_size( data_size ) { std::memcpy( this->data.get(), data, data_size ); }

spyke::communication::communication_protocols::Packet::Packet( Packet&& other ) : protocol_id( other.protocol_id ), data_size( other.data_size ), data( std::move( other.data ) ) {}

const bool spyke::communication::communication_protocols::Packet::is_valid() const { return data ? 1 : 0; }

const size_t spyke::communication::communication_protocols::Packet::get_serialization_size() const { return sizeof( protocol_id ) + sizeof( data_size ) + data_size; }

std::unique_ptr< char[] > spyke::communication::communication_protocols::Packet::serialize() const {

  if ( ! is_valid() ) return 0;

  std::unique_ptr< char[] > buffer = std::make_unique< char[] >( get_serialization_size() );

  char* buffer_ptr = buffer.get();

  std::memcpy( buffer_ptr, &protocol_id, sizeof( protocol_id ) ); buffer_ptr += sizeof( protocol_id );
  std::memcpy( buffer_ptr, &data_size, sizeof( data_size ) ); buffer_ptr += sizeof( data_size );
  std::memcpy( buffer_ptr, data.get(), data_size );

  return std::move( buffer );

}

const bool spyke::communication::communication_protocols::Packet::send( const int& socket_id ) const 
  { if ( ! is_valid() ) return 0; return connection::send_data_to( socket_id, serialize().get(), get_serialization_size() ); }


spyke::communication::communication_protocols::Packet spyke::communication::communication_protocols::Packet::receive_packet( const int& socket_id ) {

  unsigned char protocol_id; 
  if ( 
    ! connection::receive_data_from( 
      socket_id, 
      reinterpret_cast< char* >( &protocol_id ), 
      sizeof( protocol_id ) 
    ) 
  ) return Packet();

  size_t data_size;
  if ( 
    ! connection::receive_data_from( 
      socket_id, 
      reinterpret_cast< char* >( &data_size ), 
      sizeof( data_size ) 
    ) 
  ) return Packet();

  unsigned char* data = new unsigned char[ data_size ];
  if ( 
    ! connection::receive_data_from( 
      socket_id, 
      reinterpret_cast< char* >( data ), 
      data_size 
    ) 
  ) { delete[] data; return Packet(); }

  Packet packet = Packet( protocol_id, data, data_size );

  delete[] data;

  return std::move( packet );

}
