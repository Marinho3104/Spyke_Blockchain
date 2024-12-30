
cmake_minimum_required(VERSION 3.31)

project(Spyke_Blockchain)

# Global variables names
set(PRIORIRTY_QUEUE_TESTS_EXECUTABLE_NAME priority_queue_tests)
set(COMMUNICATION_CONNECTION_TESTS_EXECUTABLE_NAME communication_connection_tests)

# Asan sanitalizer
if (ASAN)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

endif()

if (PRIORIRTY_QUEUE_TESTS)

  add_executable(
    ${PRIORIRTY_QUEUE_TESTS_EXECUTABLE_NAME}

    # Main
    ${PROJECT_SOURCE_DIR}/tests/node/main_priority_queue_tests.cpp

    # Priority Queue
    ${PROJECT_SOURCE_DIR}/priority_queue/priority_queue.cpp
    ${PROJECT_SOURCE_DIR}/priority_queue/priority_queue_worker.cpp

  )

  # Add header files
  target_include_directories(
    
    ${PRIORIRTY_QUEUE_TESTS_EXECUTABLE_NAME} PUBLIC 

    ${PROJECT_SOURCE_DIR}/include/priority_queue

  )

elseif(COMMUNICATION_CONNECTION_TESTS)

  add_executable(
    ${COMMUNICATION_CONNECTION_TESTS_EXECUTABLE_NAME}

    # Main
    ${PROJECT_SOURCE_DIR}/tests/node/main_communication_connection_tests.cpp

    # Communication/Connection
    ${PROJECT_SOURCE_DIR}/communication/connection/socket_helper.cpp
    ${PROJECT_SOURCE_DIR}/communication/connection/connection.cpp
    ${PROJECT_SOURCE_DIR}/communication/connection/ip.cpp

    # Communication/Communication Protocols
    ${PROJECT_SOURCE_DIR}/communication/communication_protocols/packet.cpp

  )

  # Add header files
  target_include_directories(
    
    ${COMMUNICATION_CONNECTION_TESTS_EXECUTABLE_NAME} PUBLIC 

    ${PROJECT_SOURCE_DIR}/include/communication/communication_protocols
    ${PROJECT_SOURCE_DIR}/include/communication/connection

  )

endif()













# if (NODE_8080)
#
#   add_compile_definitions(NODE_8080)
#
# elseif (NODE_8081)
#
#   add_compile_definitions(NODE_8081)
#
# endif()

# Source files
# add_executable(
#
#   ${PROJECT_NAME}
#   
#   # Main
#   ${PROJECT_SOURCE_DIR}/tests/node/main.cpp
#
#   # Communication/Connection
#   ${PROJECT_SOURCE_DIR}/communication/connection/socket_helper.cpp
#   ${PROJECT_SOURCE_DIR}/communication/connection/connection.cpp
#   ${PROJECT_SOURCE_DIR}/communication/connection/ip.cpp
#
#   # Communication/Communication Protocols
#   ${PROJECT_SOURCE_DIR}/communication/communication_protocols/packet.cpp
#   ${PROJECT_SOURCE_DIR}/communication/communication_protocols/switch_to_stable_connection_protocol.cpp
#
#   # Priority Queue
#   ${PROJECT_SOURCE_DIR}/priority_queue/priority_queue.cpp
#   ${PROJECT_SOURCE_DIR}/priority_queue/priority_queue_worker.cpp
#
#   # Node
#   ${PROJECT_SOURCE_DIR}/node/node.cpp
#   ${PROJECT_SOURCE_DIR}/node/node_connections_manager.cpp
#   ${PROJECT_SOURCE_DIR}/node/priority_queue_tasks/priority_queue_task_add_temporary_connection.cpp
#   ${PROJECT_SOURCE_DIR}/node/priority_queue_tasks/priority_queue_task_remove_connection.cpp
#
#   # Client
#   ${PROJECT_SOURCE_DIR}/client/node_client.cpp
#
# )
#
# # Add header files
# target_include_directories(
#   
#   ${PROJECT_NAME} PUBLIC 
#
#   ${PROJECT_SOURCE_DIR}/include/communication/communication_protocols
#   ${PROJECT_SOURCE_DIR}/include/communication/connection
#
#   ${PROJECT_SOURCE_DIR}/include/priority_queue
#
#   ${PROJECT_SOURCE_DIR}/include/node/priority_queue_tasks
#   ${PROJECT_SOURCE_DIR}/include/node
#
#   ${PROJECT_SOURCE_DIR}/include/client
#
# )
