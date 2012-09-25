#include<iostream>
#include<cstring>
#include<math.h>

using namespace std;

int main()
{
 float area= 0.0;
 char errorMessage[] = "The input values do not describe a triangle"; 
 float sideA = 0.0;
 float sideB = 0.0;
 float sideC = 0.0;
 float s = 0.0;
 
 //get side a
 cout << "Please enter the length of side A: ";
 cin >> sideA;
 
 //get side b
 cout << "Please enter the length of side B: ";
 cin >> sideB;
 
 //get side C
 cout << "Please enter the length of side C: ";
 cin >> sideC;
 
 if( (sideA + sideB) < sideC)
 {
  cout << errorMessage << endl;
  return -1;
 }
 else if( (sideA + sideC) < sideB )
 {
  cout << errorMessage << endl;
  return -1;
 }
 else if( (sideB + sideC) < sideA )
 {
  cout << errorMessage << endl;
  return -1;
 }
  
  s = (sideA + sideB + sideC)/2.0;
  
  area = sqrt( s * ( s - sideA ) * ( s - sideB ) * ( s - sideC ) );
  
  cout << "The triangle with sides " << sideA << ", " << sideB << ", and " << sideC << " has an area of " << area << endl;
  
  return 0;
}