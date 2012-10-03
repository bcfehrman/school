#include<iostream>

using namespace std;

int main()
{
 float miles = 0;
 int choice = 0;
 float multiplier = 0;
 
 cout << "Please enter the actual miles: ";
 if(!(cin >> miles))
 {
  cout << "Error with input, exiting..." << endl;
  return -1;
 }
  
 cout << "1. First " << endl;
 cout << "2. Business " << endl;
 cout << "3. Economy " << endl;
 cout << "4. Discount Economy " << endl;
 cout << "5. Other " << endl;
 
 cout << endl << "Please enter the ticket fare class: ";
 
 if(!(cin >> choice))
 {
  cout << "Error with input, exiting..." << endl;
  return -1;
 }
 
 switch(choice)
 {
   case 1:
     multiplier = 1.5;
     break;
   
   case 2:
     multiplier = 1.25;
     break;
     
   case 3:
     multiplier = 1.0;
     break;
     
   case 4:
     multiplier = 0.5;
     break;
     
   case 5:
     multiplier = 0.0;
     break;
     
   default:
     cout << "Invalid choice, exiting..." <<endl;
     return -1;
 }
  
  cout << "You have earned " << miles * multiplier << " miles." << endl;
  
  return 0;
}