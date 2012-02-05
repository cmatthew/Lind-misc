

/* these might be useful at some point */
int MSG_SIZE = 4096;
int MSG_SIZE_BUF = 10;
int CALL_NUM_BUF = 10;
int VERSION_NUM_BUF = 10;
int FLAGS_BUF = 10;
int NUM_OF_ARGS_BUF = 10;
int DATA_BUF = 4046;



typedef struct {
	char msg_size[10];
	char call_num[10];
	char version_num[10];
	char flags[10];
	char num_of_args[10];
	char data[4046];/* <sizeof arg[10], arg[atoi(sizeof arg)]> */
} message;
