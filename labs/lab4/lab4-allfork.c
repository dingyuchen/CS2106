#include <unistd.h>
#include <stdio.h>
#define D 10000

double delay(unsigned long loops) // just waste CPU time
{
    unsigned long i;
    double z = i, y;

    z = i;
    for (i = loops; i >= 1 ; i--) {
        y = ((double) loops)/((double) (loops - i + 1));
        z = (y*y + z)*i/((double) loops);
    }
    return z;
}

int main(int argc, char *argv[])
{
    int child;

    while (1) {
        if ((child=fork()) > 0)
            fprintf(stderr, "[%d] ", getpid());
        else if (child == -1)
            fprintf(stderr, "FAIL ");
        else delay(D); // Pick some large D to start with
    }
}
