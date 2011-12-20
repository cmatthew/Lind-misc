#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "logger.h"


/*The logger mailbox. Do not access directly!*/
static cid _mailbox = 0;

/* The global mailbox for logger requests. */
static cid getMailbox(){
  return _mailbox;
}

/*The the global mailbox for logger requests*/
static void setMailbox(cid mailbox) {
  _mailbox = mailbox; 
}

/* Attach to a mailbox.*/
void connect(void) {
  static int connected = 0;
  if (! connected) {
      cid logger = comp_interface_attach(100, 0);
      
      if (logger < 0) {
	fprintf(stderr, "[multi-component-1] Bind has failed.\n");
	exit(EXIT_FAILURE);
      }
      setMailbox(logger);
      connected = 1;
  }
}


int log_message(int priority, char * message) {

  connect();

  struct message * mess = NULL;
  size_t len = sizeof(struct message) + strlen(message) + 1;
  mess = (struct message *) malloc(len);
  mess->call = 0;
  mess->len = strlen(message);
  mess->priority = priority;
  memcpy(&(mess->c), message, strlen(message) + 1);

  int call_status = comp_call(getMailbox(), len, (void*)mess);
  if (call_status < 0) {
    fprintf(stderr, "[multi-component-1] Send has failed.\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
