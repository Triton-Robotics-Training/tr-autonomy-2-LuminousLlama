// #ifndef YOUR_SOLUTION_SRC_YOUR_SOLUTION_H_
// #define YOUR_SOLUTION_SRC_YOUR_SOLUTION_H_


#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

using ImageMsg = sensor_msgs::msg::Image; 

class AimToCenter : public rclcpp::Node {
  public:
    AimToCenter();
  private:
    rclcpp::Subscription<ImageMsg>::SharedPtr imageSub_;

    void imageSubCallback(const ImageMsg::SharedPtr &msg);

};


// #endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_s