/*Author: Brian Fehrman
 * Class: Intro Robotics Fall 2012
 * Professor: Dr. Larry Pyeatt
 * Assignment: Program 1
 * Due: September 24th by midnight on MCS submission
 * 
 * 
 * This program reads in data from a file in which
 * the first two columns are the right and left wheel velocities
 * of a differential drive robot (velocities in that order).
 * It will then perform forward kinematics to update
 * the robots position at each time step, with each
 * row of data representing the next time step. Euler's
 * method is used to numerically approximate the
 * the integration of the velocity functions.
 * 
 */


/******** Includes **********/

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<iomanip>

/******** Using Statements ****/
using namespace std;

/******** Sructs ******/
struct triplet_t
{
  double x, y, theta;
};


/**** Function Prototypes *****/
triplet_t get_deltas(double theta, double r_vel, double l_vel, double thCoef, double xyCoef);
void skip_to_next_line(ifstream &fin);

/***** Main Function *****/
int main()
{
	double curr_t = 0;	//Current time
	double delta_t = 0.1;	//The time step
	double thCoef;	//For efficiency
	double wheel_distance = 45.72; //distance betwen wheels in cm
	ifstream fin;	//File in
	ofstream fout;	//File out
	double r_vel, l_vel;	//Current x and y wheel velocities
	double xyCoef;	//For efficiency
	triplet_t curr, delta;	//Current and delta xy position and theta
	
	//Initialize the starting position and pose of the robot
	curr.x = 0;
	curr.y = 0;
	curr.theta = 0;
	
	//Open the data file and the file that we will write to
	fout.open("data/prog1_data_out.txt");
	fin.open("data/prog1_data.txt");
	
	//Check to make sure that the data file opened correctly
	if(!fin)
	{
		cout << "Please make sure data file is in the data folder" << endl;
		return -1;
	}
	
	//Initialize the efficiency coefficients that are used by
	//the get_deltas function
	xyCoef = delta_t / 2;
	thCoef = xyCoef / wheel_distance;
	
	//Read in data form the file until the end of the file is reached
	while(!fin.eof())
	{
		//Get the next x and y velocity values
		fin >> r_vel >> l_vel;
		
		//Compute delta_x, delta_y, and delta_z based on the wheel velocities.
		//The wheel velocities here are converted to cm/s from 0.1in/sec
		delta = get_deltas(curr.theta, (r_vel / 0.254), (l_vel / 0.254), thCoef, xyCoef);
		
		//Update the current xy position and pose as well as the current time
		curr.x += delta.x;
		curr.y += delta.y;
		curr.theta += delta.theta;
		curr_t = curr_t + delta_t;
		
		//Output the current xy position and pose to the screen
		cout << fixed << setprecision(2)<< curr.x << "  " << curr.y << "  ";
		cout << fixed << setprecision(4) << curr.theta << endl;
		
		//Output the current xy position and pose to a new data file
		fout << fixed << setprecision(2)<< curr.x << "  " << curr.y << "  ";
		fout << fixed << setprecision(4) << curr.theta << endl;
				
		//Skip to the next data line in the file
		skip_to_next_line(fin);
		
	}
	
	//Close the input and output files
	fin.close();
	fout.close();

  return 0;
}

/*****
 * Function: skip_to_next_line
 * Input: 
 * 	ifstream &fin - reference to the file
 * 		in which we skip to the next line
 * Output: none
 * 
 * Utitlity:
 *	This function will read characters
 * 		until the new line or carriage return
 * 		charact is read in which effectively
 * 		skips to the next line in the file
 * 
 *****/
void skip_to_next_line(ifstream &fin)
{
	char temp;	//Temporary placeholder
	char nl = 10;	//New line character
	char cr = 13;	//Carriage return character
	
	//Get the next value from the file
	fin.get(temp);
	
	//Continue getting values until nl, cr, or end
	//of file are encountered
	while(temp != nl && temp != cr && !fin.eof() )
	{
		fin.get(temp);
	}
}

/*****
 * Function: get_deltas
 * Input: 
 * 	theta - current pose of the robot in radians
 * 	xvel - velocity of right wheel
 * 	yvel - velocity of left wheel
 * 	thCoef - efficiency coefficient for theta computation
 * 	xyCoef - efficiency coefficient for xy computation
 * 	
 * Output:
 * 	delta - triple_t struct that contains the delta x, delta y,
 * 		and delta theta that were computed
 * 
 * Utitlity: 
 * 	This function uses forward kinematics for a differential
 * 		drive robot in order to compute the change in
 * 		xy position and the pose of the robot based upon
 * 		left and right wheel velocities and the current
 * 		pose of the robot
 * 
 *****/
triplet_t get_deltas(double theta, double r_vel, double l_vel, double thCoef, double xyCoef)
{
	triplet_t delta;
	
	//Compute delta theta, delta xn and delta y based upon
	//the right and left wheel velocities
	delta.theta = thCoef * (r_vel - l_vel);
	delta.x = xyCoef * (r_vel + l_vel) * cos(theta);
	delta.y = xyCoef * (r_vel + l_vel) * sin(theta);
	
	return delta;
}