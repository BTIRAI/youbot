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

#include <yarp_condition_node.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>

using namespace yarp::os;

BT::YARPConditionNode::YARPConditionNode(std::string name, std::string server_name) : BT::ConditionNode::ConditionNode(name)
{
    std::string client_name_ = "/" + name;
    port_.open(client_name_);

    std::cout << "Waiting for the module port name "<< server_name << " to start." << std::endl;

    yarp_.sync("/" + server_name + "/cmd");//waits for the port
    if(!yarp_.connect(client_name_, "/" + server_name + "/cmd"))
    {
       std::cout << "Error! Could not connect to module " << server_name << std::endl;
       return;
    }

    BTCmd condition_server;

    condition_server_ = condition_server;
    std::cout << "Module "<< server_name << " has started." << std::endl;

    condition_server_.yarp().attachAsClient(port_);


    std::cout << "Module "<< server_name << " attached." << std::endl;

}
BT::YARPConditionNode::~YARPConditionNode()
{
    port_.close();
}



BT::ReturnStatus BT::YARPConditionNode::Tick()
{

    int32_t status = condition_server_.request_tick();
    std::cout << "tick requested" << std::endl;

    //set_status((BT::ReturnStatus)status);
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

void BT::YARPConditionNode::Finalize()
{
     port_.close();
     std::cout << "port closed" << std::endl;
}
