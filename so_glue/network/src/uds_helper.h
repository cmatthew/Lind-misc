

int MSG_SIZE = 4096;

typedef struct {
	char msg_size[10];
	char call_num[10];
	char version_num[10];
	char flags[10];
	char num_of_args[10];
	char data[4046];/* <sizeof arg, arg> */
} message;
