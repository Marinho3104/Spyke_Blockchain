
/** priority_queue.h - This implementation of a priority queue is designed to manage tasks in a multithreaded environment by prioritizing them based on their importance. 
 * Tasks are enqueued with associated priority levels, and the queue ensures that higher-priority tasks are processed before lower-priority ones. 
 * To support concurrent operations, the implementation uses thread-safe mechanisms to handle simultaneous access by multiple threads, preventing race conditions and ensuring consistency. 
 * - chatgpt.com
 * **/

#ifndef INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_H
#define INCLUDE_PRIORITY_QUEUE_PRIORITY_QUEUE_H

#include <semaphore.h>
#include <cstddef>
#include <memory>

namespace spyke::priority_queue {

  // Represents a Base class for any type of task for a priority queue
  class Priority_Queue_Task {

    public: 
      
      // Any type of priority a task can have
      enum Task_Priority : char {

        LOW = 0,
        MEDIUM = 1,
        HIGH = 2

      };

    private:

      std::unique_ptr< Priority_Queue_Task > next;  // Because the priority queue is a linked list every task as a reference to the next one
      const Task_Priority priority;                 // Priority of the task
      sem_t locker;                                 // To ensure that only one thread can access/change the task data at a time

    public:

      Priority_Queue_Task( const Priority_Queue_Task& ) = delete;

      Priority_Queue_Task( Priority_Queue_Task&& );

      Priority_Queue_Task();

      Priority_Queue_Task( const Task_Priority& );

      Priority_Queue_Task( std::unique_ptr< Priority_Queue_Task >, const Task_Priority& );

      const std::unique_ptr< Priority_Queue_Task >& get_next();

      const Task_Priority get_priority() const;

      std::unique_ptr< Priority_Queue_Task > set_next( std::unique_ptr< Priority_Queue_Task > );

      virtual void handle_task();

  };

  class Priority_Queue {

    private:

      std::unique_ptr< Priority_Queue_Task > head;  // Head of the priority queue, also the first task to be popped
      bool task_adding_enable;                      // Flag to enable new tasks to be added to the queue
      size_t available_space;                       // Number of tasks that can still be added to the queue
      sem_t locker;                                 // Locker that ensures that only one thread at the time can add/pop a task from the queue
      sem_t tasks_available;                        // Special behaviour is used to sinalize and block a pop_task function if the queue is empty
                                                    // or returning invalid if no more tasks adding are possible

    public:

      Priority_Queue( const Priority_Queue& ) = delete;

      Priority_Queue( const size_t& );

      Priority_Queue( Priority_Queue&& );

      const bool is_empty() const;

      void disable_tasks_adding();

      const bool add_new_task( std::unique_ptr< Priority_Queue_Task >& );
    
      std::unique_ptr< Priority_Queue_Task > pop_task();

  };

}

#endif
