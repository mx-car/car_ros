#include <car_com/car_com_nodelet.h>
#include <pluginlib/class_list_macros.h>

    
using namespace car::com;

void CarComNodelet::onInit() {

    NODELET_INFO ("Initializing nodelet CarComNodelet...");
        CarComROS::init(getNodeHandle(), getPrivateNodeHandle());
}



PLUGINLIB_EXPORT_CLASS (car::com::CarComNodelet, nodelet::Nodelet)
//int main(int argc, char **argv) {}
