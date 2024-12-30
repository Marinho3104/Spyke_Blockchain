
#include "ip.h"
#include <arpa/inet.h>
#include <array>
#include <cstring>
#include <netinet/in.h>


spyke::communication::connection::IP_V4::IP_V4() : address( 0 ), port( 0 ) {}

spyke::communication::connection::IP_V4::IP_V4( const int& address, const short& port ) : address( address ), port( port ) {}

const bool spyke::communication::connection::IP_V4::is_valid() const { return port; }

const int& spyke::communication::connection::IP_V4::get_address() const { return address; }

const short& spyke::communication::connection::IP_V4::get_port() const { return port; }

const bool spyke::communication::connection::IP_V4::operator==( const IP_V4& other ) const { return address == other.address && port == other.port; }

const bool spyke::communication::connection::IP_V4::operator!=( const IP_V4& other ) const { return ! operator==( other ); }

spyke::communication::connection::IP_V4 spyke::communication::connection::IP_V4::from_hint( const sockaddr_storage& hint ) {

  const sockaddr_in* hint_ip_v4 = reinterpret_cast< const sockaddr_in* >( &hint );

  const int address_inv = hint_ip_v4->sin_addr.s_addr; const short port = hint_ip_v4->sin_port;
  const int address = ( address_inv & 0xff ) << 24 | ( address_inv & 0xff00 ) << 8 | ( address_inv & 0xff0000 ) >> 8 | ( address_inv & 0xff000000 ) >> 24;

  return IP_V4( address, port );

}


spyke::communication::connection::IP_V6::IP_V6() : address{}, port( 0 ) {}

spyke::communication::connection::IP_V6::IP_V6( const std::array< char, 16 >& address, const short& port ) : address( address ), port( port ) {}

const bool spyke::communication::connection::IP_V6::is_valid() const { return port; }

const std::array< char, 16 >& spyke::communication::connection::IP_V6::get_address() const { return address; }

const short& spyke::communication::connection::IP_V6::get_port() const { return port; }

const bool spyke::communication::connection::IP_V6::operator==( const IP_V6& other ) const { return other.address == address && port == other.port; }

const bool spyke::communication::connection::IP_V6::operator!=( const IP_V6& other ) const { return ! operator==( other ); }

spyke::communication::connection::IP_V6 spyke::communication::connection::IP_V6::from_hex( const std::array< char, 40 >& hex_address_representation, const short& port ) {

  char address_raw[ 16 ];
  if( inet_pton( AF_INET6, hex_address_representation.data(), address_raw ) <= 0 ) return IP_V6();

  std::array< char, 16 > address; ::std::copy( std::begin( address_raw ), std::end( address_raw ), std::begin( address ) );

  return IP_V6( address, port ); 

}

spyke::communication::connection::IP_V6 spyke::communication::connection::IP_V6::from_hint( const sockaddr_storage& hint ) {

  const sockaddr_in6* hint_ip_v6 = reinterpret_cast< const sockaddr_in6* >( &hint );

  char address_raw[ 16 ]; ::std::memcpy( address_raw, hint_ip_v6->sin6_addr.s6_addr, sizeof( address_raw ) );

  std::array< char, 16 > address; ::std::copy( std::begin( address_raw ), std::end( address_raw ), std::begin( address ) );
  const short port = hint_ip_v6->sin6_port;

  return IP_V6( address, port );

}
