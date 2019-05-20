#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

extern double delay(int loops);

int main(int argc, char *argv[])
{
  int D; 
  double z;
  struct timeval start, end;
  
  if (argc != 2) exit(1);
  gettimeofday(&start, 0); // get start real time
  D = atoi(argv[1]);
  if (D > 0) z = delay(D);
  gettimeofday(&end, 0); // get end real time
  printf("start sec %lu usec %lu\n", start.tv_sec, start.tv_usec);
  printf("end sec %lu usec %lu\n", end.tv_sec, end.tv_usec);
  exit(z); // just try to make sure z is used
}

/*
time ./lab5ex1 200

start sec 1554473118 usec 96979
end sec 1554473118 usec 102478

real  0m0.007s
user  0m0.003s
sys 0m0.004s


time ./lab5ex1 500

start sec 1554473105 usec 329042
end sec 1554473105 usec 337998

real  0m0.011s
user  0m0.007s
sys 0m0.004s


time ./lab5ex1 1000

start sec 1554473107 usec 841020
end sec 1554473107 usec 855849

real  0m0.017s
user  0m0.013s
sys 0m0.004s
*/
