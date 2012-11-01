/*-------------------------------------------------------------------   
   Program: lab8_1.cpp
   Author: <Fill in your name>
   Course: CSC 150 - <Fill in lab section number>
   Instructor: <Fill in lab TA's name>
   Date: < Fill in today's date>
   Description
     This program computes and displays either the volume and surface
	 area of a circular cylinder or sphere.  It computes the formulas
	 for a cylinder if the user enters a 'c'.  If the user enters
	 an 's', it computes the formulas for a sphere.  For any other
	 character, the program terminates. The formulas are:
	 cylinder -
		surface area = 2. * PI * r * h  +  2. * PI * r * r
		volume = PI * r * r * h
	 sphere -
		surface area = 4. * PI * r * r
		volume = (4./3.) * PI * r * r * r
	 where r is radius and h is height.

     The program prompts the user for values for r and h (only r for
	 a sphere). Assume valid values for r and h are entered.

---------------------------------------------------------------------
  Includes
---------------------------------------------------------------------*/
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

double pi();


/********************************************************************
   Function:      main
   Description:   Program entry/exit point, prompts for inputs, 
                  performs calculations, displays results
   Parameters:    none
   Returns:       0
   Called by:     none
   Calls:         pow()
*********************************************************************/

int main()
{
	const double PI = 3.1416;

	double volume, area, radius, height;
	char solid;

	cout << setprecision (4) << fixed << showpoint;
	// Ask the user to enter type of solid
	cout << "Enter 's' for sphere, 'c' for cylinder or any other char to end: ";
	cin >> solid;

	while (solid == 'c' || solid == 's')
	{
		if (solid == 'c')
		{
			//Ask the user to enter the radius and height
			cout << "Enter value for radius: ";
			cin >> radius;
			cout << "Enter value for height: ";
			cin >> height;
			volume = PI * radius* radius * height;
			area = 2. * PI * radius * height  +  2. * PI * radius * radius;
			cout << "Cylinder's volume: " << volume << ", and area: " << area;
			cout << endl << endl;
		}
		else
		{
			//Ask the user to enter the radius
			cout << "Enter value for radius: ";
			cin >> radius;
			volume = 4. / 3. * pi() * pow(radius, 3.);
			area = 4. * pi() * radius * radius;
			cout << "Sphere's volume: " << volume << ", and area: " << area;
			cout << endl << endl;
		}
   
		cout << "Enter 's' for sphere, 'c' for cylinder or any other char to end: ";
		cin >> solid;
	}

	cout << endl << "Program ended. Bye!" << endl;

	return 0;
}

double pi()
{
   return 2.0 * asin( 1.0 );
}
