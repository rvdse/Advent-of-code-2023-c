TARGET=a.out
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
CC=cc
CFLAGS=-O2

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

all: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

.PHONY: all clean
