

#include "priority_queue_worker.h"
#include "priority_queue.h"
#include <pthread.h>
#include <semaphore.h>
#include <thread>

spyke::priority_queue::Priority_Queue_Worker::Priority_Queue_Worker( Priority_Queue_Worker&& other ) 
    : priority_queue( other.priority_queue ), locker( other.locker ), is_running( other.is_running ) { other.priority_queue.reset(); other.locker = {}; }

spyke::priority_queue::Priority_Queue_Worker::Priority_Queue_Worker() : priority_queue( 0 ), is_running( false ) { sem_init( &locker, 0, 1 ); }

spyke::priority_queue::Priority_Queue_Worker::Priority_Queue_Worker( std::shared_ptr< Priority_Queue > priority_queue ) 
  : priority_queue( priority_queue ), is_running( false ) { sem_init( &locker, 0, 1 ); }

const bool spyke::priority_queue::Priority_Queue_Worker::is_valid() const { return priority_queue.get(); }

void spyke::priority_queue::Priority_Queue_Worker::handle() {

  std::unique_ptr< Priority_Queue_Task > current_task;

  while( 1 ) {

    current_task = std::move( priority_queue->pop_task() );

    if ( ! current_task ) break;

    current_task->handle_task();

  }

  sem_wait( &locker ); is_running = false; sem_post( &locker );

}

const bool spyke::priority_queue::Priority_Queue_Worker::is_thread_running() { sem_wait( &locker ); const bool result = is_running; sem_post( &locker ); return result; }

void spyke::priority_queue::Priority_Queue_Worker::start() {

  if( is_thread_running() ) return;

  sem_wait( &locker ); is_running = true; sem_post( &locker );

  std::thread( &Priority_Queue_Worker::handle, this ).detach(); 

}
