#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

const char *morse_table[256] = {
    [ 'A' ] = ".-",   [ 'B' ] = "-...", [ 'C' ] = "-.-.", [ 'D' ] = "-..",
    [ 'E' ] = ".",    [ 'F' ] = "..-.", [ 'G' ] = "--.",  [ 'H' ] = "....",
    [ 'I' ] = "..",   [ 'J' ] = ".---", [ 'K' ] = "-.-",  [ 'L' ] = ".-..",
    [ 'M' ] = "--",   [ 'N' ] = "-.",   [ 'O' ] = "---",  [ 'P' ] = ".--.",
    [ 'Q' ] = "--.-", [ 'R' ] = ".-.",  [ 'S' ] = "...",  [ 'T' ] = "-",
    [ 'U' ] = "..-",  [ 'V' ] = "...-", [ 'W' ] = ".--",  [ 'X' ] = "-..-",
    [ 'Y' ] = "-.--", [ 'Z' ] = "--..",
    [ '1' ] = ".----", [ '2' ] = "..---", [ '3' ] = "...--", [ '4' ] = "....-",
    [ '5' ] = ".....", [ '6' ] = "-....", [ '7' ] = "--...", [ '8' ] = "---..",
    [ '9' ] = "----.", [ '0' ] = "-----"
};

void text_to_morse(const char *text, char *morse) {
    morse[0] = '\0';
    for (int i = 0; text[i]; i++) {
        char c = toupper(text[i]);
        if (morse_table[c]) {
            strcat(morse, morse_table[c]);
            strcat(morse, " ");
        } else if (c == ' ') {
            strcat(morse, "/ ");
        }
    }
}

void morse_to_text(const char *morse, char *text) {
    text[0] = '\0';
    char *token = strtok((char *)morse, " ");
    while (token) {
        for (int i = 0; i < 256; i++) {
            if (morse_table[i] && strcmp(morse_table[i], token) == 0) {
                strncat(text, (char[]){i, '\0'}, 1);
                break;
            }
        }
        token = strtok(NULL, " ");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected\n");

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);

            int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
            if (bytes_read <= 0) {
                printf("Client disconnected\n");
                break;
            }

            printf("Received: %s\n", buffer);


            char response[BUFFER_SIZE] = {0};
            if (strstr(buffer, "TEXT:")) {
                text_to_morse(buffer + 5, response);
            } else if (strstr(buffer, "MORSE:")) {
                morse_to_text(buffer + 6, response);
            } else if (strstr(buffer, "exit")) {
                printf("Client requested to exit\n");
                break;
            } else {
                strcpy(response, "Invalid request");
            }

            send(new_socket, response, strlen(response), 0);
            printf("Sent: %s\n", response);
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
