#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const char *WELCOME_MESSAGE = "Welcome to ENSEA Shell!\n";
const char *EXIT_COMMAND_MESSAGE = "If you want to exit the program type 'exit'\n";
const char *PROMPT = "enseash> \n";

// Function to display a message in the prompt
void display(const char *message) {
    if (write(STDOUT_FILENO, message, strlen(message)) == -1) {
        perror("error while writing message");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    display(WELCOME_MESSAGE);
    display(EXIT_COMMAND_MESSAGE);
    display(PROMPT);

    return 0;
}