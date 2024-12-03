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
    const char *prompt = "enseash> \n";
    if (write(STDOUT_FILENO, prompt, strlen(prompt)) == -1) {
        perror("error while writting prompt");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    
    display_welcome_message();
    display_prompt();
    return 0;
}