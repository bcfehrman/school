/*-------------------------------------------------------------------
   Program: lab6_2_SOL.cpp
   Author: MCS/SDSM&T
   Course: CSC 150 - Section M000
   Instructor: A Instructor
   Description:
	This program computes the number of effective hours worked
	by an employee during a week. 
	
	For each employee the user enters the employee ID ( a number in the 
	range 1000 – 9999), the total number of regular hours (s)he worked 
	during a week and the number of overtime for a three-day period 
	(Friday, Saturday and Sunday).  Friday is day 1, Saturday is day 2, 
	and Sunday is day 3. The program validates the employee ID.
		  
	The effective number of hours is computed as follows:

		Step 1:	Compute the total number of hours an employee worked overtime.
		Step 2:	Multiple by 1.5 the total overtime hours computed in step 1
		Step 3:	Add the value of step 2 to the total number of regular hours 
		        that the employee worked during the week.

	For each employee, the program displays the employee ID and
	the total number of	effective hours computed.   
	The program terminates when the user types CTRL-Z.  


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
   float numHours, total;
   int   day = 0;              // these are the counter for the loop
   int   employeeID;

   float  regularHours;        // number of regular hours worked
   double effectiveHours;      // calculated number of effective hours

   // Set up the formatting for floating-point numbers.
   cout << setprecision(2) << fixed << showpoint;

   // prompt for employee ID
   cout << "Please enter an employee ID (1000 - 9999) or CTRL-Z to stop: ";

   while( cin >> employeeID )
   {
	   // reset the total overtime hours to zero
	   total = 0.0;

	   // check for a valid employee ID
	   if ( employeeID >= 1000 && employeeID <= 9999 )
	   {
		   // get number of regular hours worked
		   cout << endl 
			    << "Number of regular hours worked in the week? ";
		   cin  >> regularHours;

		   // get three days of overtime hours worked
		   for( day = 1; day <= 3; day++ )
		   {
			  cout << "Number of overtime hours worked on day " << day << "? ";
			  cin >> numHours;

			  total = total + numHours;
		   }

		   // calculate the effective hours
		   effectiveHours = ( total * 1.5 ) + regularHours;

		   cout << "The total number of effective hours this week for employee "
				<< employeeID << " is " << effectiveHours << endl << endl;
		   cout << endl 
			    << "Please enter an employee ID (1000 - 9999) or CTRL-Z to stop:";
	   }
	   else
	   {
		   // prompt again for a valid employee ID
		   cout << "The employee ID is invalid. "
			    << "Please enter a valid ID (1000 - 9999):";
	   }
   }

   cout << "Thank you for using the program. Bye!" << endl;

   return 0;
}
