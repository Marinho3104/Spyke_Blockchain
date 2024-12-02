
#include "node.h"
#include "ip.h"
#include "node_connections_manager.h"
#include <bits/types/sigset_t.h>
#include <csignal>
#include <iostream>
#include <ostream>
#include <unistd.h>


template < typename SERVER_IP_TYPE >
spyke::node::Node< SERVER_IP_TYPE >::Node( Node&& other ) : settings( other.settings ), status( other.status ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node< SERVER_IP_TYPE >::Node() : settings{}, status( spyke::node::NODE_STATUS::INVALID ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node< SERVER_IP_TYPE >::Node( const node::Node_Settings< SERVER_IP_TYPE >& node_settings ) 
  : connections_manager( Node_Connections_Manager< SERVER_IP_TYPE >::create( settings.node_connections_manager_settings ) ),
    settings( node_settings ), status( spyke::node::NODE_STATUS::VALID )
      { if( ! connections_manager.is_valid() ) status = spyke::node::NODE_STATUS::INVALID; }

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node< SERVER_IP_TYPE >::is_valid() const { return status == spyke::node::NODE_STATUS::VALID; }

template < typename SERVER_IP_TYPE >
void spyke::node::Node< SERVER_IP_TYPE >::start() {

  if( ! is_valid() ) return;

  connections_manager.start();

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node< SERVER_IP_TYPE >::stop() {

  connections_manager.stop();

}

template class spyke::node::Node_Settings< spyke::communication::connection::IP_V4 >;
template class spyke::node::Node_Settings< spyke::communication::connection::IP_V6 >;
template class spyke::node::Node< spyke::communication::connection::IP_V4 >;
template class spyke::node::Node< spyke::communication::connection::IP_V6 >;
