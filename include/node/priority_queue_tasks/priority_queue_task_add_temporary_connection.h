
#ifndef INCLUDE_NODE_PRIORITY_QUEUE_TASKS_PRIORITY_QUEUE_TASK_ADD_TEMPORARY_CONNECTION_H
#define INCLUDE_NODE_PRIORITY_QUEUE_TASKS_PRIORITY_QUEUE_TASK_ADD_TEMPORARY_CONNECTION_H

#include "connection.h"
#include "node_connections_manager.h"
#include "priority_queue.h"

namespace spyke::node::priority_queue_tasks {

  template < typename SERVER_IP_TYPE, typename CONNECTION_IP_TYPE >
  class Priority_Queue_Task_Add_Temporary_Connection : public priority_queue::Priority_Queue_Task {

    private:

      communication::connection::Connection< CONNECTION_IP_TYPE > external_connection;
      std::shared_ptr< Node_Connections_Manager< SERVER_IP_TYPE > > node_connections_manager;

    public:

      Priority_Queue_Task_Add_Temporary_Connection( const Priority_Queue_Task_Add_Temporary_Connection& ) = delete;

      Priority_Queue_Task_Add_Temporary_Connection( std::shared_ptr< Node_Connections_Manager< SERVER_IP_TYPE > >, communication::connection::Connection< CONNECTION_IP_TYPE >&, priority_queue::Priority_Queue_Task::Task_Priority priority = priority_queue::Priority_Queue_Task::Task_Priority::LOW );

      void handle_task() override;

  };

}

#endif
