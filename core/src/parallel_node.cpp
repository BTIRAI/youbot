
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


#include <parallel_node.h>
#include <string>

BT::ParallelNode::ParallelNode(std::string name, int threshold_M) : ControlNode::ControlNode(name)
{
    threshold_M_ = threshold_M;
    are_children_syncronized_ = true;
}

BT::ParallelNode::~ParallelNode() {}

BT::ReturnStatus BT::ParallelNode::Tick()
{
    if (are_children_syncronized_)
    {
        return TickSync();
    }
    success_childred_num_ = 0;
    failure_childred_num_ = 0;
    // Vector size initialization. N_of_children_ could change at runtime if you edit the tree
    N_of_children_ = children_nodes_.size();

    // Routing the tree according to the sequence node's logic:
    for (unsigned int i = 0; i < N_of_children_; i++)
    {
        DEBUG_STDOUT(get_name() << "TICKING " << children_nodes_[i]->get_name());

        if (children_nodes_[i]->get_type() == BT::ACTION_NODE || children_nodes_[i]->get_type() == BT::YARP_ACTION_NODE  )
        {
            // 1) If the child i is an action, read its state.
            // Action nodes runs in another parallel, hence you cannot retrieve the status just by executing it.
            child_i_status_ = children_nodes_[i]->get_status();

            if (child_i_status_ == BT::IDLE || child_i_status_ == BT::HALTED)
            {
                // 1.1 If the action status is not running, the sequence node sends a tick to it.
                DEBUG_STDOUT(get_name() << "NEEDS TO TICK " << children_nodes_[i]->get_name());
                children_nodes_[i]->tick_engine.Tick();

                // waits for the tick to arrive to the child
                do
                {
                    child_i_status_ = children_nodes_[i]->get_status();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                while (child_i_status_ != BT::RUNNING && child_i_status_ != BT::SUCCESS
                       && child_i_status_ != BT::FAILURE);
            }
        }
        else
        {
            child_i_status_ = children_nodes_[i]->Tick();
        }
        switch (child_i_status_)
        {
        case BT::SUCCESS:
            children_nodes_[i]->set_status(BT::IDLE);  // the child goes in idle if it has returned success.
            if (++success_childred_num_ == threshold_M_)
            {
                success_childred_num_ = 0;
                failure_childred_num_ = 0;
                HaltChildren(0);  // halts all running children. The execution is done.
                set_status(child_i_status_);
                return child_i_status_;
            }
            break;
        case BT::FAILURE:
            children_nodes_[i]->set_status(BT::IDLE);  // the child goes in idle if it has returned failure.
            if (++failure_childred_num_ > N_of_children_- threshold_M_)
            {
                DEBUG_STDOUT("*******PARALLEL" << get_name()
                             << " FAILED****** failure_childred_num_:" << failure_childred_num_);

                success_childred_num_ = 0;
                failure_childred_num_ = 0;
                HaltChildren(0);  // halts all running children. The execution is hopeless.
                set_status(child_i_status_);
                return child_i_status_;
            }
            break;
        case BT::RUNNING:
            set_status(child_i_status_);
            break;
        default:
            break;
        }
    }
    return BT::RUNNING;
}

void BT::ParallelNode::Halt()
{
    success_childred_num_ = 0;
    failure_childred_num_ = 0;
    BT::ControlNode::Halt();
}

int BT::ParallelNode::DrawType()
{
    return BT::PARALLEL;
}


unsigned int BT::ParallelNode::get_threshold_M()
{
    return threshold_M_;
}

void BT::ParallelNode::set_threshold_M(unsigned int threshold_M)
{
    threshold_M_ = threshold_M;
}





BT::ReturnStatus BT::ParallelNode::TickSync()
{// when ticking the children, they are suncronized
    tick_id_ = 0;

    success_childred_num_ = 0;
    failure_childred_num_ = 0;
    // Vector size initialization. N_of_children_ could change at runtime if you edit the tree
    N_of_children_ = children_nodes_.size();

    // Routing the tree according to the sequence node's logic:
    for (unsigned int i = 0; i < N_of_children_; i++)
    {
        DEBUG_STDOUT(get_name() << "TICKING " << children_nodes_[i]->get_name());

        if (children_nodes_[i]->get_type() == BT::ACTION_NODE || children_nodes_[i]->get_type() == BT::YARP_ACTION_NODE)
        {
            // 1) If the child i is an action, read its state.
            // Action nodes runs in another parallel, hence you cannot retrieve the status just by executing it.
            child_i_status_ = children_nodes_[i]->get_status();

            if (child_i_status_ == BT::IDLE || child_i_status_ == BT::HALTED)
            {
                // 1.1 If the action status is not running, the sequence node sends a tick to it.
                DEBUG_STDOUT(get_name() << "NEEDS TO TICK " << children_nodes_[i]->get_name());
                children_nodes_[i]->tick_engine.Tick();

                // waits for the tick to arrive to the child
                do
                {
                    child_i_status_ = children_nodes_[i]->get_status();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                while (child_i_status_ != BT::RUNNING && child_i_status_ != BT::SUCCESS
                       && child_i_status_ != BT::FAILURE);
            }
        }
        else
        {
            child_i_status_ = children_nodes_[i]->Tick();
        }
        switch (child_i_status_)
        {
        case BT::SUCCESS:
            children_nodes_[i]->set_status(BT::IDLE);  // the child goes in idle if it has returned success.
            if (++success_childred_num_ == threshold_M_)
            {
                success_childred_num_ = 0;
                failure_childred_num_ = 0;
                HaltChildren(0);  // halts all running children. The execution is done.
                set_status(child_i_status_);
                return child_i_status_;
            }
            break;
        case BT::FAILURE:
            children_nodes_[i]->set_status(BT::IDLE);  // the child goes in idle if it has returned failure.
            if (++failure_childred_num_ > N_of_children_- threshold_M_)
            {
                DEBUG_STDOUT("*******PARALLEL" << get_name()
                             << " FAILED****** failure_childred_num_:" << failure_childred_num_);

                success_childred_num_ = 0;
                failure_childred_num_ = 0;
                HaltChildren(0);  // halts all running children. The execution is hopeless.
                set_status(child_i_status_);
                return child_i_status_;
            }
            break;
        case BT::RUNNING:
            set_status(child_i_status_);
            break;
        default:
            break;
        }
    }
    return BT::RUNNING;
}
