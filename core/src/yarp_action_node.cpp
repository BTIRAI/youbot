/* Copyright (C) 2015-2017 Michele Colledanchise - All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp_action_node.h>
#include <string>

using namespace yarp::os;


BT::YARPActionNode::YARPActionNode(std::string name, std::string server_name) : BT::ActionNode::ActionNode(name)
{
    type_ = YARP_ACTION_NODE;
    std::string tick_client_name_ = "/" + name + "/tick";
    std::string halt_client_name_ = "/" + name + "/halt";

    port_tick_.open(tick_client_name_);
    port_halt_.open(halt_client_name_);

    std::cout << "Waiting for the module port name "<< server_name << " to start." << std::endl;

    yarp_.sync("/" + server_name + "/cmd");//waits for the port
    if(!yarp_.connect(tick_client_name_, "/" + server_name + "/cmd"))
    {
       std::cout << "Error! Could not connect to module " << server_name << std::endl;
       return;
    }

    if(!yarp_.connect(halt_client_name_, "/" + server_name + "/cmd"))
    {
       std::cout << "Error! Could not connect to module " << server_name << std::endl;
       return;
    }


    std::cout << "Module "<< server_name << " has started." << std::endl;

    action_tick_server_.yarp().attachAsClient(port_tick_);
    action_halt_server_.yarp().attachAsClient(port_halt_);

    std::cout << "Module "<< server_name << " attached." << std::endl;
}
BT::YARPActionNode::~YARPActionNode()
{


}


BT::ReturnStatus BT::YARPActionNode::Tick()
{
    std::cout << "requesting tick" << std::endl;

    set_status(BT::RUNNING);


    int32_t status = action_tick_server_.request_tick();
    std::cout << "tick requested" << std::endl;

    set_status((BT::ReturnStatus)status);
    return (BT::ReturnStatus)status;
//    switch(status)
//    {
//    case BT::SUCCESS:
//        std::cout << "SUCCESS" << std::endl;
//        set_status(BT::SUCCESS);
//        return BT::SUCCESS;
//        break;
//    case BT::FAILURE:
//        std::cout << "FAILURE" << std::endl;
//        set_status(BT::FAILURE);
//        return BT::FAILURE;
//        break;
//    default:
//        std::cout << "RUNNING" << std::endl;
//        set_status(BT::RUNNING);
//        return BT::RUNNING;
//    }
}

void BT::YARPActionNode::Halt()
{
    std::cout << "requesting halt" << std::endl;
    action_halt_server_.request_halt();
    std::cout << "halt requested" << std::endl;

}



void BT::YARPActionNode::Finalize()
{
     port_halt_.close();
     port_tick_.close();
     std::cout << "port closed" << std::endl;
}


