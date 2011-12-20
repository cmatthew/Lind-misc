
CC = /home/lind/tmp/repy/lind/sdk/linux_x86/bin/nacl64-gcc
CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600 -Wno-unused-parameter
INCLUDE = -I /home/lind/tmp/repy/lind/sdk/linux_x86/nacl64/include/
LDFLAGS = -melf64_nacl -m64
LIBS_PATH = /home/lind/tmp/repy/lind/sdk/linux_x86/nacl64/lib64
RT_LIBS_PATH = /home/lind/tmp/repy/lind/libs
all: installed_logger com1.nexe com2.nexe
	echo ""

installed_logger: liblogger.so
	cp liblogger.so.0.0.1 $(LIBS_PATH)
	ln -s $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so.0  
	ln -s $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so  
	cp liblogger.so.0.0.1 $(RT_LIBS_PATH)
	ln -s $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so.0  
	ln -s $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so  

liblogger.so: logger.o
	$(CC) -shared -Wl,-soname,liblogger.so.0 -o liblogger.so.0.0.1 $<

com1.nexe: simple_tester.o sostub.o liblind.o
	$(CC) $(LDFLAGS) $? -o $@

com2.nexe: installed_logger soserver.o liblind.o
	$(CC) $(LDFLAGS) -l logger soserver.o liblind.o -o $@  

simple_tester.o: simple_tester.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

logger.o: logger.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDE) $< -o $@

soserver.o: soserver.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

sostub.o: sostub.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

liblind.o: liblind.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf *.o
	rm -rf *.nexe
	rm -f liblogger.so.0.0.1
	rm -f liblogger.so
	rm -f $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so.0 $(LIBS_PATH)/liblogger.so 
	rm -f $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so.0 $(RT_LIBS_PATH)/liblogger.so 
	rm -f lind.log
