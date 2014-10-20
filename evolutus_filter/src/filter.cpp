#include "sensor_msgs/PointCloud2.h"
#include <pcl/point_types.h>
#include <ros/ros.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/passthrough.h>
#include <pcl/features/normal_3d.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/extract_indices.h>


std::string pointCloudIN;
std::string pointCloudOUT;
ros::Publisher *publisherPointer;
typedef pcl::PointXYZ PointT;


void filterCallBack(const sensor_msgs::PointCloud2ConstPtr pc){
    pcl::PCLPointCloud2::Ptr cloud_v (new pcl::PCLPointCloud2);
    pcl::PCLPointCloud2::Ptr cloud_f (new pcl::PCLPointCloud2);
    pcl::PCLPointCloud2::Ptr cloud (new pcl::PCLPointCloud2);
    pcl::PCLPointCloud2::Ptr cloud_in (new pcl::PCLPointCloud2);
    pcl::PassThrough<pcl::PCLPointCloud2> pass;
    pcl::ExtractIndices<PointT> extract;
    pcl::SACSegmentation<PointT> seg;
    
    sensor_msgs::PointCloud2 cloud_msgs;

    pcl_conversions::toPCL(*pc, *cloud);
   
    
    pass.setInputCloud(cloud);
    pass.setFilterFieldName("z");
    pass.setFilterLimits(0,1.5);
    pass.filter(*cloud_in);
    
        
    pcl::StatisticalOutlierRemoval<pcl::PCLPointCloud2> sor1;
    sor1.setInputCloud(cloud_in);
    sor1.setMeanK(50);
    sor1.setStddevMulThresh (0.5);
    sor1.filter(*cloud_v);

//    pcl::VoxelGrid<pcl::PCLPointCloud2> sor2;
//    sor2.setInputCloud (cloud_f);
//    sor2.setLeafSize(0.01,0.01,0.01);
//    sor2.filter(*cloud_v);
    


    pcl::PointCloud<PointT>::Ptr inPCseg (new pcl::PointCloud<PointT>);
    pcl::PointCloud<PointT>::Ptr outPCseg (new pcl::PointCloud<PointT>);
    pcl::fromPCLPointCloud2 (*cloud_v,*inPCseg);
        
    pcl::ModelCoefficients::Ptr coefficients_plane (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers_plane (new pcl::PointIndices);
  
    seg.setOptimizeCoefficients (true);
    seg.setModelType (pcl::SACMODEL_PLANE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setMaxIterations (100);
    seg.setDistanceThreshold (0.15);
    seg.setInputCloud (inPCseg);
    // Obtain the plane inliers and coefficients
    seg.segment (*inliers_plane, *coefficients_plane);


    extract.setInputCloud (inPCseg);  
    extract.setIndices (inliers_plane);  
    extract.setNegative (true);
    
    
    pcl::PCLPointCloud2::Ptr object (new pcl::PCLPointCloud2);
    extract.filter (*outPCseg);
    pcl::toPCLPointCloud2(*outPCseg,*cloud_f);


    pcl::StatisticalOutlierRemoval<pcl::PCLPointCloud2> sor3;
    sor3.setInputCloud(cloud_f);
    sor3.setMeanK(50);
    sor3.setStddevMulThresh (0.5);
    sor3.filter(*object);


    pcl_conversions::fromPCL(*object,cloud_msgs);
    cloud_msgs.header.frame_id = pc->header.frame_id;
    publisherPointer->publish(cloud_msgs);
}


int main(int argc, char **argv) {
    ros::init(argc,argv,"filter");
    pointCloudIN  = argv[1];
    pointCloudOUT = argv[2];
    uint32_t queue_size = 1000;

    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>(pointCloudIN,queue_size,filterCallBack);
    ros::Publisher pub = nh.advertise<sensor_msgs::PointCloud2>(pointCloudOUT,queue_size);
    publisherPointer = &pub;
    ros::spin();    
}
