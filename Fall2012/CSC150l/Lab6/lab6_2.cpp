/*-------------------------------------------------------------------
   Program: lab6_2.cpp
   Author: <Fill in your name>
   Course: CSC 150 - <Fill in lab section number>
   Instructor: <Fill in lab TA's name>
   Date: < Fill in today's date>
   Description:
	This program computes the number of overtime hours worked
	by an employee on a three-day period (Friday, Saturday and
	Sunday).  Friday is day 1, Saturday is day 2, and Sunday is
	day 3.  It displays the total number of overtime hours.


---------------------------------------------------------------------
  Includes
---------------------------------------------------------------------*/
#include <iostream>
#include <iomanip>
using namespace std;

/********************************************************************
   Function:      main
   Description:   Program entry/exit point, prompts for inputs, 
                  performs calculations, displays results
   Parameters:    none
   Returns:       0
   Called by:     none
   Calls:         none
*********************************************************************/
int main()
{
   float numHours, totalOvertime, total;
   int day = 0; // these are the counter for the loop
   int employeeNumber;

   // Set up the formatting for floating-point numbers.
   cout << setprecision(2) << fixed << showpoint;

   cout << "Please enter an employee ID (1000 - 9999) or CTRL-Z to stop: ";
   
   while( cin >> employeeNumber)
   {
      if( employeeNumber >= 1000 && employeeNumber <=9999)
      {
         
         totalOvertime = 0.0;
         total = 0.0;
         numHours = 0.0;
         
         cout << "Number of regular hours worked in the week?: ";
         cin >> total;
         
         for(day = 1; day <= 3; day++)
         {
            cout << "Number of overtime hours worked on day " << day << "? ";
            cin >> numHours;

            totalOvertime += numHours;
         }
         
         total += totalOvertime * 1.5;

         cout << endl;
         cout << "The number of effective hours worked is: "
             << total  << endl << endl;
      }
      
      else
      {
         cout << "Employee number must be between 1000 and 9999" << endl;
      }
          
      cout << "Please enter an employee ID (1000 - 9999) or CTRL-Z to stop: ";
    }

   return 0;
}
