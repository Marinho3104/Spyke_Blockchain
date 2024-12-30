
#ifndef INCLUDE_COMMUNICATION_CONNECTION_IP_H
#define INCLUDE_COMMUNICATION_CONNECTION_IP_H

#include <array>
#include <sys/socket.h>
namespace spyke::communication::connection {

  class IP_V4 {

    private:

      const int address;
      const short port;

    public:

      IP_V4();

      IP_V4( const int&, const short& );

      const bool is_valid() const;

      const int& get_address() const;

      const short& get_port() const;

      const bool operator==( const IP_V4& ) const;

      const bool operator!=( const IP_V4& ) const;

      
      static IP_V4 from_hint( const sockaddr_storage& );

  };

  class IP_V6 {

    private:

      const std::array< char, 16 > address;
      const short port;

    public:

      IP_V6();

      IP_V6( const std::array< char, 16 >&, const short& );
  
      const bool is_valid() const;

      const std::array< char, 16 >& get_address() const;

      const short& get_port() const;

      const bool operator==( const IP_V6& ) const;

      const bool operator!=( const IP_V6& ) const;


      static IP_V6 from_hex( const std::array< char, 40 >&, const short& );

      static IP_V6 from_hint( const sockaddr_storage& );

  };

}

#endif
