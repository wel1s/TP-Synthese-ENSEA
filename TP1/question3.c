#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

const char *WELCOME_MESSAGE = "Welcome to ENSEA Shell!\n";
const char *EXIT_COMMAND_MESSAGE = "If you want to exit the program type 'exit'\n";
const char *PROMPT = "enseash> ";
const char *BYE_MESSAGE = "\nBye Bye\n";
char buffer[BUFFER_SIZE];


// Function to display a message in the prompt
void display(const char *message) {
    if (write(STDOUT_FILENO, message, strlen(message)) == -1) {
        perror("error while writing message");
        exit(EXIT_FAILURE);
    }
}

// Function to read user input
int read_user_input(char *buffer) {
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        if (feof(stdin)) { // handle Ctrl+D 
            return 0; 
        } else {
            perror("error while reading user input");
            exit(EXIT_FAILURE);
        }
    }
    return 1; 
}

// Function to check if the user wants to exit
int is_exit_command(const char *buffer) {
    return strncmp(buffer, "exit", 4) == 0 && (buffer[4] == '\n' || buffer[4] == '\0');
}


// Function to execute a command
void execute_command(char *command) {
    command[strcspn(command, "\n")] = '\0';

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execlp(command, command, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
}

int main(void) {
    
    display(WELCOME_MESSAGE);
    display(EXIT_COMMAND_MESSAGE);

    while (1) {
        display(PROMPT);
        if (!read_user_input(buffer) || is_exit_command(buffer)) {
            break;
        }
        execute_command(buffer);
    }

    display(BYE_MESSAGE);

    return 0;
}