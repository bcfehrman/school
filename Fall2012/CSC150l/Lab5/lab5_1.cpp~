#define _USE_MATH_DEFINES
#include<iostream>
#include<cmath>

using namespace std;

int main()
{
  float x = 0, y = 0;
  float angle = 0;
  
  cout << "Please enter the x and y coordinates of the point : ";
  cin >> x >> y;
  
  //Test if at the origin
  if(x == 0 && y == 0)
  {
    cout << "( " << x << "," << y << ") is at the origin" << endl;
  }

  else if( x == 0 )
  {
    cout << "( " << x << "," << y << ") is on the y-axis" << endl;
    if( y > 0)
    {
      cout << "The angle of the radius vector is 90" << endl;
    }
    else
    {
     cout << "The angle of the radius vector is -90" << endl; 
    }
  }
  else if( y == 0 )
  {
    cout << "( " << x << "," << y << ") is on the x-axis" << endl;
    
    if( x > 0)
    {
      cout << "The angle of the radius vector is 0" << endl;
    }
    else
    {
     cout << "The angle of the radius vector is 180" << endl; 
    }
  }
  else
  {
    angle = atan( y / x ) / M_PI * 180;
    
    if( x > 0)
    {
      if(y > 0)
      {
	cout << "( " << x << "," << y << ") is in the first quadrant" << endl;
      }
      else
      {
	cout << "( " << x << "," << y << ") is in the fourth quadrant" << endl;
      }
    }

    else
    {
      if(y > 0)
      {
	cout << "( " << x << "," << y << ") is in the second quadrant" << endl;
	angle += 180;
      }
      else
      {
	cout << "( " << x << "," << y << ") is in the third quadrant" << endl;
	angle -= 180;
      }
    }
    
    cout << "The angle of the radius vector is: " << angle << endl;
  }
  
 return 0; 
}