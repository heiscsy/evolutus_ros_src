#include "ros/ros.h"
#include "tf/transform_listener.h"
#include "tf/tf.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud2.h"
#include "laser_geometry/laser_geometry.h"

std::string targetFrame;
std::string inTopic;
std::string outTopic;
ros::Publisher *publisherPointer;

void pc2projectorCallback (const sensor_msgs::LaserScan::ConstPtr& msg)
{
  static laser_geometry::LaserProjection projector;
  static tf::TransformListener listener; 

  if (!listener.waitForTransform(
    msg->header.frame_id,
    targetFrame,
    msg->header.stamp + 
    ros::Duration().fromSec(msg->ranges.size() * msg->time_increment),
    ros::Duration(1.0))) {
    return; 
  }

  sensor_msgs::PointCloud2 cloud;
  projector.transformLaserScanToPointCloud(targetFrame,
    *msg, cloud, listener, 10);

  publisherPointer->publish(cloud);

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pc2projector");

  if (argc != 4) {
    ROS_ERROR("Need topics and target frame name!"); return -1; }
  
  inTopic = argv[1];
  outTopic = argv[2];
  targetFrame = argv[3];

  ros::NodeHandle handle;
  ros::Subscriber subscriber = handle.subscribe(inTopic, 1000, pc2projectorCallback);
  ros::Publisher publisher = handle.advertise<sensor_msgs::PointCloud2>(outTopic, 1000);
 
  publisherPointer = &publisher;

  ROS_INFO("pc2projector spinning! on %s", inTopic.c_str());

  ros::spin();
  return 0; 
}

