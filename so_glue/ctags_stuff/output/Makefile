# flag values for development and debugging
#COMPILE_FLAGS = -g -Wall -c -fprofile-arcs -ftest-coverage -O0
#LINK_FLAGS = -g -fprofile-arcs -ftest-coverage

# flags for performance (standard optimization flag only, level0 I like)
COMPILE_FLAGS = -O3 -Wall -c
LINK_FLAGS = -O3

all_executables: test

# ***** the main module for part I
test: user.o ../../network/middle_magic.o ../../network/uds_client.o
	gcc $(LINK_FLAGS) -o test user.o tagstest.o ../../network/middle_magic.o ../../network/uds_client.o

user.o: user.c tagstest.o #diskinfo.h parser.h execute.h
	gcc $(COMPILE_FLAGS) -o user.o user.c

tagstest.o: tagstest.c tagstest.h ../../network/middle_magic.o
	gcc $(COMPILE_FLAGS) -o tagstest.o tagstest.c

middle_magic.o: ../../network/middle_magic.c ../../network/uds_client.o
	gcc $(COMPILE_FLAGS) -o ../../network/middle_magic.o ../../network/middle_magic.c

uds_client.o: ../../network/uds_client.c
	gcc $(COMPILE_FLAGS) -o ../../network/uds_client.o ../../network/uds_client.c

clean:
	clear
	rm -f *.o *.gcov *.gcda *.gcno stamps
