 #include <iostream>
 #include <stdlib.h>
 #include "tbb/task_scheduler_init.h"
 #include "tbb/parallel_for.h"
 #include "tbb/blocked_range.h"
 //#include "fatals.h"
#include<ctime>

 using namespace tbb;
 using namespace std;
 
 /*------------------------------------------------------+
  | This may look like a class definition, but its not:  |
  | This is actually a TBB Parallel For BODY definition! |
  |                                                      |
  | Arguments are the data members of the "class", in    |
  | this case, p_array_a, p_array_b, and p_array_sum,    |
  | which are all of type int *                          |
  *------------------------------------------------------*/
 class ArraySummer {
 
   int * p_array_a;
   int * p_array_b;
   int * p_array_sum;
 
 public:
   // This empty constructor with an initialization list is used to setup calls to the function
   ArraySummer(int * p_a, int * p_b, int * p_sum) : p_array_a(p_a), p_array_b(p_b), p_array_sum(p_sum) { }
 
   /*----------------------------------------------------------+
    | Here is the actual body, that will be called in parallel |
    | by the TBB runtime. You MUST put this code inside the    |
    | class definition, since the compiler will be expanding   |
    | and inlining this code as part of the template process.  |
    |                                                          |
    | The blocked_range<int> is something like a list of       |
    | indexes corresponding to each invocation of the function |
    +----------------------------------------------------------*/
 
   void operator() ( const blocked_range<int>& r ) const {
     for ( int i = r.begin(); i != r.end(); i++ ) { // iterates over the entire chunk
       p_array_sum[i] = p_array_a[i] + p_array_b[i];
     }
   }
 
 };
 
 /*------------------------------------------------+
  | Here's the meat of the program -- sums two     |
  | Fibonacci-like arrays, using a for loop first  |
  | and then a parallel_for TBB template.          |
  +------------------------------------------------*/
 
 int main(int argc, char *argv[]) {
 // hrtime starttime, endtime, singlethread_time, tbb_time; // for timing
  int * p_A;
  int * p_B;
  int * p_SUM_1T;
  int * p_SUM_TBB;
  clock_t starttimeClock;
  clock_t endtimeClock;
 
  /* This is the TBB runtime... */
  task_scheduler_init init;
 
  int nElements = atoi( argv[1] );
  int nTimes = atoi(argv[2]);
 
  p_A       = new int[nElements];
  p_B       = new int[nElements];
  p_SUM_1T  = new int[nElements];
  p_SUM_TBB = new int[nElements];
 
  /* 
   * Initialize the data sets ... could do this in parallel too, but 
   * serial is easier to read
   */
  p_A[0] = p_B[0] = 0;
  p_A[1] = p_B[1] = 1;
  for( int i=2;i<nElements;i++) {
    p_A[i]   = (p_A[i-1] + p_A[i-2]) % (INT_MAX/2);
    p_B[i]   = p_A[i];
    p_SUM_1T[i] = 0;
    p_SUM_TBB[i] = 0;
  }
 
 
  /*
   * Time how long it takes to sum the arrays using a single thread
   */
  starttimeClock = clock();
for(int j = 0; j < nTimes; j++)
  for( int i=0;i<nElements;i++ ) {
    p_SUM_1T[i] = p_A[i] + p_B[i];
  }
  endtimeClock = clock();
 // singlethread_time = endtime - starttime;
 cout << "Clock: " << (double)(endtimeClock - starttimeClock) / CLOCKS_PER_SEC<< endl;
  /*
   * Now sum the arrays again using TBB, again timing the execution
   */
  //starttime = gethrtime();
   starttimeClock = clock();
     for(int j = 0; j < nTimes; j++)
  parallel_for(blocked_range<int>(0, nElements, 100),
        ArraySummer( p_A, p_B, p_SUM_TBB ) );
  endtimeClock = clock();
 // singlethread_time = endtime - starttime;
 cout << "Clock: " << (double)(endtimeClock - starttimeClock) / CLOCKS_PER_SEC / 8 << endl;
  //endtime = gethrtime();
  //tbb_time = endtime - starttime;
 
  /*
   * Verify the sums match
   */


  for(int i=0;i<nElements;i++) {
    if( p_SUM_1T[i] != p_SUM_TBB[i] ) {
      cout << p_A[i] << " + " << p_B[i] << " = " << p_SUM_1T[i] << " AND " << p_SUM_TBB[i] <<  endl;
    }
  }
 
  /* 
   * Print the times
   */
  //cout << "1T summing time:  " << singlethread_time << " ticks" << endl;
  //cout << "TBB summing time: " << tbb_time << " ticks" << endl;
 
  delete [] p_A;
  delete [] p_B;
  delete [] p_SUM_1T;
  delete [] p_SUM_TBB;
 
  return 0;
 }
