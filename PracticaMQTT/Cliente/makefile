CC = gcc
#CFLAGS = -Wall -Werror
OUTPUT = client.out
SOURCES = ClienteMQTT.c mqtt_frame.c
LIBS = -pthread MQTT_FRAME.h

all:
	$(CC) $(CFLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT) 
fresh: clean all
