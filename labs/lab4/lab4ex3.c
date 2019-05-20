#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *command[10];
    int freePosition = 0;
    int childId = 0;
    int status;

    for (int i = 1; i <= argc; i++) {
        if (i == argc || strcmp(argv[i], "+") == 0) {
            // terminated the command array
            command[freePosition] = '\0';
            // create new process to execute the program
            childId = fork();
            if (childId == 0) { // child
                extern char** environ;
                // if empty command, replace it with bin/true
                if (command[0] == NULL) {
                    command[0] = "/bin/true";
                    command[1] = '\0';
                }
                execve(command[0], command, environ);
                // handle exec error
                fprintf(stderr, "exec failure: %s", command[0]);
                for (int i = 1; i < freePosition; i++) {
                    fprintf(stderr, " %s", command[i]);
                }
                fprintf(stderr, "\n");
                exit(2);
            } else if (childId > 0) { // parent
                waitpid(childId, &status, 0);
                // exit from the master process if has error in child
                if (WIFEXITED(status)) {
                    int exit_status = WEXITSTATUS(status);
                    if (exit_status == 2) {
                        exit(3);
                    }
                }
            } else { // error on fork
                fprintf(stderr, "fork failure\n");
                exit(1);
            }
            // empty the command buffer after finishing one command
            freePosition = 0;
        } else {
            // put the string in command buffer for executing later
            command[freePosition] = argv[i];
            freePosition++;
        }
    }

    return 0;
}