# /home/lind/lind/native_client/toolchain/linux_x86/bin/nacl64-gcc -o scons-out/nacl-x86-64-glibc/obj/tests/lind/glibc_test.o -c -std=gnu99 -std=gnu99 -Wall -Wextra -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600 -I/home/lind/lind -I/home/lind/tmp/repy/include tests/lind/glibc_test.c

CC = nacl64-gcc
CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600
INCLUDE = -I /home/lind/tmp/repy/lind/sdk/linux_x86/nacl64/include/
LDFLAGS = -melf64_nacl -m64 -o libc.a

#CC = gcc
#CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600
#LDFLAGS = -m64


SOURCES = ~/lind/misc/benchmark/benchmark.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = benchmark.nexe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
