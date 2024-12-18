#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>


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

struct addrinfo* get_server_address(const char *server_name, const char *server_port) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status = getaddrinfo(server_name, server_port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    return res;
}

int create_socket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) erreur("Erreur lors de la création du socket");
    printf("Socket créée avec succès.\n");
    return sockfd;
}



int main(int argc, char *argv[]) {
    const char *server_name;
    const char *server_port;
    const char *filename;

    get_request_info(argc, argv, &server_name, &server_port, &filename);
    struct addrinfo *server_info = get_server_address(server_name, server_port);
    int sockfd = create_socket();

    freeaddrinfo(server_info);
    close(sockfd);
    return 0;
}