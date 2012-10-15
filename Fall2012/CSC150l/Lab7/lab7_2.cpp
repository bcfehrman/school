/********************************************************************
* File: Chap_7_Debugging.cpp
*
* General Instructions: Complete each step before proceeding to the 
* next.
*
* Debugging Exercise 1
*
* 1) Insert a breakpoint on the lines indicated in the code. 
* 2) Run to Breakpoint 1.
* 3) When prompted, enter your age.
* 4) When the execution stops, add a watch on age and verify that
*    the value in age is what you typed in.
* 5) Step over the if statement.
* 6) Why did the value in age change? 
* 7) Fix the problem and repeat Steps 2 – 5 to verify the
*    problem was corrected.
* 8) Stop debugging.
* 
* Debugging Exercise 2
*
* 1) Run to Breakpoint 1. 
* 2) When prompted, enter the value 25 for your age.
* 3) Step over the if statement. Execution of the program should
*    continue on the else if statement.
* 4) Verify that 25 is still stored in age.
* 5) Step over the else if.
* 6) Why is the program going to print "Teenager" for an age of 25?
* 7) Fix the problem and repeat Steps 1 – 5 to verify the
*    problem was corrected.
* 8) Stop debugging.
* 9) Remove Breakpoint1.
*
* Debugging Exercise 3
*
* 1) Run the program without debugging.
* 2) When prompted, enter the value of 10 for your age.
* 3) Why does the program print both "Child" and "Adult"?
* 4) Re-run the program this time with debugging and run to 
*    Breakpoint 2.
* 5) Why is the action with the else executing?
* 6) Fix the problem and re-run to verify the problem was corrected.
********************************************************************/

#include <iostream>
using namespace std;

int main()
{
	int age = 0;

	cout << "Enter your age: ";
	cin  >> age;

	// Breakpoint 1
	// Put a breakpoint on the following line
	if ( age = 1 )
		cout << "First Birthday" << endl;
	else if ( age >= 12 || age <= 19 )
		cout << "Teenager" << endl;
	else if ( age < 12 )
		cout << "Child" << endl;
	else if ( age > 62 )
		cout << "Senior" << endl;
	// Breakpoint 2
	// Put a breakpoint on the following line
	else;
		cout << "Adult" << endl;

	return 0;
}
