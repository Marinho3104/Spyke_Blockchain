
#include "connection.h"
#include "ip.h"
#include <cassert>
#include <iostream>

void ip_check();
void connection_check();

int main (int argc, char *argv[]) {

  ip_check();
  connection_check();

  return 0;
}

void connection_check() {

  {
    
    std::cout << "Connection invalid check: ";

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection1;
    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection2;

    assert( ! connection1.is_connected() ); // false
    assert( ! connection2.is_connected() ); // false

    std::cout << "Done" << std::endl;

  }

  {

    std::cout << "Server creation: ";

    char ip_v6_addr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };


    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, 8000 );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6( ip_v6_addr, 8000 );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( connection_ip_v4.is_connected() ); // true
    assert( connection_ip_v6.is_connected() ); // true
    
    std::cout << "Done" << std::endl;

  }

  {

    std::cout << "Client connection: ";

    char ip_v6_addr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };


    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, 8000 );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6( ip_v6_addr, 8000 );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( connection_ip_v4.is_connected() ); // true
    assert( connection_ip_v6.is_connected() ); // true
    

    std::cout << "Done" << std::endl;    

  }

}

void ip_check() {

  
  {

    std::cout << "IP_V4 check: ";

    spyke::communication::connection::IP_V4 ip1 = spyke::communication::connection::IP_V4( 0x7F000001, 8000 ), ip2;

    assert( ip1.is_valid() ); // true
    assert( ! ip2.is_valid() ); // false
    assert( ip1 == ip1 ); // true
    assert( ip1 != ip2 ); // true
    
    std::cout << "Done" << std::endl;
    
  }

  {

    std::cout << "IP_V6 check: ";

    spyke::communication::connection::IP_V6 ip1 = spyke::communication::connection::IP_V6( "5be8:dde9:7f0b:d5a7:bd01:b3be:9c69:573b", 8000 ), ip2;

    assert( ip1.is_valid() ); // true
    assert( ! ip2.is_valid() ); // false 
    assert( ip1 == ip1 ); // true
    assert( ip1 != ip2 ); // true

    std::cout << "Done" << std::endl;

  }

}
