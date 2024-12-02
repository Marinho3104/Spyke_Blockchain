

#ifndef INCLUDE_NODE_NODE_CONNECTIONS_MANAGER_PRIORITY_QUEUE_WORKER_H
#define INCLUDE_NODE_NODE_CONNECTIONS_MANAGER_PRIORITY_QUEUE_WORKER_H

#include "priority_queue.h"
#include "priority_queue_worker.h"

namespace spyke::node {

  // Forward
  template < typename SERVER_IP_TYPE >
  class Node_Connections_Manager;

  enum class TASK_ID : char {

    ADD_TEMPORARY_CONNECTION = 0,

  };

  static priority_queue::Priority_Queue_Task create_priority_queue_task( const TASK_ID&, const void* );

  template < typename SERVER_IP_TYPE >
  class Node_Connections_Manager_Priority_Queue_Worker : public priority_queue::Priority_Queue_Worker {

    private:

      Node_Connections_Manager< SERVER_IP_TYPE >* node_connections_manager; 

    protected:

      void handle_new_task( const std::unique_ptr< priority_queue::Priority_Queue_Task >& priority_queue_task );

    public:

      Node_Connections_Manager_Priority_Queue_Worker( const Node_Connections_Manager_Priority_Queue_Worker& ) = delete;

      Node_Connections_Manager_Priority_Queue_Worker( Node_Connections_Manager_Priority_Queue_Worker&& );

      Node_Connections_Manager_Priority_Queue_Worker();

      Node_Connections_Manager_Priority_Queue_Worker( priority_queue::Priority_Queue*, Node_Connections_Manager< SERVER_IP_TYPE >* );

  };

}

#endif
