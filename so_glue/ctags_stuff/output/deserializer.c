#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "deserializer.h"
#include "../../network/src/uds_helper.h"


int foo_add(int x, int y) {
  printf("HelloWorld %d\n", x+y);
  return x + y;
}


/* int deserialize_foo(message * msg) { */
/* 	printf("in here\n"); */
/* 	int start, num_of_args, arg_size; */
/* 	start = 0; */
/* 	memcpy(&num_of_args, &(msg->num_of_args)[0], sizeof(num_of_args)); */
/* 	int x; */
/* 	memcpy(&arg_size, &(msg->data)[start], 4); */
/* 	start += 4; */
/* 	printf("arg size: %d\n", arg_size); */
/* 	memcpy(&x, &(msg->data)[start], 4); */
/* 	printf("this is x: %d\n", x); */

/* 	/\* gotta fill a return value *\/ */
/* 	assert(0); */
	
/* 	return 9999; */
/* } */

#define A1_TYPE int
#define A2_TYPE int

message * deserialize_foo_add(message *msg) {
  int start;
	start = 0;
	A1_TYPE *x;
	A2_TYPE *y;

	int offset = sizeof(int);

	x = (A1_TYPE *) (msg->data + offset);
	
	offset += sizeof(A1_TYPE);
	offset += sizeof(int);
	  

	y = (A2_TYPE *) (msg->data + offset) ;
	
	printf("this is x: %d\n", *x);
	printf("this is y: %d and the offset %d\n", *y, offset);

	/* make the real call: foo_add(x, y) and memcpy it into buffer */
	int ret_val;

	ret_val = foo_add(*x, *y);

	/* overwrites the first few bytes of the buffer with the return value
	 * eventually this should get it's own section 
	 */
	message * reply = malloc (MSG_SIZE);
	memset(reply, 0, MSG_SIZE);

	reply->msg_size = sizeof(ret_val);
	reply->num_of_args = 1;
	memcpy(&(reply->data)[0], &ret_val, sizeof(ret_val));
	int tmp;
	memcpy(&tmp, &(reply->data)[0], 4);
	printf("the value i WANT to return %d, %d, %d\n", ret_val, &(reply->data), tmp);
	return reply;
}

char * foo = "dont return me.";

/* this should probably return the buffer with a return value written to it */
void * des(void * buffer) {
	message * msg;
	msg = (message *) buffer;
	printf("this is the retrieved call num: %d\n", msg->call_num);
	switch (msg->call_num) {
		//case 0: return deserialize_foo(msg); break;
		case 1: 

		  return (char *) deserialize_foo_add(msg);
          
        default : assert(0);
	}
	assert(0);
	return NULL;
}
