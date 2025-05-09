#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5555
#define MAX_PLAYERS 4

int clients[MAX_PLAYERS];
int client_count = 0;


int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024];

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, MAX_PLAYERS) < 0) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d. Waiting for players...\n", PORT);

    while (client_count<MAX_PLAYERS) {
            memset(buffer, 0, sizeof(buffer));
            client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
            if (client_sock < 0) {
                perror("Accept failed");
                continue;
            }

            clients[client_count++] = client_sock;
            buffer[0] = '0' + client_count;
            buffer[1] = '\0';
            send(client_sock, buffer, 2, 0);
    }


    close(server_sock);
    return 0;
}
