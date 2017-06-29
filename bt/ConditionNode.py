from abc import ABCMeta
from LeafNode import LeafNode


class ConditionNode(LeafNode):
    __metaclass__ = ABCMeta  # abstract class
    def __init__(self, name, param = ''):
        LeafNode.__init__(self, name)
        self.nodeType = 'Condition'
        self.param = param
        self.generic_name = name
