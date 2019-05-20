#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int commandPosition;
	int slowFactor;
	int msPerTick = 10000;
	int childId;
	int status;

	// parse command options
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-s") == 0) {
			slowFactor = atoi(argv[i+1]);
			i++;
		} else if (strcmp(argv[i], "-t") == 0) {
			msPerTick = atoi(argv[i+1]);
			i++;
		} else { // found the actual process
			commandPosition = i;
			break;
		}
	}

	// fork a child for the process
	childId = fork();
	if (childId == 0) { // child
		execvp(argv[commandPosition], &argv[commandPosition]);
	} else { // parent
		while (waitpid(childId, &status, WNOHANG) == 0) { // child not exited
			// let child run for 1 tick
			usleep(msPerTick);
			kill(childId, SIGSTOP);
			// stop for slowFactor-1 tick
			usleep(msPerTick * (slowFactor-1));
			kill(childId, SIGCONT);
		}
	}

	return 0;
}