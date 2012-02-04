# flag values for development and debugging
#COMPILE_FLAGS = -g -Wall -c -fprofile-arcs -ftest-coverage -O0
#LINK_FLAGS = -g -fprofile-arcs -ftest-coverage

# flags for performance (standard optimization flag only, level0 I like)
COMPILE_FLAGS = -O3 -Wall -c
LINK_FLAGS = -O3

all_executables: test

# ***** the main module for part I
test: ../output/user.o ../../network/output/middle_magic.o \
		../../network/output/uds_client.o
	gcc $(LINK_FLAGS) -o test ../output/user.o ../output/tmptest.o \
		../../network/output/middle_magic.o ../../network/output/uds_client.o

../output/user.o: user.c ../output/tmptest.o #diskinfo.h parser.h execute.h
	gcc $(COMPILE_FLAGS) -o ../output/user.o user.c

../output/tmptest.o: tmptest.c ../../network/output/middle_magic.o
	gcc $(COMPILE_FLAGS) -o ../output/tmptest.o tmptest.c

../../network/output/middle_magic.o: ../../network/src/middle_magic.c ../../network/output/uds_client.o
	gcc $(COMPILE_FLAGS) -o ../../network/output/middle_magic.o ../../network/src/middle_magic.c

../../network/output/uds_client.o: ../../network/src/uds_client.c
	gcc $(COMPILE_FLAGS) -o ../../network/output/uds_client.o ../../network/src/uds_client.c

clean:
	clear
	rm -f *.o *.gcov *.gcda *.gcno stamps
