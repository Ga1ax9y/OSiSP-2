CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =

SRCS = main.c invert.c
OBJS = $(SRCS:.c=.o)
TARGET = lab3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@echo "Running tests..."
	@./$(TARGET) -i lab3_input.txt -o lab3_output.txt
	@echo "Test completed. Check lab3_output.txt for results."
