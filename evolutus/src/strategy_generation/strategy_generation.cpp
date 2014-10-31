/*
 *Last updated October 30, 2014. Created on October 30, 2014.
 *Edited by Yuhan Long.
 *
 * Copyright (c) 2014, Evolutus.
 * All rights reserved .
 */


#include "sensor_msgs/PointCloud2.h"
#include <ros/ros.h>
#include <string>
#include "evolutus/CmdType.h"
#include "evolutus/PoseType.h"

std::string st_in  = "pose_origin";
std::string st_out = "pose_out";
std::string tf_in = "pose_tf";

void strategyGenerationCallBack(const evolutus::PoseType pos)
{
}

void transformationCallBack(const evolutus::PoseType tf)
{
}

int main(int argc, char **argv)
{
  uint32_t queue_size = 1000;
  ros::init(argc, argv, "strategy_generation");
  ros::NodeHandle nh;

  ros::Subscriber sub_pose = nh.subscribe<evolutus::PoseType>(st_in,queue_size,strategyGenerationCallBack);
  ros::Subscriber sub_tf = nh.subscribe<evolutus::PoseType>(tf_in,queue_size,transformationCallBack);
  ros::Publisher pub_pose = nh.advertise<evolutus::PoseType>(st_out,queue_size);
  ros::spin();
}
