CC = gcc
#CFLAGS = -Wall -Werror 
OUTPUT = server.o
SOURCES = ServidorMQTT.c com.c mqtt_frame.c
LIBS = -pthread com.h

all:
	$(CC) $(CFLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
fresh: clean all