# /home/lind/lind/native_client/toolchain/linux_x86/bin/nacl64-gcc -o scons-out/nacl-x86-64-glibc/obj/tests/lind/glibc_test.o -c -std=gnu99 -std=gnu99 -Wall -Wextra -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600 -I/home/lind/lind -I/home/lind/tmp/repy/include tests/lind/glibc_test.c

CC = ~/tmp/lind/sdk/linux_x86/bin/nacl64-gcc
CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600
INCLUDE = -I /home/lind/tmp/lind/sdk/linux_x86/nacl64/include/
LDFLAGS = -melf64_nacl -m64
INTERP = lind

ifeq ($(type),c)
	CC = gcc
	LDFLAGS = -m64
	INCLUDE =
	INTERP = strace 
endif


SOURCES = server.c client.c common.c
OBJECTS = $(SOURCES:.c=.o)
SERVER = nettest-server
CLIENT = nettest-client

all: $(SERVER) $(CLIENT)
	

$(SERVER): server.o common.o
	$(CC) $(LDFLAGS) $^ -o $@

$(CLIENT): client.o common.o
	$(CC) $(LDFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf *.o $(SERVER) $(CLIENT)

test:
	$(INTERP) ./$(SERVER) &
	sleep 2
	$(INTERP) ./$(CLIENT)
	killall $(SERVER)

