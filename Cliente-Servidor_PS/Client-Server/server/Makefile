CC = gcc
CFLAGS = -Wall -Werror -g
OUTPUT = Server.out
SOURCES = Server.c com.c mqtt_frame.c
LIBS = -pthread com.h mqtt_frame.h
# $(CFLAGS)
all:
	$(CC) $(LIBS) $(SOURCES) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT) 
fresh: clean all
