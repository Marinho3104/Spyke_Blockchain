
#include <cassert>
#include <climits>
#include <cstring>
#include <memory>
#include <unistd.h>
#include "./communication_connection_tests.cpp"
#include "./communication_communication_protocols_tests.cpp"
#include "./priority_queue_tests.cpp"
#include "./node_tests.cpp"
#include <limits.h>

void start_node_8080();
void start_node_8081();


int main (int argc, char *argv[]) {

  // ip_check();
  // connection_check();
  // arbitrary_communication_check();
  // priority_queue_check();
  // node_check();
  
  #ifdef NODE_8080

    start_node_8080();

  #else

    start_node_8081();

  #endif

  return 0;

}

void start_node_8080() {

  std::cout << "Starting 8080" << std::endl;

  std::unique_ptr< spyke::communication::connection::IP_V4[] > initial_ip_v4;
  std::unique_ptr< spyke::communication::connection::IP_V6[] > initial_ip_v6;

  spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V4 > node_connections_manager_settings = 
    spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V4 >( 10, 10, spyke::communication::connection::IP_V4( 0x7F000001, 8080 ), 5, 3, initial_ip_v4, initial_ip_v6, 0, 0 );
  
  spyke::node::Node_Settings< spyke::communication::connection::IP_V4 > node_settings = 
    spyke::node::Node_Settings< spyke::communication::connection::IP_V4 >( node_connections_manager_settings );

  if( ! spyke::client::node::setup( node_settings ) ) return;
  spyke::client::node::start< spyke::communication::connection::IP_V4 >();

}

void start_node_8081() {

  std::cout << "Starting 8081" << std::endl;

  std::unique_ptr< spyke::communication::connection::IP_V4[] > initial_ip_v4 = std::make_unique< spyke::communication::connection::IP_V4[] >( 1 );
  std::unique_ptr< spyke::communication::connection::IP_V6[] > initial_ip_v6;

  new( initial_ip_v4.get() ) spyke::communication::connection::IP_V4( 0x7F000001, 8080 );

  spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V4 > node_connections_manager_settings = 
    spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V4 >( 10, 10, spyke::communication::connection::IP_V4( 0x7F000001, 8081 ), 5, 3, initial_ip_v4, initial_ip_v6, 1, 0 );
  
  spyke::node::Node_Settings< spyke::communication::connection::IP_V4 > node_settings = 
    spyke::node::Node_Settings< spyke::communication::connection::IP_V4 >( node_connections_manager_settings );

  if( ! spyke::client::node::setup( node_settings ) ) return;
  spyke::client::node::start< spyke::communication::connection::IP_V4 >();

}
