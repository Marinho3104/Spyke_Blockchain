
#include "ip.h"
#include "priority_queue_task_add_temporary_connection.h"
#include "connection.h"
#include <memory>


template < typename SERVER_IP_TYPE, typename CONNECTION_IP_TYPE >
spyke::node::priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< SERVER_IP_TYPE, CONNECTION_IP_TYPE >::Priority_Queue_Task_Add_Temporary_Connection(
  std::shared_ptr< Node_Connections_Manager< SERVER_IP_TYPE > > node_connections_manager, communication::connection::Connection< CONNECTION_IP_TYPE >& external_connection, priority_queue::Priority_Queue_Task::Task_Priority priority ) 
    : priority_queue::Priority_Queue_Task( priority ), node_connections_manager( node_connections_manager ), external_connection( std::move( external_connection ) ) {}

template < typename SERVER_IP_TYPE, typename CONNECTION_IP_TYPE >
void spyke::node::priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< SERVER_IP_TYPE, CONNECTION_IP_TYPE >::handle_task() {

  if( ! node_connections_manager || ! node_connections_manager.get()->is_valid() || ! external_connection.is_connected() ) return;

  node_connections_manager.get()->add_new_temporary_connection( external_connection );

}


template class spyke::node::priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< spyke::communication::connection::IP_V4, spyke::communication::connection::IP_V4 >;
template class spyke::node::priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< spyke::communication::connection::IP_V4, spyke::communication::connection::IP_V6 >;
template class spyke::node::priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< spyke::communication::connection::IP_V6, spyke::communication::connection::IP_V4 >;
template class spyke::node::priority_queue_tasks::Priority_Queue_Task_Add_Temporary_Connection< spyke::communication::connection::IP_V6, spyke::communication::connection::IP_V6 >;
