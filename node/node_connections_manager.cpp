
#include "node_connections_manager.h"
#include "connection.h"
#include "ip.h"
#include "packet.h"
#include "priority_queue.h"
#include "priority_queue_task_add_temporary_connection.h"
#include "priority_queue_task_remove_connection.h"
#include "priority_queue_worker.h"
#include "socket_helper.h"
#include <csignal>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <semaphore.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <utility>
#include <type_traits>


template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager_Settings< SERVER_IP_TYPE >::Node_Connections_Manager_Settings() : temporary_connections_count{}, stable_connections_count{}, server_ip{}, priority_queue_capacity{}, priority_queue_workers_count{},
  initial_ip_v4_count{}, initial_ip_v6_count{}, initial_ip_v4(), initial_ip_v6() {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager_Settings< SERVER_IP_TYPE >::Node_Connections_Manager_Settings( Node_Connections_Manager_Settings< SERVER_IP_TYPE >&& other ) : temporary_connections_count( other.temporary_connections_count ), 
    stable_connections_count( other.stable_connections_count ), server_ip( other.server_ip ), priority_queue_capacity( other.priority_queue_capacity ), priority_queue_workers_count( other.priority_queue_workers_count ), 
      initial_ip_v4_count( other.initial_ip_v4_count ), initial_ip_v6_count( other.initial_ip_v6_count ), initial_ip_v4( std::move( other.initial_ip_v4 ) ), initial_ip_v6( std::move( other.initial_ip_v6 ) ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager_Settings< SERVER_IP_TYPE >::Node_Connections_Manager_Settings( const unsigned char temporary_connections_count, const unsigned char stable_connections_count, SERVER_IP_TYPE server_ip, const size_t priority_queue_capacity, 
  const size_t priority_queue_workers_count, std::unique_ptr< communication::connection::IP_V4[] >& initial_ip_v4, std::unique_ptr< communication::connection::IP_V6[] >& initial_ip_v6, const unsigned char initial_ip_v4_count, const unsigned char initial_ip_v6_count )
    : temporary_connections_count( temporary_connections_count ), stable_connections_count( stable_connections_count ), server_ip( server_ip ), priority_queue_capacity( priority_queue_capacity ), priority_queue_workers_count( priority_queue_workers_count ),
      initial_ip_v4_count( initial_ip_v4_count ), initial_ip_v6_count( initial_ip_v6_count ), initial_ip_v4( std::move( initial_ip_v4 ) ), initial_ip_v6( std::move( initial_ip_v6 ) ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager( spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >&& other ) 
  : ip_v4_temporary_connections( std::move( other.ip_v4_temporary_connections ) ), ip_v4_stable_connections( std::move( other.ip_v4_stable_connections ) ), 
      ip_v6_temporary_connections( std::move( other.ip_v6_temporary_connections ) ), ip_v6_stable_connections( std::move( other.ip_v6_stable_connections ) ),
        temporary_connections_current_count( other.temporary_connections_current_count ), stable_connections_current_count( other.stable_connections_current_count ),
          temporary_connections_locker( std::move( other.temporary_connections_locker ) ), stable_connections_locker( std::move( other.stable_connections_locker ) ),
            server_connection( std::move( other.server_connection ) ), settings( std::move( other.settings ) ), status( other.status ), connections_fd_set( std::move( other.connections_fd_set ) ),
              priority_queue( std::move( other.priority_queue ) ), priority_queue_workers( std::move( other.priority_queue_workers ) ), communication_loop_done( std::move( other.communication_loop_done ) ),
                locker( std::move( other.locker ) ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager() : status( spyke::node::STATUS::INVALID ), settings{}, priority_queue( 0 ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager( Node_Connections_Manager_Settings< SERVER_IP_TYPE >& settings, communication::connection::Connection< SERVER_IP_TYPE >& server_connection )
  : ip_v4_temporary_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V4 > >[] >( settings.temporary_connections_count ) ),
    ip_v4_stable_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V4 > >[] >( settings.stable_connections_count ) ),
      ip_v6_temporary_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V6 > >[] >( settings.temporary_connections_count ) ),
        ip_v6_stable_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V6 > >[] >( settings.stable_connections_count ) ),
            temporary_connections_current_count( 0 ), stable_connections_current_count( 0 ), server_connection( std::move( server_connection ) ), settings( std::move( settings ) ),
              status( spyke::node::STATUS::VALID ), priority_queue( std::make_shared< priority_queue::Priority_Queue >( settings.priority_queue_capacity ) ),
                priority_queue_workers( std::make_unique< priority_queue::Priority_Queue_Worker[] >( settings.priority_queue_workers_count ) ) { 

                    sem_init( &temporary_connections_locker, 0, 1 ); sem_init( &stable_connections_locker, 0, 1 ); sem_init( &communication_loop_done, 0, 0 ); sem_init( &locker, 0, 1 );

                    FD_ZERO( &connections_fd_set ); FD_SET( server_connection.get_socket_id(), &connections_fd_set ); setup(); 

}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager( Node_Connections_Manager_Settings< SERVER_IP_TYPE >& settings )
  : ip_v4_temporary_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V4 > >[] >( settings.temporary_connections_count ) ),
    ip_v4_stable_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V4 > >[] >( settings.stable_connections_count ) ),
      ip_v6_temporary_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V6 > >[] >( settings.temporary_connections_count ) ),
        ip_v6_stable_connections( std::make_unique< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V6 > >[] >( settings.stable_connections_count ) ),
          temporary_connections_current_count( 0 ), stable_connections_current_count( 0 ), server_connection(), settings( std::move( settings ) ), status( spyke::node::STATUS::VALID ), 
            priority_queue_workers( std::make_unique< priority_queue::Priority_Queue_Worker[] >( settings.priority_queue_workers_count ) ),
              priority_queue( std::make_shared< priority_queue::Priority_Queue >( settings.priority_queue_capacity ) ) { 

                  sem_init( &temporary_connections_locker, 0, 1 ); sem_init( &stable_connections_locker, 0, 1 ); sem_init( &communication_loop_done, 0, 0 ); sem_init( &locker, 0, 1 );

                  FD_ZERO( &connections_fd_set ); setup();

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::setup() {

  for( int _ = 0; _ < settings.priority_queue_workers_count; ++_ ) 
      
    new( priority_queue_workers.get() + _ ) priority_queue::Priority_Queue_Worker( priority_queue );

  for( int _ = 0; _ < settings.initial_ip_v4_count; ++_ ) {

    communication::connection::Connection< communication::connection::IP_V4 > initial_connection =
      communication::connection::Connection< communication::connection::IP_V4 >::connect_to( settings.initial_ip_v4[ _ ] );
    if( ! initial_connection.is_connected() ) continue;
    add_new_temporary_connection( initial_connection );

  }

  for( int _ = 0; _ < settings.initial_ip_v6_count; ++_ ) {

    communication::connection::Connection< communication::connection::IP_V6 > initial_connection =
      communication::connection::Connection< communication::connection::IP_V6 >::connect_to( settings.initial_ip_v6[ _ ] );
    if( ! initial_connection.is_connected() ) continue;
    add_new_temporary_connection( initial_connection );

  }

}

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::is_temporary_connections_full() const { return temporary_connections_current_count == settings.temporary_connections_count; }

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::is_stable_connections_full() const { return stable_connections_current_count == settings.stable_connections_count; }

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
    if( select_sts == 0 ) continue; if( select_sts == -1 ) { std::cout << "Out" << std::endl; break; }

    if( server_connection.is_connected() && FD_ISSET( server_connection.get_socket_id(), &connections_fd_set_temp ) ) {
  
      communication::connection::Accept_Connection_Request_Return accept_connection_request_return =
        communication::connection::accept_connection_request( server_connection.get_socket_id() );
      if( accept_connection_request_return.socket_id != -1 ) {

        if( accept_connection_request_return.hint.ss_family == AF_INET ) { // IPV4

          communication::connection::Connection< communication::connection::IP_V4 > new_connection =
            communication::connection::convert_connection< communication::connection::IP_V4 >( accept_connection_request_return );
          std::unique_ptr< priority_queue::Priority_Queue_Task > task = 
            std::make_unique< priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< SERVER_IP_TYPE, communication::connection::IP_V4 > >( this->shared_from_this(), new_connection );
          add_new_task_to_queue( task );

        }

        else if( accept_connection_request_return.hint.ss_family == AF_INET6 ) { // IPV6

          communication::connection::Connection< communication::connection::IP_V6 > new_connection =
            communication::connection::convert_connection< communication::connection::IP_V6 >( accept_connection_request_return );
          std::unique_ptr< priority_queue::Priority_Queue_Task > task = 
            std::make_unique< priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< SERVER_IP_TYPE, communication::connection::IP_V6 > >( this->shared_from_this(), new_connection );
          add_new_task_to_queue( task );

        }

      }

    }

    sem_wait( &stable_connections_locker );

    for( int _ = 0; _ < settings.stable_connections_count; _ ++ ) {

      if( ip_v4_stable_connections[ _ ].get() && ip_v4_stable_connections[ _ ]->is_connected() ) {

        std::unique_ptr< communication::communication_protocols::Packet > packet = 
          communication::communication_protocols::Packet::receive_packet( ip_v4_stable_connections[ _ ] );
        enqueue_task_from_packet( ip_v4_stable_connections[ _ ], packet );

      }

      if( ip_v6_stable_connections[ _ ].get() && ip_v6_stable_connections[ _ ]->is_connected() ) {

        std::unique_ptr< communication::communication_protocols::Packet > packet = 
          communication::communication_protocols::Packet::receive_packet( ip_v6_stable_connections[ _ ] );
        enqueue_task_from_packet( ip_v6_stable_connections[ _ ], packet );

      }

    }

    sem_post( &stable_connections_locker );

    sem_wait( &temporary_connections_locker );

    for( int _ = 0; _ < settings.temporary_connections_count; _ ++ ) {

      if( ip_v4_temporary_connections[ _ ].get() && ip_v4_temporary_connections[ _ ]->is_connected() ) {

        std::unique_ptr< communication::communication_protocols::Packet > packet = 
          communication::communication_protocols::Packet::receive_packet( ip_v4_temporary_connections[ _ ] );
        enqueue_task_from_packet( ip_v4_temporary_connections[ _ ], packet );

      }

      if( ip_v6_temporary_connections[ _ ].get() && ip_v6_temporary_connections[ _ ]->is_connected() ) {

        std::unique_ptr< communication::communication_protocols::Packet > packet = 
          communication::communication_protocols::Packet::receive_packet( ip_v6_temporary_connections[ _ ] );
        enqueue_task_from_packet( ip_v6_temporary_connections[ _ ], packet );

      }

    }

    sem_post( &temporary_connections_locker );

  }

  sem_post( &communication_loop_done );

}

