
#include "priority_queue.h"
#include "priority_queue_worker.h"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <semaphore.h>

// TODO: Make tests for thread implementation

void priority_queue_workers_check() {

  std::cout << "\n\n\t*** PRIORITY QUEUE WORKERS CHECK ***\n" << std::endl;

  {

    std::shared_ptr< spyke::priority_queue::Priority_Queue > priority_queue = 
      std::make_shared< spyke::priority_queue::Priority_Queue >( 5 );

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Worker > worker = 
      std::make_unique< spyke::priority_queue::Priority_Queue_Worker >( priority_queue );
    worker->start();

    std::cout << "\t\tUsing a single worker to handler all tasks from the queue: ";

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task1 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task2 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task3 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task4 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task5 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task6 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );

    assert( priority_queue.get()->add_new_task( task1 ) );
    assert( priority_queue.get()->add_new_task( task2 ) );
    assert( priority_queue.get()->add_new_task( task3 ) );
    assert( priority_queue.get()->add_new_task( task4 ) );
    assert( priority_queue.get()->add_new_task( task5 ) );
    priority_queue.get()->disable_tasks_adding();

    while( worker->is_thread_running() );

    assert( priority_queue.get()->is_empty() );

    std::cout << "Pass" << std::endl;

  }

  {

    const size_t workers_count = 1000;
    const size_t tasks_count = 1000;

    std::shared_ptr< spyke::priority_queue::Priority_Queue > priority_queue = 
      std::make_shared< spyke::priority_queue::Priority_Queue >( tasks_count );

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Worker > workers[ workers_count ];

    for( int _ = 0; _ < workers_count; _ ++ ) {

      workers[ _ ] = std::make_unique< spyke::priority_queue::Priority_Queue_Worker >( priority_queue );
      workers[ _ ]->start();

    }

    std::cout << "\t\tUsing a multiple workers to handler all tasks from the queue: ";

    for( int _ = 0; _ < tasks_count; _ ++ ) {

      std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task = 
        std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );

      assert( priority_queue.get()->add_new_task( task ) );

    }
    priority_queue.get()->disable_tasks_adding();

    for( int _ = 0; _ < workers_count; _ ++ ) {

      while( workers[ _ ]->is_thread_running() );

    }

    assert( priority_queue.get()->is_empty() );

    std::cout << "Pass" << std::endl;

  }

}

void priority_queue_add_pop_check() {

  std::cout << "\n\n\t*** PRIORITY QUEUE ADD AND POP CHECK ***\n" << std::endl;

  {

    spyke::priority_queue::Priority_Queue priority_queue = spyke::priority_queue::Priority_Queue( 5 );   

    std::cout << "\t\tAdding more than the max number of tasks: ";

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task1 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task2 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task3 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task4 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task5 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task6 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );

    assert( priority_queue.add_new_task( task1 ) );
    assert( priority_queue.add_new_task( task2 ) );
    assert( priority_queue.add_new_task( task3 ) );
    assert( priority_queue.add_new_task( task4 ) );
    assert( priority_queue.add_new_task( task5 ) );
    assert( ! priority_queue.add_new_task( task6 ) );

    std::cout << "Pass" << std::endl;

  }

  {

    spyke::priority_queue::Priority_Queue priority_queue = spyke::priority_queue::Priority_Queue( 5 );   

    std::cout << "\t\tPopping task with ascending priority: ";

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task1 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task2 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task3 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task4 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task5 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );

    assert( priority_queue.add_new_task( task1 ) );
    assert( priority_queue.add_new_task( task2 ) );
    assert( priority_queue.add_new_task( task3 ) );
    assert( priority_queue.add_new_task( task4 ) );
    assert( priority_queue.add_new_task( task5 ) );

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_1 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_2 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_3 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_4 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_5 = std::move( priority_queue.pop_task() );

    assert( pop_task_1 ); assert( pop_task_2 );
    assert( pop_task_3 ); assert( pop_task_4 );
    assert( pop_task_5 );

    assert( pop_task_1->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    assert( pop_task_2->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    assert( pop_task_3->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    assert( pop_task_4->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );
    assert( pop_task_5->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );

    std::cout << "Pass" << std::endl;

  }

  {

    spyke::priority_queue::Priority_Queue priority_queue = spyke::priority_queue::Priority_Queue( 5 );   

    std::cout << "\t\tPopping task with mixed priority: ";

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task1 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task2 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task3 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task4 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task5 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );

    assert( priority_queue.add_new_task( task1 ) );
    assert( priority_queue.add_new_task( task2 ) );
    assert( priority_queue.add_new_task( task3 ) );
    assert( priority_queue.add_new_task( task4 ) );
    assert( priority_queue.add_new_task( task5 ) );

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_1 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_2 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_3 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_4 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_5 = std::move( priority_queue.pop_task() );

    assert( pop_task_1 ); assert( pop_task_2 );
    assert( pop_task_3 ); assert( pop_task_4 );
    assert( pop_task_5 ); 

    assert( pop_task_1->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    assert( pop_task_2->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    assert( pop_task_3->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    assert( pop_task_4->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::MEDIUM );
    assert( pop_task_5->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );


    std::cout << "Pass" << std::endl;

 }

  {

    spyke::priority_queue::Priority_Queue priority_queue = spyke::priority_queue::Priority_Queue( 5 );   

    std::cout << "\t\tDisabling adding new tasks check behaviour: ";

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task1 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task2 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > task3 = std::make_unique< spyke::priority_queue::Priority_Queue_Task >( spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    
    assert( priority_queue.add_new_task( task1 ) );
    assert( priority_queue.add_new_task( task2 ) );

    priority_queue.disable_tasks_adding();
    assert( ! priority_queue.add_new_task( task3 ) );

    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_1 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_2 = std::move( priority_queue.pop_task() );
    std::unique_ptr< spyke::priority_queue::Priority_Queue_Task > pop_task_3_empty = std::move( priority_queue.pop_task() );

    assert( pop_task_1 ); assert( pop_task_2 );
    assert( ! pop_task_3_empty );

    assert( pop_task_1->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::HIGH );
    assert( pop_task_2->get_priority() == spyke::priority_queue::Priority_Queue_Task::Task_Priority::LOW );
    assert( ! pop_task_3_empty );

    std::cout << "Pass" << std::endl;

  }

}

int main (int argc, char *argv[]) {
  
  std::cout << "*** PRIORITY QUEUE TESTS ***" << std::endl;

  priority_queue_add_pop_check();
  priority_queue_workers_check();

  return 0;

}
