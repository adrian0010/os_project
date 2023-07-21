#include <stdio.h>
#include <string.h>
int main(int arg, char* argp[]) {
    // Read from standard input
    char buffer[1024];
    int count = 0;
    while (fgets(buffer, 1024, stdin)) {
        // Write to standard output
        fputs(buffer, stdout);
        if (arg > 2) {
            // Check if first argument is "-a"
            if (strcmp(argp[1], "-a") == 0)
                for (int i = 2; i < arg; i++) {
                    FILE* file = fopen(argp[i], "a");
                    fputs(buffer, file);
                    fclose(file);
                }
        }
        else if (arg > 1) {
            for (int i = 1; i < arg; i++) {
                if (count == 0) {
                    FILE* filew = fopen(argp[i], "w");
                    fclose(filew);
                }
                FILE* file = fopen(argp[i], "a");
                fputs(buffer, file);
                fclose(file);
            }
            count++;
        }
    }
    return 0;
}