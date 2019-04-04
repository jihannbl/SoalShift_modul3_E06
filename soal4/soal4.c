#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

pthread_t tid1;
pthread_t tid2;
pthread_t tid3;
pthread_t tid4;
pthread_t tid5;
pthread_t tid6;

int status1, status2;
void* save1 (void *arg)
{
	status1 = 0;
	chdir("/home/jihan/Document/FolderProses1");
	system("ps -aux | tail -10 > SimpanProses1.txt");
	status1 = 1;
}

void* save2 (void *arg)
{
	status2 = 0;
	chdir("/home/jihan/Document/FolderProses2");
	system("ps -aux | tail -10 > SimpanProses2.txt");
	status2 = 1;
}

void* zip1 (void *arg)
{
	while(status1 != 1)
    	{

    	}
	chdir("/home/jihan/Document/FolderProses1");
	system("zip KompresProses1.zip SimpanProses1.txt && rm SimpanProses1.txt");
	status1=2;
}

void* zip2 (void *arg)
{
	while(status2 != 1)
        {

        }
	chdir("/home/jihan/Document/FolderProses2");
	system("zip KompresProses2.zip SimpanProses2.txt && rm SimpanProses2.txt");
	status2=2;
}

void* unzip1 (void *arg)
{
        while(status1 != 2)
        {

        }
	sleep(15);
        chdir("/home/jihan/Document/FolderProses1");
        system("unzip KompresProses1.zip");
}

void* unzip2 (void *arg)
{
        while(status2 != 2)
        {

        }
	sleep(15);
        chdir("/home/jihan/Document/FolderProses2");
        system("unzip KompresProses2.zip");
}

int main(void)
{
	pthread_create(&(tid1), NULL, save1, NULL);
  pthread_create(&(tid2), NULL, save2, NULL);
	pthread_create(&(tid3), NULL, zip1, NULL);
  pthread_create(&(tid4), NULL, zip2, NULL);
	pthread_create(&(tid5), NULL, unzip1, NULL);
	pthread_create(&(tid6), NULL, unzip2, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);
	pthread_join(tid5, NULL);
  pthread_join(tid6, NULL);
    return 0;
}
