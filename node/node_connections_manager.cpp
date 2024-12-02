
#include "node_connections_manager.h"
#include "connection.h"
#include "ip.h"
#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/select.h>
#include <utility>
#include <type_traits>


template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager( spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >&& other ) 
  : ip_v4_temporary_connections( std::move( other.ip_v4_temporary_connections ) ), ip_v4_stable_connections( std::move( other.ip_v4_stable_connections ) ), 
      ip_v6_temporary_connections( std::move( other.ip_v6_temporary_connections ) ), ip_v6_stable_connections( std::move( other.ip_v6_stable_connections ) ),
        temporary_connections_current_count( other.temporary_connections_current_count ), stable_connections_current_count( other.stable_connections_current_count ),
          temporary_connections_locker( std::move( other.temporary_connections_locker ) ), stable_connections_locker( std::move( other.stable_connections_locker ) ),
            server_connection( std::move( other.server_connection ) ), settings( other.settings ), status( other.status ), connections_fd_set( std::move( other.connections_fd_set ) ),
              priority_queue( std::move( other.priority_queue ) ), priority_queue_workers( std::move( other.priority_queue_workers ) ), communication_loop_done( std::move( other.communication_loop_done ) ),
                locker( std::move( other.locker ) ), communication_loop_thread( std::thread( &Node_Connections_Manager::communication_loop, this ) ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager() : status( spyke::node::STATUS::INVALID ), settings{}, priority_queue( 0 ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager( const Node_Connections_Manager_Settings< SERVER_IP_TYPE >& settings, communication::connection::Connection< SERVER_IP_TYPE >& server_connection )
  : ip_v4_temporary_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V4 >[] >( settings.temporary_connections_count ) ),
    ip_v4_stable_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V4 >[] >( settings.stable_connections_count ) ),
      ip_v6_temporary_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V6 >[] >( settings.temporary_connections_count ) ),
        ip_v6_stable_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V6 >[] >( settings.stable_connections_count ) ),
            temporary_connections_current_count( 0 ), stable_connections_current_count( 0 ), server_connection( std::move( server_connection ) ), settings( settings ),
              status( spyke::node::STATUS::VALID ), priority_queue( settings.priority_queue_capacity ), communication_loop_thread( std::thread( &Node_Connections_Manager::communication_loop, this ) ),
                priority_queue_workers( std::make_unique< Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >[] >( settings.priority_queue_workers_count ) ) { 

                    sem_init( &temporary_connections_locker, 0, 1 ); sem_init( &stable_connections_locker, 0, 1 ); sem_init( &communication_loop_done, 0, 0 ); sem_init( &locker, 0, 1 );

                    FD_ZERO( &connections_fd_set ); FD_SET( server_connection.get_socket_id(), &connections_fd_set ); initiate_variables(); 

}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager( const Node_Connections_Manager_Settings< SERVER_IP_TYPE >& settings )
  : ip_v4_temporary_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V4 >[] >( settings.temporary_connections_count ) ),
    ip_v4_stable_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V4 >[] >( settings.stable_connections_count ) ),
      ip_v6_temporary_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V6 >[] >( settings.temporary_connections_count ) ),
        ip_v6_stable_connections( std::make_unique< communication::connection::Connection< communication::connection::IP_V6 >[] >( settings.stable_connections_count ) ),
            temporary_connections_current_count( 0 ), stable_connections_current_count( 0 ), server_connection(), settings( settings ), status( spyke::node::STATUS::VALID ), 
              priority_queue_workers( std::make_unique< Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >[] >( settings.priority_queue_workers_count ) ),
                priority_queue( settings.priority_queue_capacity ) { 

                  sem_init( &temporary_connections_locker, 0, 1 ); sem_init( &stable_connections_locker, 0, 1 ); sem_init( &communication_loop_done, 0, 0 ); sem_init( &locker, 0, 1 );

                  FD_ZERO( &connections_fd_set ); initiate_variables();

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::initiate_variables() {

  for( int _ = 0; _ < settings.priority_queue_workers_count; ++_ ) 
      
    new( priority_queue_workers.get() + _ ) Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >( &priority_queue, this );

}

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::is_temporary_connections_full() const { return temporary_connections_current_count == settings.temporary_connections_count; }

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::is_stable_connections_full() const { return stable_connections_current_count == settings.stable_connections_count; }

template < typename SERVER_IP_TYPE >
template < typename CONNECTION_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::add_new_temporary_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& new_connection ) {

  sem_wait( &temporary_connections_locker );

  if( is_temporary_connections_full() ) { sem_post( &temporary_connections_locker ); return 0; }

  communication::connection::Connection< CONNECTION_IP_TYPE >* arr;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V4 >::value ) { arr = ip_v4_temporary_connections.get(); }

  else if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V6 >::value ) { arr = ip_v6_temporary_connections.get(); }

  else return 0;

  for( int _ = 0; _ < settings.temporary_connections_count; _ ++ )

    if( ! arr[ _ ].is_connected() ) new( arr[ _ ] ) communication::connection::Connection< CONNECTION_IP_TYPE >( std::move( new_connection ) );

  sem_post( &temporary_connections_locker );

}

