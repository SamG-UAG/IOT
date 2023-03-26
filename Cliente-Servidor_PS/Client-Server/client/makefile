CC = gcc
CFLAGS = -Wall -Werror
OUTPUT = client.out
SOURCES = client.c mqtt_frame.c
LIBS = -pthread mqtt_frame.h

all:
	$(CC) $(CFLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT) 
fresh: clean all
