#include "vrep_utilities.h"
#include <chrono>
#include <thread>
#include <complex>
#include <mutex>

extern "C" {
    #include "extApi.h"
   // #include "extApiCustom.h" // if you wanna use custom remote API functions!
}



simxInt clientID, disc1_handle, youbot_base_handle, youbot_base_target_handle, youbot_grippers_target_handle, mode = simx_opmode_oneshot_wait;
simxUChar* data;
simxInt dataSize;
std::mutex is_arm_busy_mutex;
bool is_arm_busy;

namespace vrep_utilities
{

void startSimulation()
{

    simxStartSimulation(clientID, 0);

}
void stopSimulation()
{

    simxStopSimulation(clientID, 0);

}
    void init()
    {
        clientID = simxStart((simxChar*)"127.0.0.1",19997,true,false,2000,5);
        youbot_base_target_handle = getHandle("youBot_vehicleTargetPosition");
        youbot_grippers_target_handle = getHandle("youBot_gripperPositionTarget");
        youbot_base_handle = getHandle("youBot_vehicleReference");
        disc1_handle = getHandle("Disc1");
    }





    void openGrippers()
    {
        simxInt result = (simxInt) simxSetIntegerSignal(
          clientID, "gripperCommand", 1,
          simx_opmode_oneshot_wait);
    }

    void closeGrippers()
    {

        simxInt result = (simxInt) simxSetIntegerSignal(
          clientID, "gripperCommand", 0,
          simx_opmode_oneshot_wait);

    }
    void initArm()

    {

        simxFloat position_init[3], orientation_init[3];


        position_init[0] = 0.0;
        position_init[1] = 0.2;
        position_init[2] = 0.2;

        orientation_init[0] = 0.0;
        orientation_init[1] = 0.0;
        orientation_init[2] = 0.0;


        simxSetObjectPosition(clientID, youbot_grippers_target_handle, youbot_base_handle, position_init, mode);
        simxSetObjectOrientation(clientID, youbot_grippers_target_handle, youbot_base_handle, position_init, mode);

    }



    void stopMove()
    {


        simxFloat destination_position[3] = {0, 0, 0}, destination_orientation[3]  = {0, 0, 0};
        simxGetObjectPosition(clientID, youbot_base_target_handle, youbot_base_handle, destination_position, mode);
        simxGetObjectOrientation(clientID, youbot_base_target_handle, youbot_base_handle, destination_orientation, mode);



    }

    void dropObject()
    {

        simxFloat position_init[3], orientation_init[3];


        position_init[0] = 0.0;
        position_init[1] = 0.3;
        position_init[2] = 0.1;

        simxSetObjectPosition(clientID, youbot_grippers_target_handle, youbot_base_handle, position_init, mode);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        openGrippers();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        initArm();



    }

    void graspObject(std::string name)
    {
        {
            std::lock_guard<std::mutex> lock(is_arm_busy_mutex);
            is_arm_busy = true;

        }
        openGrippers();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        simxInt object_handle = getHandle(name);


        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            simxFloat destination_position[3], destination_orientation[3];
            simxGetObjectPosition(clientID, object_handle, -1, destination_position, mode);
            simxGetObjectOrientation(clientID, object_handle, -1, destination_orientation, mode);

            simxSetObjectPosition(clientID, youbot_grippers_target_handle, -1, destination_position, mode);
            simxSetObjectOrientation(clientID, youbot_grippers_target_handle, -1, destination_position, mode);


            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            closeGrippers();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            initArm();
            {
                std::lock_guard<std::mutex> lock(is_arm_busy_mutex);
                is_arm_busy = false;

            }
    }

    void approachObject(std::string name)
    {
        invPose(getHandle(name));
        gotoObject("Disc2");
    }


    simxFloat *getObjectPosition(simxInt object_handle)
    {
        simxFloat object_position[3];
        simxGetObjectPosition(clientID, object_handle, -1, object_position, mode);
        return object_position;
    }


    simxFloat* getObjectOrientation(simxInt object_handle)
    {
        simxFloat object_orientation[3];
        simxGetObjectOrientation(clientID, object_handle, -1, object_orientation, mode);
        return object_orientation;
    }

    simxFloat getDistance(simxInt object1_handle, simxInt object2_handle)
    {
        simxFloat relative_position[3];

        simxGetObjectPosition(clientID, object1_handle, object2_handle, relative_position, mode);

        return std::sqrt(relative_position[0]*relative_position[0] + relative_position[1]*relative_position[1]
                + relative_position[2]*relative_position[2]);

    }


    simxFloat get2DDistance(simxInt object1_handle, simxInt object2_handle)
    {
        simxFloat relative_position[3];

        simxGetObjectPosition(clientID, object1_handle, object2_handle, relative_position, mode);

        return std::sqrt(relative_position[0]*relative_position[0] + relative_position[1]*relative_position[1]);

    }

