#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

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



int main(int argc, char *argv[]) {
    const char *server_name;
    const char *server_port;
    const char *filename;

    get_request_info(argc, argv, &server_name, &server_port, &filename);
    struct addrinfo *server_info = get_server_address(server_name, server_port);

    const struct sockaddr_in *addr = (const struct sockaddr_in *)server_info->ai_addr;
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);
    printf("Server IP: %s\n", ip);
    printf("Server Port: %d\n", ntohs(addr->sin_port));

    freeaddrinfo(server_info);
    return 0;
}