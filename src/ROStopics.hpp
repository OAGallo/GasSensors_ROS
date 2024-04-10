#ifndef ROStopics_hpp
#define ROStopics_hpp

#include <ros.h>
#include <std_msgs/Float32.h>

ros::NodeHandle node;

std_msgs::Float32 getCO2;
ros::Publisher CO2("CO2", &getCO2);

std_msgs::Float32 getHexano;
ros::Publisher Hx("Hx", &getHexano);

//metano
std_msgs::Float32 getMetano;
ros::Publisher Metano("Metano", &getMetano);

//H2
std_msgs::Float32 getHidrogen;
ros::Publisher Hidrogen("Hidrogen", &getHidrogen);

std_msgs::Float32 getCO;
ros::Publisher CO("CO", &getCO);

#endif

