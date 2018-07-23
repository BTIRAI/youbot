#pragma once
#include <action_node.h>
#include <condition_node.h>

namespace BT
{

class Grasp : public BT::ActionNode
{
public:
    Grasp(std::string object_name);
    ~Grasp();
    BT::ReturnStatus Tick();
    void Halt();
private:
    std::string object_name_;
};


class MoveCloseTo : public BT::ActionNode
{
public:
    MoveCloseTo(std::string object_name);
    ~MoveCloseTo();
    BT::ReturnStatus Tick();
    void Halt();
private:
    std::string object_name_;
};

class PutInFront : public BT::ActionNode
{
public:
    PutInFront(std::string object_name);
    ~PutInFront();
    BT::ReturnStatus Tick();
    void Halt();
private:
    std::string object_name_;
};

class PutAside : public BT::ActionNode
{
public:
    PutAside(std::string object_name);
    ~PutAside();
    BT::ReturnStatus Tick();
    void Halt();
private:
    std::string object_name_;
};

class IsRobotCloseTo : public BT::ConditionNode
{
public:
    IsRobotCloseTo(std::string object_name);
    ~IsRobotCloseTo();
    BT::ReturnStatus Tick();
private:
   std::string object_name_;
};


class IsObjectAt : public BT::ConditionNode
{
public:
    IsObjectAt(std::string object_name, std::string at_name);
    ~IsObjectAt();
    BT::ReturnStatus Tick();
private:
   std::string object_name_;
   std::string at_name_;
};

class IsObjectGrasped : public BT::ConditionNode
{
public:
    IsObjectGrasped(std::string object_name);
    ~IsObjectGrasped();
    BT::ReturnStatus Tick();
private:
   std::string object_name_;
};


}
