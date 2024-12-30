
#include "connection.h"
#include "ip.h"
#include "packet.h"
#include "socket_helper.h"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <future>
#include <iostream>
#include <memory>

struct Get_Server_Client_Connections_Return {
  
  spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_ip_v4, server_client_connection_ip_v4, client_connection_ip_v4;
  spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_ip_v6, server_client_connection_ip_v6, client_connection_ip_v6;

};

template < typename IP_TYPE >
spyke::communication::connection::Connection< IP_TYPE > connect_with_server( const IP_TYPE& server_ip ) { return spyke::communication::connection::Connection< IP_TYPE >::connect_to( server_ip ); }

Get_Server_Client_Connections_Return get_server_client_connections( short& port ) {

  char ip_v6_addr[] = "::1";

  spyke::communication::connection::IP_V4 ip_v4 = spyke::communication::connection::IP_V4( 0x7F000001, port ++ );
  spyke::communication::connection::IP_V6 ip_v6 = spyke::communication::connection::IP_V6::from_hex( ip_v6_addr, port ++ );

  spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_ip_v4 =
    std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 >::create_server( ip_v4 ) );

  spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_ip_v6 =
    std::move( spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 >::create_server( ip_v6 ) );

  assert( server_ip_v4.is_connected() ); assert( server_ip_v6.is_connected() ); // true

  std::future< spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > > future_client_ip_v4 =
    std::async( std::launch::async, connect_with_server< spyke::communication::connection::IP_V4 >, ip_v4 );

  std::future< spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > > future_client_ip_v6 =
    std::async( std::launch::async, connect_with_server< spyke::communication::connection::IP_V6 >, ip_v6 );

  spyke::communication::connection::Accept_Connection_Request_Return accept_connection_request_return_ip_v4 =
    spyke::communication::connection::accept_connection_request( server_ip_v4.get_socket_id() );
  spyke::communication::connection::Accept_Connection_Request_Return accept_connection_request_return_ip_v6 =
    spyke::communication::connection::accept_connection_request( server_ip_v6.get_socket_id() );
  assert( ! ( accept_connection_request_return_ip_v4.socket_id == -1 ) ); assert( ! ( accept_connection_request_return_ip_v6.socket_id == -1 ) ); // false

  spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_client_connection_ip_v4 =
    spyke::communication::connection::convert_connection< spyke::communication::connection::IP_V4 >( accept_connection_request_return_ip_v4 );
  spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_client_connection_ip_v6 =
    spyke::communication::connection::convert_connection< spyke::communication::connection::IP_V6 >( accept_connection_request_return_ip_v6 );
  assert( server_client_connection_ip_v4.is_connected() ); assert( server_client_connection_ip_v6.is_connected() ); // true

  spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > client_connection_ip_v4 = std::move( future_client_ip_v4.get() );
  spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > client_connection_ip_v6 = std::move( future_client_ip_v6.get() );

  assert( client_connection_ip_v4.is_connected() ); assert( client_connection_ip_v6.is_connected() ); // true

  return {

    std::move( server_ip_v4 ), std::move( server_client_connection_ip_v4 ), std::move( client_connection_ip_v4 ),
    std::move( server_ip_v6 ), std::move( server_client_connection_ip_v6 ), std::move( client_connection_ip_v6 )

  };

}

