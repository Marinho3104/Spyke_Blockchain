
#ifndef INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_WORKER_H
#define INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_WORKER_H

#include "priority_queue.h"
#include <memory>
#include <semaphore.h>

namespace spyke::priority_queue {

  class Priority_Queue_Worker {

    private:

      std::shared_ptr< Priority_Queue > priority_queue; // Reference to the priority queue where this worker will get the tasks from
      bool is_running;                                  // Sinalizes if the thread start() is running
      sem_t locker;                                     // Used to sinalize that the worker have exited the start_handling function

      void handle();

    public:

      Priority_Queue_Worker( const Priority_Queue_Worker& ) = delete;

      Priority_Queue_Worker( Priority_Queue_Worker&& );

      Priority_Queue_Worker();

      Priority_Queue_Worker( std::shared_ptr< Priority_Queue > );

      const bool is_valid() const;

      const bool is_thread_running();

      void start();

  };

}

#endif
