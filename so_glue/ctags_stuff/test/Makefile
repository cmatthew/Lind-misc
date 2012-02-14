# flag values for development and debugging
#COMPILE_FLAGS = -g -Wall -c -fprofile-arcs -ftest-coverage -O0
#LINK_FLAGS = -g -fprofile-arcs -ftest-coverage

# flags for performance (standard optimization flag only, level0 I like)
COMPILE_FLAGS = -O3 -Wall -c
LINK_FLAGS = -O3
CC = gcc

all: test

# ***** the main module for part I
test: ../output/user.o ../../network/output/uds_client.o ../output/tmptest.o ../output/tagstmp.o
	$(CC) $(LINK_FLAGS) $^ -o $@

../output/user.o: user.c
	$(CC) $(COMPILE_FLAGS) $^ -o $@

../output/tmptest.o: tmptest.c
	$(CC) $(COMPILE_FLAGS) $^ -o $@

../output/tagstmp.o: ../output/tagstmp_mm.c
	$(CC) $(COMPILE_FLAGS) $^ -o $@

../../network/output/uds_client.o: ../../network/src/uds_client.c
	$(CC) $(COMPILE_FLAGS) $^ -o $@


clean: #fix this mess
	rm -f *.o ../output/*.o ./test ../../network/src/uds_client.o
