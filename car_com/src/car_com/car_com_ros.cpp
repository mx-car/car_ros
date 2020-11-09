#include <car_com/car_com_ros.h>

using namespace car::com;

void CarComROS::init(ros::NodeHandle &public_nh, ros::NodeHandle &private_nh) {

    ROS_INFO ("Initializing CarComROS...");
    nh_ = public_nh;
    private_nh_ = private_nh;
    private_nh_.param<double> ( "watchdog_duration", watchdog_duration_, 5.0 );
    reconfigureFnc_ = boost::bind ( &CarComROS::callbackConfigCarCom, this,  _1, _2 );
    reconfigureServer_.setCallback ( reconfigureFnc_ );
    sub_cmd_twist_ = nh_.subscribe("cmd_vel", 10, &CarComROS::callbackCmdTwist, this);
    timer_ = nh_.createTimer(ros::Duration(watchdog_duration_), &CarComROS::callbackTimerWatchdog, this);
    connect();
}

void CarComROS::callbackTimerWatchdog(const ros::TimerEvent& event) {
    ROS_DEBUG ("callbackTimerWatchdog");    /// @note there are some issues with 'rosparam get /use_sim_time'
}

void CarComROS::callbackConfigCarCom ( car_com::CarComConfig &config, uint32_t level ) {
    ROS_INFO ("callbackConfig");
    config_ = config;
    if(config_.debug_loglevel) {
        if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
            ros::console::notifyLoggerLevelsChanged();
        }
    }
}

void CarComROS::callbackCmdTwist ( const geometry_msgs::Twist::ConstPtr& msg) {
    ROS_DEBUG ("callbackCmdTwist");
    if(!command_ackermann_) command_ackermann_ = std::shared_ptr<objects::CommandAckermann>(new objects::CommandAckermann);
    command_ackermann_->set(msg->linear.x,  msg->angular.z, objects::CommandAckermann::UNIT_DIRECT, true);
}

void CarComROS::callbackSerial ( Message &header,  Objects & objects ) {
    if(!serial_interface_) return;
    objects::StateAckermann state;
    objects::ConfigAckermann config;
    objects::CommandAckermann command;
    objects::Text text;
    for ( Objects::iterator it=objects.begin(); it!=objects.end(); ++it ) {
        objects::Object &object = it->second;
        switch ( it->first ) {
        case objects::TYPE_SYNC_REQUEST:
            ROS_DEBUG ("Sync request");
            break;
        case objects::TYPE_STATE_ACKERMANN:
            object.get ( state );
            ROS_DEBUG ("State %s", state.getToStringReadable().c_str());
            break;
        case objects::TYPE_COMMAND_ACKERMANN:
            object.get ( command );
            ROS_DEBUG ("command %s", command.getToStringReadable().c_str());
            break;
        case objects::TYPE_CONFIG_ACKERMANN:
            object.get ( config );
            ROS_DEBUG ("config %s", config.getToStringReadable().c_str());
            break;
        case objects::TYPE_TEXT:
            object.get ( text );
            ROS_INFO ("Text %s", text.txt);
            break;
        default:
            ROS_WARN_STREAM("Type id: " << object.type << ", of size: " << object.size);
        }
        if(command_ackermann_){
            serial_interface_->addObject(objects::Object(*command_ackermann_, car::com::objects::TYPE_COMMAND_ACKERMANN));
        }
    }
}

void CarComROS::connect() {
    if(!serial_interface_) serial_interface_= std::shared_ptr<pc::SerialInterface>(new pc::SerialInterface);
    pc::Parameters serial_interface_parameter;
    serial_interface_parameter.baudrate = config_.baudrate;
    serial_interface_parameter.port = config_.port;    
    serial_interface_->init ( serial_interface_parameter, std::bind ( &CarComROS::callbackSerial, this, std::placeholders::_1,  std::placeholders::_2 ) );
}
