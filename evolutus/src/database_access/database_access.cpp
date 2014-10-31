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

std::string cmd="command_out_topic";
std::string pc="pc_model";
std::string st="pose_tf";

void databaseAccessCallBack(const evolutus::CmdType cmd)
{
}


int main(int argc, char **argv)
{
  uint32_t queue_size = 1000;
  ros::init(argc, argv, "database_access");
  ros::NodeHandle nh;

  ros::Subscriber sub_cmd = nh.subscribe<evolutus::CmdType>(cmd,queue_size,databaseAccessCallBack);
  ros::Publisher pub_pc = nh.advertise<sensor_msgs::PointCloud2>(pc,queue_size);
  ros::Publisher strategy_out = nh.advertise<evolutus::PoseType>(st,queue_size);
  ros::spin();
}