template < typename SERVER_IP_TYPE >
template < typename CONNECTION_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::add_new_stable_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& new_connection ) {

  sem_wait( &stable_connections_locker );

  if( is_stable_connections_full() ) { sem_post( &stable_connections_locker ); return 0; }

  communication::connection::Connection< CONNECTION_IP_TYPE >* arr;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V4 >::value ) { arr = ip_v4_stable_connections.get(); }

  else if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V6 >::value ) { arr = ip_v6_stable_connections.get(); }

  else return 0;

  for( int _ = 0; _ < settings.stable_connections_count; _ ++ )

    if( ! arr[ _ ].is_connected() ) new( arr[ _ ] ) communication::connection::Connection< CONNECTION_IP_TYPE >( std::move( new_connection ) );

  sem_post( &stable_connections_locker );

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::communication_loop() {

  if( ! set_signal_mask() ) return;
 
  void (*prevHandler)(int) = signal( SIGUSR1, signal_handler );
  if( prevHandler == SIG_ERR ) return;

  struct timeval select_timeout;
  select_timeout.tv_sec = 2; select_timeout.tv_usec = 0;

  fd_set connections_fd_set_temp;
  int select_sts;

  while( 1 ) {

    sem_wait( &locker ); connections_fd_set_temp = connections_fd_set; sem_post( &locker );

    select_sts = select( settings.temporary_connections_count + settings.stable_connections_count + 1, &connections_fd_set_temp, NULL, NULL, &select_timeout );
    if( select_sts == 0 ) continue; if( select_sts == -1 ) break;

    std::cout << "Data received" << std::endl;

  }

  sem_post( &communication_loop_done );

}

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::is_valid() const { return status == spyke::node::STATUS::VALID; }

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager::start() { 

  for( int _ = 0; _ < settings.priority_queue_workers_count; ++_ ) {

    priority_queue_workers.get()[ _ ].start_handling();

  }

  communication_loop_thread.detach(); 

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::stop() { 

  std::cout << "*** TERMINATIONG NODE CONNECTIONS MANAGER ***\n" << std::endl;

  std::cout << "\tStopping communication loop: ";
  sem_wait( &locker ); memset( &connections_fd_set, 0xFF, sizeof( connections_fd_set ) ); sem_post( &locker );
  kill( getpid(), SIGUSR1 ); sem_wait( &communication_loop_done ); std::cout << "Done" << std::endl;

  std::cout << "\tStopping Priority Queue Works: ";
  priority_queue.disable_tasks_adding();
  for( int _ = 0; _ < settings.priority_queue_workers_count; ++_ ) sem_wait( &priority_queue_workers.get()[ _ ].get_done() );
  std::cout << "Done" << std::endl;

}

template < typename SERVER_IP_TYPE >
template < typename CONNECTION_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::add_external_connection( communication::connection::Connection< CONNECTION_IP_TYPE > external_connection ) {

  if( ! external_connection.is_connected() ) return 0;



}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE > spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::create( const Node_Connections_Manager_Settings< SERVER_IP_TYPE >& settings ) {

  if( ! settings.server_ip.is_valid() ) return Node_Connections_Manager< SERVER_IP_TYPE >( settings );

  communication::connection::Connection< SERVER_IP_TYPE > server_connection =
    communication::connection::Connection< SERVER_IP_TYPE >::create_server( settings.server_ip );

  if( ! server_connection.is_connected() ) return Node_Connections_Manager();

  return Node_Connections_Manager( settings, server_connection );

}


template class spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V4 >;
template class spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V6 >;
