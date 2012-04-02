CC = /home/lind/tmp/lind/sdk/linux_x86/bin/nacl64-gcc

CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600

INCLUDE = -I /home/lind/tmp/lind/repy/lind/sdk/linux_x86/nacl64/include/

LDFLAGS = -melf64_nacl -m64

EXECUTABLE = myclient

SOURCES = myclient.c

OBJECTS = $(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o
