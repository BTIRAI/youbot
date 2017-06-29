from ActionNode import ActionNode
from NodeStatus import *
import time
class ActionTest(ActionNode):

    def __init__(self,name):
        ActionNode.__init__(self,name)


    def Execute(self, args):
        self.SetStatus(NodeStatus.Running)
        self.SetColor(NodeColor.Gray)

        while self.GetStatus() == NodeStatus.Running:
            print('Running Action')
            time.sleep(0.1)

