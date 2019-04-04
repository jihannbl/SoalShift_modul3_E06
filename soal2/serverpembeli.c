#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PORT 8000

int total_connection = 0;
key_t key = 1234;
int *value;

void *message(void *arg){
    char message[100];
    int socket = *((int *)arg);
    int valread;
    while( valread = recv(socket, message, 100, 0) > 0){
        // printf("%s\n", message);
        if (strcmp(message, "beli") == 0){
            if (*value > 0){
                *value = *value - 1;
                send(socket, "transaksi berhasil", sizeof("transaksi berhasil"), 0);
            }else{
                send(socket, "transaksi gagal", sizeof("transaksi gagal"), 0);
            }
        }else{
            send(socket, "transaksi gagal", sizeof("transaksi gagal"), 0);
        }
        memset(message, 0, sizeof(message));
    }
    if (valread == 0){
        printf("Client disconnects.\n");
    }
    total_connection--;
}

int main(int argc, char const *argv[]) {

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    // *value = 10;

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *reject = "-1";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        if (total_connection == 1){
            send(new_socket, reject, sizeof(reject), 0);
            close(new_socket);
        }else{
            printf("New Clients detected.\n");
            total_connection++;
            int *socket = (int*)malloc(sizeof(int));
            *socket = new_socket;
            pthread_t messageHandler;
            pthread_create(&messageHandler, NULL, &message, (void *)socket);
        }
    }
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
