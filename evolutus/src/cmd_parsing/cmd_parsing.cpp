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

std::string cmd_in_topic = "command_in_topic";
std::string cmd_out_topic = "command_out_topic";

void commandParseCallBack(const evolutus::CmdType cmd)
{
    /*
     *Parse the command from the rviz topic to command type
     *
     *this part needs to be further modified
     *
     *
     */
}


int main(int argc, char **argv)
{
  uint32_t queue_size = 1000;  
  ros::init(argc, argv, "cmd_parsing");
  ros::NodeHandle nh;  

  /*this part need further modification
   * the input topic is from rviz
   */

  ros::Subscriber sub_cmd = nh.subscribe<evolutus::CmdType>(cmd_in_topic,queue_size,commandParseCallBack);
  ros::Publisher pub_cmd = nh.advertise<evolutus::CmdType>(cmd_out_topic,queue_size);
  ros::spin();
}

