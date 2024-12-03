#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

// Function to display the welcome message
void display_welcome_message() {
    const char *welcome_message = "Welcome to ENSEA Shell!\n";
    const char *exit_command_message = "If you want to exit the program type 'exit'\n";
    if (write(STDOUT_FILENO, welcome_message, strlen(welcome_message)) == -1) {
        perror("error while writing welcome message");
        exit(EXIT_FAILURE);
    }
    if (write(STDOUT_FILENO, exit_command_message, strlen(exit_command_message)) == -1) {
        perror("error while writing exit command message");
        exit(EXIT_FAILURE);
    }
}

// Function to display the prompt
void display_prompt() {
    const char *prompt = "enseash> ";
    if (write(STDOUT_FILENO, prompt, strlen(prompt)) == -1) {
        perror("error while writing prompt");
        exit(EXIT_FAILURE);
    }
}

// Function to read user input
void read_user_input(char *buffer) {
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        perror("error while reading user input");
        exit(EXIT_FAILURE);
    }
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
    char buffer[BUFFER_SIZE];
    display_welcome_message();

    while (1) {
        display_prompt();
        read_user_input(buffer);
        execute_command(buffer);
    }

    return 0;
}