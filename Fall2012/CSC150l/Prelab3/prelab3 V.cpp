#include <iostream>
#include <cmath>
using namespace std;

int main()
{
	/*declare variables*/
	double A;
	double B;
	double C;
	
	/*read in two variables A and B*/
	/*prompt user to input the length of side A*/
	cout << "Input the length for side A: ";
	/*read A*/
	cin >> A;
	/*prompt user to input the length of side B*/
	cout << "Input the length for side B: ";
	/*read B*/
	cin >> B;

	/*calculate the hypotenuse and store it in C*/
	C=sqrt(A*A + B*B);

	/*output A*/
	cout << "A=" << A << endl;
	/*output B*/
	cout << "B=" << B << endl;
	/*output C*/
	cout << "The hypotenuse is: " << C << endl;



	/*read in two variables C and A*/
	/*prompt user to input the length of side C*/
	cout << "\nInput the length for the hypotenuse: ";
	/*read hypotenuse*/
	cin >> C;
	/*prompt user to input the length of side A*/
	cout << "input the length for side A: ";
	/*read A*/
	cin >> A;

	/*calculate the side and store it in B*/
	B=sqrt(C*C - A*A);

	/*output C*/
	cout << "hypotenuse=" << C << endl;
	/*output A*/
	cout << "A=" << A << endl;
	/*output B*/
	cout << "The length of side B is: " << B << endl;

	return 0;
}