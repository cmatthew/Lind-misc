# Build the so_glue system. There is a server, which this compiles and a client library (someday).


#CC = nacl64-gcc
#CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -Werror -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600
#INCLUDE = -I /home/lind/tmp/repy/lind/sdk/linux_x86/nacl64/include/
#LDFLAGS = -melf64_nacl -m64

CC = gcc
CFLAGS = -c -std=gnu99 -Wall -Wextra -Wno-long-long -O2 -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600
LDFLAGS = -m64


SOURCES = uds_server.c ../../ctags_stuff/output/deserializer.c ../../ctags_stuff/output/fake_implementation.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ../output/uds_server

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lcrypt

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(OBJECTS)
