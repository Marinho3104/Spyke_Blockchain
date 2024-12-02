
#ifndef INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_WORKER_H
#define INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_WORKER_H

#include "priority_queue.h"
#include <semaphore.h>
#include <memory>
#include <thread>

namespace spyke::priority_queue {

  class Priority_Queue_Worker {

    private:

      Priority_Queue* priority_queue;
      std::thread worker_thread;
      sem_t done;

      void start();

    protected:

      virtual void handle_new_task( const std::unique_ptr< Priority_Queue_Task >& ) = 0;

    public:

      Priority_Queue_Worker( const Priority_Queue_Worker& ) = delete;

      Priority_Queue_Worker( Priority_Queue_Worker&& );

      Priority_Queue_Worker();

      Priority_Queue_Worker( Priority_Queue* );

      sem_t& get_done();

      void start_handling();

  };

}

#endif
