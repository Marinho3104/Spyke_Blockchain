
#include "ip.h"
#include <cassert>


void ip_check();

int main (int argc, char *argv[]) {

  ip_check();

  return 0;
}

void ip_check() {

  
  {

    spyke::communication::connection::IP_V4 ip1 = spyke::communication::connection::IP_V4( 0x7F000001, 8000 ), ip2;

    assert( ip1.is_valid() ); // true
    assert( ! ip2.is_valid() ); // false
    assert( ip1 == ip1 ); // true
    assert( ip1 != ip2 ); // true
    
  }

  {

    spyke::communication::connection::IP_V6 ip1 = spyke::communication::connection::IP_V6( "5be8:dde9:7f0b:d5a7:bd01:b3be:9c69:573b", 8000 ), ip2;

    assert( ip1.is_valid() ); // true
    assert( ! ip2.is_valid() ); // false 
    assert( ip1 == ip1 ); // true
    assert( ip1 != ip2 ); // true

  }

}
