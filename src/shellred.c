#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_TOKENS 100
#define MAX_PATH_LEN 1024
#define WC_PATH "/home/adrian/Documents/GitHub/os_project/bin/wc"
#define TEE_PATH "/home/adrian/Documents/GitHub/os_project/bin/tee"
#define YES_PATH "/home/adrian/Documents/GitHub/os_project/bin/yes"

void handle_sigint(int sig) {
    printf("\n");
    char cwd[MAX_PATH_LEN];
    getcwd(cwd, sizeof(cwd));
    char prompt[MAX_PATH_LEN + 10];
    sprintf(prompt, "%s > ", cwd);
    rl_forced_update_display();
}

void get_input_and_split(char** tokens, int* num_tokens) {
    char cwd[MAX_PATH_LEN];
    getcwd(cwd, sizeof(cwd));
    char prompt[MAX_PATH_LEN + 10];
    sprintf(prompt, "%s > ", cwd);
    char* input = readline(prompt);
    if (!input) {
        return;
    }
    if (input[0] != '\0') { // check if input is empty
        add_history(input);
    }
    *num_tokens = 0;
    char* token = strtok(input, " ");
    while (token != NULL && *num_tokens < MAX_TOKENS) {
        tokens[(*num_tokens)++] = token;
        token = strtok(NULL, " ");
    }
}

void redirect(char** tokens, int num_tokens) {
    int redirect_input = 0;
    int redirect_output = 0;
    char* input_file = NULL;
    char* output_file = NULL;
    for (int i = 0; i < num_tokens; i++) {
        if (strcmp(tokens[i], "<") == 0) {
            redirect_input = 1;
            input_file = tokens[i + 1];
            tokens[i] = NULL;
        }
        else if (strcmp(tokens[i], ">") == 0) {
            redirect_output = 1;
            output_file = tokens[i + 1];
            tokens[i] = NULL;
        }
    }
    if (redirect_input) {
        int fd = open(input_file, O_RDONLY);
        if (fd == -1) {
            perror("open input file");
            exit(1);
        }
        dup2(fd, 0);
        close(fd);
    }
    if (redirect_output) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC);
        if (fd == -1) {
            perror("open output file");
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
    }
}

int main(void) {
    char* tokens[MAX_TOKENS];
    int num_tokens;
    while (1) {
        signal(SIGINT, handle_sigint);
        get_input_and_split(tokens, &num_tokens);
        if (num_tokens > 0) {
            if (strcmp(tokens[0], "exit") == 0) {
                exit(0);
            }
            if (strcmp(tokens[0], "yes") == 0) {
                tokens[0] = YES_PATH;
            }
            if (strcmp(tokens[0], "tee") == 0) {
                tokens[0] = TEE_PATH;
            }
            if (strcmp(tokens[0], "wc") == 0) {
                tokens[0] = WC_PATH;
            }
            tokens[num_tokens] = NULL;
            pid_t pid = fork();
            if (pid == 0) {
                if (num_tokens > 0) {
                    redirect(tokens, num_tokens);
                    execvp(tokens[0], tokens);
                    perror("execvp");
                    exit(1);
                }
            }
            else {
                int status;
                wait(&status);
            }
        }
    }
}