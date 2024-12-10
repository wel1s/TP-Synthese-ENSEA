#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

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
void get_command_status(int status, long elapsed_time_ms, char *status_string, size_t size) {
    if (WIFEXITED(status)) {
        snprintf(status_string, size, "[exit:%d|%ldms]", WEXITSTATUS(status), elapsed_time_ms);
    } else if (WIFSIGNALED(status)) {
        snprintf(status_string, size, "[sign:%d|%ldms]", WTERMSIG(status), elapsed_time_ms);
    } else {
        status_string[0] = '\0'; 
    }
}

// Function to execute a command, measure its execution time, and return its status
int execute_command(char *command, long *elapsed_time_ms) {
    command[strcspn(command, "\n")] = '\0';

    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime (start)");
        exit(EXIT_FAILURE);
    }

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

        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
            perror("clock_gettime (end)");
            exit(EXIT_FAILURE);
        }

        *elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
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

        long elapsed_time_ms = 0;
        int raw_status = execute_command(buffer, &elapsed_time_ms);
        get_command_status(raw_status, elapsed_time_ms, status_string, STATUS_SIZE);
    }

    display(BYE_MESSAGE);

    return 0;
}
