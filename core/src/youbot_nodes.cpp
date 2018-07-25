#include <iostream>
#include <behavior_tree.h>
#include <youbot_nodes.h>
#include <vrep_utilities.h>


BT::Grasp::Grasp(std::string object_name) : ActionNode::ActionNode("Grasp "+object_name)
{

object_name_ = object_name;
}


BT::ReturnStatus BT::Grasp::Tick()
{
    std::cout << "Doing Grasp" << std::endl;

    vrep_utilities::graspObject(object_name_);

    return BT::SUCCESS;
}


void BT::Grasp::Halt()
{
std::cout << "The Grasp Action is performing its Halt routine" << std::endl;
set_status(BT::HALTED);

}




BT::MoveCloseTo::MoveCloseTo(std::string object_name) : ActionNode::ActionNode("Move Close To "+object_name)
{

object_name_ = object_name;
}


BT::ReturnStatus BT::MoveCloseTo::Tick()
{
    std::cout << "Doing MoveCloseTo" << std::endl;

    vrep_utilities::approachObject(object_name_);
    return BT::RUNNING;
}


void BT::MoveCloseTo::Halt()
{
    std::cout << "The MoveCloseTo Action is performing its Halt routine" << std::endl;
    vrep_utilities::stopMove();
    set_status(BT::HALTED);
}





BT::PutInFront::PutInFront(std::string object_name) : ActionNode::ActionNode("Put "+object_name+ " in Front")
{

object_name_ = object_name;
}


BT::ReturnStatus BT::PutInFront::Tick()
{
    std::cout << "Doing PutInFront" << std::endl;

    vrep_utilities::dropObject();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    return BT::RUNNING;
}


void BT::PutInFront::Halt()
{
    std::cout << "The PutInFront Action is performing its Halt routine" << std::endl;
    set_status(BT::HALTED);

}



BT::PutAside::PutAside(std::string object_name) : ActionNode::ActionNode("Put "+object_name+ " on the Side")
{

object_name_ = object_name;
}


BT::ReturnStatus BT::PutAside::Tick()
{
    std::cout << "Doing PutAside" << std::endl;

    // **TODO**
    return BT::RUNNING;
}


void BT::PutAside::Halt()
{
    std::cout << "The PutAside Action is performing its Halt routine" << std::endl;
    set_status(BT::HALTED);

}



BT::IsRobotCloseTo::IsRobotCloseTo(std::string object_name) : ConditionNode::ConditionNode("Is Robot close to "+object_name)
{

object_name_ = object_name;
}


BT::ReturnStatus BT::IsRobotCloseTo::Tick()
{
    std::cout << "Checking IsRobotCloseTo" << std::endl;

    if (vrep_utilities::isRobotCloseTo(object_name_) || vrep_utilities::isObjectGrasped(object_name_))
    {

        std::cout << "*****************ROBOT CLOSE TO" << object_name_ << std::endl;


        return BT::SUCCESS;
    }
    else
    {
        return BT::FAILURE;

    }

}

BT::IsObjectAt::IsObjectAt(std::string object_name, std::string at_name) : ConditionNode::ConditionNode("Is  "+object_name+ " at "+ at_name)
{

    object_name_ = object_name;
    at_name_ = at_name;
}


BT::ReturnStatus BT::IsObjectAt::Tick()
{
    std::cout << "Checking IsObjectAt" << std::endl;

    if (vrep_utilities::isObjectAt(object_name_, at_name_))
    {

        return BT::SUCCESS;
    }
    else
    {
        return BT::FAILURE;

    }
}



BT::IsObjectGrasped::IsObjectGrasped(std::string object_name) : ConditionNode::ConditionNode("Is "+object_name+ " grasped")
{

object_name_ = object_name;
}


BT::ReturnStatus BT::IsObjectGrasped::Tick()
{
    std::cout << "Checking IsObjectGrasped" << std::endl;

    if (vrep_utilities::isObjectGrasped(object_name_))
    {

        return BT::SUCCESS;
    }
    else
    {
        return BT::FAILURE;

    }

}





