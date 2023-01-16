#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>

sem_t* semaphore;
pid_t other;
sigset_t sigset;
pid_t pid;

int resource;//1 or 2 depending on who changes it

void signalHandler1(int sigNum)
{
	printf("SIGNAL HANDLER 1: Read Signal: %d\n", sigNum);
	printf("SIGNAL HANDLER 1: exiting\n");
	sem_post(semaphore);
	_exit(0);
}

void signalHandler2()
{
	printf("SIGNAL HANDLER 2: I'M ALIVE\n");
}

void *changeResource(int* num)
{
	//Both processes have access to this function
	//It alows them to change the value of the resource integer
	
	//WHen attempting to obtain the semaphore, it checks it's value first.
	printf("attempting to obtain semaphore...");
	int value;
	sem_getvalue(semaphore, &value);
	
	//If it's not available, it prompts the user to attempt a release or do nothing
	if (value == 0){
		printf("semaphore not available\n");
		printf("release semaphore?\n");
		printf("0 = No, 1 = Yes\n");
		
		int input = -1;
		while (input != 1 && input != 0){
			scanf("%d", &input);
			//Exits if user chooses not to attempt release
			if (input == 0){
				printf("not attempting to obtain semaphore...\n");
				printf("exiting...\n");
				_exit(0);
			}
			//Kills other process to obtain the semaphore if user decides to release it
			else if (input == 1){
				if (sem_trywait(semaphore) != 0){
					pthread_t tid1;
					
					printf("PARENT PROCESS: killing child process\n");
					kill(other, SIGUSR1);
					printf("PARENT PROCESS: child process killed\n");
					printf("PARENT PROCESS: testing if child process killed\n");
					sleep(5);
					kill(other, SIGUSR2);
					sleep(1);
					printf("PARENT PROCESS: done testing if child process killed\n");
					
					sem_getvalue(semaphore, &value);
					printf("PARENT PROCESS: semaphore value of %d\n", value);
					if (sem_trywait(semaphore) != 0){
						if (value == 0){
							sem_post(semaphore);
						}
						printf("PARENT PROCESS: got semaphore\n");
						//When obtained, change resource value
						printf("Semaphore obtained\n");
						resource = *num;
						printf("resource changed to %d\n", resource);
						if (pid == 0){
							printf("Sleeping...\n");
							sleep(60);
						}
						sem_post(semaphore);
						printf("Semaphore released\n");
					}
					//When obtained, change resource value
					else{
						printf("PARENT PROCESS: got semaphore\n");
						printf("Semaphore obtained\n");
						resource = *num;
						printf("resource changed to %d\n", resource);
						if (pid == 0){
							printf("Sleeping...\n");
							sleep(60);
						}
						sem_post(semaphore);
						printf("Semaphore released\n");
					}	
				}
			}
			//Only inputs of 1 or 0 are accepted
			else{
				printf("Please input 1 or 0\n");
			}
		}
		
		
		
	}
	//If semaphore is available, obtain it and change the resource value
	else{
		sem_wait(semaphore);
		sem_getvalue(semaphore, &value);
		if (value == 0){
			printf("semaphore obtained\n");
			resource = *num;
			printf("resource changed to %d\n", resource);
			if (pid == 0){
				printf("Sleeping...\n");
				sleep(60);
			}
			sem_post(semaphore);
			printf("Semaphore released\n");
		}
		else{
			printf("ERROR: semaphore not obtained\n");
			exit(EXIT_FAILURE);
		}
	}
	
	
}

void childProcess()
{
	//Define Signals
	signal(SIGUSR1, signalHandler1);
	signal(SIGUSR2, signalHandler2);
	int value;
	int num = 1;
	//Calls shared function
	changeResource(&num);
	printf("CHILD PROCESS: exiting\n");
	_exit(0);
}

void parentProcess()
{
	//Sleeps allowing child to obtain semaphore first
	sleep(10);
	if (getpgid(other) >= 0){
		printf("PARENT PROCESS: child process is running\n");
	}
	int num = 2;
	//Calls shared function
	changeResource(&num);
	printf("PARENT PROCESS: exiting\n");
	_exit(0);
}

int main(int argc, char* argv[])
{
	//create semaphore
	semaphore = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (sem_init(semaphore, 1, 1) != 0){
		printf("ERROR: semifore not created\n");
		exit(EXIT_FAILURE);
	}
	
	//fork and obtain pid
	pid = fork();
	if (pid < 0){
		printf("ERROR: fork failed\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0){
		printf("CHILD PROCESS: created\n");
		other = getppid();
		childProcess();
	}
	else{
		printf("PARENT PROCESS: created\n");
		other = pid;
		parentProcess();
		
	}
	
	//Cleanup
	sem_destroy(semaphore);
	
	return 0;
}
