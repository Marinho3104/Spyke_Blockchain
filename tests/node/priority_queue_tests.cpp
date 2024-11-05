
#include "priority_queue.h"
#include <cassert>
#include <cstring>
#include <iostream>


void priority_queue_add_pop_check() {

  std::cout << "\n*** PRIORITY QUEUE ADD AND POP CHECK ***\n" << std::endl;

  {

    priority_queue::Priority_Queue priority_queue = priority_queue::Priority_Queue( 5 );   

    std::cout << "\tPopping on empty queue: ";

    std::unique_ptr< priority_queue::Priority_Queue_Task > task = priority_queue.pop_task();

    assert( ! task );

    std::cout << "Pass" << std::endl;

  }

  {

    priority_queue::Priority_Queue priority_queue = priority_queue::Priority_Queue( 5 );   

    std::cout << "\tAdding more than the max number of tasks: ";

    const char* task1_data = "task1"; size_t task1_data_size = strlen( task1_data ) + 1;
    const char* task2_data = "task2"; size_t task2_data_size = strlen( task2_data ) + 1;
    const char* task3_data = "task3"; size_t task3_data_size = strlen( task3_data ) + 1;
    const char* task4_data = "task4"; size_t task4_data_size = strlen( task4_data ) + 1;
    const char* task5_data = "task5"; size_t task5_data_size = strlen( task5_data ) + 1;
    const char* task6_data = "task6"; size_t task6_data_size = strlen( task6_data ) + 1;

    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task1_data, task1_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task2_data, task2_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task3_data, task3_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task4_data, task4_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task5_data, task5_data_size ) );
    assert( ! priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task6_data, task6_data_size ) );

    std::cout << "Pass" << std::endl;

  }

  {

    priority_queue::Priority_Queue priority_queue = priority_queue::Priority_Queue( 5 );   

    std::cout << "\tPopping task with ascending priority: ";

    const char* task1_data = "task1"; size_t task1_data_size = strlen( task1_data ) + 1;
    const char* task2_data = "task22"; size_t task2_data_size = strlen( task2_data ) + 1;
    const char* task3_data = "task333"; size_t task3_data_size = strlen( task3_data ) + 1;
    const char* task4_data = "task4444"; size_t task4_data_size = strlen( task4_data ) + 1;
    const char* task5_data = "task55555"; size_t task5_data_size = strlen( task5_data ) + 1;

    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task1_data, task1_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task2_data, task2_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::MEDIUM, ( unsigned char* ) task3_data, task3_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::LOW, ( unsigned char* ) task4_data, task4_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::LOW, ( unsigned char* ) task5_data, task5_data_size ) );

    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_1 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_2 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_3 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_4 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_5 = std::move( priority_queue.pop_task() );

    assert( pop_task_1 ); assert( pop_task_2 );
    assert( pop_task_3 ); assert( pop_task_4 );
    assert( pop_task_5 );

    assert( std::memcmp( pop_task_1->get_task_data().get(), task1_data, task1_data_size ) == 0 );
    assert( std::memcmp( pop_task_2->get_task_data().get(), task2_data, task2_data_size ) == 0 );
    assert( std::memcmp( pop_task_3->get_task_data().get(), task3_data, task3_data_size ) == 0 );
    assert( std::memcmp( pop_task_4->get_task_data().get(), task4_data, task4_data_size ) == 0 );
    assert( std::memcmp( pop_task_5->get_task_data().get(), task5_data, task5_data_size ) == 0 );

    std::cout << "Pass" << std::endl;

  }

  {

    priority_queue::Priority_Queue priority_queue = priority_queue::Priority_Queue( 5 );   

    std::cout << "\tPopping task with mixed priority: ";

    const char* task1_data = "task1"; size_t task1_data_size = strlen( task1_data ) + 1;
    const char* task2_data = "task22"; size_t task2_data_size = strlen( task2_data ) + 1;
    const char* task3_data = "task333"; size_t task3_data_size = strlen( task3_data ) + 1;
    const char* task4_data = "task4444"; size_t task4_data_size = strlen( task4_data ) + 1;
    const char* task5_data = "task55555"; size_t task5_data_size = strlen( task5_data ) + 1;

    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task1_data, task1_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::LOW, ( unsigned char* ) task2_data, task2_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::MEDIUM, ( unsigned char* ) task3_data, task3_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::HIGH, ( unsigned char* ) task4_data, task4_data_size ) );
    assert( priority_queue.add_new_task( priority_queue::Task_Priority::MEDIUM, ( unsigned char* ) task5_data, task5_data_size ) );

    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_1 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_2 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_3 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_4 = std::move( priority_queue.pop_task() );
    std::unique_ptr< priority_queue::Priority_Queue_Task > pop_task_5 = std::move( priority_queue.pop_task() );

    assert( pop_task_1 ); assert( pop_task_2 );
    assert( pop_task_3 ); assert( pop_task_4 );
    assert( pop_task_5 );

    assert( std::memcmp( pop_task_1->get_task_data().get(), task1_data, task1_data_size ) == 0 );
    assert( std::memcmp( pop_task_2->get_task_data().get(), task4_data, task4_data_size ) == 0 );
    assert( std::memcmp( pop_task_3->get_task_data().get(), task3_data, task3_data_size ) == 0 );
    assert( std::memcmp( pop_task_4->get_task_data().get(), task5_data, task5_data_size ) == 0 );
    assert( std::memcmp( pop_task_5->get_task_data().get(), task2_data, task2_data_size ) == 0 );

    std::cout << "Pass" << std::endl;

  }

}
