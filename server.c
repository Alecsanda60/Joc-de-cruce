#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5555
#define MAX_PLAYERS 4

int clients[MAX_PLAYERS];
int client_count = 0;

void Impartire_carti (int *carti_amestecate, int carti_jucatori[4][6])
{
    srand(time(NULL));
    
    for (int i = 22; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = carti_amestecate[i];
        carti_amestecate[i] = carti_amestecate[j];
        carti_amestecate[j] = temp;
    }

    for(int i = 0; i < 24; i=i+4)
    {
        carti_jucatori[1][i/4] = carti_amestecate[i];
        carti_jucatori[2][i/4] = carti_amestecate[i+1];
        carti_jucatori[3][i/4] = carti_amestecate[i+2];
        carti_jucatori[0][i/4] = carti_amestecate[i+3];
    }
}

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
            buffer[0] = '0' + client_count%4;
            buffer[1] = '\0';
            send(client_sock, buffer, 2, 0);
    }

    printf("All players connected. Starting the game...\n");
    strcpy(buffer, "Start");
    for (int i = 0; i < client_count; i++) 
    {
        send(clients[i], buffer, 1024, 0);
    }

    int carti_de_joc[24];
    for (int i = 0; i < 24; i++)
    {
        carti_de_joc[i] = i;
    }
    
    // Primire punctajul la care se termina jocul
    recv(clients[0], buffer, 1024, 0);
    int punctaj_ales = buffer[0] * 10 + 1;
    for (int i = 1; i < client_count; i++) 
    {
        send(clients[i], buffer, strlen(buffer), 0);
    }

    // Inlantuirea jocului
    int carti_jucatori[4][6];
    int runda=1;
    int punctaj_echipa_impara=0, punctaj_echipa_para=0;

    while(punctaj_echipa_impara < punctaj_ales && punctaj_echipa_para < punctaj_ales)
    {
        //Impartirea cartilor
        Impartire_carti(carti_de_joc, carti_jucatori);
        for (int i = 0; i < 4; i++) 
        {
            send(clients[i], carti_jucatori, sizeof(carti_jucatori), 0);
        }

        // Cate face fiecare jucator
        int cate_faci[4], cine_va_face;
        for(int j=0; j<4; j++)
        {
            recv(clients[(j+runda)%2], cate_faci, sizeof(cate_faci), 0);
            recv(clients[(j+runda)%2], &cine_va_face, sizeof(cine_va_face), 0);
            for (int i = 1; i < client_count; i++) 
            {
                send(clients[(i+runda)%2], cate_faci, sizeof(cate_faci), 0);
                send(clients[(i+runda)%2], &cine_va_face, sizeof(cine_va_face), 0);
            }
        }

    }
    close(server_sock);
    return 0;
}
