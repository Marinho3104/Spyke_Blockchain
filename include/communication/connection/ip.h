
#ifndef INCLUDE_COMMUNICATION_CONNECTION_IP_H
#define INCLUDE_COMMUNICATION_CONNECTION_IP_H

namespace spyke::communication::connection {

  class IP_V4 {

    private:

      const int address;
      const short port;

    public:

      IP_V4();

      IP_V4( const int&, const short& );

      const bool is_valid() const;

      const int get_address() const;

      const short get_port() const;

      const bool operator==( const IP_V4& ) const;

      const bool operator!=( const IP_V4& ) const;

  };

  class IP_V6 {

    private:

      char address[ 16 ];
      short port;

    public:

      IP_V6();

      IP_V6( const char[ 16 ], const short& );
  
      const bool is_valid() const;

      const char* get_address() const;

      const short get_port() const;

      const bool operator==( const IP_V6& ) const;

      const bool operator!=( const IP_V6& ) const;

  };

}

#endif
