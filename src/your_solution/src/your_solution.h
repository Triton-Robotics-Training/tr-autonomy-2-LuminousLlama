// #ifndef YOUR_SOLUTION_SRC_YOUR_SOLUTION_H_
// #define YOUR_SOLUTION_SRC_YOUR_SOLUTION_H_


#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/float32.hpp"
#include "image_transport/image_transport.hpp"

using ImageMsg = sensor_msgs::msg::Image; 
using Float32Msg = std_msgs::msg::Float32; 

class AimToCenter : public rclcpp::Node {
  public:
    AimToCenter();
  private:
    double current_angle_rads_ = 0.0;

    image_transport::Subscriber imageSub_;
    rclcpp::Subscription<Float32Msg>::SharedPtr current_angle_sub_;
    
    rclcpp::Publisher<Float32Msg>::SharedPtr desired_angle_pub_;

    void imageSubCallback(const ImageMsg::ConstSharedPtr &msg);
    void current_angle_callback(const Float32Msg::SharedPtr msg);

};


// #endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_s