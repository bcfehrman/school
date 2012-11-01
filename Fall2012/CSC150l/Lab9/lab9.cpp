//lab 9 - business summary
// V. Manes  3/15/12

#include <iostream>
#include <iomanip>
using namespace std;

const int NUM_DIV = 3;

   void get_data( int curr_div[], int num_periods    );
   void calc_total( float total[ ], int curr_total_index, int curr_div[], int num_periods );
   void calc_percent(     );
   void display_summary( float total[], int curr_div_num, int total_sales );

int main( )
{
    //declare arrays - three for divisions, one for totals
    const int NUM_PERIODS = 4;
    int div1[NUM_PERIODS], div2[NUM_PERIODS], div3[NUM_PERIODS];
    float total[3];

    int total_sales = 0;
    int i;

    cout << fixed << showpoint << setprecision(2);

    //get the sales date for each division
    get_data( div1, NUM_PERIODS );
    get_data( div2, NUM_PERIODS );
    get_data( div3, NUM_PERIODS );

    //calculate total sales for each division,store to totals array
    calc_total( total, 0, div1, NUM_PERIODS );
    calc_total( total, 1, div2, NUM_PERIODS );
    calc_total( total, 2, div3, NUM_PERIODS );


    //compute the grand total for sales


    cout << endl;
    cout << "Division    Sales    Percentage" << endl;
   //display each division's summary



    cout << "Total" << setw(12) << total_sales << endl;

    cout << endl;
    return 0;
}

//write your functions here
void get_data( int curr_div[], int num_periods )
{
   int i = 0;
   
   for( i = 0; i < num_periods; i++ )
   {
      cout << "Period " << i + 1 << " : ";
      cin >> curr_div[ i ];
   }
   
   cout << endl;
}

void calc_total( float total[ ], int curr_total_index, int curr_div[], int num_periods )
{
   int i = 0;
   int sum = 0;
   
   for( i = 0; i < num_periods; i++ )
   {
      sum += curr_div[i];
   }
   
   total[curr_total_index] = sum;
}
