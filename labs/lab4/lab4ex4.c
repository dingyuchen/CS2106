#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int maxParallel = 1;
    int currentParallel = 0;
    char inputBuffer[256]; // buffer to read one input line
    char delim[2] = " "; // to parse the input line
    char *token; // to hold each token in the input
    char *command[argc]; // to hold the real command template
    int commandPosition = 0; // index for command
    int status;

    // process the command template
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-j") == 0) { // set max # of parallel processes
            maxParallel = atoi(argv[i+1]);
            i++;
        } else if (strcmp(argv[i], "-p") == 0) { // set new PATH
            char *newPath = argv[i+1];
            setenv("PATH", newPath, 1);
            i++;
        } else {
            command[commandPosition] = argv[i];
            commandPosition++;
        }
    }

    // at the end of the loop, commandPosition points to the yet to be 'NULL' position
    command[commandPosition] = '\0';

    // read inputs and create child processes
    while ( fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL ) {
        // wait for one child to exit
        if (maxParallel == currentParallel) {
            wait(&status);
            if (WIFEXITED(status)) { // some one exited
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 2) { // some error happend
                    exit(3);
                } else { // continue for this process
                    currentParallel--;
                }
            }
        }

        // make a copy of the command template
        char *currentCommand[argc];
        for (int i = 0; i <= commandPosition; i++) {
            currentCommand[i] = command[i];
        }

        // break the line into words, and replace the % in currentCommand with the word
        int replacePosition = 0;
        token = strtok(inputBuffer, delim);
        while (token != NULL) { // keep parsing input tokens and replace % in command template
            while (replacePosition < commandPosition) {
                if (strcmp(currentCommand[replacePosition], "%") == 0) { // replace here
                    break;
                }
                replacePosition ++;
            }
            // two breaking condition: (1) found valid %; (2) replacePosition == last NULL byte
            if (replacePosition == commandPosition) {
                // no more % to replace, ready to execute
                break;
            }
            currentCommand[replacePosition] = token;
            replacePosition++;
            // get new token
            token = strtok(NULL, delim);
        }
        // no more token, but there can still be %. Discard everything after the first %
        while (replacePosition < commandPosition) {
            if (strcmp(currentCommand[replacePosition], "%") == 0) {
                currentCommand[replacePosition] = '\0';
                break;
                // ready to execute
            }
            replacePosition++;
        }

        // execute
        int childId = fork();
        if (childId == 0) {
            execvp(currentCommand[0], currentCommand);
            // handle exec error
            fprintf(stderr, "exec failure: %s", currentCommand[0]);
            int i = 1;
            while (currentCommand[i] != NULL) {
                fprintf(stderr, " %s", currentCommand[i]);
                i++;
            }
            fprintf(stderr, "\n");
            exit(2);
        } else if (childId > 0) { // parent
            currentParallel++;
        } else { // fork error
            fprintf(stderr, "fork failure\n");
            exit(1);
        }
    }

    return 0;
}