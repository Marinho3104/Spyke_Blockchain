
#include "priority_queue.h"
#include <cstring>
#include <memory>
#include <semaphore.h>

spyke::priority_queue::Priority_Queue_Task::Priority_Queue_Task( Priority_Queue_Task&& other ) : next( std::move( other.next ) ), priority( other.priority ), locker( other.locker ) { other.locker = {}; }

spyke::priority_queue::Priority_Queue_Task::Priority_Queue_Task() : next(), priority( Task_Priority::LOW ) { sem_init( &locker, 0, 1 ); }

spyke::priority_queue::Priority_Queue_Task::Priority_Queue_Task( const Task_Priority& priority ) : next(), priority( priority ) { sem_init( &locker, 0, 1 ); }

spyke::priority_queue::Priority_Queue_Task::Priority_Queue_Task( std::unique_ptr< Priority_Queue_Task > next, const Task_Priority& priority )
  : next( std::move( next ) ), priority( priority ) { sem_init( &locker, 0, 1 ); }

const std::unique_ptr< spyke::priority_queue::Priority_Queue_Task >& spyke::priority_queue::Priority_Queue_Task::get_next() { 

  sem_wait( &locker ); const std::unique_ptr< Priority_Queue_Task >& next = this->next; sem_post( &locker ); 
  return next;

}

const spyke::priority_queue::Priority_Queue_Task::Task_Priority spyke::priority_queue::Priority_Queue_Task::get_priority() const { return priority; }

std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > spyke::priority_queue::Priority_Queue_Task::set_next( std::unique_ptr< Priority_Queue_Task > new_next ) {

  sem_wait( &locker );

  std::unique_ptr< Priority_Queue_Task > old_next = std::move( next );
  next = std::move( new_next );

  sem_post( &locker );

  return std::move( old_next );

}

void spyke::priority_queue::Priority_Queue_Task::handle_task() {}


spyke::priority_queue::Priority_Queue::Priority_Queue( const size_t& available_space ) : head(), available_space( available_space ), task_adding_enable( 1 ) { sem_init( &locker, 0, 1 ); sem_init( &tasks_available, 0, 0 ); }

spyke::priority_queue::Priority_Queue::Priority_Queue( Priority_Queue&& other ) 
  : head( std::move( other.head ) ), available_space( other.available_space ), locker( other.locker ), tasks_available( other.tasks_available ), task_adding_enable( other.task_adding_enable ) {}

const bool spyke::priority_queue::Priority_Queue::is_empty() const { return head ? 0 : 1; }

void spyke::priority_queue::Priority_Queue::disable_tasks_adding() { sem_wait( &locker ); task_adding_enable = 0; sem_post( &locker ); sem_post( &tasks_available ); }

const bool spyke::priority_queue::Priority_Queue::add_new_task( std::unique_ptr< Priority_Queue_Task >& new_node ) {

  sem_wait( &locker );

  if ( ! available_space || ! task_adding_enable ) { sem_post( &locker ); return 0; }

  if( is_empty() ) sem_post( &tasks_available );

  // Queue is empty
  if ( ! head ) head = std::move( new_node );

  else {

    if ( head->get_priority() < new_node->get_priority() ) { new_node->set_next( std::move( head ) ); head = std::move( new_node ); }

    else {

        Priority_Queue_Task* current_node = head.get();

        while ( current_node->get_next() && current_node->get_next()->get_priority() >= new_node->get_priority() ) current_node = current_node->get_next().get();

        std::unique_ptr< Priority_Queue_Task > current_old_next = current_node->set_next( std::unique_ptr< Priority_Queue_Task >() );
        new_node->set_next( std::move( current_old_next ) );
        current_node->set_next( std::move( new_node ) );
    
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

  std::unique_ptr< Priority_Queue_Task > new_head = head->set_next( std::unique_ptr< Priority_Queue_Task >() );
  std::unique_ptr< Priority_Queue_Task > previous_head = std::move( head );
  head = std::move( new_head );
  available_space ++;

  if( ! is_empty() ) sem_post( &tasks_available );

  sem_post( &locker );
  
  return std::move( previous_head );

}
