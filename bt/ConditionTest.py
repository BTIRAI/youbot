from ConditionNode import ConditionNode
from NodeStatus import *
from random import *
import time
class ConditionTest(ConditionNode):

    def __init__(self,name):
        ConditionNode.__init__(self,name)
        self.value = True


    def Execute(self, args):

        if self.value:
            self.SetStatus(NodeStatus.Success)
            self.SetColor(NodeColor.Green)
            print('Condition Success')
        else:
            self.SetStatus(NodeStatus.Failure)
            self.SetColor(NodeColor.Red)
            print('Condition Failed')


    def set_value(self, value):
        print('Setting value to', value)
        self.value = value