template < typename SERVER_IP_TYPE >
template < typename CONNECTION_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::enqueue_task_from_packet( std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > > connection, std::unique_ptr< spyke::communication::communication_protocols::Packet >& packet ) {

  std::cout << "Enqueue task from packet" << std::endl;

  if( ! packet->is_valid() ) {

    std::unique_ptr< priority_queue::Priority_Queue_Task > task =
      std::make_unique< priority_queue_tasks::Priority_Queue_Task_Remove_Connection< SERVER_IP_TYPE, CONNECTION_IP_TYPE > >( this->shared_from_this(), connection );
    add_new_task_to_queue( task );

  }

}

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::is_valid() const { return status == spyke::node::STATUS::VALID; }

template < typename SERVER_IP_TYPE >
template < typename CONNECTION_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::add_new_temporary_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& new_connection ) {

  sem_wait( &temporary_connections_locker );

  if( is_temporary_connections_full() ) { sem_post( &temporary_connections_locker ); return 0; }

  std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > >* arr;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V4 >::value ) { arr = ip_v4_temporary_connections.get(); }

  else if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V6 >::value ) { arr = ip_v6_temporary_connections.get(); }

  else { sem_post( &temporary_connections_locker ); return 0; }

  for( int _ = 0; _ < settings.temporary_connections_count; _ ++ )

    if( ! arr[ _ ] ) arr[ _ ] = std::make_shared< communication::connection::Connection< CONNECTION_IP_TYPE > >( std::move( new_connection ) );

  temporary_connections_current_count ++;
  sem_wait( &locker ); FD_SET( new_connection.get_socket_id(), &connections_fd_set ); sem_post( &locker );

  sem_post( &temporary_connections_locker );

  return 1;

}

