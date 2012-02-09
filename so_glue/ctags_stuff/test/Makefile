# flag values for development and debugging
#COMPILE_FLAGS = -g -Wall -c -fprofile-arcs -ftest-coverage -O0
#LINK_FLAGS = -g -fprofile-arcs -ftest-coverage

# flags for performance (standard optimization flag only, level0 I like)
COMPILE_FLAGS = -O3 -Wall -c
LINK_FLAGS = -O3
CC = gcc


all_executables: test

# ***** the main module for part I
test: ../output/user.o  \
		../../network/output/uds_client.o 
	$(CC) $(LINK_FLAGS) -o test ../output/user.o ../output/tmptest.o \
		../../network/output/middle_magic.o ../../network/output/uds_client.o \
		../output/tagstmp.o

../output/user.o: user.c ../output/tmptest.o ../output/tagstmp.o #diskinfo.h parser.h execute.h
	$(CC) $(COMPILE_FLAGS) -o ../output/user.o user.c

../output/tmptest.o: tmptest.c ../output/tagstmp.o ../../network/output/middle_magic.o
	$(CC) $(COMPILE_FLAGS) -o ../output/tmptest.o tmptest.c

../output/tagstmp.o: ../output/tagstmp_mm.c
	$(CC) $(COMPILE_FLAGS) -o ../output/tagstmp.o ../output/tagstmp_mm.c


../../network/output/uds_client.o: ../../network/src/uds_client.c ../../network/src/uds_client.h
	$(CC) $(COMPILE_FLAGS) -o ../../network/output/uds_client.o ../../network/src/uds_client.c


clean: #fix this mess
	clear
	rm -f *.o *.gcov *.gcda *.gcno stamps ../output/tagstmp_mm.*
