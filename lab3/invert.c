#include "invert.h"
#include <string.h>

void invert_line(char *line) {
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        len--;
    }
    for (int i = 0; i < len / 2; i++) {
        char temp = line[i];
        line[i] = line[len - i - 1];
        line[len - i - 1] = temp;
    }
}
