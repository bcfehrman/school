#include<iostream>

using namespace std;

int main()
{
   int i,j;
   for (i = 0; i <= 4; i = i + 1)
   {
      for (j = 0; j <= 3; j = j + 1)
      {
         if(i == 2)
            break;
      }
   }
   
   cout << i << endl;

   return 0;
}
