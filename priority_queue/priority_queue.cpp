
#include "priority_queue.h"
#include <cstring>
#include <memory>
#include <semaphore.h>


priority_queue::Priority_Queue_Task::Priority_Queue_Task() : next( nullptr ), task_data( nullptr ), priority( LOW ), task_data_size( 0 ) {}

priority_queue::Priority_Queue_Task::Priority_Queue_Task( const Task_Priority& protocol_id, const unsigned char data[], const size_t& data_size )
  : task_data( std::make_unique< unsigned char[] >( data_size ) ), priority( protocol_id ), task_data_size( data_size ) { std::memcpy( task_data.get(), data, data_size ); }

priority_queue::Priority_Queue_Task::Priority_Queue_Task( Priority_Queue_Task&& other )
  : next( std::move( other.next ) ), task_data( std::move( other.task_data ) ), priority( other.priority ), task_data_size( other.task_data_size ) {}

const bool priority_queue::Priority_Queue_Task::is_valid() const { return task_data ? 1 : 0; }

std::unique_ptr< priority_queue::Priority_Queue_Task >& priority_queue::Priority_Queue_Task::get_next() { return next; }

const std::unique_ptr< unsigned char[] >& priority_queue::Priority_Queue_Task::get_task_data() const { return task_data; }

const priority_queue::Task_Priority priority_queue::Priority_Queue_Task::get_priority() const { return priority; }

const size_t priority_queue::Priority_Queue_Task::get_task_data_size() const { return task_data_size; }


priority_queue::Priority_Queue::Priority_Queue( const size_t& available_space ) : head(), available_space( available_space ) { sem_init( &locker, 0, 1 ); }

priority_queue::Priority_Queue::Priority_Queue( Priority_Queue&& other ) : head( std::move( other.head ) ), available_space( other.available_space ), locker( other.locker ) {}


const bool priority_queue::Priority_Queue::add_new_task( const Task_Priority& protocol_id, const unsigned char data[], const size_t& data_size ) {

  sem_wait( &locker );

  if ( ! available_space ) { sem_post( &locker ); return 1; }

  Priority_Queue_Task* current_node = head.get();

  // Queue is empty
  if ( ! current_node->is_valid() ) head = std::make_unique< Priority_Queue_Task >( protocol_id, data, data_size );

  else {

    

  }
    
  sem_post( &locker );

  return 1;

}


priority_queue::Priority_Queue_Task priority_queue::Priority_Queue::pop_task() {



}