template < typename SERVER_IP_TYPE >
template < typename CONNECTION_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::switch_to_stable_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& new_connection ) {

  sem_wait( &stable_connections_locker );
  sem_wait( &temporary_connections_locker );

  if( is_stable_connections_full() ) { sem_post( &stable_connections_locker ); sem_post( &temporary_connections_locker ); return 0; }

  communication::connection::Connection< CONNECTION_IP_TYPE >* stable_arr, *temporary_arr;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V4 >::value ) { stable_arr = ip_v4_stable_connections.get(); temporary_arr = ip_v4_temporary_connections.get(); }

  else if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V6 >::value ) { stable_arr = ip_v6_stable_connections.get(); temporary_arr = ip_v6_temporary_connections.get(); }

  else { sem_post( &stable_connections_locker ); sem_post( &temporary_connections_locker ); return 0; }

  bool found_temporary_connection = 0;
  for( int _ = 0; _ < settings.temporary_connections_count; _ ++ ) 

    if( temporary_arr[ _ ] == new_connection ) { found_temporary_connection = 1; break; }

  if( ! found_temporary_connection ) { sem_post( &temporary_connections_locker ); sem_post( &stable_connections_locker ); return 0; }

  for( int _ = 0; _ < settings.stable_connections_count; _ ++ )

    if( ! stable_arr[ _ ].is_connected() ) new( stable_arr[ _ ] ) communication::connection::Connection< CONNECTION_IP_TYPE >( std::move( new_connection ) );

  stable_connections_current_count ++; temporary_connections_current_count --;
  sem_wait( &locker ); FD_SET( new_connection.get_socket_id(), &connections_fd_set ); sem_post( &locker );

  sem_post( &temporary_connections_locker );
  sem_post( &stable_connections_locker );

  return 1;

}

