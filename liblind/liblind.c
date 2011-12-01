#include "component.h"
#include "stubwarning.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


#define LIND_CIA_IOCTL 4001

struct cia_s {
  cid who;
  mailbox interface;
};

struct call_s {
  mailbox dest;
  size_t siz;
  void* message;
};


static int _lind_fd = 0;

int lind_fd() {
  return _lind_fd;
}

/** If this process has not already opened a connection to the lind server, do it now. */
static void lind_connect() {
  
  if (!_lind_fd) {
      _lind_fd = open("lind://component-manager", 0, O_RDONLY|O_NONBLOCK);
  }
  if (_lind_fd == -1) {
    perror("Problem opening ioctl channel.");
  }


}

/* Sender: make a connection to component WHO  */
int comp_interface_attach(cid who, mailbox interface) {
  lind_connect();
  struct cia_s cia = {0,0};
  
  cia.who = who;
  cia.interface = interface;

  int rc = ioctl(lind_fd(), LIND_CIA_IOCTL, &cia);
  if(rc == -1) {
    perror("Problem with ioctl channel.");
    return -1;
  }   
  
  return rc;

}


/* Send MESSAGESIZE byte located at MESSAGE to DEST via the lind ioctl interface. */
int comp_call(mailbox dest, size_t message_size, void* message) {
  lind_connect();

  struct call_s call = {0,0};
  
  call.dest = dest;
  call.siz = message_size;
  call.message = message;

  int rc = ioctl(lind_fd(), LIND_CALL_IOCTL, &call);

  if(rc == -1) {
    perror("Problem with ioctl channel.");
    return -1;
  }   

  return rc;

}

/** Wait for communication from CID in mailbox MB.
 */
stub_warning(comp_accept);
cid comp_accept(mailbox mb) {
  lind_connect();
  
  struct cia_s cia = {0,0};
  
  cia.who = who;
  cia.interface = interface;

  int rc = ioctl(lind_fd(), LIND_CIA_IOCTL, &cia);
  if(rc == -1) {
    perror("Problem with ioctl channel.");
    return -1;
  }   

  return rc;



}


/**/
stub_warning(comp_recv);
int comp_recv(void* buff, size_t max) {
  lind_connect();

  struct cia_s cia = {0,0};
  
  cia.who = who;
  cia.interface = interface;

  int rc = ioctl(lind_fd(), LIND_CIA_IOCTL, &cia);
  if(rc == -1) {
    perror("Problem with ioctl channel.");
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



