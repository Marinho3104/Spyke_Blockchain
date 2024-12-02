
#include "priority_queue.h"
#include <cstring>
#include <memory>
#include <semaphore.h>


spyke::priority_queue::Priority_Queue_Task::Priority_Queue_Task() : next(), task_data(), priority( LOW ), task_id( 0 ), task_data_size( 0 ) {}

spyke::priority_queue::Priority_Queue_Task::Priority_Queue_Task( const Task_Priority& priority, const char& task_id, const unsigned char data[], const size_t& data_size )
  : task_data( std::make_unique< unsigned char[] >( data_size ) ), priority( priority ), task_id( task_id ), task_data_size( data_size ) { std::memcpy( task_data.get(), data, data_size ); }

spyke::priority_queue::Priority_Queue_Task::Priority_Queue_Task( Priority_Queue_Task&& other )
  : next( std::move( other.next ) ), task_data( std::move( other.task_data ) ), priority( other.priority ), task_id( other.task_id ), task_data_size( other.task_data_size ) {}

std::unique_ptr< spyke::priority_queue::Priority_Queue_Task >& spyke::priority_queue::Priority_Queue_Task::get_next() { return next; }

const std::unique_ptr< unsigned char[] >& spyke::priority_queue::Priority_Queue_Task::get_task_data() const { return task_data; }

const spyke::priority_queue::Task_Priority spyke::priority_queue::Priority_Queue_Task::get_priority() const { return priority; }

const size_t spyke::priority_queue::Priority_Queue_Task::get_task_data_size() const { return task_data_size; }


spyke::priority_queue::Priority_Queue::Priority_Queue( const size_t& available_space ) : head(), available_space( available_space ), task_adding_enable( 1 ) { sem_init( &locker, 0, 1 ); sem_init( &tasks_available, 0, 0 ); }

spyke::priority_queue::Priority_Queue::Priority_Queue( Priority_Queue&& other ) : head( std::move( other.head ) ), available_space( other.available_space ), locker( other.locker ), tasks_available( other.tasks_available ) {}

const bool spyke::priority_queue::Priority_Queue::is_empty() const { return head ? 0 : 1; }

void spyke::priority_queue::Priority_Queue::disable_tasks_adding() { sem_wait( &locker ); task_adding_enable = 0; sem_post( &locker ); sem_post( &tasks_available ); }

const bool spyke::priority_queue::Priority_Queue::add_new_task( std::unique_ptr< Priority_Queue_Task >& new_node ) {

  sem_wait( &locker );

  if ( ! available_space || ! task_adding_enable ) { sem_post( &locker ); return 0; }

  if( is_empty() ) sem_post( &tasks_available );

  // Queue is empty
  if ( ! head ) head = std::move( new_node );

  else {

    if ( head->get_priority() < new_node->get_priority() ) { new_node->get_next() = std::move( head ); head = std::move( new_node ); }

    else {

        Priority_Queue_Task* current_node = head.get();

        while ( current_node->get_next() && current_node->get_next()->get_priority() >= new_node->get_priority() ) current_node = current_node->get_next().get();

        new_node->get_next() = std::move( current_node->get_next() );
        current_node->get_next() = std::move( new_node );
    
    }

  }

  available_space --;
    
  sem_post( &locker );

  return 1;

}


std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > spyke::priority_queue::Priority_Queue::pop_task() {

  sem_wait( &tasks_available ); sem_wait( &locker );

  // Only gets here when the priority queue as finish adding tasks
  // and this is used to sinalize to the worker thread that there are no more tasks 
  // and the priority queue is finished
  if ( ! head ) { sem_post( &locker ); sem_post( &tasks_available ); return std::unique_ptr< Priority_Queue_Task >(); }

  std::unique_ptr< Priority_Queue_Task > previous_head = std::move( head );

  head = std::move( previous_head->get_next() );
  available_space ++;

  if( ! is_empty() || ! task_adding_enable ) sem_post( &tasks_available );

  sem_post( &locker );
  
  return std::move( previous_head );

}
