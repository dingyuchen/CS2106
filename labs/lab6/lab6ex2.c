#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int nextIndex(int, int);

int main(int argc, char *argv[]) {
	int msPerTick = 10000;
	char inputBuffer[128]; // buffer to read the child process command
	char delim[2] = " ";
	char *token; // to hold each token in the input
	char *command[128]; // temporary holder for one child command
	int commandReadPos = 0;
	int children[128]; // number of possible children not known yet
	int totalChild = 0;

	// parse command options
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0) {
			msPerTick = atoi(argv[i+1]);
		}
	}

	// read the processes which are to be scheduled
	while ( fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL ) {
		commandReadPos = 0;
		// parse one command
		token = strtok(inputBuffer, delim);
		command[commandReadPos++] = token;
		while (token != NULL) {
			token = strtok(NULL, delim);
			command[commandReadPos++] = token;
		}
		// mark the terminate of one command
		command[commandReadPos] = NULL;
		
		// create a child process and put it to sleep
		int childId = fork();
		if (childId == 0) { // child
			execvp(command[0], command);
		} else { // parent
			children[totalChild++] = childId;
			kill(childId, SIGSTOP);
		}
	}

	totalChild--;

	// schedule the created child processes
	int currentChildIndex = 0;
	while (1) { // may need to synchronize
		int currentChildId = children[currentChildIndex];
		kill(currentChildId, SIGCONT);
		usleep(msPerTick);
		kill(currentChildId, SIGSTOP);
		currentChildIndex = nextIndex(currentChildIndex, totalChild);
	}
}

int nextIndex(int currentIndex, int lastIndex) {
	if (currentIndex == lastIndex) {
		return 0;
	} else {
		return currentIndex + 1;
	}
}