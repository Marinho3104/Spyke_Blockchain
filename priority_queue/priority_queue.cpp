
#include "priority_queue.h"
#include <cstring>
#include <memory>
#include <semaphore.h>


priority_queue::Priority_Queue_Task::Priority_Queue_Task() : next(), task_data(), priority( LOW ), task_data_size( 0 ) {}

priority_queue::Priority_Queue_Task::Priority_Queue_Task( const Task_Priority& protocol_id, const unsigned char data[], const size_t& data_size )
  : task_data( std::make_unique< unsigned char[] >( data_size ) ), priority( protocol_id ), task_data_size( data_size ) { std::memcpy( task_data.get(), data, data_size ); }

priority_queue::Priority_Queue_Task::Priority_Queue_Task( Priority_Queue_Task&& other )
  : next( std::move( other.next ) ), task_data( std::move( other.task_data ) ), priority( other.priority ), task_data_size( other.task_data_size ) {}

std::unique_ptr< priority_queue::Priority_Queue_Task >& priority_queue::Priority_Queue_Task::get_next() { return next; }

const std::unique_ptr< unsigned char[] >& priority_queue::Priority_Queue_Task::get_task_data() const { return task_data; }

const priority_queue::Task_Priority priority_queue::Priority_Queue_Task::get_priority() const { return priority; }

const size_t priority_queue::Priority_Queue_Task::get_task_data_size() const { return task_data_size; }


priority_queue::Priority_Queue::Priority_Queue( const size_t& available_space ) : head(), available_space( available_space ) { sem_init( &locker, 0, 1 ); }

priority_queue::Priority_Queue::Priority_Queue( Priority_Queue&& other ) : head( std::move( other.head ) ), available_space( other.available_space ), locker( other.locker ) {}

const bool priority_queue::Priority_Queue::add_new_task( const Task_Priority& priority, const unsigned char data[], const size_t& data_size ) {

  sem_wait( &locker );

  if ( ! available_space ) { sem_post( &locker ); return 0; }

  // Queue is empty
  if ( ! head ) head = std::make_unique< Priority_Queue_Task >( priority, data, data_size );

  else {

    std::unique_ptr< Priority_Queue_Task > new_node = std::make_unique< Priority_Queue_Task >( priority, data, data_size );

    if ( head->get_priority() < priority ) { new_node->get_next() = std::move( head ); head = std::move( new_node ); }

    else {

        Priority_Queue_Task* current_node = head.get();

        while ( current_node->get_next() && current_node->get_next()->get_priority() >= priority ) current_node = current_node->get_next().get();

        new_node->get_next() = std::move( current_node->get_next() );
        current_node->get_next() = std::move( new_node );
    
    }

  }

  available_space --;
    
  sem_post( &locker );

  return 1;

}


std::unique_ptr< priority_queue::Priority_Queue_Task > priority_queue::Priority_Queue::pop_task() {

  sem_wait( &locker );

  if ( ! head ) { sem_post( &locker ); return std::unique_ptr< Priority_Queue_Task >(); }

  std::unique_ptr< Priority_Queue_Task > previous_head = std::move( head );

  head = std::move( previous_head->get_next() );
  available_space ++;

  sem_post( &locker );
  
  return std::move( previous_head );

}

