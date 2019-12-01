CC=gcc
CFLAGS=-c
LIBS=-lpthread -lwiringPi
ALL=$(TARGET)
TARGET=home_client
OBJS=client.o display.o
all:$(ALL)
$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)
.c.o:
	$(CC) $(CFLAGS) $<
clean:
	rm -rf $(ALL) *.o
