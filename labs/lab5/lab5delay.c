#include <math.h>

// delay is just CPU bound instructions which take some time to execute
// what is computed is not important, only the amount of time given
// the loops argument, the return value is just to reduce the possibility
// of the compiler removing the loop
//
double delay(int loops)
{
  int i, j;
  double z = i, y;
  z = i; // use an uninitialized value
  for (i = 0; i < loops ; i++) { 
    // just some random computation to take CPU time
    for (j = 0; j < 1000; j++) {
      y = ((double) loops)/((double) (loops - i + 1));
      z = sqrt((y*y + z)*i/((double) loops));
    }
  }
  return z;
}

