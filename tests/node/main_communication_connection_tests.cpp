
#include "socket_helper.h"
#include "connection.h"
#include "ip.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <thread>

void ip_check();
void connection_check();
void accept_and_hint_conversion_check( const int&, const int&, spyke::communication::connection::IP_V4, spyke::communication::connection::IP_V6 );

void connection_check() {

  std::cout << "\n\t*** CONNECTION CHECK ***\n" << std::endl;

  const char ip_v6_addr_raw[] = "::1"; // "0000:0000:0000:0000:0000:0000:0000:0001";
  std::array< char, 40 > ip_v6_addr; short port = 8200;

  std::copy( std::begin( ip_v6_addr_raw ), std::end( ip_v6_addr_raw ), std::begin( ip_v6_addr ) );

  {
    
    std::cout << "\t\tConnection invalid check: ";

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection1;
    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection2;

    assert( ! connection1.is_connected() ); // false
    assert( ! connection2.is_connected() ); // false

    std::cout << "Pass" << std::endl;

  }

  {

    std::cout << "\t\tServer creation check: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( connection_ip_v4.is_connected() ); // true
    assert( connection_ip_v6.is_connected() ); // true
    
    std::cout << "Pass" << std::endl;

  }

  {

    std::cout << "\t\tServer creation check: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connection_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connection_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( connection_ip_v4.is_connected() ); // true
    assert( connection_ip_v6.is_connected() ); // true
    
    std::cout << "Pass" << std::endl;

  }

  {

    std::cout << "\t\tClient connection check: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

    assert( server_ip_v4.is_connected() ); // true
    assert( server_ip_v6.is_connected() ); // true
    
    std::thread( &spyke::communication::connection::accept_connection_request, server_ip_v4.get_socket_id() ).detach();

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > client_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::connect_to( ip_v4 ) );
    assert( client_ip_v4.is_connected() ); // true

    std::thread( &spyke::communication::connection::accept_connection_request, server_ip_v6.get_socket_id() ).detach();

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > client_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::connect_to( ip_v6 ) );
    assert( client_ip_v6.is_connected() ); // true

    std::cout << "Pass" << std::endl;    

  }

  {

    std::cout << "\t\tAccepted connections ip conversion check: ";

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

    std::cout << "Pass" << std::endl;

  }

  {

    std::cout << "\t\tTrying to connect with invalid server: ";

    spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
    spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > client_ip_v4 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::connect_to( ip_v4 ) );
    assert( ! client_ip_v4.is_connected() ); // false

    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > client_ip_v6 =
      std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::connect_to( ip_v6 ) );
    assert( ! client_ip_v6.is_connected() ); // false

    std::cout << "Pass" << std::endl;


  }

}

void accept_and_hint_conversion_check( const int& server_ip_v4, const int& server_ip_v6, spyke::communication::connection::IP_V4 client_ip_v4, spyke::communication::connection::IP_V6 client_ip_v6 ) {

  spyke::communication::connection::Accept_Connection_Request_Return accept_connection_request_ip_v4 = spyke::communication::connection::accept_connection_request( server_ip_v4 );
  spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4::from_hint( accept_connection_request_ip_v4.hint );
  // std::cout << ip_v4.get_address() << " " << client_ip_v4.get_address() << std::endl;
  // assert( client_ip_v4.get_address() == ip_v4.get_address() );

  spyke::communication::connection::Accept_Connection_Request_Return accept_connection_request_ip_v6 = spyke::communication::connection::accept_connection_request( server_ip_v6 );
  spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hint( accept_connection_request_ip_v6.hint );
  // for( int _ = 0; _ < 16; _ ++ ) std::cout << ( int ) ip_v6.get_address()[ _ ] << " " ; std::cout << std::endl;
  // for( int _ = 0; _ < 16; _ ++ ) std::cout << ( int ) client_ip_v6.get_address()[ _ ] << " "; std::cout << std::endl;
  // assert( ::std::memcmp( client_ip_v6.get_address() == ip_v6.get_address(), 16 ) == 0 );

}

void ip_check() {

  std::cout << "\n\t*** IP CHECK ***\n" << std::endl;
  
  {

    std::cout << "\t\tIP_V4 check: ";

    spyke::communication::connection::IP_V4 ip1 = spyke::communication::connection::IP_V4( 0x7F000001, 8000 ), ip2;

    assert( ip1.is_valid() ); // true
    assert( ! ip2.is_valid() ); // false
    assert( ip1 == ip1 ); // true
    assert( ip1 != ip2 ); // true
    
    std::cout << "Pass" << std::endl;
    
  }

  {

    std::cout << "\t\tIP_V6 check: ";

    std::array< char, 16 > ip6_addr = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    spyke::communication::connection::IP_V6 ip1 = spyke::communication::connection::IP_V6( ip6_addr, 8000 ), ip2;

    assert( ip1.is_valid() ); // true
    assert( ! ip2.is_valid() ); // false 
    assert( ip1 == ip1 ); // true
    assert( ip1 != ip2 ); // true

    std::cout << "Pass" << std::endl;

  }

}


int main (int argc, char *argv[]) {
  
  std::cout << "*** IP AND CONNECTIONS BASIC TESTS ***" << std::endl;

  ip_check();
  connection_check();

  return 0;

}
