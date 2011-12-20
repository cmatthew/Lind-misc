#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include <unistd.h>
#include "logger.h"


/*Write this message to the log. */
int log_message(int priority, char * message) {
  printf("Logger Says: %s\n", message);
  fflush(stdout);
  return 0;
}


void test(void) {
  log_message(1, "Test1");
  log_message(2, "Test2");
  log_message(3, "Test3");
  log_message(1, "Test21");
  log_message(2, "Test22");
  log_message(3, "Test23");

}
