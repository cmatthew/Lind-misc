# /home/lind/lind/native_client/toolchain/linux_x86/bin/nacl64-gcc -o scons-out/nacl-x86-64-glibc/obj/tests/lind/glibc_test.o -c -std=gnu99 -std=gnu99 -Wall -Wextra -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600 -I/home/lind/lind -I/home/lind/tmp/repy/include tests/lind/glibc_test.c

CC = /home/lind/tmp/lind/sdk/linux_x86/bin/nacl64-gcc
CFLAGS = -c -std=gnu99 -Wall -Wextra -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -m64 -D_GNU_SOURCE=1
INCLUDE = -I /home/lind/tmp/repy/lind/sdk/linux_x86/nacl64/include/
LDFLAGS = -melf64_nacl -m64
EXECUTABLE = glibc_tester.nexe


# CC = gcc
# CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600
# LDFLAGS = -m64
#EXECUTABLE = glibc_tester


SOURCES = glibc_test.c
OBJECTS = $(SOURCES:.c=.o)



all: $(SOURCES) $(EXECUTABLE) getifadder.nexe gethostbyname.nexe recvfrom.nexe dns.nexe; 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

getifaddr_test.o: getifaddr_tester.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

getifadder.nexe: getifaddr_test.o
	$(CC) $(LDFLAGS) $^ -o $@

gethostbyname_test.o: gethostbyname.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

gethostbyname.nexe: gethostbyname_test.o
	$(CC) $(LDFLAGS) $^ -o $@

recvfrom.o: recvfrom.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

recvfrom.nexe: recvfrom.o
	$(CC) $(LDFLAGS) $^ -o $@

dns.o: dns.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

dns.nexe: dns.o
	$(CC) $(LDFLAGS) $^ -o $@

check-syntax:
	gcc $(CFLAGS) -o nul -S ${CHK_SOURCES}


clean:
	rm -rf $(EXECUTABLE) *.o