

#ifndef NODE_PRIORITY_QUEUE_TASKS_PRIORITY_QUEUE_TASK_REMOVE_CONNECTION_H
#define NODE_PRIORITY_QUEUE_TASKS_PRIORITY_QUEUE_TASK_REMOVE_CONNECTION_H

#include "connection.h"
#include "node_connections_manager.h"
#include <memory>
namespace spyke::node::priority_queue_tasks {

  template < typename SERVER_IP_TYPE, typename CONNECTION_IP_TYPE >
  class Priority_Queue_Task_Remove_Connection : public priority_queue::Priority_Queue_Task {
  
    private:

      std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > > connection;
      std::shared_ptr< Node_Connections_Manager< SERVER_IP_TYPE > > node_connections_manager;


    public:

      Priority_Queue_Task_Remove_Connection( const Priority_Queue_Task_Remove_Connection& ) = delete;

      Priority_Queue_Task_Remove_Connection( std::shared_ptr< Node_Connections_Manager< SERVER_IP_TYPE > >, std::shared_ptr< communication::connection::Connection< CONNECTION_IP_TYPE > >, priority_queue::Priority_Queue_Task::Task_Priority priority = priority_queue::Priority_Queue_Task::Task_Priority::LOW );

      void handle_task() override;

  };

}

#endif
