#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "deserializer.h"
#include "../../network/src/uds_helper.h"
#include "fake_implementation.h"

message * deserialize_foo(message *msg) {
	int start;
	start = 0;
	int  *x;

	int offset = sizeof(int);

	x = (int  *) (msg->data + offset);

	offset += sizeof(int  );
	offset += sizeof(int);

	int ret_val;

	ret_val = foo(*x);

	message * reply = malloc (MSG_SIZE);
	memset(reply, 0, MSG_SIZE);

	reply->msg_size = sizeof(ret_val);
	reply->num_of_args = 1;
	memcpy(&(reply->data)[0], &ret_val, sizeof(ret_val));

	return reply;
}


message * deserialize_foo_add(message *msg) {
	int start;
	start = 0;
	int  *x;
	int  *y;

	int offset = sizeof(int);

	x = (int  *) (msg->data + offset);

	offset += sizeof(int  );
	offset += sizeof(int);

	y = (int  *) (msg->data + offset);

	offset += sizeof(int  );
	offset += sizeof(int);

	int ret_val;

	ret_val = foo_add(*x,*y);

	message * reply = malloc (MSG_SIZE);
	memset(reply, 0, MSG_SIZE);

	reply->msg_size = sizeof(ret_val);
	reply->num_of_args = 1;
	memcpy(&(reply->data)[0], &ret_val, sizeof(ret_val));

	return reply;
}


message * deserialize_no_comments(message *msg) {
	int start;
	start = 0;
	char *  *string;

	int offset = sizeof(int);

	string = (char *  *) (msg->data + offset);

	offset += sizeof(char *  );
	offset += sizeof(int);

	int ret_val;

	ret_val = no_comments(*string);

	message * reply = malloc (MSG_SIZE);
	memset(reply, 0, MSG_SIZE);

	reply->msg_size = sizeof(ret_val);
	reply->num_of_args = 1;
	memcpy(&(reply->data)[0], &ret_val, sizeof(ret_val));

	return reply;
}


message * deserialize_one_line(message *msg) {
	int start;
	start = 0;
	char *  *string;

	int offset = sizeof(int);

	string = (char *  *) (msg->data + offset);

	offset += sizeof(char *  );
	offset += sizeof(int);

	int ret_val;

	ret_val = one_line(*string);

	message * reply = malloc (MSG_SIZE);
	memset(reply, 0, MSG_SIZE);

	reply->msg_size = sizeof(ret_val);
	reply->num_of_args = 1;
	memcpy(&(reply->data)[0], &ret_val, sizeof(ret_val));

	return reply;
}


void * des (void * buffer) {
	message *msg;
	msg = (message *) buffer;
	switch  (msg->call_num) {
		case 0:
			 return (void *) deserialize_foo(msg);
		case 1:
			 return (void *) deserialize_foo_add(msg);
		case 2:
			 return (void *) deserialize_no_comments(msg);
		case 3:
			 return (void *) deserialize_one_line(msg);
			default : assert(0);
		}
	assert(0);
	return NULL;
}