
#include <cassert>
#include <cstring>
#include <unistd.h>
#include "./communication_connection_tests.cpp"
#include "./communication_communication_protocols_tests.cpp"
#include "./priority_queue_tests.cpp"


int main (int argc, char *argv[]) {

  // ip_check();
  // connection_check();
  // arbitrary_communication_check();
  priority_queue_add_pop_check();

  return 0;

}

