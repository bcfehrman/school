#include<iostream>
#include<cstring>
#include<iomanip>
#include<cmath>

using namespace std;

void outputDays( int m, int inY );
void outputMonthBase( char* monthName, int year );

int main()
{
   int i = 0;
   int inY = 0;
   int m = 0;
   
   //Get desired year from user
   cout << "Please enter a year: ";
   cin >> inY;
   
   //Check if valid. Output message and ask for another value if invalid
   while( inY < 0 )
   {
      cout << "Year must be greater than zero" << endl;
      cout << "Please enter a year: ";
      cin >> inY;
   }
   
   outputMonthBase( "January", inY );
   outputDays( 11, inY );
   
   outputMonthBase( "February", inY );
   outputDays( 12, inY );
   
   outputMonthBase( "March", inY );
   outputDays( 1, inY );
   
   outputMonthBase( "April", inY );
   outputDays( 2, inY );
   
   outputMonthBase( "May", inY );
   outputDays( 3, inY );
   
   outputMonthBase( "June", inY );
   outputDays( 4, inY );
   
   outputMonthBase( "July", inY );
   outputDays( 5, inY );
   
   outputMonthBase( "August", inY );
   outputDays( 6, inY );
   
   outputMonthBase( "September", inY );
   outputDays( 7, inY );
   
   outputMonthBase( "October", inY );
   outputDays( 8, inY );
   
   outputMonthBase( "November", inY ); 
   outputDays( 9, inY );
   
   outputMonthBase( "December", inY );
   outputDays( 10, inY );
   
   return 0;
}

void outputDays( int m, int inY )
{
   int c = 0;
   int d = 1;
   int i = 0;
   int numDays = 0;
   int numerator = 1;
   int adjustedY = inY;
   int y = 0;
   int spaces = 5;
   int w = 0;
   
   if( m == 12 )
   {
      if( !(adjustedY% 4) && (adjustedY % 100 ) || (adjustedY % 400) )
      {
         numDays = 29;
      }
      else
      {
         numDays = 28;
      }
   }
   else if( m == 2 || m == 4 || m == 7 || m == 9 )
   {
      numDays = 30;
   }
   else
   {
      numDays = 31;
   }
   
   //Special case for y. If month is Jan or Feb
   //then need to substract 1 from the year
   if( m == 11 || m ==12 )
   {
      adjustedY--;
   }
   
   c = adjustedY / 100;
   y = adjustedY - c * 100;
   
   numerator = int(d + floor( 2.6 * m - 0.2 ) + y + floor( y / 4.0 ) + floor( c / 4.0 ) - 2 * c );
   
   if(numerator < 0 )
   {
      numerator *= -1;
      numerator %= 7;
      numerator = 7 - numerator;
   }
   
   w =  numerator % 7;
   
   for( i = 0; i < w; i++)
   {
      cout << setw( spaces ) << "";
   }
   
   cout << setw( spaces ) << d;
   
   if( w == 6 )
      cout << endl; 
   
   for( d = 2; d <= numDays; d++)
   {
      numerator = int(d + floor( 2.6 * m - 0.2 ) + y + floor( y / 4.0 ) + floor( c / 4.0 ) - 2 * c );
   
      if(numerator < 0 )
      {
         numerator *= -1;
         numerator %= 7;
         numerator = 7 - numerator;
      }
      
      w =  numerator % 7;
      
      cout << setw(spaces) << d;
      
      if( w == 6 )
         cout << endl;
   }
   
   cout << endl;
   
}

void outputMonthBase( char* monthName, int year )
{
   int i = 0;
   const int totalSpaces = 38;   //Total spaces in output
   int monthLength = strlen( monthName );
   int yearLength = 0;
   int divider = 1;
   int spaces = 0;
   
   //Determine the number of digits in the given year
   while( ( year / divider ) != 0)
   {
      yearLength++;
      divider *= 10;
   } 
   
   //Caclulate spaces needed to center the month and year
   spaces = ( totalSpaces / 2 ) - ( ( yearLength + monthLength + 1) / 2 );
   
   cout << setw(spaces) << ""<< monthName << " " << year << endl;
   
   //Output the line
   for(i = 0; i < totalSpaces; i++)
   {
      cout << "_";
   }
   
   cout << endl;
   
   //Output each day
   spaces = 5;
   cout << setw(spaces) << "Sun";
   cout << setw(spaces) << "Mon";
   cout << setw(spaces) << "Tue";
   cout << setw(spaces) << "Wed";
   cout << setw(spaces) << "Thu";
   cout << setw(spaces) << "Fri";
   cout << setw(spaces) << "Sat";
   
   cout << endl << endl;
}
