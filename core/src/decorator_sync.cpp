#include <decorator_sync.h>

#include <boost/interprocess/shared_memory_object.hpp>



using namespace boost::interprocess;

BT::DecoratorSync::DecoratorSync(std::string name, std::string link_name) : ControlNode::ControlNode(name)
{

    link_ = tick_engine.CreateSyncLink(link_name);
    tick_id_ = 0;

}


BT::ReturnStatus BT::DecoratorSync::Tick()
{
    if (children_nodes_[0]->get_type() == BT::ACTION_NODE)
    {
        throw std::string("DecoratorSync can only have actions as children");
    }
    if(link_->is_tick_id_ready(tick_id_))
    {
        BT::ReturnStatus status = children_nodes_[0]->TickSync(tick_id_);
        tick_id_++;
        if (status == BT::SUCCESS || status == BT::FAILURE)
        {
            set_status(status);
            return status;
        }
    }
    set_status(BT::RUNNING);
    return BT::RUNNING;
}

void BT::DecoratorSync::Halt()
{

    tick_id_ = 0;
    ControlNode::Halt();

}


int BT::DecoratorSync::DrawType()
{
    // Lock acquistion

    return BT::DECORATOR;
}
