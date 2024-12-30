

#ifndef INCLUDE_COMMUNICATION_COMMUNICATION_PROTOCOLS_PACKET_H
#define INCLUDE_COMMUNICATION_COMMUNICATION_PROTOCOLS_PACKET_H

#include <cstddef>
#include <memory>
#include <semaphore.h>

namespace spyke::communication::communication_protocols {

  enum PROTOCOL_ID : unsigned char {

    INVALID = 0,
    SWITCH_TO_STABLE_CONNECTION = 1,

  };
  
  class Packet {

    private:

      const PROTOCOL_ID protocol_id;

    public:

      Packet( const Packet& ) = delete;

      Packet();

      Packet( const PROTOCOL_ID& );

      Packet( Packet&& );

      const bool is_valid() const;

      virtual const size_t get_serialization_size() const;

      virtual std::unique_ptr< char[] > serialize() const;

      // template < typename CONNECTION_IP_TYPE >
      // const bool send( std::shared_ptr< spyke::communication::connection::Connection< CONNECTION_IP_TYPE > > );

    
      // template < typename CONNECTION_IP_TYPE >
      // static std::unique_ptr< Packet > receive_packet( std::shared_ptr< spyke::communication::connection::Connection< CONNECTION_IP_TYPE > > );

  };

}

#endif
