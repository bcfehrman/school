#include <iostream>
using namespace std;

int main(void)
{
int year; // holds the year entered by the user

cout << "Input the year: ";
cin >> year;

if ( (year % 400 == 0)  || ( (year % 4 == 0 ) && (year % 100 > 0) ) )
   cout << year << " IS a leap year." << endl;
else
   cout << year << " IS NOT a leap year." << endl;

return 0;
}