void arbitrary_communication_check() {

 //  std::cout << "\n*** ARBITRARY COMMUNICATION TEST ***\n" << std::endl;
 //
 //  short port = 8500;
 //  
 //  {
 //  
 //    std::cout << "\tSending arbitrary data check: ";
 //
 //    Get_Server_Client_Connections_Return get_server_client_connections_return = get_server_client_connections( port );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_client_connection_ip_v4 = 
 //      std::move( get_server_client_connections_return.server_client_connection_ip_v4 );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_client_connection_ip_v6 = 
 //      std::move( get_server_client_connections_return.server_client_connection_ip_v6 );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > client_connection_ip_v4 = 
 //      std::move( get_server_client_connections_return.client_connection_ip_v4 );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > client_connection_ip_v6 = 
 //      std::move( get_server_client_connections_return.client_connection_ip_v6 );
 //
 //    unsigned char protocol_id = 0; size_t data_size = sizeof( "Testing with random data!!" );
 //    unsigned char data[ data_size ]; std::memcpy( data, "Testing with random data!!", data_size );
 //
 //    spyke::communication::communication_protocols::Packet packet = 
 //      spyke::communication::communication_protocols::Packet( protocol_id, data, data_size );
 //    assert( packet.is_valid() ); // true
 //    
 //    const bool sts_send_packet_client_ip_v4 = packet.send( client_connection_ip_v4.get_socket_id() );
 //    assert( sts_send_packet_client_ip_v4 ); // true
 //
 //    const bool sts_send_packet_client_ip_v6 = packet.send( client_connection_ip_v6.get_socket_id() );
 //    assert( sts_send_packet_client_ip_v6 ); // true
 //     
 //    const bool sts_send_packet_server_client_ip_v4 = packet.send( server_client_connection_ip_v4.get_socket_id() );
 //    assert( sts_send_packet_server_client_ip_v4 ); // true
 //
 //    const bool sts_send_packet_server_client_ip_v6 = packet.send( server_client_connection_ip_v6.get_socket_id() );
 //    assert( sts_send_packet_server_client_ip_v6 ); // true
 //
 //    const spyke::communication::communication_protocols::Packet client_packet_received_ip_v4 = 
 //      spyke::communication::communication_protocols::Packet::receive_packet( client_connection_ip_v4.get_socket_id() );
 //    const spyke::communication::communication_protocols::Packet client_packet_received_ip_v6 = 
 //      spyke::communication::communication_protocols::Packet::receive_packet( client_connection_ip_v6.get_socket_id() );
 //    assert( client_packet_received_ip_v4.is_valid() ); assert( client_packet_received_ip_v6.is_valid() ); // true
 //
 //    const spyke::communication::communication_protocols::Packet server_client_packet_received_ip_v4 = 
 //      spyke::communication::communication_protocols::Packet::receive_packet( server_client_connection_ip_v4.get_socket_id() );
 //    const spyke::communication::communication_protocols::Packet server_client_packet_received_ip_v6 = 
 //      spyke::communication::communication_protocols::Packet::receive_packet( server_client_connection_ip_v6.get_socket_id() );
 //    assert( server_client_packet_received_ip_v4.is_valid() ); assert( server_client_packet_received_ip_v6.is_valid() ); // true
 // 
 //    std::unique_ptr< char[] > original_packet_serialized = std::move( packet.serialize() );
 //    size_t original_packet_serialized_size = packet.get_serialization_size();
 //
 //    std::unique_ptr< char[] > client_packet_received_ip_v4_serialized = std::move( client_packet_received_ip_v4.serialize() );
 //    size_t client_packet_received_ip_v4_serialized_size = client_packet_received_ip_v4.get_serialization_size();
 //
 //    std::unique_ptr< char[] > client_packet_received_ip_v6_serialized = std::move( client_packet_received_ip_v6.serialize() );
 //    size_t client_packet_received_ip_v6_serialized_size = client_packet_received_ip_v6.get_serialization_size();
 //
 //    std::unique_ptr< char[] > server_client_packet_received_ip_v4_serialized = std::move( server_client_packet_received_ip_v4.serialize() );
 //    size_t server_client_packet_received_ip_v4_serialized_size = server_client_packet_received_ip_v4.get_serialization_size();
 //
 //    std::unique_ptr< char[] > server_client_packet_received_ip_v6_serialized = std::move( server_client_packet_received_ip_v6.serialize() );
 //    size_t server_client_packet_received_ip_v6_serialized_size = server_client_packet_received_ip_v6.get_serialization_size();
 //
 //    assert( ! std::memcmp( original_packet_serialized.get(), server_client_packet_received_ip_v4_serialized.get(), original_packet_serialized_size ) ); // true
 //    assert( ! std::memcmp( original_packet_serialized.get(), server_client_packet_received_ip_v6_serialized.get(), original_packet_serialized_size ) ); // true
 //    assert( ! std::memcmp( original_packet_serialized.get(), client_packet_received_ip_v4_serialized.get(), original_packet_serialized_size ) ); // true
 //    assert( ! std::memcmp( original_packet_serialized.get(), client_packet_received_ip_v6_serialized.get(), original_packet_serialized_size ) ); // true
 //  
 //    std::cout << "Pass" << std::endl;
 //
 //  }
 //
 //  {
 //  
 //    std::cout << "\tSending arbitrary data, when one peer is closed/invalid check: ";
 //
 //    Get_Server_Client_Connections_Return get_server_client_connections_return = get_server_client_connections( port );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > server_client_connection_ip_v4 = 
 //      std::move( get_server_client_connections_return.server_client_connection_ip_v4 );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > server_client_connection_ip_v6 = 
 //      std::move( get_server_client_connections_return.server_client_connection_ip_v6 );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > client_connection_ip_v4 = 
 //      std::move( get_server_client_connections_return.client_connection_ip_v4 );
 //    spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > client_connection_ip_v6 = 
 //      std::move( get_server_client_connections_return.client_connection_ip_v6 );
 //
 //    unsigned char protocol_id = 0; size_t data_size = sizeof( "Testing with random data!!" );
 //    unsigned char data[ data_size ]; std::memcpy( data, "Testing with random data!!", data_size );
 //
 //    spyke::communication::communication_protocols::Packet packet = 
 //      spyke::communication::communication_protocols::Packet( protocol_id, data, data_size );
 //    assert( packet.is_valid() ); // true
 //    
 //    // Make server client connection invalid
 //    server_client_connection_ip_v4.~Connection();
 //    
 //    const bool sts_send_packet_server_client_ip_v4 = packet.send( server_client_connection_ip_v4.get_socket_id() );
 //    assert( ! sts_send_packet_server_client_ip_v4 ); // false 
 //    
 //    const spyke::communication::communication_protocols::Packet server_client_packet_received_ip_v4 =
 //      spyke::communication::communication_protocols::Packet::receive_packet( server_client_connection_ip_v4.get_socket_id() );
 //    assert( ! server_client_packet_received_ip_v4.is_valid() ); // false
 //
 //    // Cause the connection peer were closed this should also return an error
 //    const spyke::communication::communication_protocols::Packet client_packet_received_ip_v4 =
 //      spyke::communication::communication_protocols::Packet::receive_packet( client_connection_ip_v4.get_socket_id() );
 //    assert( ! client_packet_received_ip_v4.is_valid() ); // false
 //
 //    // Send to valid client and then close the connection
 //    const bool sts_send_packet_client_ip_v6 = packet.send( client_connection_ip_v6.get_socket_id() );
 //    assert( sts_send_packet_client_ip_v6 ); // true
 //
 //    // Close the connection
 //    client_connection_ip_v6.~Connection();
 //
 //    // Should not be able to get the information also, maybe some to be fixed FIXME
 //    const spyke::communication::communication_protocols::Packet client_packet_received_ip_v6 =
 //      spyke::communication::communication_protocols::Packet::receive_packet( client_connection_ip_v6.get_socket_id() );
 //    assert( ! client_packet_received_ip_v6.is_valid() ); // false
 //
 //    std::cout << "Pass" << std::endl;
 //
 //  }


}


// Forward
spyke::communication::connection::Connection< spyke::communication::connection::IP_V4 > connect_with_server( const spyke::communication::connection::IP_V4& );
spyke::communication::connection::Connection< spyke::communication::connection::IP_V6 > connect_with_server( const spyke::communication::connection::IP_V6& );
