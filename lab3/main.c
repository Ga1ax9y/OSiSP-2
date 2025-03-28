#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "invert.h"

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;
    int opt;
    extern char *optarg;
    extern int optind;

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                input = fopen(optarg, "r");
                if (!input) {
                    perror("Failed to open input file");
                    return 1;
                }
                break;
            case 'o':
                output = fopen(optarg, "w");
                if (!output) {
                    perror("Failed to open output file");
                    return 1;
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-i input_file] [-o output_file]\n", argv[0]);
                return 1;
        }
    }

    char line[1024];
    while (fgets(line, sizeof(line), input)) {
        invert_line(line);
        fputs(line, output);
    }

    if (input != stdin) {
        fclose(input);
    }
    if (output != stdout) {
        fclose(output);
    }

    return 0;
}
