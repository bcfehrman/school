////////////////////////////////////////////////////////////////////
//             Generate Weights for Backprop Network              //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include "constant.h"

using namespace std;

const double MAX = double(RAND_MAX);

int main()
  {
  float    double_off;
  int      i;
  int      j;
  float    offset;
  ofstream outfile;
  float    x;

  offset = 0.1;
  double_off = offset * 2.0;

  outfile.open("genw1.out");
  srand(time(NULL));
  outfile.setf(ios::fixed);

  for (i = 0; i < MAXH; i++)
    {
    for (j = 0; j < MAXIN + 1; j++)
      {
      x = (rand() / MAX) * double_off - offset;
      outfile << setprecision(6);
      outfile << setw(10) << x;
      if ((j + 1) % 7 == 0)
        outfile << endl;
      }
    outfile << endl;
    }
  outfile << endl;

  for (i = 0; i < MAXOUT; i++)
    {
    for (j = 0; j < MAXH + 1; j++)
      {
      x = (rand() / MAX) * double_off - offset;
      outfile << setprecision(6);
      outfile << setw(10) << x;
      if ((j + 1) % 7 == 0)
        outfile << endl;
      }
    outfile << endl;
    }
  outfile << "0 iterations" << endl;
  outfile << "Random weights above" << endl;
  outfile << "==============================================";
  outfile << endl;
  outfile.close();
  }

////////////////////////////////////////////////////////////////////
//                  end of program                                //
////////////////////////////////////////////////////////////////////
