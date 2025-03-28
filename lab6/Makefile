CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SERVER_TARGET = server
CLIENT_TARGET = client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): server.c
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) server.c

$(CLIENT_TARGET): client.c
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) client.c

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)

.PHONY: all clean
