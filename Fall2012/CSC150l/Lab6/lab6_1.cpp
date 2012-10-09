/*-------------------------------------------------------------------
   Program: lab6_1.cpp
   Author: <Fill in your name >
   Course: CSC 150 - <Fill in lab section number>
   Instructor: <Fill in lab TA's name>
   Date: < Fill in today's date>
   Description:  
     This program computes and displays the first n elements of 
	 the Fibonacci sequence.  The value n must be between 2 and 
	 45. The first seven Fibonacci elements are 1, 1, 2, 3, 5, 
	 8, 13.  The user inputs the number n from the keyboard. 


---------------------------------------------------------------------
  Includes
---------------------------------------------------------------------*/
#include <iostream>
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
	int number;				// input number
	int counter = 2;		// count number of elements displayed
	long fib1 = 1, fib2 = 1;	// first and second element
	long next = 1;				// store the next element

   do
   {
      cout << "Enter the number of elements of the Fibonacci sequence or negative value to quit: ";
      cin >> number;
      
      if (number == 0 || number > 45)
         cout << "Please enter a value between 1 and 45 or a negative value to quit.\n" << endl;
      else if( number > 0 )
      {
         next = 1;
         
         //Determine the other elements of the sequence
         for( counter = 2; counter < number; counter++ )
         {
            next = fib1 + fib2;
            fib1 = fib2;
            fib2 = next;   
         }
           
         cout << "The " << number << "th Fibonacci number is: " << next << endl;
      }
   }while( number >= 0);
   
   cout << "Program exiting" << endl;

	return 0;

}
