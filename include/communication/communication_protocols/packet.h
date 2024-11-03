

#ifndef INCLUDE_COMMUNICATION_COMMUNICATION_PROTOCOLS_PACKET_H
#define INCLUDE_COMMUNICATION_COMMUNICATION_PROTOCOLS_PACKET_H

#include <cstddef>
#include <memory>

namespace spyke::communication::communication_protocols {
  
  class Packet {

    private:

      std::unique_ptr< unsigned char[] > data;
      const unsigned char protocol_id;
      const size_t data_size;

    public:

      Packet( const Packet& ) = delete;

      Packet();

      Packet( const unsigned char&, unsigned char[], const size_t& );

      Packet( Packet&& );

      const bool is_valid() const;

      const size_t get_serialization_size() const;

      std::unique_ptr< char[] > serialize() const;

      const bool send( const int& ) const;

    
      static Packet receive_packet( const int& );

  };

}

#endif
