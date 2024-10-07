#include <cstdio> //need this?
#include "your_solution.h"
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.hpp>


using std::placeholders::_1;

int main(int argc, char **argv)
{

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AimToCenter>());
  rclcpp::shutdown();

  return 0;
}

const double IMG_WIDTH = 640.0;
const double FOV = 90.0;
const double ANGLE_PER_PIXEL_DEGREES = FOV / IMG_WIDTH;

AimToCenter::AimToCenter() : Node("AimToCenter")
{
  imageSub_ = image_transport::create_subscription(this, "robotcam", std::bind(&AimToCenter::imageSubCallback, this, _1), "raw");
  current_angle_sub_ = this->create_subscription<Float32Msg>("/current_angle", 10, std::bind(&AimToCenter::current_angle_callback, this, _1));

  desired_angle_pub_ = this->create_publisher<Float32Msg>("/desired_angle", 10);
}


void AimToCenter::current_angle_callback(const Float32Msg::SharedPtr msg){
  current_angle_rads_ = msg->data;
}

void AimToCenter::imageSubCallback(const ImageMsg::ConstSharedPtr &msg){
  cv_bridge::CvImagePtr cvPtr = cv_bridge::toCvCopy(msg, msg->encoding);

  cv::Mat hsv;
  cv::cvtColor(cvPtr->image, hsv, cv::COLOR_BGR2HSV);

  cv::Scalar lowerRed(0, 100, 100);
  cv::Scalar upperRed(10,255,255);
  cv::Mat mask;
  cv::inRange(hsv, lowerRed, upperRed, mask);

  cv::Moments m = cv::moments(mask, true);
  if(m.m00 == 0){
    RCLCPP_INFO(this->get_logger(), "NO RED DETECTED");
  } else{
    cv::Point2f blob_center(m.m10 / m.m00, m.m01 / m.m00);
    double offset = blob_center.x - (IMG_WIDTH / 2);

    double angle_offset = -1.0f * offset * ANGLE_PER_PIXEL_DEGREES;
    angle_offset = angle_offset * (M_PI / 180);
    double desired_angle_rads = current_angle_rads_ + angle_offset;

    RCLCPP_INFO(this->get_logger(), "offset: %f | current_angle: %f | angle_offset: %f | desired_angle: %f", offset, current_angle_rads_, angle_offset, desired_angle_rads);

    Float32Msg::SharedPtr msg = std::make_shared<Float32Msg>();
    msg->data = desired_angle_rads;
    desired_angle_pub_->publish(*msg);
  }



}
