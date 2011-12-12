#ifndef _COMPONENT_H_
#define _COMPONENT_H_


/******************************************************************************/
/* WARNING: this file is copied by the build system.  The original is in      */
/*  the liblind directory, all others are copies.                             */ 
/******************************************************************************/


/* use stddef for ssize_t */
#include <stddef.h>

/* For ioctl bounds checking, the lowest lind ioctl number */
#define LIND_LOW_IOCTL 10001
/* IOCTL request number for component interface attach*/

#define LIND_CIA_IOCTL 10001
/* IOCTL request number for component call*/
#define LIND_CALL_IOCTL 10002
/* IOCTL request number for component accept*/
#define LIND_ACCEPT_IOCTL 10003
/* IOCTL request number for component message recieve*/
#define LIND_RECV_IOCTL 10004

/* For ioctl bounds checking, the higest lind ioctl number */
#define LIND_HIGH_IOCTL 10004

/* the component system will always give us handle 9 back for component IO */
#define COMP_FIXED_FD 9

/* Component ID */
typedef int cid;

/* Mailbox ID */
typedef int mailbox;

struct comp_accept_s {
  /* the mailbox number to wait on */
  mailbox mb;
  /* the buffer to recv into */
  void* buff;
  /* copy at most this many bytes in to the buffer */
  size_t max;
};

/* component interface attach parameters */
struct cia_s {
  /* Which component to attach to. */
  cid who;
  /* For that component, which interface to attach to. */
  mailbox interface;
};

/* Parameters to make a call to another component */
struct call_s {
  /*The other component's mailbox*/
  mailbox dest;
  /* size of the message in bytes. */
  size_t siz;
  /* location of the message to copy */
  void* message;
};

/*  Find the COMPONENT_NAME-VERSION-INTERFACE component in the system, and if it is
   not started, start it, then return the CID of the start component.  */
cid comp_start(const char * component_name, const char * version, const char * interface);

/*  Find a component with the given INTERFACE, and optionally VERSION and NAME, and
   return its CID*/
cid comp_lookup(const char * name, const char * version, const char * interface);

/* Sender: make a connection to component WHO  */
int comp_interface_attach(cid who, mailbox interface);

/* Create a mailbox for this component's INTERFACE in the system.
 TYPE is either MAILBOX_SHARED or MAILBOX_SPLIT*/
mailbox comp_create_mailbox(const char * interface, int type);

/** Wait for communication from CID in mailbox MB.
 */
cid comp_accept(mailbox mb, void* buff, size_t max);

/* Send MESSAGESIZE byte located at MESSAGE to DEST. */
int comp_call(mailbox dest, size_t messagesize, void* message);

#endif /* _COMPONENT_H_ */
