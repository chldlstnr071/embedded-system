CC=gcc
CFLAGS=-c
LIBS=-lpthread -lwiringPi
ALL=$(TARGET)
TARGET=home_server
OBJS=homeapp.o server.o temperature.o sonar.o
all:$(ALL)
$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)
.c.o:
	$(CC) $(CFLAGS) $<
clean:
	rm -rf $(ALL) *.o
