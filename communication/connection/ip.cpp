
#include "ip.h"
#include <cstring>


spyke::communication::connection::IP_V4::IP_V4() : address( 0 ), port( 0 ) {}

spyke::communication::connection::IP_V4::IP_V4( const int& address, const short& port ) : address( address ), port( port ) {}

const bool spyke::communication::connection::IP_V4::is_valid() const { return port; }

const bool spyke::communication::connection::IP_V4::operator==( const IP_V4& other ) const { return address == other.address && port == other.port; }

const bool spyke::communication::connection::IP_V4::operator!=( const IP_V4& other ) const { return ! operator==( other ); }


spyke::communication::connection::IP_V6::IP_V6() : port( 0 ) {}

spyke::communication::connection::IP_V6::IP_V6( const char address[ 16 ], const short& port ) : port( port ) { ::std::memcpy( this->address, address, sizeof( this->address ) ); }

const bool spyke::communication::connection::IP_V6::is_valid() const { return port; }

const bool spyke::communication::connection::IP_V6::operator==( const IP_V6& other ) const { return ::std::memcmp( address, other.address, sizeof( address ) ) == 0 && port == other.port; }

const bool spyke::communication::connection::IP_V6::operator!=( const IP_V6& other ) const { return ! operator==( other ); }
