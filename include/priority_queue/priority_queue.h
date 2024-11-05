
#ifndef INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_H
#define INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_H

#include <semaphore.h>
#include <cstddef>
#include <memory>

namespace priority_queue {

  enum Task_Priority {

    LOW = 0,
    MEDIUM = 1,
    HIGH = 2

  };

  class Priority_Queue_Task {

    private:

      std::unique_ptr< Priority_Queue_Task > next;
      std::unique_ptr< unsigned char[] > task_data;
      const Task_Priority priority;
      const size_t task_data_size;

    public:

      Priority_Queue_Task( const Priority_Queue_Task& ) = delete;

      Priority_Queue_Task();

      Priority_Queue_Task( const Task_Priority&, const unsigned char[], const size_t& );

      Priority_Queue_Task( Priority_Queue_Task&& );

      std::unique_ptr< Priority_Queue_Task >& get_next();

      const std::unique_ptr< unsigned char[] >& get_task_data() const;

      const Task_Priority get_priority() const;

      const size_t get_task_data_size() const;

  };

  class Priority_Queue {

    private:

      std::unique_ptr< Priority_Queue_Task > head;
      size_t available_space;
      sem_t locker;

    public:

      Priority_Queue( const Priority_Queue& ) = delete;

      Priority_Queue( const size_t& );

      Priority_Queue( Priority_Queue&& );

      const bool add_new_task( const Task_Priority&, const unsigned char[], const size_t& );
    
      std::unique_ptr< Priority_Queue_Task > pop_task();

  };

}

#endif
