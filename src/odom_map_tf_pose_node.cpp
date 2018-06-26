#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_listener.h>



int main(int argc, char ** argv)
{
  ros::init(argc, argv, "odom_map_tf_pose");
  
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/robot_map",1000);
  tf::TransformListener listener;
  
  
  
  ros::Rate rate(10.0);
  
  while(n.ok())
  {
    tf::StampedTransform transform;
    try
    {
      listener.waitForTransform("/map","/odom",ros::Time(0),ros::Duration(2.0));
      listener.lookupTransform("/map","/odom",ros::Time(0),transform);
      
      
    }
    catch(tf::TransformException &ex)
    {
      ROS_ERROR("%s",ex.what());
    }
    
    geometry_msgs::PoseStamped pose;
    pose.header.stamp = transform.stamp_;
    pose.header.frame_id = "map";
    pose.pose.position.x = transform.getOrigin().x();
    pose.pose.position.y = transform.getOrigin().y();
    pose.pose.position.z = transform.getOrigin().z();
    pose.pose.orientation.x = transform.getRotation().getX();
    pose.pose.orientation.y = transform.getRotation().getY();
    pose.pose.orientation.z = transform.getRotation().getZ();
    pose.pose.orientation.w = transform.getRotation().getW();
      
    pub.publish(pose);
    
    
    rate.sleep();
  }
  
  return 0;
}