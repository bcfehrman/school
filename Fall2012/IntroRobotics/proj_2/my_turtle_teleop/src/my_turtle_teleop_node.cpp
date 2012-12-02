/*
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ros/ros.h>
#include <Color.h>
#include <Velocity.h>
#include <geometry_msgs/Twist.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"

#define KEYCODE_R 0x43 
#define KEYCODE_L 0x6C
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71
#define KEYCODE_I 0x69 
#define KEYCODE_M 0x6D
#define KEYCODE_J 0x6A
#define KEYCODE_K 0x6B
#define KEYCODE_SPACE 0x20


class TurtlebotTeleop
{
public:
  TurtlebotTeleop();
  void keyLoop();
  void watchdog();
  void publish();

private:

  
  ros::NodeHandle nh_,ph_;
  double linear_, angular_;
  ros::Time first_publish_;
  ros::Time last_publish_;
  double l_scale_, a_scale_;
  ros::Publisher vel_pub_;

  boost::mutex publish_mutex_;

};

TurtlebotTeleop::TurtlebotTeleop():
  ph_("~"),
  linear_(0),
  angular_(0),
  l_scale_(1.0),
  a_scale_(1.0)
{
  ph_.param("scale_angular", a_scale_, a_scale_);
  ph_.param("scale_linear", l_scale_, l_scale_);

  vel_pub_ = nh_.advertise<turtlesim::Velocity>("/turtle1/command_velocity", 1);
}

int kfd = 0;
struct termios cooked, raw;

void quit(int sig)
{
  tcsetattr(kfd, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "turtlebot_teleop1");
  TurtlebotTeleop turtlebot_teleop;
  ros::NodeHandle n;

  signal(SIGINT,quit);

  boost::thread key_thread(boost::bind(&TurtlebotTeleop::keyLoop, &turtlebot_teleop));
  boost::thread pub_thread(boost::bind(&TurtlebotTeleop::publish, &turtlebot_teleop));
  
  
  ros::Timer timer = n.createTimer(ros::Duration(0.1), boost::bind(&TurtlebotTeleop::watchdog, &turtlebot_teleop));

  ros::spin();

  key_thread.interrupt() ;
  key_thread.join() ;
  pub_thread.interrupt() ;
  pub_thread.join() ;
      
  return(0);
}


void TurtlebotTeleop::watchdog()
{
  boost::mutex::scoped_lock lock(publish_mutex_);
  if ((ros::Time::now() > last_publish_ + ros::Duration(0.15)) && 
      (ros::Time::now() > first_publish_ + ros::Duration(0.50)))
    publish();
}


void TurtlebotTeleop::keyLoop()
{
  char c;

	angular_ = 0;
	linear_ = 0;

  // get the console in raw mode                                                              
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  // Setting a new line, then end of file                         
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);

  puts("Reading from keyboard");
  puts("---------------------------");
  puts("Use arrow keys to move the turtlebot.");



  while (ros::ok())
  {    

    boost::mutex::scoped_lock lock(publish_mutex_);
    if (ros::Time::now() > last_publish_ + ros::Duration(1.0)) { 
      first_publish_ = ros::Time::now();
    }
    last_publish_ = ros::Time::now();
    
    
     ROS_DEBUG("value: 0x%02X\n", c);
     
    // get the next event from the keyboard  ls
    if(read(kfd, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }

    switch(c)
    {
      case KEYCODE_J:
        ROS_DEBUG("J");
        angular_ += 1.0;
        break;
      case KEYCODE_L:
        ROS_DEBUG("L");
        angular_ += -1.0;
        break;
      case KEYCODE_I:
        ROS_DEBUG("I");
        linear_ += 1.0;
        break;
      case KEYCODE_M:
        ROS_DEBUG("M");
        linear_ += -1.0;
        break;
        
      case KEYCODE_K:
      case KEYCODE_SPACE:
        ROS_DEBUG("K");
        linear_  = 0.0;
        angular_ = 0.0;
        break;
    }
  }

  return;
}

void TurtlebotTeleop::publish()  
{
  ros::Rate loop_rate(10);
      char key1[ 56 ];
while (ros::ok())
  { 
	      sprintf(key1,"%f",angular_);
    puts( key1 );
     
    sprintf(key1,"%f", linear_);
    puts( key1  );
    
	  
    turtlesim::Velocity vel;
    vel.angular = a_scale_*angular_;
    vel.linear = l_scale_*linear_;

    vel_pub_.publish(vel);    

    ros::spinOnce();

    loop_rate.sleep();

	}
  return;
}



