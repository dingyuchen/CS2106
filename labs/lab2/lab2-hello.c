#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void do_cleanup(void)
{
	printf("do cleanup\n");
} 

int main(int argc, char *argv[]) 
{
	int pid; // you can use these variables
	char buf[100];

	atexit(do_cleanup); // LINE 1
	setvbuf(stdout, 0, _IOLBF, 0); // LINE 2
	// setvbuf(stdout, 0, _IOFBF, 0); // LINE 3

	printf("Enter Name? "); // LINE 4
	pid = getpid(); // LINE 5
	scanf("%s", buf); // LINE 6
	printf("Hello %s\n", buf); // LINE 7
	printf(" your PID = %d\n", pid);  // LINE 8

	exit(EXIT_SUCCESS); // LINE 9
	//_exit(EXIT_SUCCESS);  // LINE 10
}
