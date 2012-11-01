/*-------------------------------------------------------------------   
   Program: lab8_2.cpp
   Author: <Fill in your name>
   Course: CSC 150 - <Fill in lab section number>
   Instructor: <Fill in lab TA's name>
   Date: < Fill in today's date>
   Description
       The program will let user to enter the monthly salary of an employee, 
   the amount of monthly contribution to pre-tax retirement plans, and 
   the income tax rate. For simplicity, we assume the income tax rate 
   is a single number, expressed in percentage, applicable to the entire 
   taxable income. The program subtracts the retirement contribution from 
   the monthly salary, then subtracts the income tax from the remaining 
   taxable income, and finally display monthly payment after retirement 
   contribution and income tax. The program should validate the monthly 
   salary, monthly retirement contribution, and income tax. All values 
   must be positive. In addition, the retirement contribution cannot exceed 
   8% of the salary, and tax rate cannot exceed 35%.  




---------------------------------------------------------------------
  Includes
---------------------------------------------------------------------*/
#include <iostream>
#include <iomanip>
using namespace std;


//-------------------------------------------------------------------
// Function Prototypes
//-------------------------------------------------------------------
void computePayment (double & salary, double retirement, float taxRate);


/********************************************************************
   Function:      main
   Description:   Program entry/exit point, prompts for inputs, 
                  performs calculations, displays results
   Parameters:    none
   Returns:       0
   Called by:     none
   Calls:         computePayment ()

*********************************************************************/
int main()
{
	double salary = -1.0;
	double retirement = -1.0;
	float taxRate    = -1.0;

	//Ask the user to enter monthly salary and then validate it 
	while ( salary  < 0)
	{
		cout << "Please enter the employee's monthly salary: ";
		cin >> salary;
		if (salary < 0)
			cout << "*** Error, salary cannot be negative" << endl;
	}

	//Ask the user to enter monthly contribution to retirement plan and then validate it
   while ( retirement  < 0 || (retirement / salary) > .08)
	{
		cout << "Please enter the monthly contribution to retirement plan: ";
		cin >> retirement;
		if (retirement  < 0 || (retirement / salary) > .08)
			cout << "*** Error, monthly contribution to retirement plan must be positive and less than 8% of salary" << endl;
	}


	//Ask the user to enter the income tax rate and then validate it
   while ( taxRate < 0 || taxRate > 35)
	{
		cout << "Please enter the employee's taxRate: ";
		cin >> taxRate;
		if (taxRate < 0 || taxRate > 35)
			cout << "*** Error, Tax Rate must be between 0 and 35" << endl;
	}



	// call computePayment function to calculate the payment
	computePayment( salary, retirement, taxRate );

	// output the the payment after retirement contribution and income tax have been subtracted
	
   cout << "The monthly payment after retirement contribution and tax is: " << salary << endl;
   
	return 0;
}


/********************************************************************
   Function: computePayment
   Description:
      This function subtracts the retirement contribution and income tax 
	  from a monthly salary. Salary is updated to the payment after retirement 
	  contribution and tax.
   Parameters:
   
   param[in/out] salary: double * with contents equal to monthly salary
                (passed by reference)
   param[in]: retirement: double with value equal to monthly retirement contribution
                (passed by value)
   param[in]: taxRate : float with value equal to income tax rate (passed by value)
   
   Return value: void
*********************************************************************/

void computePayment( double &salary, double retirement, float taxRate )
{
   taxRate /= 100.0;
   
   salary = salary - retirement;
   salary -= taxRate * salary;
}