template < typename SERVER_IP_TYPE >
template < typename CONNECTION_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::remove_connection( std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > > to_remove ) {

  sem_wait( &stable_connections_locker );
  sem_wait( &temporary_connections_locker );
  sem_wait( &locker );

  if( ! to_remove || ! to_remove.get()->is_connected() ) { sem_post( &stable_connections_locker ); sem_post( &temporary_connections_locker );
     
  sem_post( &locker );
    return 0; }

  std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > >* stable_arr, *temporary_arr;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V4 >::value ) { stable_arr = ip_v4_stable_connections.get(); temporary_arr = ip_v4_temporary_connections.get(); }

  else if constexpr( std::is_same< CONNECTION_IP_TYPE, communication::connection::IP_V6 >::value ) { stable_arr = ip_v6_stable_connections.get(); temporary_arr = ip_v6_temporary_connections.get(); }

  else { sem_post( &stable_connections_locker ); sem_post( &temporary_connections_locker ); return 0; }

  bool found = 0;
  for( int _ = 0; _ < settings.temporary_connections_count; _ ++ ) 

    if( *temporary_arr[ _ ].get() == *to_remove.get() ) { found = 1; FD_CLR( temporary_arr[ _ ].get()->get_socket_id(), &connections_fd_set ); temporary_arr[ _ ].reset(); temporary_connections_current_count --; goto out; }

  for( int _ = 0; _ < settings.stable_connections_count; _ ++ ) 

    if( *stable_arr[ _ ].get() == *to_remove.get() ) { found = 1; FD_CLR( stable_arr[ _ ].get()->get_socket_id(), &connections_fd_set ); stable_arr[ _ ].reset(); stable_connections_current_count --; goto out; }

