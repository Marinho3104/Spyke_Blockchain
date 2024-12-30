
#include "connection.h"
#include "node.h"
#include <csignal>
#include <cstdio>
#include <ios>
#include <iostream>
#include <ostream>
#include <semaphore.h>
#include <unistd.h>
#include "node_client.h"


template < typename SERVER_IP_TYPE >
const bool spyke::client::node::setup( spyke::node::Node_Settings< SERVER_IP_TYPE >& settings ) {

  std::cout << "Setup " << std::endl;
  new( & spyke::client::node::node_instance< SERVER_IP_TYPE > ) spyke::node::Node< SERVER_IP_TYPE >( settings );
  if( ! spyke::client::node::node_instance< SERVER_IP_TYPE >.is_valid() ) return 0;

  if( ! set_signal_mask() ) return 0;

  void (*prevHandler)(int) = signal( SIGINT, SIGINT_handler< SERVER_IP_TYPE > );
  if( prevHandler == SIG_ERR ) return 0;

  return 1;

}

template < typename SERVER_IP_TYPE >
void spyke::client::node::start() {

  std::cout << "Starting" << std::endl;

  spyke::client::node::node_instance< SERVER_IP_TYPE >.start();

  input_loop< SERVER_IP_TYPE >();

}

template < typename SERVER_IP_TYPE >
void spyke::client::node::input_loop() {

  std::string input;

  while( 1 ) {

    std::cout << " -> ";

    std::getline( std::cin, input );

    if( std::cin.fail() ) break;

  }

}

const bool spyke::client::node::set_signal_mask() {

  sigset_t signal_mask; sigfillset( &signal_mask ); sigdelset( &signal_mask, SIGINT );
  int sigprocmask_sts = sigprocmask( SIG_SETMASK, &signal_mask, NULL );

  return sigprocmask_sts == 0;

}

template < typename SERVER_IP_TYPE >
void spyke::client::node::SIGINT_handler( int ) {

  spyke::client::node::node_instance< SERVER_IP_TYPE >.stop();

  std::cin.setstate( std::ios_base::failbit ); std::cout << "Press Enter to exit ... " << std::flush;

}


/** NODE CONNECTIONS MANAGER **/

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::signal_handler( int ) {}

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::set_signal_mask() {

  sigset_t signal_mask; sigfillset( &signal_mask ); sigdelset( &signal_mask, SIGUSR1 );
  int sigprocmask_sts = sigprocmask( SIG_SETMASK, &signal_mask, NULL );
  return sigprocmask_sts == 0;

}



template const bool spyke::client::node::setup( spyke::node::Node_Settings< communication::connection::IP_V4 > & );
template const bool spyke::client::node::setup( spyke::node::Node_Settings< communication::connection::IP_V6 > & );
template void spyke::client::node::input_loop< spyke::communication::connection::IP_V4 >();
template void spyke::client::node::input_loop< spyke::communication::connection::IP_V6 >();
template void spyke::client::node::start< spyke::communication::connection::IP_V4 >();
template void spyke::client::node::start< spyke::communication::connection::IP_V6 >();

template const bool spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V4 >::set_signal_mask();
template const bool spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V6 >::set_signal_mask();
template void spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V4 >::signal_handler( int );
template void spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V6 >::signal_handler( int );
