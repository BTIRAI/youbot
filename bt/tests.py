import unittest
from BehaviorTree import *
from ActionTest import *
from ConditionTest import *


class TestBT(unittest.TestCase):


    def test_simple_sequence(self):



        root = SequenceNode('Seq')
        action = ActionTest('Action')
        condition = ConditionTest('Condition')

        root.AddChild(condition)
        root.AddChild(action)

        root.Execute(None)

        self.assertEqual(action.GetStatus(), NodeStatus.Running)
        condition.set_value(False)
        self.assertEqual(action.GetStatus(), NodeStatus.Running)
        root.Execute(None)
        self.assertEqual(action.GetStatus(), NodeStatus.Halted)
        condition.set_value(True)
        self.assertEqual(action.GetStatus(), NodeStatus.Halted)
        root.Execute(None)
        self.assertEqual(action.GetStatus(), NodeStatus.Running)
        condition.set_value(False)
        root.Execute(None)
        self.assertEqual(action.GetStatus(), NodeStatus.Halted)
        action.is_destroyed = True


    def test_simple_fallback(self):


        root = FallbackNode('Fal')
        action = ActionTest('Action')
        condition = ConditionTest('Condition')
        condition.set_value(False)


        root.AddChild(condition)
        root.AddChild(action)

        root.Execute(None)

        self.assertEqual(action.GetStatus(), NodeStatus.Running)
        condition.set_value(True)
        self.assertEqual(action.GetStatus(), NodeStatus.Running)
        root.Execute(None)
        self.assertEqual(action.GetStatus(), NodeStatus.Halted)
        condition.set_value(False)
        self.assertEqual(action.GetStatus(), NodeStatus.Halted)
        root.Execute(None)
        self.assertEqual(action.GetStatus(), NodeStatus.Running)
        condition.set_value(True)
        root.Execute(None)
        self.assertEqual(action.GetStatus(), NodeStatus.Halted)
        action.is_destroyed = True
