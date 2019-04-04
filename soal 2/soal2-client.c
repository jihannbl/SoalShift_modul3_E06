#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    
    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char tipe[200];
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "192.168.1.2", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    while(1){
        if (~scanf("%s", tipe)){
            send(sock , tipe , strlen(tipe) , 0 );
        }
        char message[100];
        if (recv(sock, message, 100, 0)){
            if (strcmp(message, "-1") == 0) {
                printf("Connection rejected\n");
                return 0;
            }else {
                printf("%s\n",message);
            }
        }
    }
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

//pake thread buat message handler
//thread buat 5s
//shared memory
