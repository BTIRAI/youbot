from ControlNode import ControlNode
from NodeStatus import *
import time

class SequenceNode(ControlNode):

    def __init__(self,name):
        ControlNode.__init__(self,name)
        self.nodeType = 'Sequence'



    def Execute(self,args = None):
        #print 'Starting Children Threads'
        self.SetStatus(NodeStatus.Idle)


        if True:
            #check if you have to tick a new child or halt the current
            i = 0
            for c in self.Children:
                if self.GetStatus() is NodeStatus.Halted:
                    break
                i = i + 1

                if c.nodeType == 'Action':
                    c.SendTick()
                else:
                    c.Execute(args)

                while c.GetStatus() == NodeStatus.Idle:
                    time.sleep(0.1)


                if c.GetStatus() == NodeStatus.Running:
                    self.SetStatus(NodeStatus.Running)
                    self.SetColor(NodeColor.Gray)
                    self.HaltChildren(i + 1)
                    break
                elif c.GetStatus() == NodeStatus.Success:
                    c.SetStatus(NodeStatus.Idle)
                    self.SetStatus(NodeStatus.Running)

                    if i == len(self.Children):
                        self.SetStatus(NodeStatus.Success)
                        self.SetColor(NodeColor.Green)
                        break


                elif c.GetStatus() == NodeStatus.Failure:
                    if c.nodeClass is not 'Leaf':
                        c.SetStatus(NodeStatus.Idle)

                    self.HaltChildren(i + 1)
                    self.SetStatus(NodeStatus.Failure)
                    self.SetColor(NodeColor.Red)
                    break

                elif c.GetStatus() == NodeStatus.Halted:
                    if c.nodeClass is not 'Leaf':
                        c.SetStatus(NodeStatus.Halted)

                    self.HaltChildren(i + 1)
                    break


