#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>

#define OUTPUT (1)

int compute(int x) // just compute something and make CPU work
{
    int i, j;
    for (i=0; i < 10000; i++)
        for (j=0; j < 10000; j++)
            x *= x;
    return x;
}

int main() {
    char buf[100];
    int childid = 0;

    if ((childid = fork()) > 0) {
        // nice(19); sched_yield();
        compute(childid);
        strcpy(buf, "parent\n");
    } else if (childid == 0) {
        if (fork() > 0) {
            //nice(19); sched_yield();
            strcpy(buf, "child 1\n");
        } else
            strcpy(buf, "child 2\n");
        compute(childid);
    } else {
        perror("oops why did fork fail?"); // print system error
        exit(1);
    }
    write(OUTPUT, buf, strlen(buf)); // write syscall - we will cover later
    exit(0);
}

