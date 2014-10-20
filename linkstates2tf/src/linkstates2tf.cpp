#include "ros/ros.h"
#include "gazebo_msgs/LinkStates.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Transform.h"
#include "geometry_msgs/Quaternion.h"
#include "tf/tf.h"
#include "tf/transform_broadcaster.h"

std::string targetFrame;

void linkstates2tfCallBack(const gazebo_msgs::LinkStates::ConstPtr& msg)
{
  static tf::TransformBroadcaster broadcaster;

  for (int index = 0; index < msg->name.size(); index++) {

    tf::Transform transform(
      tf::Quaternion(
        msg->pose[index].orientation.x, 
        msg->pose[index].orientation.y,
        msg->pose[index].orientation.z,
        msg->pose[index].orientation.w), 
      tf::Vector3(
        msg->pose[index].position.x,
        msg->pose[index].position.y,
        msg->pose[index].position.z));

    broadcaster.sendTransform(tf::StampedTransform(
      transform,          // the transform,
      ros::Time::now(),   // the time,
      targetFrame,        // the target frame,
      msg->name[index])); // the source frame.
   
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "linkstates2tf");

  if (argc != 2) {
    ROS_ERROR("targetFrame not specified!"); return -1; }
  targetFrame = argv[1];

  ros::NodeHandle handle;
  ros::Subscriber subscriber = handle.subscribe(
    "/gazebo/link_states", 1000, linkstates2tfCallBack);

  ROS_INFO("linkstates2tf spinning!");
  ros::spin();
  return 0; 
}
