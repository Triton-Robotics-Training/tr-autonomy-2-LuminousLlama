#include <cstdio> //need this?
#include "your_solution.h"
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.hpp>


using std::placeholders::_1;

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  printf("hello world your_solution package\n");
  return 0;
}

AimToCenter::AimToCenter() : Node("AimToCenter")
{
  imageSub_ = this->create_subscription<ImageMsg>("robotcam",
                                                  10,
                                                  std::bind(&AimToCenter::imageSubCallback, this, _1));




}

void AimToCenter::imageSubCallback(const ImageMsg::SharedPtr &msg){
  
  
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
    cv::Point2f img_center(cvPtr->image.cols / 2.0f, cvPtr->image.rows / 2.0f);

    cv::Point2f offset;
    offset.x = blob_center.x - img_center.x;
    offset.y = blob_center.y - img_center.y;

    RCLCPP_INFO(this->get_logger(), "offset: (%.2f, %.2f)", offset.x, offset.y);
  }



}
