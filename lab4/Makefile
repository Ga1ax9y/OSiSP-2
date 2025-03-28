CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = lab4

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) counter.txt

.PHONY: all clean