out:
  sem_post( &stable_connections_locker );
  sem_post( &temporary_connections_locker );
  sem_post( &locker );

  std::cout << "Remove found -> " << found << std::endl;

  return found;

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::Node_Connections_Manager::start() { 

  for( int _ = 0; _ < settings.priority_queue_workers_count; ++_ ) {

    priority_queue_workers.get()[ _ ].start_handling();

  }

  std::thread( &Node_Connections_Manager::communication_loop, this ).detach(); 

}

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::stop() { 

  std::cout << "\n*** TERMINATING NODE CONNECTIONS MANAGER ***\n" << std::endl;

  std::cout << "\tStopping communication loop: ";
  sem_wait( &locker ); memset( &connections_fd_set, 0xFF, sizeof( connections_fd_set ) ); sem_post( &locker );
  kill( getpid(), SIGUSR1 ); sem_wait( &communication_loop_done ); std::cout << "Done" << std::endl;

  std::cout << "\tStopping Priority Queue Works: ";
  priority_queue.get()->disable_tasks_adding();
  for( int _ = 0; _ < settings.priority_queue_workers_count; ++_ ) sem_wait( &priority_queue_workers.get()[ _ ].get_done() );
  std::cout << "Done" << std::endl;

}

template < typename SERVER_IP_TYPE >
const bool spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::add_new_task_to_queue( std::unique_ptr< priority_queue::Priority_Queue_Task >& task ) 
  { if( ! is_valid() ) return 0; return priority_queue.get()->add_new_task( task ); }

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager< SERVER_IP_TYPE > spyke::node::Node_Connections_Manager< SERVER_IP_TYPE >::create( Node_Connections_Manager_Settings< SERVER_IP_TYPE >& settings ) {

  if( ! settings.server_ip.is_valid() ) return Node_Connections_Manager< SERVER_IP_TYPE >( settings );

  communication::connection::Connection< SERVER_IP_TYPE > server_connection =
    communication::connection::Connection< SERVER_IP_TYPE >::create_server( settings.server_ip );

  if( ! server_connection.is_connected() ) return Node_Connections_Manager();

  return Node_Connections_Manager( settings, server_connection );

}


template struct spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V4 >;
template struct spyke::node::Node_Connections_Manager_Settings< spyke::communication::connection::IP_V6 >;
template class spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V4 >;
template class spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V6 >;

template const bool spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V4 >::remove_connection< spyke::communication::connection::IP_V4 >( std::shared_ptr< communication::connection::Connection< spyke::communication::connection::IP_V4>  > );
template const bool spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V4 >::remove_connection< spyke::communication::connection::IP_V6 >( std::shared_ptr< communication::connection::Connection< spyke::communication::connection::IP_V6 > > );
template const bool spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V6 >::remove_connection< spyke::communication::connection::IP_V4 >( std::shared_ptr< communication::connection::Connection< spyke::communication::connection::IP_V4 > > );
template const bool spyke::node::Node_Connections_Manager< spyke::communication::connection::IP_V6 >::remove_connection< spyke::communication::connection::IP_V6 >( std::shared_ptr< communication::connection::Connection< spyke::communication::connection::IP_V6 > > );
