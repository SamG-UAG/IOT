CC = gcc
CFLAGS = -Wall-Werror -gcc
OUTPUT = server.o
SOURCES = server.c com.c mqtt_frames
LIBS = -pthread com.h

all:
	$(CC) $(CFLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
fresh: clean all