
#ifndef INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_WORKER_H
#define INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_WORKER_H

#include "priority_queue.h"

namespace priority_queue {

  class Priority_Queue_Worker {

    private:

      Priority_Queue& priority_queue;

    public:

      Priority_Queue_Worker( const Priority_Queue_Worker& ) = delete;

      Priority_Queue_Worker() = delete;

      Priority_Queue_Worker( Priority_Queue& );
  
      Priority_Queue_Worker( Priority_Queue_Worker&& );

      void start();

      virtual void handle_new_task() = 0;

  };

}

#endif
