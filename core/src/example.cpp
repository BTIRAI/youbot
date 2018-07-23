#include <iostream>
#include <behavior_tree.h>


class MyCondition : public BT::ConditionNode
{
public:
    MyCondition(std::string name);
    ~MyCondition();
    BT::ReturnStatus Tick();
};

MyCondition::MyCondition(std::string name) : BT::ConditionNode::ConditionNode(name)
{

}

BT::ReturnStatus MyCondition::Tick()
{
    std::cout << "The Condition is true" << std::endl;

    return BT::SUCCESS;
}


class MyAction : public BT::ActionNode
{
public:
    MyAction(std::string name);
    ~MyAction();
    BT::ReturnStatus Tick();
    void Halt();
};

MyAction::MyAction(std::string name) : ActionNode::ActionNode(name)
{

}


BT::ReturnStatus MyAction::Tick()
{
    std::cout << "The Action is doing some operations" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if (is_halt_requested())
    {
        return BT::HALTED;
    }

    std::cout << "The Action is doing some others operations" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if (is_halt_requested())
    {
        return BT::HALTED;
    }

    std::cout << "The Action is doing more operations" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if (is_halt_requested())
    {
        return BT::HALTED;
    }

    return BT::SUCCESS;
}


void MyAction::Halt()
{
std::cout << "The Action is performing its Halt routine" << std::endl;
}


int main(int argc, char *argv[])
{

   // BT::ParallelNode* par = new BT::ParallelNode("Parallel",2);
    BT::SequenceNode* seq = new BT::SequenceNode("Sequence");
    //BT::YARPConditionNode* my_con_1 = new BT::YARPConditionNode("Do You Wanna Walk?", "/condition_client", "/bt_interval_condition");
    //BT::YARPActionNode* my_act_1 = new BT::YARPActionNode("Walk", "WalkingModule");
    //BT::YARPActionNode* my_act_2 = new BT::YARPActionNode("Move Head", "/action2_client", "/bt_action_head");

    int tick_time_milliseconds = 1000;


    //par->AddChild(my_act_2);
    //seq->AddChild(my_con_1);
    //seq->AddChild(my_act_1);


    //par->AddChild(seq);

    Execute(seq, tick_time_milliseconds);

    return 0;
}
