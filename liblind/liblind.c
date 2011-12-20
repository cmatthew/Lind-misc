#include "component.h"
#include "stubwarning.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>



static int _lind_fd = 0;

int lind_fd() {
  return _lind_fd;
}

/** If this process has not already opened a connection to the lind server, do it now.
    Then save that connection in a global var so we dont have to connect again later.*/
static void lind_connect() {
  printf("Connecting\n");
  fflush(stdout);
  if (!_lind_fd) {
    printf("firsttime\n");
    fflush(stdout);

    _lind_fd = open("lind://component-manager", 0, O_RDONLY|O_NONBLOCK);
    if (_lind_fd == -1) {
      perror("Connect failed: Problem opening ioctl channel");
      
    }
  }
  printf("Connected\n");
  fflush(stdout);


}

/* Sender: make a connection to component WHO  */
int comp_interface_attach(cid who, mailbox interface) {
  lind_connect();
  struct cia_s cia = {0,0};
  
  cia.who = who;
  cia.interface = interface;
  int fd = lind_fd();
  int rc = ioctl(fd, LIND_CIA_IOCTL, sizeof(cia), &cia);

  if(rc == -1) {
    perror("Problem with ioctl channel in compoent interface attach");
    return -1;
  }   
  
  return rc;

}


/* Send MESSAGESIZE byte located at MESSAGE to DEST via the lind ioctl interface. */
int comp_call(mailbox dest, size_t message_size, void * message) {
  lind_connect();
  struct call_s call = {0,0, NULL};
  call.dest = dest;
  call.siz = message_size;
  call.message = message;
  int fd = lind_fd();
  int rc = ioctl(fd, LIND_CALL_IOCTL, sizeof(call), &call);

  if(rc == -1) {
    perror("Problem with ioctl channel.");
    return -1;
  }   

  return rc;

}




/** Wait for communication from CID in mailbox MB.
 */
cid comp_accept(mailbox mb, void* buff, size_t max) {
  lind_connect(); 
  int rc = -1;
  struct comp_accept_s args = {0, NULL, 0};
  args.mb = mb; 
  args.buff = buff; 
  args.max = max; 
  int fd = lind_fd();
  rc = ioctl(fd, LIND_ACCEPT_IOCTL, sizeof(args),  &args);

  if(rc == -1) {
    perror("Problem with ioctl channel in component accept.");
    return -1;
  }

  return rc; 

}


/** ^^^ implement all above first.  vvvv all below second */


/*  Find the COMPONENT_NAME-VERSION-INTERFACE component in the system, and if it is
   not started, start it, then return the CID of the start component.  */
stub_warning(comp_start);
cid comp_start(const char * component_name, const char * version, const char * interface) {


}


/*  Find a component with the given INTERFACE, and optionally VERSION and NAME, and
   return its CID*/
stub_warning(comp_lookup);
cid comp_lookup(const char * name, const char * version, const char * interface) {

}


/* Create a mailbox for this component's INTERFACE in the system.
 TYPE is either MAILBOX_SHARED or MAILBOX_SPLIT*/
stub_warning(comp_create_mailbox);
mailbox comp_create_mailbox(const char * interface, int type) {
  
}



