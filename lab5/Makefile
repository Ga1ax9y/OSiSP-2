CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
TARGET = lab5

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)

.PHONY: all clean
