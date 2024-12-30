

#ifndef INCLUDE_NODE_CONNECTION_MANAGER_H
#define INCLUDE_NODE_CONNECTION_MANAGER_H

#include "connection.h"
#include "ip.h"
#include "packet.h"
#include "priority_queue.h"
#include "priority_queue_worker.h"
#include <memory>
#include <semaphore.h>
#include <sys/select.h>

namespace spyke::node {

  enum class STATUS : char {

    VALID = 0,
    INVALID = 1,

  };

  template < typename SERVER_IP_TYPE >
  struct Node_Connections_Manager_Settings {

    const unsigned char temporary_connections_count, stable_connections_count;
    SERVER_IP_TYPE server_ip;

    const size_t priority_queue_capacity, priority_queue_workers_count;

    std::unique_ptr< communication::connection::IP_V4[] > initial_ip_v4;
    std::unique_ptr< communication::connection::IP_V6[] > initial_ip_v6;
    const unsigned char initial_ip_v4_count, initial_ip_v6_count;


    Node_Connections_Manager_Settings();

    Node_Connections_Manager_Settings( Node_Connections_Manager_Settings< SERVER_IP_TYPE >&& );

    Node_Connections_Manager_Settings( const unsigned char, const unsigned char, SERVER_IP_TYPE, const size_t, const size_t, std::unique_ptr< communication::connection::IP_V4[] >&, std::unique_ptr< communication::connection::IP_V6[] >&, const unsigned char, const unsigned char );

  };

  template < typename SERVER_IP_TYPE >
  class Node_Connections_Manager : public std::enable_shared_from_this< Node_Connections_Manager< SERVER_IP_TYPE > > {

    private:

      Node_Connections_Manager_Settings< SERVER_IP_TYPE > settings;

      std::unique_ptr< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V4 > >[] > ip_v4_temporary_connections, ip_v4_stable_connections;
      std::unique_ptr< std::shared_ptr< communication::connection::Connection< communication::connection::IP_V6 > >[] > ip_v6_temporary_connections, ip_v6_stable_connections;
      unsigned char temporary_connections_current_count, stable_connections_current_count;
      communication::connection::Connection< SERVER_IP_TYPE > server_connection;
      sem_t temporary_connections_locker, stable_connections_locker;

      std::unique_ptr < spyke::priority_queue::Priority_Queue_Worker[] > priority_queue_workers;
      std::shared_ptr< priority_queue::Priority_Queue > priority_queue;

      fd_set connections_fd_set;

      STATUS status;

      sem_t communication_loop_done;
      sem_t locker;

      void setup();

      const bool is_temporary_connections_full() const;

      const bool is_stable_connections_full() const;

      void communication_loop();    

      template< typename CONNECTION_IP_TYPE >
      void enqueue_task_from_packet( std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > >, std::unique_ptr< spyke::communication::communication_protocols::Packet >& );

    public:

      Node_Connections_Manager( const Node_Connections_Manager& ) = delete;

      Node_Connections_Manager( Node_Connections_Manager&& );

      Node_Connections_Manager();

      Node_Connections_Manager( Node_Connections_Manager_Settings< SERVER_IP_TYPE >&, communication::connection::Connection< SERVER_IP_TYPE >& );

      Node_Connections_Manager( Node_Connections_Manager_Settings< SERVER_IP_TYPE >& );

      const bool is_valid() const;

      template < typename CONNECTION_IP_TYPE >
      const bool add_new_temporary_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& );

      template < typename CONNECTION_IP_TYPE >
      const bool switch_to_stable_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& );

      template < typename CONNECTION_IP_TYPE >
      const bool remove_connection( std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > > );

      void start();

      void stop();

      const bool add_new_task_to_queue( std::unique_ptr< priority_queue::Priority_Queue_Task >& );

      static Node_Connections_Manager< SERVER_IP_TYPE > create( Node_Connections_Manager_Settings< SERVER_IP_TYPE >& );

      // This 2 functions are set by each given client
      // and not by the normal .cpp file
      static void signal_handler( int );

      static const bool set_signal_mask();

  };

}

#endif