    void invPose(simxInt object_handle)
    {


        simxInt dummy_disk_handle = getHandle("Disc2");

        simxFloat object_position[3], object_orientation[3], dummy_disk_position[3], dummy_disk_position_abs[3], dummy_disk_orientation[3],
                new_position[3], new_orientation[3];
        simxGetObjectPosition(clientID, object_handle, -1, object_position, mode);
        simxGetObjectPosition(clientID, dummy_disk_handle, object_handle, dummy_disk_position, mode);
        simxGetObjectPosition(clientID, dummy_disk_handle, -1, dummy_disk_position_abs, mode);

        simxGetObjectOrientation(clientID, dummy_disk_handle, -1, dummy_disk_orientation, mode);
        simxGetObjectOrientation(clientID, dummy_disk_handle, -1, object_orientation, mode);

        simxFloat min_distance = 10000, current_distance, min_position[3], min_orientation[3];

        //trying first position

        simxFloat shift = 0.3;

        new_position[0] =  - shift;
        new_position[1] = 0;
        new_position[2] = dummy_disk_position[2];
        simxSetObjectPosition(clientID, dummy_disk_handle, object_handle, new_position, mode);


        new_orientation[0] = 0;
        new_orientation[1] = 0;
        new_orientation[2] = 0 - 3.14142/2;
        simxSetObjectOrientation(clientID, dummy_disk_handle, object_handle, new_orientation, mode);
        current_distance = getDistance(dummy_disk_handle, youbot_base_handle);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            for (int i = 0; i<3; i++)
            {
                min_position[i] =  new_position[i];
                min_orientation[i] = new_orientation[i];
            }
        }
     //   std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        //trying second position

        shift = 0.3;

        new_position[0] =  shift;
        new_position[1] = 0;
        new_position[2] = dummy_disk_position[2];
        simxSetObjectPosition(clientID, dummy_disk_handle, object_handle, new_position, mode);


        new_orientation[0] = 0;
        new_orientation[1] = 0;
        new_orientation[2] = 0 + 3.14142/2;
        simxSetObjectOrientation(clientID, dummy_disk_handle, object_handle, new_orientation, mode);


        current_distance = getDistance(dummy_disk_handle, youbot_base_handle);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            for (int i = 0; i<3; i++)
            {
                min_position[i] =  new_position[i];
                min_orientation[i] = new_orientation[i];
            }
        }

        //trying third position
       // std::this_thread::sleep_for(std::chrono::milliseconds(5000));


        shift = 0.3;

        new_position[0] = 0;
        new_position[1] = - shift;
        new_position[2] = dummy_disk_position[2];
        simxSetObjectPosition(clientID, dummy_disk_handle, object_handle, new_position, mode);


        new_orientation[0] = 0;
        new_orientation[1] = 0;
        new_orientation[2] = 0;
        simxSetObjectOrientation(clientID, dummy_disk_handle, object_handle, new_orientation, mode);

        current_distance = getDistance(dummy_disk_handle, youbot_base_handle);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            for (int i = 0; i<3; i++)
            {
                min_position[i] =  new_position[i];
                min_orientation[i] = new_orientation[i];
            }
        }

        //trying fourth position

     //   std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        shift = 0.3;

        new_position[0] = 0;
        new_position[1] = shift;
        new_position[2] = dummy_disk_position[2];
        simxSetObjectPosition(clientID, dummy_disk_handle, object_handle, new_position, mode);


        new_orientation[0] = 0;
        new_orientation[1] = 0;
        new_orientation[2] = 0 + 3.14142;
        simxSetObjectOrientation(clientID, dummy_disk_handle, object_handle, new_orientation, mode);
        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        current_distance = getDistance(dummy_disk_handle, youbot_base_handle);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            for (int i = 0; i<3; i++)
            {
                min_position[i] =  new_position[i];
                min_orientation[i] = new_orientation[i];
            }
        }

        simxSetObjectPosition(clientID, dummy_disk_handle, object_handle, min_position, mode);
        simxSetObjectOrientation(clientID, dummy_disk_handle, object_handle, min_orientation, mode);
    }




    bool isObjectGrasped(std::string object_name)
    {
        simxInt handle;
        simxGetObjectHandle(clientID, object_name.c_str(), &handle, mode);

        simxFloat distance = getDistance(handle, youbot_grippers_target_handle);
        bool busy;

        {
            std::lock_guard<std::mutex> lock(is_arm_busy_mutex);
            busy = is_arm_busy;

        }
        return distance < 0.01 && !busy;
    }


    bool isRobotCloseTo(std::string object_name)
    {
        simxInt handle;
        simxGetObjectHandle(clientID, object_name.c_str(), &handle, mode);

        simxFloat distance = get2DDistance(handle, youbot_base_handle);

        std::cout << "[vrep utilities]*****************ROBOT CLOSE TO" << object_name << "distance " << distance << std::endl;

        return distance < 0.32;
    }


    bool isObjectAt(std::string object_name, std::string at_name)
    {

        std::cout << "2d distance: "<< object_name << " " << at_name << " : " << get2DDistance(getHandle(object_name), getHandle(at_name)) << std::endl;

        return get2DDistance(getHandle(object_name), getHandle(at_name)) < 0.01;
    }

    simxInt getHandle(std::string name)
    {

        simxInt handle;
        simxGetObjectHandle(clientID, name.c_str(), &handle, mode);
        return handle;
    }


    void gotoObject(std::string name)
    {

        simxInt destination_handle = getHandle(name);

        simxFloat destination_position[3], destination_orientation[3];
        simxGetObjectPosition(clientID, destination_handle, -1, destination_position, mode);
        simxGetObjectOrientation(clientID, destination_handle, -1, destination_orientation, mode);

        simxSetObjectPosition(clientID, youbot_base_target_handle, -1, destination_position, mode);
        simxSetObjectOrientation(clientID, youbot_base_target_handle, -1, destination_orientation, mode);


    }
}
