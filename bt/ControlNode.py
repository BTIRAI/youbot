from TreeNode import TreeNode
from abc import ABCMeta, abstractmethod
from NodeStatus import NodeStatus

class ControlNode(TreeNode):
    __metaclass__ = ABCMeta  # abstract class

    def __init__(self,name):
        TreeNode.__init__(self,name)
        self.nodeClass = 'Control'
        self.Children = []

    def AddChild(self,child, index = -1):
        if index is -1:
            self.Children.append(child)
        else:
            self.Children.insert(index, child)

    def GetChildren(self):
        return self.Children

    def RemoveChild(self, child):
        self.Children.remove(child)

    def IncreasePriorityOfChild(self,child):
        child_index = self.Children.index(child)
        if child_index is 0:
            raise Exception('Cannot increase the priority of ' + child.name + ' any further')
        child = self.Children.pop(child_index)
        self.Children.insert(child_index - 1, child)
        print('Priority increased. Index: ', child_index)


    def SetChild(self,index,child):
        self.Children[index] = child

    def ReverseChildren(self):
        self.Children.reverse()

    def HaltChildren(self,h):
        i = 0
        for c in self.Children:
            i = i + 1
            if i >= h:
                if c.GetStatus() != NodeStatus.Idle:
                    #print 'Halting Child' + str(c.name)
                    c.Halt()
    def Halt(self):
        self.SetStatus(NodeStatus.Idle)
        self.HaltChildren(0)

    def Print(self):
        print(self.name)
        for child in self.Children:
                child.Print()