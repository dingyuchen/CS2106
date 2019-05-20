#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>

void distribute(int[], int, int);
void removeFromArray(int[], int, int);
void removeStringFromArray(char *arr[], int, int);
int nextIndex(int, int);
char* trimLeft(char*);

int main(int argc, char *argv[]) {
	int msPerTick = 10000;
	char inputBuffer[128]; // buffer to read the child process command
	char delim[2] = " ";
	char *token; // to hold each token in the input
	char *command[128]; // temporary holder for one child command
	int commandReadPos = 0;
	int children[128]; // number of possible children not known yet
	int share[128];
	int totalChild = 0;
	int status;
	char *allCommands[128]; // array of pointer to each string of commands

	// parse command options
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0) {
			msPerTick = atoi(argv[i+1]);
		}
	}
	// read the processes which are to be scheduled
	while ( fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL ) {
		// copy buffer for back up
		allCommands[totalChild] = (char*) malloc(128 * sizeof(char));
		strcpy(allCommands[totalChild], inputBuffer);
		// reset command read position
		commandReadPos = 0;
		// get the share for this command
		token = strtok(inputBuffer, delim);
		share[totalChild] = atoi(token);

		// parse the command
		while (token != NULL) {
			token = strtok(NULL, delim);
			command[commandReadPos++] = token;
		}
		// mark the termination of one command
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

	// schedule the proceses in round robin
	int currentChildIndex = 0;

	// hold the time for each process during one tick
	// process time during a tick is fixed at the beginning of the tick, so that
	// during a tick, all the live processes will be run
	int roundTime[totalChild+1];
	for (int i = 0; i <= totalChild; i++) {
		roundTime[i] = msPerTick / 100 * share[i];
	}

	int tick = 0; // indicate a tick timer, start from 0

	while (1) { // may need to synchronize
		int currentChildId = children[currentChildIndex];
		kill(currentChildId, SIGCONT);
		usleep(roundTime[currentChildIndex]);
		// check whether the current process has terminated
		if (waitpid(currentChildId, &status, WNOHANG) != 0) { // this child exited
			// print msg to stderr
			char* rawPrint = allCommands[currentChildIndex];
			fprintf(stderr, "%d: %s", tick, trimLeft(rawPrint));
			// get the share for distribution
			int extraShare = share[currentChildIndex];
			// remove id and share and roundTime from the arrays
			removeFromArray(children, totalChild, currentChildIndex);
			removeFromArray(share, totalChild, currentChildIndex);
			removeFromArray(roundTime, totalChild, currentChildIndex);
			removeStringFromArray(allCommands, totalChild, currentChildIndex);
			totalChild--;
			// re-distribute his share to other processes
			distribute(share, totalChild, extraShare);
			// the current child index should go back one, so that the next can be accurate
			currentChildIndex--;
			// if no more child, exit the program
			if (totalChild < 0) exit(0);
		} else { // this child not terminated
			kill(currentChildId, SIGSTOP);
		}
		currentChildIndex = nextIndex(currentChildIndex, totalChild);
		if (currentChildIndex == 0) {
			// reached the end of current tick, re-caliberate the roundTime
			for (int i = 0; i <= totalChild; i++) {
				roundTime[i] = msPerTick / 100 * share[i];
			}
			// increment to next tick
			tick++;
		}
	}
}

void distribute(int share[], int lastIndex, int extra) {
	int current = 0;
	int left;
	for (left = extra; left > 0; left--) {
		share[current]++;
		current = nextIndex(current, lastIndex);
	}
}

// lastIndex is the last valid index in the array
// the size of array is managed elsewhere
void removeFromArray(int arr[], int lastIndex, int pos) {
	int i;
	for (i = pos; i < lastIndex; i++) {
		arr[i] = arr[i+1];
	}
}

void removeStringFromArray(char *arr[], int lastIndex, int pos) {
	int i;
	for (i = pos; i < lastIndex; i++) {
		arr[i] = arr[i+1];
	}
}

// lastIndex is the last valid index
int nextIndex(int currentIndex, int lastIndex) {
	if (currentIndex == lastIndex) {
		return 0;
	} else {
		return currentIndex + 1;
	}
}

char* trimLeft(char* s) {
    while(!isspace(*s)) {
        s++;
    }
    s++;
    return s;
}
