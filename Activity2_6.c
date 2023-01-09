#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

sem_t* semi1;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int num_chats = 2;
int offset = 1;

void chat()
{
	pthread_mutex_lock(&mutex1);	
	printf("Chatting Now\n");
	printf("Exit Chat\n");
	pthread_mutex_unlock(&mutex1);
	num_chats += offset;
}
void *enter_chat(void *a)
{
	sem_wait(semi1);
		
	//Critical section where person is connected to chat
	num_chats -= offset;
	
	printf("...\n");
	printf("User Entering Chatroom Available Chatrooms: %d\n", num_chats);
	chat();
	printf("...\n");
	//Out of critical section
	
	sem_post(semi1);
}

void main()
{
	pthread_t tid1, tid2, tid3;
	
	semi1 = sem_open("Mutex", 0, 00644, 2);
	
	pthread_create(&tid1, NULL, enter_chat, NULL);
	pthread_create(&tid2, NULL, enter_chat, NULL);
	pthread_create(&tid3, NULL, enter_chat, NULL);
	
	if (pthread_join(tid1, NULL)){
		printf("ERROR: Thread 1 Not Joined\n");
		exit(1);
	}
	if (pthread_join(tid2, NULL)){
		printf("ERROR: Thread 2 Not Joined\n");
		exit(1);
	}
	if (pthread_join(tid3, NULL)){
		printf("ERROR: Thread 3 Not Joined\n");
		exit(1);
	}
	
	sem_close(semi1);
 	pthread_exit(NULL);
}
