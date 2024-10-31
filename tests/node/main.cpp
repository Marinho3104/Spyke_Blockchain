
#include "connection.h"
#include "ip.h"
#include "socket_helper.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>

void ip_check();
void connection_check();
void accept_and_hint_conversion_check( const int&, const int&, spyke::communication::connection::IP_V4, spyke::communication::connection::IP_V6 );

int main (int argc, char *argv[]) {

  ip_check();
  connection_check();

  return 0;
}

void connection_check() {

  char ip_v6_addr[] = "::1"; short port = 8005;

  {
    
    std::cout << "Connection invalid check: ";

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection1;
    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection2;

    assert( ! connection1.is_connected() ); // false
    assert( ! connection2.is_connected() ); // false

    std::cout << "Done" << std::endl;

  }

  {

    std::cout << "Server creation check: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( connection_ip_v4.is_connected() ); // true
    assert( connection_ip_v6.is_connected() ); // true
    
    std::cout << "Done" << std::endl;

  }

  {

    std::cout << "Server creation check: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( connection_ip_v4.is_connected() ); // true
    assert( connection_ip_v6.is_connected() ); // true
    
    std::cout << "Done" << std::endl;

  }

  {

    std::cout << "Client connection check: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( server_ip_v4.is_connected() ); // true
    assert( server_ip_v6.is_connected() ); // true
    
    std::thread( &spyke::communication::socket_helper::accept_connection_request, server_ip_v4.get_socket_id() ).detach();

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > client_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::connect_to( ip_v4 ) );
    assert( client_ip_v4.is_connected() ); // true

    std::thread( &spyke::communication::socket_helper::accept_connection_request, server_ip_v6.get_socket_id() ).detach();

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > client_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::connect_to( ip_v6 ) );
    assert( client_ip_v6.is_connected() ); // true

    std::cout << "Done" << std::endl;    

  }

  {

    std::cout << "Accepted connections ip conversion check: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( server_ip_v4.is_connected() ); // true
    assert( server_ip_v6.is_connected() ); // true
    
    std::thread( &accept_and_hint_conversion_check, server_ip_v4.get_socket_id() , server_ip_v6.get_socket_id(), ip_v4, ip_v6 ).detach();

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > client_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::connect_to( ip_v4 ) );
    assert( client_ip_v4.is_connected() ); // true

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > client_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::connect_to( ip_v6 ) );
    assert( client_ip_v6.is_connected() ); // true

    std::cout << "Done" << std::endl;    

  }

}

void accept_and_hint_conversion_check( const int& server_ip_v4, const int& server_ip_v6, spyke::communication::connection::IP_V4 client_ip_v4, spyke::communication::connection::IP_V6 client_ip_v6 ) {

  spyke::communication::socket_helper::Accept_Connection_Request_Return accept_connection_request_ip_v4 = spyke::communication::socket_helper::accept_connection_request( server_ip_v4 );
  spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4::from_hint( accept_connection_request_ip_v4.hint );
  // std::cout << ip_v4.get_address() << " " << client_ip_v4.get_address() << std::endl;
  assert( client_ip_v4.get_address() == ip_v4.get_address() );

  spyke::communication::socket_helper::Accept_Connection_Request_Return accept_connection_request_ip_v6 = spyke::communication::socket_helper::accept_connection_request( server_ip_v6 );
  spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hint( accept_connection_request_ip_v6.hint );
  // for( int _ = 0; _ < 16; _ ++ ) std::cout << ( int ) ip_v6.get_address()[ _ ] << " " ; std::cout << std::endl;
  // for( int _ = 0; _ < 16; _ ++ ) std::cout << ( int ) client_ip_v6.get_address()[ _ ] << " "; std::cout << std::endl;
  assert( ::std::memcmp( client_ip_v6.get_address(), ip_v6.get_address(), 16 ) == 0 );

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

/*
* 
  char ip_v6_addr[] = "::";


*/
