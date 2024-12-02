
#include "ip.h"
#include "node.h"
#include "node_client.h"

void node_single_tests() {

  spyke::node::Node_Settings< spyke::communication::connection::IP_V4 > node_settings = {

    .node_connections_manager_settings = {

      .temporary_connections_count = 10,
      .stable_connections_count = 10,
      .server_ip = spyke::communication::connection::IP_V4( 0x7F000001, 8010 ),
      .priority_queue_capacity = 5,
      .priority_queue_workers_count = 3,

    }

  };

  spyke::client::node::setup( node_settings ); spyke::client::node::start< spyke::communication::connection::IP_V4 >();

}

void node_check() {

  node_single_tests(); 

}


