#pragma once

#include "BehaviorTreeNodeModel.hpp"


class ActionNodeModel : public BehaviorTreeNodeModel
{
public:
    ActionNodeModel():
        BehaviorTreeNodeModel("Action", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ACTION;
    }
    virtual ~ActionNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new ActionNodeModel ); }

    virtual QString name() const override { return QString("Action"); }
//    virtual QString name() const override { return QString("Action"); }
};



class MoveCloseToObjectNodeModel : public BehaviorTreeNodeModel
{
public:
    MoveCloseToObjectNodeModel():
        BehaviorTreeNodeModel("MoveCloseTo", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::MOVETO;
    }
    virtual ~MoveCloseToObjectNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new MoveCloseToObjectNodeModel ); }

    virtual QString name() const override { return QString("MoveCloseTo"); }
};


class GraspObjectNodeModel : public BehaviorTreeNodeModel
{
public:
    GraspObjectNodeModel():
        BehaviorTreeNodeModel("GraspObject", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::GRASP;
    }
    virtual ~GraspObjectNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new GraspObjectNodeModel ); }

    virtual QString name() const override { return QString("GraspObject"); }
};


class PutObjectInFrontNodeModel : public BehaviorTreeNodeModel
{
public:
    PutObjectInFrontNodeModel():
        BehaviorTreeNodeModel("PutObjectInFront", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::PUTFRONT;
    }
    virtual ~PutObjectInFrontNodeModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new PutObjectInFrontNodeModel ); }

    virtual QString name() const override { return QString("PutObjectInFront"); }
};


class PutObjectOnSideModel : public BehaviorTreeNodeModel
{
public:
    PutObjectOnSideModel():
        BehaviorTreeNodeModel("PutObjectOnSide", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::PUTSIDE;
    }
    virtual ~PutObjectOnSideModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new PutObjectOnSideModel ); }

    virtual QString name() const override { return QString("PutObjectOnSide"); }
};


class IsRobotCloseToModel : public BehaviorTreeNodeModel
{
public:
    IsRobotCloseToModel():
        BehaviorTreeNodeModel("IsRobotCloseTo", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ISROBOTCLOSE;
    }
    virtual ~IsRobotCloseToModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new IsRobotCloseToModel ); }

    virtual QString name() const override { return QString("IsRobotCloseTo"); }
};


class IsObjectGraspedModel : public BehaviorTreeNodeModel
{
public:
    IsObjectGraspedModel():
        BehaviorTreeNodeModel("IsObjectGrasped", NodeFactory::get().getActionParameterModel() )
    { }
    int BTType()
    {
        return BT::ISOBJECTGRASPED;
    }
    virtual ~IsObjectGraspedModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new IsObjectGraspedModel ); }

    virtual QString name() const override { return QString("IsObjectGrasped"); }
};



class IsObjectAtObjectModel : public BehaviorTreeNodeModel
{
public:
    QLineEdit* _another_line_edit;

    IsObjectAtObjectModel():
        BehaviorTreeNodeModel("IsObjectAtObject", NodeFactory::get().getActionParameterModel() )
    {

    }
    int BTType()
    {
        return BT::ISOBJECTONOBJECT;
    }
    virtual ~IsObjectAtObjectModel() {}

    virtual unsigned int  nPorts(PortType portType) const override
    { return (portType==PortType::In) ? 1:0; }

    virtual std::unique_ptr<NodeDataModel> clone() const override
    { return  std::unique_ptr<NodeDataModel>( new IsObjectAtObjectModel ); }

    virtual QString name() const override { return QString("IsObjectAtObject"); }
};

