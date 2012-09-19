#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<iomanip>

using namespace std;

struct triplet_t
{
  double x, y, theta;
};

triplet_t get_deltas(double theta, double xvel, double yvel, double thCoef, double xyCoef);
void skip_to_next_line(ifstream &fin);

int main()
{
	double curr_t = 0;
	double delta_t = 0.1;
	double wheel_radius = 18; //in cm
	double wheel_distance = 12; //in cm
	char temp;
	double thCoef;
	ifstream fin;
	ofstream fout;
	double xvel, yvel;
	double xyCoef;
	triplet_t curr, delta;
	
	curr.x = 0;
	curr.y = 0;
	curr.theta = 0;
	
	fout.open("data/prog1_data_out.txt");
	fin.open("data/prog1_data.txt");
	
	if(!fin)
	{
		cout << "Please make sure data file is in the data folder" << endl;
		return -1;
	}
	
	xyCoef = (wheel_radius * delta_t) / 2;
	thCoef = xyCoef / wheel_distance;
	
	while(!fin.eof())
	{
		//Get the next x and y velocity values
		fin >> xvel >> yvel;
		
		delta = get_deltas(curr.theta, (xvel / 0.254), (yvel / 0.254), thCoef, xyCoef);
		
		curr.x += delta.x;
		curr.y += delta.y;
		curr.theta += delta.theta;
		
		cout << fixed << setprecision(2)<< curr.x << "  " << curr.y << "  ";
		cout << fixed << setprecision(4) << curr.theta << endl;
		
		fout << fixed << setprecision(2)<< curr.x << "  " << curr.y << "  ";
		fout << fixed << setprecision(4) << curr.theta << endl;
				
		
		skip_to_next_line(fin);
		
	}
	
	fin.close();
	fout.close();

  return 0;
}

void skip_to_next_line(ifstream &fin)
{
	char temp;
	char nl = 10;
	char cr = 13;
	
	fin.get(temp);
	
	while(temp != nl && temp != cr && !fin.eof() )
	{
		fin.get(temp);
	}
}

triplet_t get_deltas(double theta, double xvel, double yvel, double thCoef, double xyCoef)
{
	triplet_t delta;
	
	delta.theta = thCoef * (xvel - yvel);
	delta.x = xyCoef * (xvel + yvel) * cos(theta);
	delta.y = xyCoef * (xvel + yvel) * sin(theta);
	
	return delta;
}