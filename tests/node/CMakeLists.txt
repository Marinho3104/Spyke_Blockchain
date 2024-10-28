
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

  # Communication
  ${PROJECT_SOURCE_DIR}/communication/connection/connection.cpp
  ${PROJECT_SOURCE_DIR}/communication/connection/ip.cpp

)

# Add header files
target_include_directories(
  
  ${PROJECT_NAME} PUBLIC 

  ${PROJECT_SOURCE_DIR}/include/communication/connection

)
