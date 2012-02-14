#include <stdlib.h>



#ifndef _UDS_HELPER_H_
#define _UDS_HELPER_H_

/* these might be useful at some point */
#define _MSG_SIZE 4096

extern int MSG_SIZE;

//int MSG_SIZE_BUF = 10;
//int CALL_NUM_BUF = 10;
//int VERSION_NUM_BUF = 10;
//int FLAGS_BUF = 10;
//int NUM_OF_ARGS_BUF = 10;
//int DATA_BUF = 4046;

/* The name of our linux domain socket */
#define SOCK_PATH "demo_socket" 


typedef struct {
	int msg_size;
	unsigned int call_num;
	unsigned int version_num;
	unsigned int flags;
	unsigned int num_of_args;
    char data[_MSG_SIZE - (sizeof(unsigned int) * 4) - sizeof(int)];/* <sizeof arg[10], arg[atoi(sizeof arg)]> */
} message;


#endif /* _UDS_HELPER_H_ */




