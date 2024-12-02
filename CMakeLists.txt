
cmake_minimum_required(VERSION 3.0)

project(Spyke_Blockchain)

if(NODE)

  include(${PROJECT_SOURCE_DIR}/tests/node/CMakeLists.txt)

endif()
