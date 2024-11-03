
#include <cassert>
#include <cstring>
#include <unistd.h>
#include "./communication_connection_tests.cpp"
#include "./communication_communication_protocols_tests.cpp"

int main (int argc, char *argv[]) {

  ip_check();
  connection_check();
  arbitrary_communication_check();

  return 0;
}

