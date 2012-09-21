/* Author: Brian Fehrman
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

int compute_pos_diff_drive( string file_in_name, string file_out_name, float delta_t,
			      triplet_t curr, float wheel_distance);

triplet_t get_deltas(double theta, double r_vel, double l_vel, double thCoef, double xyCoef);

void skip_to_next_line(ifstream &fin);

/******* Program **********/

/*****
 * Function: main
 * Input: 
 * 	argc - number of command line argument
 * 	argv - array of command line arguments
 * 
 * Output: 0 if everything went ok, -1 if not
 * 
 * Utitlity:
 *	Gets the command line arguments (if any)
 * 	and creates an output file if there were any
 * 	arguments given. If no arguments are given
 * 	then the program uses the default file names.
 * 	In this function the wheel distance, time step, and
 * 	starting position and pose for the robot are specified.
 * 	The function to compute the xy position and theta pose
 * 	at each time step is called.
 * 
 *****/
int main(int argc, char* argv[])
{
  	triplet_t curr;	//Current and delta xy position and theta
	double delta_t = 0.1; //time step in seconds
	string file_in_name = "data/prog1_data.txt";  //Default file in
	string file_out_name = "data/prog1_data_out.txt"; //Default file out
	unsigned int i = 0;
	int return_value = 0;
	double wheel_distance = 22.86; //distance betwen wheels in cm
	
	//Check to see if an argument was given. If so, set the
	//input file name to that argument. Then extract the name of
	//the file up to the period or end of string and
	//append _out.txt for the output file name. If a filename
	//is not given, the default filename will be used
	if(argc > 1)
	{
	 file_in_name = argv[1];
	 
	 i = file_in_name.size() - 1;
	 
	 //Start at the end of the string since the beginning
	 //might contain ".." to go up a directory
	 while(file_in_name.compare(i, 1, ".") != 0 && i > 0)
	 {
	   i--;
	 }
	 
	 file_out_name = file_in_name.substr(0, i);
	 
	 file_out_name += "_out.txt";
	}
	
	//Initialize the starting position and pose of the robot
	curr.x = 0;
	curr.y = 0;
	curr.theta = 0;
	
	//Call the function to compute the position and pose for each time step
	return_value = compute_pos_diff_drive(file_in_name, file_out_name, delta_t, curr, wheel_distance);

  return return_value;
}

/*****
 * Function: compute_pos_diff_drive
 * Input: 
 * 	file_in_name - name of the data file to read
 * 	file_out_name - name of the file to write to
 * 	delta_t - time step to use
 * 	curr - contains the starting position and pose of the robot
 * 	wheel_distance - 1/2 distance from wheel to wheel
 * 	
 * Output: none
 * 
 * Utitlity: 
 * 	This function uses the Euler method to update the position
 * 	of a differential drive robot given a file name to read the
 * 	the data from. The file contains columns of data recorded
 * 	by robot sensors. Of the most importance now are the first
 * 	two columns which have the right and left wheel velocities
 * 	respectively. This functions reads those in, computes the
 * 	calls the forward kinematics function "get_deltas" to
 * 	get the change in xy position and theta pose. These values
 * 	are then used to update the current xy position and pose.
 * 	At each time step, the current xy position and theta pose
 *	are printed to the screen and to the output file whose
 * 	name is specified as an argument.
 * 
 *****/
int compute_pos_diff_drive( string file_in_name, string file_out_name, float delta_t,
			      triplet_t curr, float wheel_distance)
{
	triplet_t delta;
  	ifstream fin;	//File in
	ofstream fout;	//File output
	double r_vel, l_vel;	//Current x and y wheel velocities
	double thCoef;	//For efficiency
	double xyCoef;	//For efficiency
	
  	//Open the data file and the file that we will write to
	fout.open(file_out_name.c_str());
	fin.open(file_in_name.c_str());
	
	//Check to make sure that the data file opened correctly
	if(!fin)
	{
		cout << "Please make sure data file is in the correct location" << endl;
		return -1;
	}
	
	//Initialize the efficiency coefficients that are used by
	//the get_deltas function
	xyCoef = delta_t/2;
	thCoef = xyCoef / wheel_distance;
	
	//Read in data form the file until the end of the file is reached
	while(!fin.eof())
	{
		//Get the next x and y velocity values
		fin >> r_vel >> l_vel;
		
		//Compute delta_x, delta_y, and delta_z based on the wheel velocities.
		//The wheel velocities here are converted to cm/s from 0.1in/sec
		delta = get_deltas(curr.theta, (r_vel * 0.254), (l_vel * 0.254), thCoef, xyCoef);
		
		//Update the current xy position and pose as well as the current time
		curr.x += delta.x;
		curr.y += delta.y;
		curr.theta += delta.theta;
		
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