
#include "node_connections_manager_priority_queue_worker.h"
#include "ip.h"


spyke::priority_queue::Priority_Queue_Task spyke::node::create_priority_queue_task( const TASK_ID& task_id, const void* data ) {

  

}


template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >::Node_Connections_Manager_Priority_Queue_Worker( Node_Connections_Manager_Priority_Queue_Worker&& other ) 
  : Priority_Queue_Worker( std::move( other ) ), node_connections_manager( other.node_connections_manager ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >::Node_Connections_Manager_Priority_Queue_Worker() : Priority_Queue_Worker(), node_connections_manager( 0 ) {}

template < typename SERVER_IP_TYPE >
spyke::node::Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >::Node_Connections_Manager_Priority_Queue_Worker( priority_queue::Priority_Queue* priority_queue, Node_Connections_Manager< SERVER_IP_TYPE >* node_connections_manager ) 
  : Priority_Queue_Worker( priority_queue ), node_connections_manager( node_connections_manager ) {}

template < typename SERVER_IP_TYPE >
void spyke::node::Node_Connections_Manager_Priority_Queue_Worker< SERVER_IP_TYPE >::handle_new_task( const std::unique_ptr< priority_queue::Priority_Queue_Task >& priority_queue_task ) {

   

}



template class spyke::node::Node_Connections_Manager_Priority_Queue_Worker< spyke::communication::connection::IP_V4 >;
template class spyke::node::Node_Connections_Manager_Priority_Queue_Worker< spyke::communication::connection::IP_V6 >;
