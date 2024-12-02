

#ifndef INCLUDE_NODE_CONNECTION_MANAGER_H
#define INCLUDE_NODE_CONNECTION_MANAGER_H

#include "connection.h"
#include "ip.h"
#include "node_connections_manager_priority_queue_worker.h"
#include "priority_queue.h"
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
    const SERVER_IP_TYPE server_ip;

    const size_t priority_queue_capacity, priority_queue_workers_count;

  };

  template < typename SERVER_IP_TYPE >
  class Node_Connections_Manager {

    private:

      const Node_Connections_Manager_Settings< SERVER_IP_TYPE > settings;

      std::unique_ptr< communication::connection::Connection< communication::connection::IP_V4 >[] > ip_v4_temporary_connections, ip_v4_stable_connections;
      std::unique_ptr< communication::connection::Connection< communication::connection::IP_V6 >[] > ip_v6_temporary_connections, ip_v6_stable_connections;
      unsigned char temporary_connections_current_count, stable_connections_current_count;
      communication::connection::Connection< SERVER_IP_TYPE > server_connection;
      sem_t temporary_connections_locker, stable_connections_locker;

      std::unique_ptr < Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >[] > priority_queue_workers;
      priority_queue::Priority_Queue priority_queue;

      std::thread communication_loop_thread;

      fd_set connections_fd_set;

      STATUS status;

      sem_t communication_loop_done;
      sem_t locker;

      void initiate_variables();

      const bool is_temporary_connections_full() const;

      const bool is_stable_connections_full() const;

      template < typename CONNECTION_IP_TYPE >
      const bool add_new_temporary_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& );

      template < typename CONNECTION_IP_TYPE >
      const bool add_new_stable_connection( communication::connection::Connection< CONNECTION_IP_TYPE >& );

      void communication_loop();    

    public:

      Node_Connections_Manager( const Node_Connections_Manager& ) = delete;

      Node_Connections_Manager( Node_Connections_Manager&& );

      Node_Connections_Manager();

      Node_Connections_Manager( const Node_Connections_Manager_Settings< SERVER_IP_TYPE >&, communication::connection::Connection< SERVER_IP_TYPE >& );

      Node_Connections_Manager( const Node_Connections_Manager_Settings< SERVER_IP_TYPE >& );

      const bool is_valid() const;

      void start();

      void stop();

      template < typename CONNECTION_IP_TYPE >
      const bool add_external_connection( communication::connection::Connection< CONNECTION_IP_TYPE > );

      static Node_Connections_Manager< SERVER_IP_TYPE > create( const Node_Connections_Manager_Settings< SERVER_IP_TYPE >& );

      // This 2 functions are set by each given client
      // and not by the normal .cpp file
      static void signal_handler( int );

      static const bool set_signal_mask();

  };

}

#endif
