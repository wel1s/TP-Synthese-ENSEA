#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define STATUS_SIZE 50

const char *WELCOME_MESSAGE = "Welcome to ENSEA Shell!\n";
const char *EXIT_COMMAND_MESSAGE = "If you want to exit the program type 'exit'\n";
const char *BYE_MESSAGE = "\nBye Bye\n";

// Function to display a message
void display(const char *message) {
    if (write(STDOUT_FILENO, message, strlen(message)) == -1) {
        perror("error while writing message");
        exit(EXIT_FAILURE);
    }
}

// Function to read user input
int read_user_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        if (feof(stdin)) { // Handle Ctrl+D 
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

// Function to process the status and return a formatted string
void get_command_status(int status, char *status_string, size_t size) {
    if (WIFEXITED(status)) {
        snprintf(status_string, size, "[exit:%d]", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        snprintf(status_string, size, "[sign:%d]", WTERMSIG(status));
    } else {
        status_string[0] = '\0';
    }
}

// Function to execute a command and return its status
int execute_command(char *command) {
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
        return status; 
    }
}

int main(void) {
    char buffer[BUFFER_SIZE];
    char status_string[STATUS_SIZE] = ""; 
    
    display(WELCOME_MESSAGE);
    display(EXIT_COMMAND_MESSAGE);

    while (1) {
        
        char prompt[BUFFER_SIZE];
        snprintf(prompt, BUFFER_SIZE, "enseash %s > ", status_string);
        
        display(prompt);

        if (!read_user_input(buffer, BUFFER_SIZE) || is_exit_command(buffer)) {
            break;
        }

        int raw_status = execute_command(buffer);
        get_command_status(raw_status, status_string, STATUS_SIZE);
    }

    display(BYE_MESSAGE);

    return 0;
}
