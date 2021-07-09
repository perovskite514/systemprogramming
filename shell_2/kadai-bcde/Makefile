CC = gcc
CFLAGS = -Wall -O -g
SRCS = $(shell find . -name  "*.c")
OBJS = $(SRCS:.c=.o)

TARGET = ish

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) $(OBJS) *~