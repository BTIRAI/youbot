#ifndef DECORATORSYNC_H
#define DECORATORSYNC_H

#include <control_node.h>

namespace BT
{
class DecoratorSync : public ControlNode
{
public:
    DecoratorSync(std::string name, std::string link_name);
    BT::ReturnStatus Tick();
    void Halt();
    int DrawType();

private:
    TickEngine::SyncLink* link_;
    unsigned int tick_id_;
};
}
#endif // DECORATORSYNC_H
