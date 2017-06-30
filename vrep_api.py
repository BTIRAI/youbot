try:
    import vrep
except:
    print ('--------------------------------------------------------------')
    print ('"vrep.py" could not be imported. This means very probably that')
    print ('either "vrep.py" or the remoteApi library could not be found.')
    print ('Make sure both are in the same folder as this file,')
    print ('or appropriately adjust the file "vrep.py"')
    print ('--------------------------------------------------------------')
    print ('')


import numpy as np

import time
import copy

from threading import RLock
from shapely.geometry import Point
from shapely.geometry.polygon import Polygon

class vrep_api:
    def __init__(self):
        vrep.simxFinish(-1)  # just in case, close all opened connections
        self.clientID = vrep.simxStart(b'127.0.0.1', 19999, True, True, 5000, 5)  # Connect to V-REP
        if self.clientID != -1:
            print('Connected to remote API server')

        else:
            print('Failed connecting to remote API server')

        self.youbot_vehicle_target_id = self.get_id(b'youBot_vehicleTargetPosition')
        self.youbot_ref_id = self.get_id(b'youBot_vehicleReference')
        self.youbot_front_ref_id = self.get_id(b'youBot_frontReference')
        self.gripper_id = self.get_id(b'youBot_gripperPositionTarget')
        self.objects_id_list = []
        self.object_grasped_id = None
        self.blue_cube_id = self.get_id(b'blueRectangle')
        self.yellow_cube_id = self.get_id(b'yellowRectangle')
        self.objects_id_list.append(self.blue_cube_id)
        self.objects_id_list.append(self.yellow_cube_id)

        self.samples_id_list = []

        self.samples_id_list.append(self.get_id(b'Disc1'))
        self.samples_id_list.append(self.get_id(b'Disc2'))
        self.samples_id_list.append(self.get_id(b'Disc3'))
        self.samples_id_list.append(self.get_id(b'Disc4'))
        self.samples_id_list.append(self.get_id(b'Disc5'))
        self.samples_id_list.append(self.get_id(b'Disc6'))
        self.samples_id_list.append(self.get_id(b'Disc7'))
        self.samples_id_list.append(self.get_id(b'Disc8'))
        self.samples_id_list.append(self.get_id(b'Disc9'))
        self.samples_id_list.append(self.get_id(b'Disc10'))

        self.current_object_goal_id = None
        self.current_object_to_move_to_goal = None

        self.current_polygon = None


        self.object_grasped_id_lock = RLock()


    def get_id(self, name):

        error_id, target_id = vrep.simxGetObjectHandle(self.clientID, name, vrep.simx_opmode_oneshot_wait)

        if error_id:
            raise Exception('Error! id:' ,name, ' is not in the scene')
        else:
            return target_id


    def get_position(self, object_id, relative_id):

        error_id, position = vrep.simxGetObjectPosition(self.clientID, object_id, relative_id,
                                                        vrep.simx_opmode_oneshot_wait)

        if error_id:
            # raise Exception('Error! cannot retrive pose of' ,object_id)
            return [0, 0, 0]
        else:
            return position


    def get_orientation(self, object_id, relative_id):

        error_id, orientation = vrep.simxGetObjectOrientation(self.clientID, object_id, relative_id,
                                                              vrep.simx_opmode_oneshot_wait)

        if error_id:
            raise Exception('Error! cannot retrive position of' ,object_id)
        else:
            return orientation


    def set_position(self, object_id, relative_id, position):

        error_id = vrep.simxSetObjectPosition(self.clientID, object_id, relative_id, position,
                                              vrep.simx_opmode_oneshot_wait)

        if error_id:
            raise Exception('Error! cannot set position of', object_id)


    def set_orientation(self, object_id, relative_id, orientation):

        error_id = vrep.simxSetObjectOrientation(self.clientID, object_id, relative_id, orientation,
                                                 vrep.simx_opmode_oneshot_wait)

        if error_id:
            raise Exception('Error! cannot set orientation of', object_id)



    def get_pose(self, object_id, relative_id):
        return self.get_position(object_id, relative_id) + self.get_orientation(object_id, relative_id)


    def set_pose(self, object_id, relative_id, pose):
        self.set_position(object_id, relative_id, pose[0:3])
        self.set_orientation(object_id, relative_id, pose[3:6])





    def close_connection(self):
        # Before closing the connection to V-REP, make sure that the last command sent out had time to arrive. You can guarantee this with (for example):
        vrep.simxGetPingTime(self.clientID)

        # Now close the connection to V-REP:
        vrep.simxFinish(self.clientID)

        print('Connection to remote API server closed')

    def is_object_between_poses(self, pose_1, pose_2, offset):
        #TODO: this is not correct if the youbot moves only sideways
        rectangle_tr_id = self.get_id(b'DiscTopRight')
        rectangle_tl_id = self.get_id(b'DiscTopLeft')
        rectangle_br_id = self.get_id(b'DiscBottomRight')
        rectangle_bl_id = self.get_id(b'DiscBottomLeft')


        rectangle_tr_pose = list(pose_2) #copy
        rectangle_tl_pose = list(pose_2) #copy
        rectangle_br_pose = list(pose_1) #copy
        rectangle_bl_pose = list(pose_1) #copy
        rectangle_br_pose[3:6] = rectangle_tr_pose[3:6]
        rectangle_bl_pose[3:6] = rectangle_tl_pose[3:6]



        self.set_pose(rectangle_tr_id, -1, rectangle_tr_pose)
        self.set_pose(rectangle_tl_id, -1, rectangle_tl_pose)
        self.set_pose(rectangle_br_id, -1, rectangle_br_pose)
        self.set_pose(rectangle_bl_id, -1, rectangle_bl_pose)

        offset_pose_plus = [offset, 0, 0.1, 0, 0, 0]
        offset_pose_minus = [-offset, 0, 0.1, 0, 0, 0]

        self.set_pose(rectangle_tr_id, rectangle_tr_id, offset_pose_plus)
        self.set_pose(rectangle_tl_id, rectangle_tl_id, offset_pose_minus)
        self.set_pose(rectangle_br_id, rectangle_br_id, offset_pose_plus)
        self.set_pose(rectangle_bl_id, rectangle_bl_id, offset_pose_minus)

        rectangle_tr_pose = self.get_pose(rectangle_tr_id, -1)
        rectangle_tl_pose = self.get_pose(rectangle_tl_id, -1)
        rectangle_br_pose = self.get_pose(rectangle_br_id, -1)
        rectangle_bl_pose = self.get_pose(rectangle_bl_id,-1)

        self.current_polygon = Polygon([(rectangle_tr_pose[0], rectangle_tr_pose[1]), (rectangle_tl_pose[0], rectangle_tl_pose[1]),(rectangle_bl_pose[0], rectangle_bl_pose[1]), (rectangle_br_pose[0], rectangle_br_pose[1])])


        for object_id in self.objects_id_list:
            object_position = self.get_position(object_id, -1)
            point = Point(object_position[0], object_position[1])
            if self.current_polygon.contains(point):
                return True, object_id
        return False, None






    def is_object_between_objects(self, object_1_id, object_2_id, offset):
        object_1_pose = self.get_pose(object_1_id,-1)
        object_2_pose = self.get_pose(object_2_id,-1)
        return self.is_object_between_poses(object_1_pose, object_2_pose, offset)


    def is_path_to_collision_free(self, object_id):

        is_collision, colliding_object = self.is_object_between_objects(self.youbot_ref_id, object_id, 0.4)

        if self.object_grasped_id is not None and self.object_grasped_id is not self.current_object_to_move_to_goal:
            return True, self.object_grasped_id
        if self.current_object_to_move_to_goal is colliding_object and self.object_grasped_id is colliding_object:
            return False, None


        return is_collision, colliding_object




    def get_closest_inverse_pose(self,object_id,ref_id,type = 'cube'):

        if type is 'cube':
            z_shift = -0.03
        elif type is 'goal':
            z_shift = 0

        distance = 10000000000
        distance = 10000000000
        closest_inverse_pose = None
        dummy_id = self.get_id(b'Disc0')
        shift = 0.3

        #shift in x
        empty_position = [0,0,z_shift,0,0,0]
        empty_position[0] += shift
        self.set_pose(dummy_id,object_id,empty_position)
        self.set_orientation(dummy_id,object_id,[0,0,3.14/2])

        dummy_rel_position = self.get_position(dummy_id,ref_id)
        dummy_distance = np.linalg.norm(dummy_rel_position)
        if dummy_distance < distance:
            closest_inverse_pose = self.get_pose(dummy_id,-1)
            distance = dummy_distance

        empty_position = [0,0,z_shift,0,0,0]
        empty_position[0] -= shift

        self.set_pose(dummy_id,object_id,empty_position)
        self.set_orientation(dummy_id,object_id,[0,0,-3.14/2])
        dummy_rel_position = self.get_position(dummy_id,ref_id)
        dummy_distance = np.linalg.norm(dummy_rel_position)
        if dummy_distance < distance:
            closest_inverse_pose = self.get_pose(dummy_id,-1)
            distance = dummy_distance


        #shift in y
        empty_position = [0,0,-0.03,0,0,0]
        empty_position[1] += shift
        self.set_pose(dummy_id,object_id,empty_position)
        self.set_orientation(dummy_id,object_id,[0,0,3.14])

        dummy_rel_position = self.get_position(dummy_id,ref_id)
        dummy_distance = np.linalg.norm(dummy_rel_position)
        if dummy_distance < distance:
            closest_inverse_pose = self.get_pose(dummy_id,-1)
            distance = dummy_distance

        empty_position = [0,0,-0.03,0,0,0]
        empty_position[1] -= shift

        self.set_pose(dummy_id,object_id,empty_position)
        dummy_rel_position = self.get_position(dummy_id,ref_id)
        dummy_distance = np.linalg.norm(dummy_rel_position)
        if dummy_distance < distance:
            closest_inverse_pose = self.get_pose(dummy_id,-1)
            distance = dummy_distance

        return closest_inverse_pose



    def grasp_object(self,object_id):

        self.open_gripper()
        time.sleep(2)

        self.set_position(self.gripper_id, object_id, [0,0,0])

        time.sleep(2)

        self.close_gripper()

        time.sleep(2)

        self.init_arm()
        self.object_grasped_id = object_id

        time.sleep(2)


    def drop_object(self):
        with self.object_grasped_id_lock:
            time.sleep(4)


            self.set_position(self.gripper_id,self.gripper_id,[0,0.1,-0.1])

            time.sleep(2)
            self.open_gripper()
            time.sleep(2)

            self.init_arm()
            time.sleep(2)

            self.object_grasped_id = None
            self.current_object_goal_id = None
            self.current_object_to_move_to_goal = None

    def ungrasp_object(self):
        with self.object_grasped_id_lock:
            time.sleep(4)

            self.set_position(self.gripper_id,self.gripper_id,[0.2,0.1,-0.1])

            time.sleep(2)
            self.open_gripper()
            time.sleep(2)

            self.init_arm()
            time.sleep(2)

            self.object_grasped_id = None
            self.current_object_goal_id = None
            self.current_object_to_move_to_goal = None


    def get_object_grasped_id(self):
        with self.object_grasped_id_lock:
            return self.object_grasped_id


    def init_arm(self):

        self.set_position(self.gripper_id,self.youbot_ref_id,[0,0.2,0.2])


    def open_gripper(self):
        vrep.simxSetIntegerSignal(self.clientID, b'gripperCommand', 1, vrep.simx_opmode_oneshot_wait)


    def close_gripper(self):
        vrep.simxSetIntegerSignal(self.clientID, b'gripperCommand', 0, vrep.simx_opmode_oneshot_wait)


    def move_close_to_object(self,object_id, type='cube'):



        print('moving close to object', object_id)
        cip = self.get_closest_inverse_pose(object_id, self.youbot_vehicle_target_id, type)
        self.set_pose(self.youbot_vehicle_target_id, -1, cip)

    def is_robot_close_2d(self,object_id, threshold):
        position = self.get_position(object_id,self.youbot_ref_id)
        return np.linalg.norm(position) < threshold


    def are_objects_close(self,object_1_id,object_2_id,threshold):
        position = self.get_position(object_1_id,object_2_id)

        return np.linalg.norm(position) < threshold

    def are_objects_close2d(self,object_1_id,object_2_id,threshold):
        self.current_object_goal_id = object_2_id
        self.current_object_to_move_to_goal = object_1_id
        position = self.get_position(object_1_id,object_2_id)
        position[1] = position[1]-0.1

        return np.linalg.norm(position) < threshold



    def get_sample_id_outside_region(self):
        for sample_id in self.samples_id_list:
            sample_position = self.get_position(sample_id, -1)
            point = Point(sample_position[0], sample_position[1])
            is_collision, colliding_object, = self.is_path_to_collision_free(sample_id)
            if not is_collision or (is_collision and colliding_object is self.object_grasped_id):
                self.samples_id_list.remove(sample_id)
                return sample_id
            else:
                print('Sample', sample_id, 'is in collision with', colliding_object)

        raise Exception('ERROR, there is no sample outside the region')


