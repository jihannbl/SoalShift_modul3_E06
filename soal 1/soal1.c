#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int arg[500];
pthread_t thread[500];
int status;

void *factorial(void *num){
	long long ans = 1;
	int fak = *((int *)num);
	pthread_t id = pthread_self();
	for (int i = 1; i <= fak; i++){
		ans = ans * i;
	}
	while (!pthread_equal(id, thread[status]));
	printf("%d! = %lld\n",fak, ans);
	status++;
}

int main(int argc, char **argv){
    	for (int i = 1; i < argc; ++i){
		arg[i-1] = atoi(argv[i]);
	}
	status = 0;
	qsort(arg, argc-1, sizeof(int), cmpfunc);
	for (int i = 0; i < argc-1; i++) {
		int err = pthread_create(&(thread[i]), NULL, &factorial, &(arg[i]));
		if (err != 0){
			printf("\n can't create thread for factorial %d : [%s]",arg[i], strerror(err));
		}
	}
	for (int i = 0; i < argc-1; i++) pthread_join(thread[i], NULL);
	return 0;
}
