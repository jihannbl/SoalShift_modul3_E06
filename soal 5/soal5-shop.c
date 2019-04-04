#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>
#include <sys/time.h>

pthread_t menu;

void changemode(int dir){
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 
int kbhit (void){
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

void *printMenu(void *arg){
    int *stock = (int *)arg;
    while(1){
        printf("Shop\n");
        printf("Food stock: %d\n", *stock);
        printf("Choices\n");
        printf("1. Restock\n");
        printf("2. Exit\n");
        sleep(1);
        system("clear");
    }    
}

int main(){
    key_t key = 1000;
    int *stock;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);
    pthread_create(&menu, NULL, &printMenu, (void *)stock);
    changemode(1);
    while(1){
        while(!kbhit()){
            char c = getchar();
            if (c == '1'){
                *stock = *stock + 1;
            }else if (c == '2'){
                shmdt(stock);
                shmctl(shmid, IPC_RMID, NULL);
                changemode(0);
                return 0;
            }
        }
    }
    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
    changemode(0);
    return 0;
}