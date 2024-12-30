
#include "ip.h"
#include "node.h"
#include "node_client.h"
#include "node_connections_manager.h"

void node_single_tests() {

  std::unique_ptr< spyke::communication::connection::IP_V4[] > initial_ip_v4;
  std::unique_ptr< spyke::communication::connection::IP_V6[] > initial_ip_v6;

  spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V4 > node_connections_manager_settings = 
    spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V4 >( 10, 10, spyke::communication::connection::IP_V4( 0x7F000001, 8010 ), 5, 3, initial_ip_v4, initial_ip_v6, 0, 0 );
  
  spyke::node::Node_Settings< spyke::communication::connection::IP_V4 > node_settings = 
    spyke::node::Node_Settings< spyke::communication::connection::IP_V4 >( node_connections_manager_settings );

  spyke::client::node::setup( node_settings ); spyke::client::node::start< spyke::communication::connection::IP_V4 >();

}

void node_check() {

  node_single_tests(); 

}
