#include "sensor_msgs/PointCloud2.h"
#include <pcl/point_types.h>
#include <ros/ros.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/conversions.h>
#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>

std::string pointCloudInLeft;//input frame name left
std::string pointCloudInRight;//input frame name right
std::string pointCloudOut;//output frame name
ros::Publisher *publisherPointer;//point to publisher
static int msgcount=0;



/*call back function for subscriber*/
void preprocessCallback (const sensor_msgs::PointCloud2ConstPtr pc){
//void preprocessCallback (pcl::PCLPointCloud2::Ptr pc){

    static pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_integrate (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_rev (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PCLPointCloud2::Ptr cloud_pc2(new pcl::PCLPointCloud2);
//    static pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_integrate (new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_rev (new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filter (new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::PCLPointCloud2::Ptr cloud;
    sensor_msgs::PointCloud2 cloud;
    pcl_conversions::toPCL(*pc, *cloud_pc2);
    if(cloud_pc2!=NULL)
    pcl::fromPCLPointCloud2 (*cloud_pc2, *cloud_rev); //* convert from pcl/PCLPointCloud2 to pcl::PointCloud<T>
//    pcl::fromROSMsg(pc,cloud_integrate);
   // pcl::concatenatePointCloud(cloud_integrate, cloud_rev, cloud_integrate)   
  //  static pcl::PCLPointCloud2::Ptr cloud (new pcl::PCLPointCloud2());
    *cloud_integrate = *cloud_integrate+ *cloud_rev;
    msgcount++;
   
    if(msgcount == 800) {// (2 topics, each with data rate at 40hz, integrate for 20s)
     //build voxelization filter
        ROS_ERROR("20s (1600 point cloud) past!");
       // pcl::VoxelGrid<pcl::PointXYZ> sor;
       // sor.setInputCloud (cloud_integrate);
      //  sor.setLeafSize(0.05,0.05,0.05);
      //  sor.filter(*cloud_filter);
      //  pcl::toPCLPointCloud2(*cloud_filter, *cloud_pc2);
        pcl::toPCLPointCloud2(*cloud_integrate, *cloud_pc2);
        pcl_conversions::fromPCL(*cloud_pc2,cloud);
        cloud.header.frame_id=pc->header.frame_id;
        publisherPointer->publish(cloud);
       // msgcount = 0;
    }
        
}

int main(int argc, char **argv)
{

    ros::init (argc, argv, "preprocess");
    pointCloudInLeft = argv[1];
    pointCloudInRight = argv[2];
    pointCloudOut = argv[3];
    ros::NodeHandle nh;
    uint32_t queue_size = 1000;
    
    ros::Subscriber sub_1 = nh.subscribe<sensor_msgs::PointCloud2> (pointCloudInLeft, queue_size, preprocessCallback);
    ros::Subscriber sub_2 = nh.subscribe<sensor_msgs::PointCloud2> (pointCloudInRight, queue_size, preprocessCallback);
    ros::Publisher pub = nh.advertise<sensor_msgs::PointCloud2> (pointCloudOut, queue_size);
   // ros::Publisher pub = nh.advertise<pcl::PCLPointCloud2> (pointCloudOut, queue_size);
   // ros::Subscriber sub_1 = nh.subscribe<pcl::PCLPointCloud2> (pointCloudInLeft, queue_size, preprocessCallback);
   // ros::Subscriber sub_2 = nh.subscribe<pcl::PCLPointCloud2> (pointCloudInRight, queue_size, preprocessCallback);

    publisherPointer = &pub;  

    ros::spin();
}



