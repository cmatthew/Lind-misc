
struct message {
  int call;
  int len;
  int priority;
  char c;
};

/*Write this message to the log. */
int log_message(int priority, char * message);

void test(void);
