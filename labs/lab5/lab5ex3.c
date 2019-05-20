/* Lab 5 results
    DISCUSS YOUR RESULTS HERE:
      The result from lab5ex3 is much more accurate than the result from lab5ex2.

      In lab5ex2, the delay function is called 100 times, which means that a lot of machines instructions
      related to function stack frame setup and teardown need to be executed as well. The extra machine
      instructions make the timing between the two ticks longer than we expect it to be, and thus the final
      user time tested on lab5ex1 is much shorter than 1s.

      My solution is accurate because it takes a different approach. Instead of accumulating the timing of a lot
      of delay function call with small D, my solution tries to find the acceptable D value right away by calling
      delay function with that. This reduces the number of machine instructions dealing with the overheads.
      From the rough value obtained in lab5ex2, I have found a reasonable boundary to find this D value within.
      Doing binary search within the range minimizes the number of delay function call before finding the correct D.
      In fact, only a few delay function calls are made before D is found.

    CPUINFO:
      model name	: Intel(R) Core(TM) i7-7700 CPU @ 3.60GHz

    VERSION:
      Linux version 4.15.0-47-generic (buildd@lgw01-amd64-028) (gcc version 5.4.0 20160609
      (Ubuntu 5.4.0-6ubuntu1~16.04.10)) #50~16.04.1-Ubuntu SMP Fri Mar 15 16:06:21 UTC 2019

    value of D and lab5ex1 user time from lab5ex3:
      (1) D: 84687.5, user time: 0.996s
      (2) D: 84687.5, user time: 0.996s
      (3) D: 84687.5, user time: 0.996s
      (4) D: 84687.5, user time: 1.022s
      (5) D: 84687.5, user time: 1.024s

    value of D and lab5ex1 user time from lab5ex2:
      (1) D: 24740.2, user time: 0.287s
      (2) D: 24734.1, user time: 0.324s
      (3) D: 24746.3, user time: 0.319s
      (4) D: 24673.1, user time: 0.294s
      (5) D: 24740.2. user time: 0.290s
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern double delay(int loops);
double get_d(double lower, double upper);
double get_delay_timing(double d_val);

double get_d(double lower, double upper)
{
  double mid;
  double timing;
  clock_t base_tick;
  clock_t new_tick;
  double total_time;

  base_tick = clock(); // guard against TLE

  while (lower <= upper)
  {
    // guard against TLE
    new_tick = clock();
    total_time = (new_tick-base_tick) / ((double) CLOCKS_PER_SEC);
    if (total_time > 18) return mid;

    // binary search for D
    mid = lower+(upper-lower)/2;
    timing = get_delay_timing(mid);
    if (timing >= 0.99 && timing <= 1.01) {
      return mid;
    } else if (timing < 0.99) {
      lower = mid;
    } else {
      upper = mid;
    }
  }
}

double get_delay_timing(double d_val)
{
  clock_t base_tick;
  clock_t new_tick;
  double z;

  base_tick = clock();
  z = delay(d_val);
  new_tick = clock();

  return (new_tick-base_tick) / ((double) CLOCKS_PER_SEC);
}

int main(int argc, char *argv[])
{
  double the_delay;

  the_delay = get_d(20000, 200000);
  printf("Delay value for 1s = %g\n", the_delay);
  exit(0);
}
