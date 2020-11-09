# car_com

## To build option for the nodelet
### carcom
This node needs carcom installed. Therfore download and install [carcom](https://github.com/mx-car/carcom).
### node
This crates a normal node
```
catkin_make -DCMAKE_BUILD_TYPE=Debug -DNODELET_DYNAMIC_LOAD=OFF
```
### nodelet
This crates a node which loads the nodelet
```
catkin_make -DCMAKE_BUILD_TYPE=Debug -DNODELET_DYNAMIC_LOAD=ON
```
