/* use stddef for ssize_t */
#include <stddef.h>

/* Component ID */
typedef int cid;

/* Mailbox ID */
typedef int mailbox;


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
cid comp_accept(mailbox mb);


/* Send MESSAGESIZE byte located at MESSAGE to DEST. */
int comp_call(mailbox dest, size_t messagesize, void* message);


int comp_recv(void* buff, size_t max);

