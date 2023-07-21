#include <stdio.h>
#include <string.h>


int main(int arg, char* argp[]) {
    // Initialize variables to hold counts
    int lines = 0, words = 0, bytes = 0, max_length = 0;
    int total_lines = 0, total_words = 0, total_bytes = 0, total_max_length = 0;

    int current_length = 0;
    int flag_l = 0, flag_w = 0, flag_c = 0, flag_L = 0;
    int inword = 0;
    // Parse command line arguments
    for (int i = 1; i < arg; i++) {
        if (argp[i][0] == '-') {
            for (int j = 1; j < strlen(argp[i]); j++) {
                switch (argp[i][j]) {
                case 'c':
                    flag_c = 1;
                    break;
                case 'w':
                    flag_w = 1;
                    break;
                case 'l':
                    flag_l = 1;
                    break;
                case 'L':
                    flag_L = 1;
                    break;
                default:
                    fprintf(stderr, "wc: invalid option -- '%c'\n", argp[i][j]);
                    return 1;
                }
            }
        }
        else {
            // Open the file
            FILE* file = fopen(argp[i], "r");
            if (file == NULL) {
                fprintf(stderr, "wc: %s: No such file or directory\n", argp[i]);
                return 1;
            }

            // Count lines, words, bytes, max_length in the file
            int c;
            while ((c = fgetc(file)) != EOF) {
                current_length++;
                if (c == '\n') {
                    if (flag_L) {
                        max_length = (max_length > current_length) ? max_length : current_length;
                        current_length = 0;
                    }
                    lines++;
                }
                if (c == ' ' || c == '\t' || c == '\n') {
                    inword = 0;
                }
                else if (!inword) {
                    inword = 1;
                    words++;
                }
                bytes++;
            }

            if (!flag_L) current_length = 0;

            // Print counts
            if (flag_l) printf("%d ", lines);
            if (flag_w) printf("%d ", words);
            if (flag_c) printf("%d ", bytes);
            if (flag_L) printf("%d ", max_length);
            if (!(flag_l || flag_w || flag_c || flag_L)) printf("%d %d %d ", lines, words, bytes);
            printf("%s\n", argp[i]);

            // Add the counts tothe global variables
            total_lines += lines;
            total_words += words;
            total_bytes += bytes;
            total_max_length += max_length;
            fclose(file);
        }
    }

    // Print the total counts if multiple files were inputted
    if (arg > 2) {
        if (flag_l) printf("%d ", total_lines);
        if (flag_w) printf("%d ", total_words);
        if (flag_c) printf("%d ", total_bytes);
        if (flag_L) printf("%d ", total_max_length);
        if (!(flag_l || flag_w || flag_c || flag_L)) printf("%d %d %d ", total_lines, total_words, total_bytes);
        printf("total\n");
    }

    return 0;
}