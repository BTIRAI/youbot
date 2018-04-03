#include <iostream>
#include <chrono>
#include <thread>
#include "vrep_utilities.h"
using namespace std;

extern "C" {
    #include "extApi.h"
/*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
}



int main()
{


vrep_utilities::init();
vrep_utilities::startSimulation();
//vrep_utilities::gotoObject("Disc1");
vrep_utilities::approachObject("yellowRectangle");
std::this_thread::sleep_for(std::chrono::milliseconds(10000));
vrep_utilities::graspObject("yellowRectangle");
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
vrep_utilities::approachObject("goalRegion");
std::this_thread::sleep_for(std::chrono::milliseconds(10000));

while(vrep_utilities::isObjectGrasped("yellowRectangle"))
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}


vrep_utilities::dropObject();
std::this_thread::sleep_for(std::chrono::milliseconds(3000));


vrep_utilities::approachObject("blueRectangle");
std::this_thread::sleep_for(std::chrono::milliseconds(10000));
vrep_utilities::graspObject("blueRectangle");
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
vrep_utilities::approachObject("goalRegion");
std::this_thread::sleep_for(std::chrono::milliseconds(10000));
vrep_utilities::dropObject();
std::this_thread::sleep_for(std::chrono::milliseconds(3000));


vrep_utilities::approachObject("greenRectangle");
std::this_thread::sleep_for(std::chrono::milliseconds(10000));
vrep_utilities::graspObject("greenRectangle");
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
vrep_utilities::approachObject("goalRegion");
std::this_thread::sleep_for(std::chrono::milliseconds(10000));
vrep_utilities::dropObject();
std::this_thread::sleep_for(std::chrono::milliseconds(3000));




//vrep_utilities::closeGrippers();
//std::this_thread::sleep_for(std::chrono::milliseconds(2000));

//vrep_utilities::openGrippers();
// std::this_thread::sleep_for(std::chrono::milliseconds(20000));

//    int clientID=simxStart((simxChar*)"127.0.0.1",10015,true,false,2000,5);
//    if (clientID!=-1)
//    {
//        cout << "Starting Simulation!" << endl;


//        std::this_thread::sleep_for(std::chrono::milliseconds(3000));


//        simxInt disc1_handle, youbot_base_target_handle, mode;
//        simxFloat disc1_position[3], disc1_orientation[3];
//        mode = simx_opmode_oneshot_wait;



//        cout << "Initialization!" << endl;

//        cout << "Getting Disk Handle!" << endl;

//        simxGetObjectHandle(clientID, "Disc1", &disc1_handle, mode);

//        simxGetObjectHandle(clientID, "youBot_vehicleTargetPosition", &youbot_base_target_handle, mode);

//        cout << "Getting Disk postion!: "<< youbot_base_target_handle  << endl;

//        simxGetObjectPosition(clientID, disc1_handle, -1, disc1_position, mode);
//        simxGetObjectOrientation(clientID, disc1_handle, -1, disc1_orientation, mode);

//        cout << "setting youbot target !" << disc1_position  << endl;

//        simxSetObjectPosition(clientID, youbot_base_target_handle,-1, disc1_position, mode);
//        simxGetObjectOrientation(clientID, youbot_base_target_handle, -1, disc1_orientation, mode);

//        std::this_thread::sleep_for(std::chrono::milliseconds(20000));
//        simxStopSimulation(clientID, mode);
//    }
//    else
//    {

//        cout << "no Hello World!" << endl;

//    }



    return 0;
}
