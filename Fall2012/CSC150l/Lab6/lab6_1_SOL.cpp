/*-------------------------------------------------------------------
   Program: lab6_1_SOL.cpp
   Author: MCS/SDSM&T
   Course: CSC 150 - Section M000
   Instructor: A Instructor
   
   
   Description:  
     This program computes the nth element of the Fibonacci 
	 sequence up to the 45th element, where n is a value entered
	 by the user. It terminates when the user enters a negative 
	 number.  It displays an appropriate message if the user 
	 enters a value of 0 or greater than 45.


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
	long next;				// store the next element
	bool sentinel = false;	// Negative input terminates the program

	do
	{
		fib1 = 1;
		fib2 = 1;
		cout << "Enter the desired Fibonacci number: ";
		cin >> number;

		if (number < 0)
			sentinel = true;
		else if (number == 0)
			cout << "Please enter a value between 1 and 45.\n" << endl;
		else if (number > 45)
			cout << "This program can only find up to the 45th element.\n" << endl;
		else if (number == 1 || number == 2)
			cout << "This number is: 1\n" << endl;
		else
		{
			//Determine the other elements of the sequence
			for( counter = 2; counter < number; counter++ )
			{
				next = fib1 + fib2;
				fib1 = fib2;
				fib2 = next;   
			}
			cout << "This number is: " << next << endl << endl;
		}

	} while (sentinel == false);

	cout << "Program exiting!" << endl;

	return 0;

}
