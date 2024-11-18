#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

#include <string>
using namespace std;

cv::VideoWriter video_writer;

// params below conform to the python script provided; adjust if necessary
// IMPORTANT NOTE: stereo_rig_name should be named "test" !! Alternatively, change the 1st parameter in img_handle.subscribe()
int fps = 30;
int w = 1280;
int h = 1024;

string output_name = "output_video.avi";

void image_callback(const sensor_msgs::ImageConstPtr msg) {

    try {
        cv::Mat cv_img = cv_bridge::toCvShare(msg, "bgr8")->image;
        
        cv::Mat resized_img;
        cv::resize(cv_img, resized_img, cv::Size(w,h));

        video_writer.write(resized_img);
    }

    catch (cv_bridge::Exception& exception) {
        ROS_ERROR("cv_bridge throws exception: %s", exception.what());
    }

}



int main(int argc, char** argv) {
    ros::init(argc, argv, "video_subscriber_tool");
    ros::NodeHandle nh;
    
    video_writer.open(output_name, cv::VideoWriter::fourcc('X','V','I','D'), fps, cv::Size(w,h));

    if(!video_writer.isOpened()) {
        ROS_ERROR("Could not create/open output file!");
        return -1;
    }

    image_transport::ImageTransport img_handle(nh);
    image_transport::Subscriber subs = img_handle.subscribe("/test/right/image_raw", 1, image_callback);

    ros::spin();

    video_writer.release();
    return 0;


}