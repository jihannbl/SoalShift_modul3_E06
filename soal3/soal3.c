#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

char orang1[10]="Agmal";
char orang2[10]="Iraj";
int WakeUp_Status;
int Spirit_Status;
int pilih, inputAgmal, inputIraj;

pthread_t tid1;
pthread_t tid2;
pthread_t tid3;
pthread_t tid4;

void* fitur(void *arg)
{
	printf("\nFitur Utama:\n");
        printf("1. Tampilkan All Status\n");
        printf("2. Agmal Ayo Bangun\n");
        printf("3. Iraj Ayo Tidur\n");
	      printf("4. Exit\n\n");
        printf("Pilih Fitur: "); scanf("%d", &pilih);
}

void* AllStatus(void *arg)
{
	printf("\nSemua Status: \n");
	printf("Agmal : %d\n", WakeUp_Status);
	printf("Iraj : %d\n\n", Spirit_Status);
}

void* Agmal_Bangun(void *arg)
{
	if(inputIraj == 3)
        {
                sleep(10);
		inputIraj = 0;
        }
	else
	{
		WakeUp_Status += 15;
//		printf("wakeup nambah %d\n", WakeUp_Status);
	}
}

void* Iraj_Tidur(void *arg)
{
	if(inputAgmal == 3)
	{
		sleep(10);
		inputAgmal = 0;
	}
	else
	{
		Spirit_Status -= 20;
//		printf("spirit kurang %d\n", Spirit_Status);
	}
}

int main()
{
	WakeUp_Status = 0;
	Spirit_Status = 100;

	while(1)
	{
		if(WakeUp_Status >= 100)
		{
			printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
			break;
		}
		else if(Spirit_Status <= 0)
		{
			printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
			break;
		}

		pthread_create(&(tid1), NULL, fitur, NULL);
		pthread_join(tid1, NULL);

		if(pilih==1)
		{
			pthread_create(&(tid2), NULL, AllStatus, NULL);
			pthread_join(tid2, NULL);
		}

		else if(pilih==2)
		{
			if(inputIraj == 3)
                        {
                                printf("Fitur Agmal Ayo Bangun disabled 10 s\n");
                                pthread_create(&(tid3), NULL, Agmal_Bangun, NULL);
                        }
			else
			{
				pthread_create(&(tid3), NULL, Agmal_Bangun, NULL);
				pthread_join(tid3, NULL);
				inputAgmal++;
			}
		}

		else if(pilih==3)
		{
			if(inputAgmal == 3)
			{
				printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
				pthread_create(&(tid4), NULL, Iraj_Tidur, NULL);
			}
			else
			{
				pthread_create(&(tid4), NULL, Iraj_Tidur, NULL);
				pthread_join(tid4, NULL);
				inputIraj++;
			}
		}
		else if(pilih==4)
		{
			exit(0);
		}
		else
		{
			printf("Input Salah\n");
		}
	}
	return 0;
}
