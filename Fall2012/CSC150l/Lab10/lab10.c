#include<stdio.h>
#include<math.h>

void get_xy( int xy[ ] );

int main()
{
   int xy_1[ 2 ];
   int xy_2[ 2 ];
   
   get_xy( xy_1 );
   get_xy( xy_2 );
   
   return 0;
}

void get_xy( int xy[ ] )
{
   printf("Enter the 'x' value: ");
   scanf("%i", &xy[ 0 ]);
   printf("Enter the 'y' value: ");
   scanf("%i", &xy[ 1 ]);   
}
