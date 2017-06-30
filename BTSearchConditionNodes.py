from ConditionNode import ConditionNode
from NodeStatus import *
import time


class IsRobotCloseTo(ConditionNode):
    def __init__(self, name, object_id, vrep_api):


        ConditionNode.__init__(self, name)
        self.vrep = vrep_api
        self.name = name
        self.object_id = object_id

    def Execute(self, args):
        if self.vrep.is_robot_close_2d(self.object_id, 0.32):
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)
        else:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)


class IsObjectGrasped(ConditionNode):
    def __init__(self, name, object_id, vrep_api):

        ConditionNode.__init__(self, name)
        self.vrep = vrep_api
        self.name = name
        self.object_id = object_id

    def Execute(self, args):
        if  self.vrep.get_object_grasped_id() is self.object_id:
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)
        else:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)


class IsObjectAt(ConditionNode):
    def __init__(self, name, object_id, at, vrep_api):

        ConditionNode.__init__(self, name)
        self.vrep = vrep_api
        self.name = name
        self.object_id = object_id
        self.at = at

    def Execute(self, args):
        self.vrep.get_object_grasped_id()#just for waiting the lock
        if self.vrep.are_objects_close2d(self.object_id, self.at,0.12):
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)
        else:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)

class IsPathToObjectCollisionFree(ConditionNode):
    def __init__(self, name, object_id, vrep_api):
        for i in list(fluent.parameters_dict.values()):
            name = name + '_' + str(i)
        ConditionNode.__init__(self, name)
        self.vrep = vrep_api
        self.name = name
        self.object_id = object_id

    def Execute(self, args):
        is_collision, object_colliding_id = self.vrep.is_path_to_collision_free(self.object_id)

        if not is_collision:
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)
        else:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)