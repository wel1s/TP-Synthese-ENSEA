#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void erreur(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

void get_request_info(int argc, char *argv[], const char **server_name, const char **server_port, const char **filename) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_name> <server_port> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    *server_name = argv[1];
    *server_port = argv[2];
    *filename = argv[3];
}

int main(int argc, char *argv[]) {
    const char *server_name;
    const char *server_port;
    const char *filename;

    get_request_info(argc, argv, &server_name, &server_port, &filename);

    printf("Server Name: %s\n", server_name);
    printf("Server Port: %s\n", server_port);
    printf("Filename: %s\n", filename);

    return 0;
}