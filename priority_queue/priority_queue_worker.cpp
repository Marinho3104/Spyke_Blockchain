

#include "priority_queue_worker.h"
#include "priority_queue.h"
#include <pthread.h>
#include <semaphore.h>

spyke::priority_queue::Priority_Queue_Worker::Priority_Queue_Worker( Priority_Queue_Worker&& other ) 
    : priority_queue( other.priority_queue ), worker_thread( std::move( other.worker_thread ) ), done( other.done ) {}

spyke::priority_queue::Priority_Queue_Worker::Priority_Queue_Worker() : priority_queue( 0 ) {}

spyke::priority_queue::Priority_Queue_Worker::Priority_Queue_Worker( Priority_Queue* priority_queue ) 
  : priority_queue( priority_queue ), worker_thread( std::thread( &Priority_Queue_Worker::start, this ) ) { sem_init( &done, 0, 0 ); }

void spyke::priority_queue::Priority_Queue_Worker::start() {

  std::unique_ptr< Priority_Queue_Task > current_task;

  while( 1 ) {

    current_task = std::move( priority_queue->pop_task() );

    if ( ! current_task ) break;

    handle_new_task( std::move( current_task ) );

  }

  sem_post( &done );

}

sem_t& spyke::priority_queue::Priority_Queue_Worker::get_done() { return done; }

void spyke::priority_queue::Priority_Queue_Worker::start_handling() { worker_thread.detach(); }
