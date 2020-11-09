#ifndef CAR_COM_NODELET_H
#define CAR_COM_NODELET_H

#include <nodelet/nodelet.h>
#include <car_com/car_com_ros.h>


namespace car {
namespace com {


class CarComNodelet: public CarComROS, public nodelet::Nodelet {
public:
    virtual void onInit();
};

};
};
#endif  // CAR_COM_NODELET_H
