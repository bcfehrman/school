#include<fstream>
#include<string>
#include<iostream>

using namespace std;

int main(int argc, char**argv)
{
  ifstream fin;
  ofstream fout;
  int throwAway;
  const int sizeKeep = 1000;
  char keep[sizeKeep];
  
  if(argc < 3)
  {
   cout << "Correct usage is: ./remove_lines_numbers file_in_name file_out_name" << endl;
   return -1;
  }
  
  fin.open(argv[1]);
  
  if(!fin)
  {
   cout << "Could not open file: " << argv[2] << endl;
   return -1;
  }
  
  fout.open(argv[2]);

  while(!fin.eof())
  {
    //Get the line number and toss it
    fin >> throwAway;
    fin.getline(keep, sizeKeep);
    fout << keep << endl;
  }

  return 0;
}