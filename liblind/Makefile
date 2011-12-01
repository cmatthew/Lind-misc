
CC = /home/lind/tmp/repy/lind/sdk/linux_x86/bin/nacl64-gcc
CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600 -Wno-unused-parameter
INCLUDE = -I /home/lind/tmp/repy/lind/sdk/linux_x86/nacl64/include/
LDFLAGS = -melf64_nacl -m64


all: com1.nexe com2.nexe
	

com1.nexe: simple_tester.o liblind.o
	$(CC) $(LDFLAGS) $? -o $@

com2.nexe: logger.o liblind.o
	$(CC) $(LDFLAGS) $? -o $@

simple_tester.o: simple_tester.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

logger.o: logger.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

liblind.o: liblind.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf *.o
