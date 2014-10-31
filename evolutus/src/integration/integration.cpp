/*
 *Last updated October 30, 2014. Created on October 30, 2014.
 *Edited by Yuhan Long.
 *
 * Copyright (c) 2014, Evolutus.
 * All rights reserved .
 */

#include "sensor_msgs/PointCloud2.h"
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <string>
#include "evolutus/CmdType.h"
#include "evolutus/PoseType.h"

std::string pc_left = "pc_left";
std::string pc_right = "pc_right";
std::string cmd = "command_out_topic";
std::string pc_out_topic = "pc_integrate";

void integrationCallBack(const sensor_msgs::PointCloud2ConstPtr pc)
{
}

void signalCallBack(const evolutus::CmdType cmd)
{
}



int main(int argc, char **argv)
{
  uint32_t queue_size = 1000;  
  ros::init(argc, argv, "integration");
  ros::NodeHandle nh;  


  ros::Subscriber sub_pc_left = nh.subscribe<sensor_msgs::PointCloud2>(pc_left,queue_size,integrationCallBack);
  ros::Subscriber sub_pc_right = nh.subscribe<sensor_msgs::PointCloud2>(pc_right,queue_size,integrationCallBack);
  ros::Subscriber sub_cmd = nh.subscribe<evolutus::CmdType>(cmd,queue_size,signalCallBack);
  ros::Publisher pub_cmd = nh.advertise<sensor_msgs::PointCloud2>(pc_out_topic,queue_size);
  ros::spin();
}

