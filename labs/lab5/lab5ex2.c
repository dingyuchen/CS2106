#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern double delay(int loops);

double get_tick(clock_t base_tick)
{
  int i;
  clock_t new_tick;
  double z;

  for (i=0; i < 100; i++) z = delay(1);
  new_tick = clock();
  if (new_tick == base_tick) {
    	fprintf(stderr, "OOPS: delay too small\n");
    	exit(1);
  }
  return ((double) (100) *  CLOCKS_PER_SEC)/ (new_tick-base_tick);
}

int main(int argc, char *argv[])
{
  double the_delay;
  
  the_delay = get_tick(clock()); 
  printf("Delay value for 1s = %g\n", the_delay);
  exit(0);
}

/*
./lab5ex2
Delayed value for 1s = 24789.3
Delayed value for 1s = 24777
Delayed value for 1s = 24795.4
Delayed value for 1s = 24820.1
Delayed value for 1s = 24801.6
*/
