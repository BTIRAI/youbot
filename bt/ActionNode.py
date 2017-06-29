from abc import ABCMeta
from LeafNode import LeafNode
import threading
import time
from NodeStatus import *

class ActionNode(LeafNode):
    def __init__(self, name, param = ''):
        LeafNode.__init__(self, name)
        self.nodeType = 'Action'
        self.param = param
        self.generic_name = name

        self.lock = threading.Lock()
        self.tick_received = False
        self.is_destroyed = False

        self.execution_thread = threading.Thread(target=self.wait_for_tick, args=(None,))
        self.execution_thread.start()


    def wait_for_tick(self,args):
        while not self.is_destroyed:
            self.lock.acquire()
            if self.tick_received:
                self.lock.release()

                self.Execute(args)

                self.lock.acquire()

            self.tick_received = False
            self.lock.release()

            time.sleep(0.1)


    def SendTick(self):

        with self.lock:
            if not self.tick_received:
                self.SetStatus(NodeStatus.Running)
                self.tick_received = True

    def Halt(self):
        super(ActionNode, self).Halt()
        with self.lock:
             self.tick_received = False

