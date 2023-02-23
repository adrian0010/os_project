#include <stdio.h>
int main(int argc, char* argv[]) {
    // Check if we have more than one argument
    if (argc > 1) {
        // Starts infinite loop
        while (1) {
            // Print the first command argument
            printf("%s\n", argv[1]);
        }
    }
    //If there is only one argument or none
    else {
        // Starts infinite loop
        while (1) {
            // Print "y"
            printf("y\n");
        }
    }
}