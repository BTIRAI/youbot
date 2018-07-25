#ifndef VREP_UTILITIES_H
#define VREP_UTILITIES_H

#include <iostream>

extern "C" {
    #include "extApi.h"
/*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
}


namespace vrep_utilities
{
    void init();
    void startSimulation();
    void stopSimulation()
;
    void closeGrippers();
    void openGrippers();
    simxInt getHandle(std::string name);
    void gotoObject(std::string name);
    void graspObject(std::string name);
    void invPose(simxInt object_handle);
    bool isObjectGrasped(std::string name);
    bool isObjectAt(std::string object_name, std::string at_name);
    bool isRobotCloseTo(std::string object_name);

    void approachObject(std::string name);
    void dropObject();
    void stopMove();
}

#endif // VREP_UTILITIES_H
