#ifndef CAR_COM_ROS_H
#define CAR_COM_ROS_H

#include <ros/ros.h>

#include <geometry_msgs/Twist.h>
#include <dynamic_reconfigure/server.h>
#include <car_com/CarComConfig.h>
#include <car/com/pc/interface.h>

namespace car {
namespace com {
namespace pc {
    class SerialInterface;
}

class CarComROS {
    
private:
    ros::Subscriber sub_cmd_twist_;
    ros::Timer timer_;
    ros::NodeHandle nh_;
    ros::NodeHandle private_nh_;
    double watchdog_duration_;
    car_com::CarComConfig config_;
    dynamic_reconfigure::Server<car_com::CarComConfig> reconfigureServer_; /// parameter server stuff
    dynamic_reconfigure::Server<car_com::CarComConfig>::CallbackType reconfigureFnc_;  /// parameter server stuff
    void callbackConfigCarCom ( car_com::CarComConfig &config, uint32_t level ); /// callback function on incoming parameter changes
    void callbackCmdTwist ( const geometry_msgs::Twist::ConstPtr& msg);
    void callbackTimerWatchdog(const ros::TimerEvent& event);
    std::shared_ptr<pc::SerialInterface> serial_interface_;
    std::shared_ptr<objects::CommandAckermann> command_ackermann_;

public:
    void init(ros::NodeHandle &public_nh, ros::NodeHandle &private_nh);
    void connect();
    void callbackSerial ( Message &header,  Objects & objects );

};
};
};

#endif  // CAR_COM_ROS_H
