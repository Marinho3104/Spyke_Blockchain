
#include "node.h"
#include "ip.h"
#include "node_connections_manager.h"
#include <bits/types/sigset_t.h>
#include <csignal>
#include <iostream>
#include <memory>
#include <ostream>
#include <unistd.h>


template < typename SERVER_IP_TYPE >
spyke::node::Node_Settings< SERVER_IP_TYPE >::Node_Settings() : node_connections_manager_settings{} {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Settings< SERVER_IP_TYPE >::Node_Settings( Node_Settings< SERVER_IP_TYPE >&& other ) : node_connections_manager_settings( std::move( other.node_connections_manager_settings ) ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Settings< SERVER_IP_TYPE >::Node_Settings( Node_Connections_Manager_Settings< SERVER_IP_TYPE >& node_connections_manager_settings ) 
  : node_connections_manager_settings( std::move( node_connections_manager_settings ) ) {}


template < typename SERVER_IP_TYPE >
spyke::node::Node< SERVER_IP_TYPE >::Node( Node&& other ) : settings( std::move( other.settings ) ), connections_manager( other.connections_manager ), status( other.status ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node< SERVER_IP_TYPE >::Node() : settings{}, status( spyke::node::NODE_STATUS::INVALID ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node< SERVER_IP_TYPE >::Node( node::Node_Settings< SERVER_IP_TYPE >& node_settings ) 
  : connections_manager( std::make_shared< Node_Connections_Manager< SERVER_IP_TYPE > >( std::move( Node_Connections_Manager< SERVER_IP_TYPE >::create( settings.node_connections_manager_settings ) ) ) ),
    settings( std::move( node_settings ) ), status( spyke::node::NODE_STATUS::VALID )
      { if( ! connections_manager || ! connections_manager.get()->is_valid() ) status = spyke::node::NODE_STATUS::INVALID; }

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node< SERVER_IP_TYPE >::is_valid() const { return status == spyke::node::NODE_STATUS::VALID; }

template < typename SERVER_IP_TYPE >
void spyke::node::Node< SERVER_IP_TYPE >::start() {

  if( ! is_valid() || ! connections_manager || ! connections_manager.get()->is_valid() ) return;

  connections_manager.get()->start();

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node< SERVER_IP_TYPE >::stop() {

  if( ! is_valid() || ! connections_manager || ! connections_manager.get()->is_valid() ) return;

  std::cout << "\n\n*** TERMINATING NODE ***\n" << std::endl;

  connections_manager.get()->stop();

}

template < typename SERVER_IP_TYPE >
std::shared_ptr< spyke::node::Node_Connections_Manager< SERVER_IP_TYPE > > spyke::node::Node< SERVER_IP_TYPE >::get_connections_manager() { return connections_manager; }



template class spyke::node::Node_Settings< spyke::communication::connection::IP_V4 >;
template class spyke::node::Node_Settings< spyke::communication::connection::IP_V6 >;
template class spyke::node::Node< spyke::communication::connection::IP_V4 >;
template class spyke::node::Node< spyke::communication::connection::IP_V6 >;
