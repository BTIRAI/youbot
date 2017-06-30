import sys
sys.path.insert(0, 'bt/')
#
from ActionNode import *
from NodeStatus import *
import time
import threading


class MoveCloseTo(ActionNode):

    def __init__(self,name,object_id, vrep_api):
        ActionNode.__init__(self,name)
        self.vrep = vrep_api
        self.name = name
        self.object_id = object_id


    def Execute(self,args):
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.vrep.move_close_to_object(self.object_id)




class GraspObject(ActionNode):

    def __init__(self,name,object_id, vrep_api):
        ActionNode.__init__(self,name)
        self.vrep = vrep_api
        self.name = name
        self.object_id = object_id

    def Execute(self,args):
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.vrep.grasp_object(self.object_id)




class DropObject(ActionNode):

    def __init__(self,name, vrep_api):
        ActionNode.__init__(self, name)
        self.vrep = vrep_api


    def Execute(self,args):
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.vrep.drop_object()


    def Halt(self):
        if self.GetStatus() == NodeStatus.Running:
            ActionNode.Halt(self)
            self.vrep.init_arm()



class UngraspObject(ActionNode):

    def __init__(self,name, vrep_api):
        ActionNode.__init__(self, name)
        self.vrep = vrep_api


    def Execute(self,args):
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)
        self.vrep.ungrasp_object()

    def Halt(self):
        if self.GetStatus() == NodeStatus.Running:
            ActionNode.Halt(self)
            self.vrep.init_arm()