#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/shm.h>
// #include <curses.h>
// #include <ncurses.h>/

// 1 standy 2 battle 3 shop
int gameMode;
int alive;
int bathCooldown;
int healthEnemy;
int foodStock;
int *shopStock;
pthread_t menu, hunger, hygiene, health;
pthread_t bath;
key_t key = 1000;

typedef struct T_monster{
    char *nama;
    int hunger_status;
    int hygiene_status;
    int health_status;
}Monster;

Monster* Monster_new(char *nama){
    Monster* newMonster = (Monster*)malloc(sizeof(Monster));
    newMonster->nama = nama;
    newMonster->hunger_status = 200;
    newMonster->hygiene_status = 100;
    newMonster->health_status = 300;
}

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

void *hungerStatusTimer(void *arg){
    int timeNow = 1;
    Monster *player = (Monster *)arg;
    while(1){
        timeNow = timeNow % 10;
        if (timeNow == 0){
            player->hunger_status = player->hunger_status - 5;
            if (player->hunger_status <= 0) alive = 0;
        }
        timeNow++;
        sleep(1);
    }
}

void *hygieneStatusTimer(void *arg){
    int timeNow = 1;
    Monster *player = (Monster *)arg;
    while(1){
        timeNow = timeNow % 30;
        if (timeNow == 0){
            player->hygiene_status = player->hygiene_status - 10;
            if (player->hygiene_status <= 0) alive = 0;
        }
        timeNow++;
        sleep(1);
    }
}

void *healthStatusTimer(void *arg){
    int timeNow = 1;
    Monster *player = (Monster *)arg;
    while(1){
        if (gameMode == 1){
            timeNow = timeNow % 10;
            if (timeNow == 0){
                player->health_status = player->health_status + 5;
            }
            timeNow++;
            sleep(1);
        }else{
            continue;
        }
    }
}

void *cooldown(void *arg){
    int *cd = (int *)arg;
    while(1){
        sleep(1);
        *cd = *cd - 1;
        if (*cd == 0) break;
    }
}

void *printMenu(void *arg){
    Monster *player = (Monster*)arg;
    while(1){
        if (gameMode == 1){
            printf("Standby Mode\n");
            printf("Health : %d\n", player->health_status);
            printf("Hunger : %d\n", player->hunger_status);
            printf("Hygiene : %d\n", player->hygiene_status);
            printf("Food left : %d\n", foodStock); //shared memory
            if (bathCooldown == 0){
                printf("Bath is ready\n");
            }else{
                printf("Bath will be ready in %ds\n", bathCooldown);
            }
            printf("Choices\n");
            printf("1. Eat\n");
            printf("2. Bath\n");
            printf("3. Battle\n");
            printf("4. Shop\n");
            printf("5. Exit\n");
        }else if (gameMode == 2){
            printf("Battle Mode\n");
            printf("Monster's Health: %d\n", player->health_status);
            printf("Enemny's Health: %d\n", healthEnemy);
            printf("Choices\n");
            printf("1. Attack\n");
            printf("2. Run\n");
        }else if (gameMode == 3){
            printf("Shop Mode\n");
            printf("Shop food stock: %d\n", *shopStock);
            printf("Your food stock: %d\n", foodStock);
            printf("Choices\n");
            printf("1. Buy\n");
            printf("2. Back\n");
        }
        sleep(1);
        system("clear");
    }    
}

int main(){
    char nama[100];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    shopStock = shmat(shmid, NULL, 0);
    printf("Masukkan nama monster: ");
    scanf("%s", nama);
    gameMode = 1;
    Monster *player = Monster_new(nama);
    bathCooldown = 0;
    system("clear");
    pthread_create(&menu, NULL, &printMenu, (void *)player);
    pthread_create(&hunger, NULL, &hungerStatusTimer, (void *)player);
    pthread_create(&health, NULL, &healthStatusTimer, (void *)player);
    pthread_create(&hygiene, NULL, &hygieneStatusTimer, (void *)player);
    healthEnemy = 100;
    alive = 1;
    foodStock = 10;
    *shopStock = 5;
    changemode(1);
    while(1){
        while(!kbhit()){
            if (gameMode == 1){
                char c = getchar();
                if (c == '3'){
                    gameMode = 2;
                }else if (c == '5'){
                    shmdt(shopStock);
                    shmctl(shmid, IPC_RMID, NULL);
                    changemode(0);
                    return 0;
                }else if (c == '1'){
                    if (foodStock > 0){
                        foodStock--;
                        player->hunger_status = player->hunger_status + 15;
                    }
                }else if (c == '2'){
                    player->hygiene_status = player->hygiene_status + 30;
                    bathCooldown = 20;
                    pthread_create(&bath, NULL, &cooldown, (void*)&bathCooldown);
                }else if (c == '4'){
                    gameMode = 3;
                }
            }else if (gameMode == 2){
                char c = getchar();
                if (c == '1'){
                    healthEnemy = healthEnemy - 20;
                    if (healthEnemy <= 0){
                        healthEnemy = 0;
                        gameMode = 1;
                        healthEnemy = 100;
                        continue;
                    }
                    player->health_status = player->health_status - 20;
                    if (player->health_status <= 0){
                        alive = 0;
                    }
                }else if (c == '2'){
                    gameMode = 1;
                    healthEnemy = 100;
                }
            }else if (gameMode == 3){
                char c = getchar();
                if (c == '1'){
                    if (shopStock > 0){
                        foodStock++;
                        *shopStock = *shopStock - 1;
                    }
                }else if (c == '2'){
                    gameMode = 1;
                }
            }
        }
        if (alive == 0){
            printf("mati cuk\n");
            shmdt(shopStock);
            shmctl(shmid, IPC_RMID, NULL);
            changemode(0);
            return 0;
        }
    }
    shmdt(shopStock);
    shmctl(shmid, IPC_RMID, NULL);
    changemode(0);
    return 0;
}