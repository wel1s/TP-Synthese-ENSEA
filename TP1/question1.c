#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Function to display the welcome message
void display_welcome_message() {
    const char *welcome_message = "Welcome to ENSEA Shell!\n";
    const char *exit_command_message = "If you want to exit the program type 'exit'\n";
    if (write(STDOUT_FILENO, welcome_message, strlen(welcome_message)) == -1) {
        perror("error while writting welcome message");
        exit(EXIT_FAILURE);
    }
    if (write(STDOUT_FILENO, exit_command_message, strlen(exit_command_message)) == -1) {
        perror("error while writting exit command message");
        exit(EXIT_FAILURE);
    }
}

// Function to display the prompt
void display_prompt() {
    const char *prompt = "enseash> ";
    if (write(STDOUT_FILENO, prompt, strlen(prompt)) == -1) {
        perror("error while writting prompt");
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

// Function to check if the user wants to exit
int is_exit_command(const char *buffer) {
    return strncmp(buffer, "exit", 4) == 0 && (buffer[4] == '\n' || buffer[4] == '\0');
}

int main(void) {
    
    char buffer[BUFFER_SIZE];
    display_welcome_message();

    while (1) {
        display_prompt();

        read_user_input(buffer);

        if (is_exit_command(buffer)) {
            break;
        }
    }

    return 0;
}