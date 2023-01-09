#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <err.h>
#include <errno.h>
#include <sys/mman.h>

int WAKEUP = SIGUSR1;
pid_t otherPid;
sigset_t sigSet;

struct CIRCULAR_BUFFER
{
	int count; //num items in buffer
	int lower; //next to read
	int upper; //next to write
	int buffer[100];
};
struct CIRCULAR_BUFFER *buffer = NULL;

void sleepTillWake()
{
	//Waits for WAKEUP signal before continuing
	int nSig;
	printf("Sleeping...\n");
	sigwait(&sigSet, &nSig);
	printf("Awoken!\n");
}
void consumer()
{
	sigemptyset(&sigSet);
	sigaddset(&sigSet, WAKEUP);
	sigprocmask(SIG_BLOCK, &sigSet, NULL);
	
	//Consumer is put to sleep to give producer a head start
	printf("Putting Consumer to Sleep...\n");
	sleepTillWake();
	
	//Consumer is woken and begins reading the data
	printf("Running Consumer Process...\n");
	while (buffer->lower < 100){
		//The if statement prevents it from reading null data
		//I realize that signals would be better, but was unable to
		//figure out how to get produver to read and awknowlege a signal
		//and use that information to sent a WAKEUP signal back after having
		//read some data
		if (&(buffer->buffer[buffer->lower]) != NULL){
			printf("CONSUMER reads %d\n", buffer->buffer[buffer->lower]);
			sleep(1);
			buffer->lower+=1;
		}
	}
	//When done reading, wake up the producer to exit both processes
	if (buffer->lower >= 100){
		printf("Waking Up Producer...\n");
		kill(otherPid, WAKEUP);
	}
	_exit(1);
}
void producer()
{
	int count = 0;
	printf("Running Producer Process...\n");
	while (buffer->upper < 100){
		//Write count to buffer
		count +=1;
		buffer->buffer[buffer->upper] = count;
		printf("PRODUCER writes %d\n", buffer->buffer[buffer->upper]);
		buffer->upper +=1;
		sleep(1);
		//Wake up consumer after one pass to get a headstart
		if (count == 1){
			printf("Waking Up Consumer...\n");
			kill(otherPid, WAKEUP);
		}
	}
	//Sleep when reach end of data, waiting for WAKEUP signal
	if (buffer->upper >= 100){
		sleepTillWake();
	}
	_exit(1);
}
int main()
{

	
	buffer = (struct CIRCULAR_BUFFER*)mmap(0, sizeof(buffer), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	buffer->count = 0;
	buffer->lower = 0;
	buffer->upper = 0;
	
	pid_t pid = fork();
	if (pid == -1){
		printf("ERROR Failed Fork\n");
	}
	if (pid == 0){
		otherPid = getppid();
		producer();
	}
	else{
		otherPid = pid;
		consumer();
	}
	return 0;
}
