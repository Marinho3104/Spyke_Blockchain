
cmake_minimum_required(VERSION 3.0)

project(Spyke_Blockchain)

# Satan sanitalizer
if (SATAN)

  set(CMAKE_CXX_FLAGS "-g -fsanitize=address")

endif()

# Source files
add_executable(

  ${PROJECT_NAME}
  
  # Main
  ${PROJECT_SOURCE_DIR}/tests/node/main.cpp

  # Communication/Connection
  ${PROJECT_SOURCE_DIR}/communication/connection/socket_helper.cpp
  ${PROJECT_SOURCE_DIR}/communication/connection/connection.cpp
  ${PROJECT_SOURCE_DIR}/communication/connection/ip.cpp

  # Communication/Communication Protocols
  ${PROJECT_SOURCE_DIR}/communication/communication_protocols/packet.cpp

  # Priority Queue
  ${PROJECT_SOURCE_DIR}/priority_queue/priority_queue.cpp
  ${PROJECT_SOURCE_DIR}/priority_queue/priority_queue_worker.cpp

  # Node
  ${PROJECT_SOURCE_DIR}/node/node.cpp
  ${PROJECT_SOURCE_DIR}/node/node_connections_manager.cpp
  ${PROJECT_SOURCE_DIR}/node/node_connections_manager_priority_queue_worker.cpp

  # Client
  ${PROJECT_SOURCE_DIR}/client/node_client.cpp

)

# Add header files
target_include_directories(
  
  ${PROJECT_NAME} PUBLIC 

  ${PROJECT_SOURCE_DIR}/include/communication/communication_protocols
  ${PROJECT_SOURCE_DIR}/include/communication/connection

  ${PROJECT_SOURCE_DIR}/include/priority_queue

  ${PROJECT_SOURCE_DIR}/include/node

  ${PROJECT_SOURCE_DIR}/include/client

)
